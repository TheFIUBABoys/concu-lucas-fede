#include <iostream>
#include "src/Util/Logger/Logger.h"

using namespace std;

int main() {
    Logger::logger().log("Launching app...");

    Logger::logger().log("Exiting app");
    return 0;
}