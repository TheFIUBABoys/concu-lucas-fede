//
// Created by lucas on 4/22/15.
//

#include <unistd.h>
#include "Cook.h"
#include "../../Util/Logger/Logger.h"
#include "../../Config/Config.h"

//Create receptionist in new thread and start polling for orders
Cook::Cook(Pipe orderChannel) {
    Logger::logger().log("Cook waking up");
    processedOrdersChannel = orderChannel;
    startPollingForOrders();
    Logger::logger().log("Cook dying");
}

void Cook::startPollingForOrders() {

    char buffer[MESSAGE_LENGTH];
    int timeout = 0;
    while (true) {
        ssize_t bytesLeidos = processedOrdersChannel.leer(buffer, MESSAGE_LENGTH);
        std::string orderStr = buffer;
        orderStr.resize(MESSAGE_LENGTH);
        if (bytesLeidos > 0) {
            cookOrder(orderStr);
        } else {
            Logger::logger().log("Lei algo vacio");
            timeout++;
        }
        sleep(3);
        if (timeout > 3) break;
    }
}

void Cook::cookOrder(string &orderStr) {
    Logger::logger().log(string("Me llego esta orden: ") + string(orderStr));
    string processedOrder = string("Preparé ") + string(orderStr);
    processedOrdersChannel.escribir(processedOrder.c_str(), (int const) processedOrder.size());
}