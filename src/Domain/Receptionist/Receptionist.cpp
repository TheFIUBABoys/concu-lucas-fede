//
// Created by lucas on 4/20/15.
//

#include "Receptionist.h"
#include "../../Util/Logger/Logger.h"


//Create receptionist in new thread and start polling for orders
Receptionist::Receptionist(Pipe& orderChannel) {
    Logger::logger().log("Receptionist waking up");
    char buffer[BUFFSIZE];
    ssize_t bytesLeidos = orderChannel.leer (buffer, BUFFSIZE);
    std::string mensaje = buffer;
    mensaje.resize ((unsigned long) bytesLeidos);
    Logger::logger().log(string("Lei")+ string(mensaje));
    Logger::logger().log("Receptionist dying");
}

void Receptionist::startPollingForOrders() {
    int i = 0;
    while (true) {
        if (i > 10) break;
        Logger::logger().log("Waiting for orders ");
        i++;
    }

}
