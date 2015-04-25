//
// Created by lucas on 4/20/15.
//

#ifndef CONCUTP_RECEPTIONIST_H
#define CONCUTP_RECEPTIONIST_H


#include "../../Util/Pipes/Pipe.h"

class Receptionist {

public:
    Receptionist(Pipe& orderChannel);
    bool shouldReturn;
private:
    int p_id;
    Receptionist() { }
    void startPollingForOrders();
};


#endif //CONCUTP_RECEPTIONIST_H
