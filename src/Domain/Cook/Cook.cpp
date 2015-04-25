//
// Created by lucas on 4/22/15.
//

#include <unistd.h>
#include "Cook.h"
#include "../../Util/Logger/Logger.h"

void startPollingForOrders();

Pizza Cook::cookOrder(Order order) {
    return Pizza();
}

//Create receptionist in new thread and start polling for orders
Cook::Cook(Pipe orderChannel) {
    Logger::logger().log("Cook waking up");
    startPollingForOrders();
    Logger::logger().log("Cook dying");
}

void startPollingForOrders() {
    sleep(5);
}