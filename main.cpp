#include <iostream>
#include <list>
#include <unistd.h>
#include "src/Util/Logger/Logger.h"
#include "src/Domain/Order/Order.h"
#include "src/Domain/Receptionist/Receptionist.h"

list<Order> createOrders();

int createReceptionists(int amount);

using namespace std;

int main() {
    Logger::logger().log("Launching app...");
    list<Order> orders = createOrders();

    int resultReceptionist = createReceptionists(2);
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