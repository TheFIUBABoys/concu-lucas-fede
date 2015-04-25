#include <iostream>
#include <list>
#include <unistd.h>
#include "src/Util/Logger/Logger.h"
#include "src/Util/INIReader/INIReader.h"
#include "src/Domain/Order/Order.h"
#include "src/Domain/Receptionist/Receptionist.h"
#include "src/Domain/Cook/Cook.h"
#include "src/Util/Pipes/Pipe.h"

typedef enum ProcessType {
    ProcessTypeFather,
    ProcessTypeChild
} ProcessType;

#define CONFIG_FILE	"/home/lucas/ClionProjects/concu-lucas-fede/config_file.cfg"

list<Order> createOrders();

ProcessType createReceptionists(int amount, Pipe &orderChannel);

ProcessType createCooks(int amount, Pipe &orderChannel);

using namespace std;

int main() {
    Pipe channel;
    Logger::logger().log("Launching app...");

    INIReader reader(CONFIG_FILE);

    if (reader.ParseError() < 0) {
        std::cout << "Can't load 'config_file.ini'\n";
        return 1;
    }

    long recepcionistsQuantity = reader.GetInteger("parameters", "recepcionists_quantity", -1);
    long cookersQuantity = reader.GetInteger("parameters", "cookers_quantity", -1);
    long cadetsQuantity = reader.GetInteger("parameters", "cadets_quantity", -1);
    long ovensQuantity = reader.GetInteger("parameters", "ovens_quantity", -1);

    list<Order> orders = createOrders();

    //Create receptionist processes
    ProcessType resultReceptionist = createReceptionists(2, channel);
    if (resultReceptionist == ProcessTypeChild) return 0;

    //Create cook processes
    ProcessType resultCook = createCooks(2, channel);
    if (resultCook == ProcessTypeChild) return 0;

    sleep(4);
    std::string dato = "Hola mundo pipes!!";
    channel.escribir(dato.c_str(), (int const) dato.size());
    Logger::logger().log("Exiting app");
    return 0;
}

ProcessType createReceptionists(int amount, Pipe &orderChannel) {
    for (int i = 0; i < amount; i++) {
        if (!fork()) {
            Receptionist r = Receptionist(orderChannel);
            return ProcessTypeChild;
        }
    }

    return ProcessTypeFather;
}

ProcessType createCooks(int amount, Pipe &orderChannel) {
    for (int i = 0; i < amount; i++) {
        if (!fork()) {
            Cook(Pipe());
            return ProcessTypeChild;
        }
    }

    return ProcessTypeFather;
}

list<Order> createOrders() {
    return {Order()};
}