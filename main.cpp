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

ProcessType createReceptionists(long amount);

ProcessType createCadets(int amount);

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
    ProcessType resultReceptionist = createReceptionists(receptionistQuantity);
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
    ProcessType resulCadet = createCadets(cadetsQuantity);
    if (resulCadet == ProcessTypeChild)
        return 0;

    sleep(2);

    FifoEscritura fifo = FifoEscritura(Receptionist::getOrderFifoName());
    fifo.abrir();
    for (int i = 1; i < 5; i++) {
        std::string dato = "Orden ";
        dato = dato + to_string(i);
        dato.resize(MESSAGE_LENGTH);
        fifo.escribir(dato.c_str(), (int const) dato.size());
    }
    fifo.cerrar();

    //Esperar por los hijos
    for (int i = 0; i < receptionistQuantity + cookQuantity + cadetsQuantity + ovenQuantity; i++) {
        wait(NULL);
    }


    /*
     * Commemting this out fede, we need to use a different pipe channel for
     * every line on our old diagram.
    //Create oven processes
    ProcessType resulOven = createOvens(2, channel);
    if (resulOven == ProcessTypeChild) return 0;

    //Create Supervisor
    ProcessType resulSupervisor = createSupervisor(channel);
    if (resulSupervisor == ProcessTypeChild) return 0;

    sleep(4);
    std::string dato = "Hola mundo pipes!!";
    channel.escribir(dato.c_str(), (int const) dato.size());
     */
    Logger::logger().log("Exiting app");
    return 0;
}


ProcessType createReceptionists(long amount) {
    for (int i = 0; i < amount; i++) {
        if (!fork()) {
            Receptionist();
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

ProcessType createCadets(int amount) {
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
        Supervisor s = Supervisor(Pipe());
        return ProcessTypeChild;
    }

    return ProcessTypeFather;
}

list<Order> createOrders() {
    return {Order()};
}