//
// Created by lucas on 4/22/15.
//

#include <unistd.h>
#include "Cook.h"
#include "../../Util/Logger/Logger.h"

Pizza Cook::cookOrder(Order order) {
    return Pizza();
}

//Create receptionist in new thread and start polling for orders
Cook::Cook(Pipe orderChannel) {
    Logger::logger().log("Cook waking up");
    processedOrdersChannel = orderChannel;
    startPollingForOrders();
    Logger::logger().log("Cook dying");
}

void Cook::startPollingForOrders() {

    char buffer[BUFFSIZE];
    int timeout = 0;
    while (true) {
        ssize_t bytesLeidos = processedOrdersChannel.leer(buffer, BUFFSIZE);
        std::string mensaje = buffer;
        mensaje.resize((unsigned long) bytesLeidos);
        if (bytesLeidos > 0) {
            Logger::logger().log(string("Me llego esta orden: ") + string(mensaje));
            string processedOrder = string("Preparé ") + string(mensaje);
            processedOrdersChannel.escribir(processedOrder.c_str(), (int const) processedOrder.size());
        } else {
            Logger::logger().log("Lei algo vacio");
            timeout++;
        }
        if (timeout > 10) break;
        sleep(3);
    }
    Logger::logger().log("Cook waking up");
}