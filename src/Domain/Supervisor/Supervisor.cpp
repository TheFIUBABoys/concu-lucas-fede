//
// Created by fede on 25/04/15.
//

#include "Supervisor.h"
#include "../../Util/Logger/Logger.h"

//Create receptionist in new thread and start polling for orders
Supervisor::Supervisor() {
    Logger::logger().log("Supervisor waking up");
}