//
// Created by lucas on 4/22/15.
//

#ifndef CONCUTP_OVEN_H
#define CONCUTP_OVEN_H


#include "../Pizza/Pizza.h"
#include "../Order/Order.h"
#include "../../Util/Pipes/Pipe.h"

class Oven {
public:
    Oven(Pipe orderChannel);
    Pizza cookOrder(Order order);
};


#endif //CONCUTP_OVEN_H
