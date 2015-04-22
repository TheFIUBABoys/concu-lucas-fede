#include <iostream>
#include <list>
#include <unistd.h>
#include "src/Util/Logger/Logger.h"
#include "src/Domain/Order/Order.h"
#include "src/Domain/Receptionist/Receptionist.h"

list<Order> createOrders();

list<int> createReceptionists(int ammount);

using namespace std;

int main() {
    Logger::logger().log("Launching app...");
    list<Order> orders = createOrders();
    list<int> receptionists = createReceptionists(1);
    Logger::logger().log("Exiting app");
    return 0;
}

list<int> createReceptionists(int ammount) {
    list<int> pidList = list<int>();
    for (int i = 0; i<ammount; i++){
        int pid = fork();
        if (pid==0){
            //Break the loop in the children
            return NULL;
        }else{
            pidList.push_back(pid);
        }
    }
    return pidList;

}

list<Order> createOrders() {
    return {Order()};
}