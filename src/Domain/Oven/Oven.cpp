//
// Created by lucas on 4/22/15.
//

#include <unistd.h>
#include "Oven.h"
#include "../../Util/Logger/Logger.h"
#include "../Cook/Cook.h"

Pizza Oven::cookOrder(Order order) {
    return Pizza();
}

Oven::Oven(Pipe orderChannel) {
    Logger::logger().log("Preheating oven");
}