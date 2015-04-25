//
// Created by lucas on 4/20/15.
//

#include "Receptionist.h"
#include "../../Util/Logger/Logger.h"


//Create receptionist in new thread and start polling for orders
Receptionist::Receptionist(Pipe &theOrderChannel, Pipe &theProcessedOrdersChannel) {
    Logger::logger().log("Receptionist waking up");
    orderChannel = theOrderChannel;
    processedOrdersChannel = theProcessedOrdersChannel;
    startPollingForOrders();
    Logger::logger().log("Receptionist dying");
}

void Receptionist::startPollingForOrders() {
    char buffer[BUFFSIZE];
    int timeout = 0;
    while (true) {
        ssize_t bytesLeidos = orderChannel.leer(buffer, BUFFSIZE);
        std::string mensaje = buffer;
        mensaje.resize((unsigned long) bytesLeidos);
        if (bytesLeidos > 0) {
            Logger::logger().log(string("Lei esto: ") + string(mensaje));
            sleep(3); //Procesar
            string processedOrder = string("Procesada ") + string(mensaje);
            processedOrdersChannel.escribir(processedOrder.c_str(), (int const) processedOrder.size());
        } else {
            Logger::logger().log("Lei algo vacio");
            timeout++;
        }
        if (timeout > 10) break;
        sleep(3);
    }

}
