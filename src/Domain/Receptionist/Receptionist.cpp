//
// Created by lucas on 4/20/15.
//

#include "Receptionist.h"
#include "../../Util/Logger/Logger.h"
#include "../../Util/Seniales/SignalHandler.h"


#define PROCESSING_DELAY 4


Receptionist::Receptionist(int cookAmount) : Process() {
    Logger::logger().log("Receptionist waking up");
    initWithCookAmount(cookAmount);
    startPollingForOrders();
    Logger::logger().log("Receptionist dying");
}

void Receptionist::initWithCookAmount(int cookAmount) {
    Receptionist::cookAmount = cookAmount;
    orderChannel.abrir();
    if (processedOrderAmount.crear(LOCKFILE_HANDLED_ORDERS, 'L') != SHM_OK) {
        string error = "Error creando memoria compartida de cant. pedidos procesados";
        perror(error.c_str());
        Logger::logger().log(error);
    }
    processedOrderChannel.abrir();
}

void Receptionist::startPollingForOrders() {
    char buffer[MESSAGE_LENGTH];
    while (!sigint_handler.getGracefulQuit()) {
        int uncookedOrderAmount = processedOrderAmount.leer();
        ssize_t readBytes = orderChannel.leer(buffer, MESSAGE_LENGTH);
        Logger::logger().log("Hay " + to_string(cookAmount) + " cocineras y " + to_string(uncookedOrderAmount) +
                             " pedidos sin procesar");
        while (uncookedOrderAmount > 2 * cookAmount && readBytes == MESSAGE_LENGTH) {
            string order = buffer;
            order.resize(MESSAGE_LENGTH);
            Logger::logger().log("Recepcionista ignora " + order);
            uncookedOrderAmount = processedOrderAmount.leer();
            //Ignore orders
            readBytes = orderChannel.leer(buffer, MESSAGE_LENGTH);
        }
        if (readBytes == MESSAGE_LENGTH) {
            std::string orderStr = buffer;
            orderStr.resize(MESSAGE_LENGTH);
            Logger::logger().log(string("Recepcionista recibe ") + string(orderStr));
            processOrder(orderStr);
        } else {
            Logger::logger().log("Recepcionista lee EOF");
            break;
        }
    }
    //Graceful quit
    Logger::logger().log("Graceful quit Recepcionista");
    processedOrderChannel.cerrar();
    orderChannel.cerrar();
    processedOrderAmount.liberar();
    SignalHandler::destruir();

}

void Receptionist::processOrder(string &orderStr) {
    string processedOrder = string("Recepcionista procesa ") + orderStr;
    sleep(PROCESSING_DELAY);
    Logger::logger().log(processedOrder);
    processedOrder.resize(MESSAGE_LENGTH);
    if (!sigint_handler.getGracefulQuit()) {
        if (int written = processedOrderChannel.escribir(orderStr.c_str(), MESSAGE_LENGTH) != MESSAGE_LENGTH) {
            Logger::logger().log(string("Error al escribir procesada") + to_string(written));
            perror("Proccessed pipe");
        } else {
            int takenOrders = processedOrderAmount.leer();
            Logger::logger().log("Cant pedidos tomados: " + to_string(takenOrders));
            processedOrderAmountLock.tomarLockWr();
            processedOrderAmount.escribir(takenOrders + 1);
            processedOrderAmountLock.liberarLock();
        }
    }
}

std::string Receptionist::getOrderFifoName() {
    return "OrderFifo";
}

std::string Receptionist::getProcessedOrderFifoName() {
    return "ProcessedOrderFifo";
}
