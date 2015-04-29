#include <iostream>
#include <list>
#include <unistd.h>
#include "src/Util/Logger/Logger.h"
#include "src/Util/INIReader/INIReader.h"
#include "src/Domain/Order/Order.h"
#include "src/Domain/Receptionist/Receptionist.h"
#include "src/Domain/Cook/Cook.h"
#include "src/Util/Pipes/Pipe.h"
#include "src/Config/Config.h"
#include <sys/types.h>
#include <sys/wait.h>
#include "src/Domain/Oven/Oven.h"
#include "src/Domain/Cadet/Cadet.h"
#include "src/Domain/Supervisor/Supervisor.h"
#include "src/Util/Fifos/FifoEscritura.h"

typedef enum ProcessType {
    ProcessTypeFather,
    ProcessTypeChild
} ProcessType;


ProcessType createCooks(long amount);

ProcessType createReceptionists(long amount, int cookAmount);

ProcessType createCadets(long amount);

ProcessType createSupervisor(Pipe &orderChannel);

ProcessType createOvens(long amount) ;

using namespace std;

int main() {
    Logger::logger().log("Launching app...");

    INIReader reader(CONFIG_FILE);
    if (reader.ParseError() < 0) {
        std::cout << "Can't load 'config_file.ini'\n";
        return 1;
    }

    long receptionistQuantity = reader.GetInteger("parameters", "receptionists", -1);
    long cookQuantity = reader.GetInteger("parameters", "cooks", -1);
    long cadetsQuantity = reader.GetInteger("parameters", "cadets", -1);
    long ovenQuantity = reader.GetInteger("parameters", "ovens", -1);


    //Create receptionist processes
    ProcessType resultReceptionist = createReceptionists(receptionistQuantity, cookQuantity);
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
    for (int i = 0; i < receptionistQuantity + cookQuantity + cadetsQuantity + ovenQuantity; i++) {
        wait(NULL);
    }

    Logger::logger().log("Exiting app");
    return 0;
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

ProcessType createSupervisor(Pipe &orderChannel) {
    if (!fork()) {
        Supervisor s = Supervisor();
        return ProcessTypeChild;
    }

    return ProcessTypeFather;
}
