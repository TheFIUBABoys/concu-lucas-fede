#include <iostream>
#include <list>
#include <unistd.h>
#include "src/Util/Logger/Logger.h"
#include "src/Util/INIReader/INIReader.h"
#include "src/Domain/Order/Order.h"
#include "src/Domain/Receptionist/Receptionist.h"

#define CONFIG_FILE			 "/home/fede/ClionProjects/concu-lucas-fede/config_file.cfg"

list<Order> createOrders();

int createReceptionists(int amount);

using namespace std;

int main() {
    Logger::logger().log("Launching app...");

    INIReader reader(CONFIG_FILE);

    if (reader.ParseError() < 0) {
        std::cout << "Can't load 'config_file.ini'\n";
        return 1;
    }

    int recepcionistsQuantity = reader.GetInteger("parameters", "recepcionists_quantity", -1);
    int cookersQuantity = reader.GetInteger("parameters", "cookers_quantity", -1);
    int cadetsQuantity = reader.GetInteger("parameters", "cadets_quantity", -1);
    int ovensQuantity = reader.GetInteger("parameters", "ovens_quantity", -1);

    list<Order> orders = createOrders();

    int resultReceptionist = createReceptionists(recepcionistsQuantity);
    if (resultReceptionist == 0) return 0;

    Logger::logger().log("Exiting app");
    return 0;
}

int createReceptionists(int amount) {
    for (int i = 0; i < amount; i++) {
        int pid = fork();
        if (pid == 0) {
            Receptionist();
            return 0;
        }
    }

    return 1;
}

list<Order> createOrders() {
    return {Order()};
}
