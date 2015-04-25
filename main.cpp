#include <iostream>
#include <list>
#include <unistd.h>
#include "src/Util/Logger/Logger.h"
#include "src/Domain/Order/Order.h"
#include "src/Domain/Receptionist/Receptionist.h"
#include "src/Domain/Cook/Cook.h"

typedef enum ProcessType{
    ProcessTypeFather,
    ProcessTypeChild
};

list<Order> createOrders();

ProcessType createReceptionists(int amount);

ProcessType createCooks(int amount) ;

using namespace std;

int main() {
    Logger::logger().log("Launching app...");
    list<Order> orders = createOrders();

    //Create receptionist processes
    ProcessType resultReceptionist = createReceptionists(2);
    if (resultReceptionist == ProcessTypeChild) return 0;

    //Create cook processes
    ProcessType resultCook = createCooks(2);
    if (resultCook == ProcessTypeChild) return 0;

    Logger::logger().log("Exiting app");
    return 0;
}

ProcessType createReceptionists(int amount) {
    for (int i = 0; i < amount; i++) {
        int pid = fork();
        if (pid == 0) {
            Receptionist();
            return ProcessTypeChild;
        }
    }

    return ProcessTypeFather;
}

ProcessType createCooks(int amount) {
    for (int i = 0; i < amount; i++) {
        int pid = fork();
        if (pid == 0) {
            Cook();
            return ProcessTypeChild;
        }
    }

    return ProcessTypeFather;
}

list<Order> createOrders() {
    return {Order()};
}