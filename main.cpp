#include <iostream>
#include <list>
#include <unistd.h>
#include "src/Util/Logger/Logger.h"
#include "src/Util/INIReader/INIReader.h"
#include "src/Domain/Order/Order.h"
#include "src/Domain/Receptionist/Receptionist.h"
#include "src/Domain/Cook/Cook.h"
#include <sys/wait.h>
#include "src/Domain/Oven/Oven.h"
#include "src/Domain/Supervisor/Supervisor.h"
#include <fstream>

typedef enum ProcessType {
    ProcessTypeFather,
    ProcessTypeChild
} ProcessType;


ProcessType createCooks(long amount);

ProcessType createReceptionists(long amount, int cookAmount);

ProcessType createCadets(long amount);

ProcessType createSupervisor();

ProcessType createOvens(long amount);

void createTempLockfiles();

void deleteTempLockfiles();

void cleanup();

using namespace std;

vector<__pid_t> spawnedProcesses;
int mainpid;
int supervisorPID;

void signalCallback(int signum){
    if (signum==SIGINT && getpid() == mainpid) {
        Logger::logger().log("Received signal " + to_string(signum));
        for (int i = 0; i < spawnedProcesses.size(); i++) {
            Logger::logger().log("Killing child pid: " + to_string(spawnedProcesses[i]));

            kill(spawnedProcesses[i], signum);
        }

        cleanup();
        exit(0);
    }
}

int main() {
    remove(LOG_FILE);
    signal(SIGINT, signalCallback);

    INIReader reader(CONFIG_FILE);
    if (reader.ParseError() < 0) {
        std::cout << "Can't load 'config_file.cfg'\n";
        return 1;
    }

    mainpid = getpid();

    long receptionistQuantity = reader.GetInteger("parameters", "receptionists", -1);
    long cookQuantity = reader.GetInteger("parameters", "cooks", -1);
    long cadetsQuantity = reader.GetInteger("parameters", "cadets", -1);
    long ovenQuantity = reader.GetInteger("parameters", "ovens", -1);
    long debug = reader.GetInteger("parameters", "debug", -1);

    if (debug != 0) {
        Logger::logger().debug = true;
    }

    Logger::logger().log("Launching app...");
    createTempLockfiles();

    //Create receptionist processes
    ProcessType resultReceptionist = createReceptionists(receptionistQuantity, (int) cookQuantity);
    if (resultReceptionist == ProcessTypeChild) return 0;


    //Init semaphore
    Semaforo(SEMAPHORE_NAME, 0);

    //Create cook processes
    ProcessType resultCook = createCooks(cookQuantity);
    if (resultCook == ProcessTypeChild) {
        return 0;
    }

    //Create oven processes
    ProcessType resultOvens = createOvens(ovenQuantity);
    if (resultOvens == ProcessTypeChild) {
        return 0;
    }

    //Create cadet processes
    ProcessType resultCadet = createCadets(cadetsQuantity);
    if (resultCadet == ProcessTypeChild) {
        return 0;
    }

    ProcessType resultSupervisor = createSupervisor();
    if (resultSupervisor == ProcessTypeChild) {
        return 0;
    }

    FifoEscritura fifo = FifoEscritura(Receptionist::getOrderFifoName());
    fifo.abrir();
    for (int i = 0; i < 10; i++) {
        std::string dato = "Orden ";
        dato = dato + to_string(i);
        dato.resize(MESSAGE_LENGTH);
        sleep(3);
        fifo.escribir(dato.c_str(), (int const) dato.size());
    }
    fifo.cerrar();

    //Esperar por los hijos
    for (int i = 0; i < receptionistQuantity + cookQuantity + cadetsQuantity + ovenQuantity; i++) {
        wait(NULL);
    }

    //Kill supervisor
    Logger::logger().log("Killing child pid: " + to_string(supervisorPID));
    kill(supervisorPID, SIGINT);

    cleanup();

    return 0;
}

void cleanup() {
    Logger::logger().log("Cleaning up resources");
    deleteTempLockfiles();
    Semaforo(SEMAPHORE_NAME).eliminar();
    FifoEscritura(Receptionist::getOrderFifoName()).eliminar();
    FifoEscritura(Cook::getPizzaFifoName()).eliminar();
    FifoEscritura(Cadet::getPizzaCookedFifoName()).eliminar();
    Logger::logger().log("Exiting app");
}

void deleteTempLockfiles() {
    remove(LOCKFILE_PAYDESK);
    remove(LOCKFILE_HANDLED_ORDERS);
}

void createTempLockfiles() {//Creating temp lock files
    ofstream myfile;
    myfile.open(LOCKFILE_PAYDESK);
    myfile.close();
    myfile.open(LOCKFILE_HANDLED_ORDERS);
    myfile.close();
}


ProcessType createReceptionists(long amount, int cookAmount) {
    for (int i = 0; i < amount; i++) {
        __pid_t pid = fork();
        if (!pid) {
            Receptionist r = Receptionist(cookAmount);
            return ProcessTypeChild;
        } else {
            spawnedProcesses.push_back(pid);
        }
    }

    return ProcessTypeFather;
}

ProcessType createCooks(long amount) {
    for (int i = 0; i < amount; i++) {
        __pid_t pid = fork();
        if (!pid) {
            Cook();
            return ProcessTypeChild;
        } else {
            spawnedProcesses.push_back(pid);
        }
    }

    return ProcessTypeFather;
}


ProcessType createOvens(long amount) {
    for (int i = 0; i < amount; i++) {
        __pid_t pid = fork();
        if (!pid) {
            Oven();
            return ProcessTypeChild;
        } else {
            spawnedProcesses.push_back(pid);
        }
    }

    return ProcessTypeFather;
}

ProcessType createCadets(long amount) {
    for (int i = 0; i < amount; i++) {
        __pid_t pid = fork();
        if (!pid) {
            Cadet();
            return ProcessTypeChild;
        } else {
            spawnedProcesses.push_back(pid);
        }
    }

    return ProcessTypeFather;
}

ProcessType createSupervisor() {
    __pid_t pid = fork();
    if (!pid) {
        Supervisor s = Supervisor();
        return ProcessTypeChild;
    } else {
        spawnedProcesses.push_back(pid);
        supervisorPID = pid;
    }

    return ProcessTypeFather;
}
