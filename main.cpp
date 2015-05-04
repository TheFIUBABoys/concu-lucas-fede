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

using namespace std;

int main() {
    remove(LOG_FILE);


    INIReader reader(CONFIG_FILE);
    if (reader.ParseError() < 0) {
        std::cout << "Can't load 'config_file.cfg'\n";
        return 1;
    }

    long receptionistQuantity = reader.GetInteger("parameters", "receptionists", -1);
    long cookQuantity = reader.GetInteger("parameters", "cooks", -1);
    long cadetsQuantity = reader.GetInteger("parameters", "cadets", -1);
    long ovenQuantity = reader.GetInteger("parameters", "ovens", -1);
    long debug = reader.GetInteger("parameters", "debug", -1);

    if (debug != 0){
        Logger::logger().debug = true;
    }

    Logger::logger().log("Launching app...");
    createTempLockfiles();
    //Create receptionist processes
    ProcessType resultReceptionist = createReceptionists(receptionistQuantity, (int) cookQuantity);
    if (resultReceptionist == ProcessTypeChild) return 0;

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
        sleep(1);
        fifo.escribir(dato.c_str(), (int const) dato.size());
    }
    fifo.cerrar();

    //Esperar por los hijos
    for (int i = 0; i < receptionistQuantity + cookQuantity + cadetsQuantity + ovenQuantity + 1; i++) {
        wait(NULL);
    }

    deleteTempLockfiles();
    Logger::logger().log("Exiting app");
    return 0;
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
        if (!fork()) {
            Receptionist r = Receptionist(cookAmount);
            return ProcessTypeChild;
        }
    }

    return ProcessTypeFather;
}

ProcessType createCooks(long amount) {
    for (int i = 0; i < amount; i++) {
        if (!fork()) {
            Cook();
            return ProcessTypeChild;
        }
    }

    return ProcessTypeFather;
}


ProcessType createOvens(long amount) {
    for (int i = 0; i < amount; i++) {
        if (!fork()) {
            Oven();
            return ProcessTypeChild;
        }
    }

    return ProcessTypeFather;
}

ProcessType createCadets(long amount) {
    for (int i = 0; i < amount; i++) {
        if (!fork()) {
            Cadet();
            return ProcessTypeChild;
        }
    }

    return ProcessTypeFather;
}

ProcessType createSupervisor() {
    if (!fork()) {
        Supervisor s = Supervisor();
        return ProcessTypeChild;
    }

    return ProcessTypeFather;
}
