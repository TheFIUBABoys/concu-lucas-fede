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

typedef enum ProcessType {
    ProcessTypeFather,
    ProcessTypeChild
} ProcessType;


list<Order> createOrders();


ProcessType createCooks(int amount, Pipe &processedOrdersChannel);

ProcessType createReceptionists(int amount, Pipe &orderChannel, Pipe &processedOrdersChannel);

void sendOrder(Pipe &orderChannel, string &dato);

using namespace std;

int main() {
    Pipe orderChannel;
    Pipe processedOrdersChannel;
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
    ProcessType resultReceptionist = createReceptionists(1, orderChannel, processedOrdersChannel);
    if (resultReceptionist == ProcessTypeChild) return 0;

    //Create cook processes
    ProcessType resultCook = createCooks(1, processedOrdersChannel);
    if (resultCook == ProcessTypeChild) return 0;

    sleep(2);
    std::string dato = "Orden 1";
    sendOrder(orderChannel, dato);
    dato = "Orden 2";
    sendOrder(orderChannel, dato);
    orderChannel.cerrar();
    processedOrdersChannel.cerrar();
    Logger::logger().log("Exiting app");
    return 0;
}

void sendOrder(Pipe &orderChannel, string &dato) {
    dato.resize(MESSAGE_LENGTH);
    orderChannel.escribir(dato.c_str(), (int const) dato.size());
}

ProcessType createReceptionists(int amount, Pipe &orderChannel, Pipe &processedOrdersChannel) {
    for (int i = 0; i < amount; i++) {
        if (!fork()) {
            Receptionist r = Receptionist(orderChannel, processedOrdersChannel);
            return ProcessTypeChild;
        }
    }

    return ProcessTypeFather;
}

ProcessType createCooks(int amount, Pipe &processedOrdersChannel) {
    for (int i = 0; i < amount; i++) {
        if (!fork()) {
            Cook c = Cook(processedOrdersChannel);
            return ProcessTypeChild;
        }
    }

    return ProcessTypeFather;
}

list<Order> createOrders() {
    return {Order()};
}