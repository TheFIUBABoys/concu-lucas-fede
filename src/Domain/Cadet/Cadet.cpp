//
// Created by fede on 25/04/15.
//

#include "Cadet.h"
#include "../../Util/Logger/Logger.h"

//Create receptionist in new thread and start polling for orders
Cadet::Cadet(Pipe orderChannel) {
    Logger::logger().log("Cadet waking up");
}