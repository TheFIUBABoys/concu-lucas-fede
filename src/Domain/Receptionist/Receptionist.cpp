//
// Created by lucas on 4/20/15.
//

#include <unistd.h>
#include "Receptionist.h"
#include "../../Util/Logger/Logger.h"


//Create receptionist in new thread and start polling for orders
Receptionist::Receptionist() {
    startPollingForOrders();
}

void Receptionist::startPollingForOrders() {
    int i = 0;
    while (true) {
        if (i > 10) break;
        Logger::logger().log("Polling for resources");
        i++;
    }
}

/*int main() {
    Receptionist();
    return 0;
}*/