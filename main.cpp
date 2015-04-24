#include <iostream>
#include <list>
#include <unistd.h>
#include "src/Util/Logger/Logger.h"
#include "src/Domain/Order/Order.h"
#include "src/Domain/Receptionist/Receptionist.h"

list<Order> createOrders();

void createReceptionists(int index, int amount);

using namespace std;

int main() {
    Logger::logger().log("Launching app...");
    list<Order> orders = createOrders();
    createReceptionists(0, 2);
    Logger::logger().log("Exiting app");
    return 0;
}

void createReceptionists(int index, int amount) {
    if (index < amount) {
        int pid = fork();
        if (pid == 0) {
            Receptionist();
        } else {
            //Father
            createReceptionists(index + 1, amount);
        }
    }
}

list<Order> createOrders() {
    return {Order()};
}