//
// Created by lucas on 4/20/15.
//

#ifndef CONCUTP_RECEPTIONIST_H
#define CONCUTP_RECEPTIONIST_H


class Receptionist {

public:
    Receptionist();
    bool shouldReturn;
private:
    int p_id;

    void startPollingForOrders();
};


#endif //CONCUTP_RECEPTIONIST_H
