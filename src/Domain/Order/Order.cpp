//
// Created by lucas on 4/20/15.
//

#include "Order.h"

vector<Order> Order::createOrders(int numberOfOrders) {
    vector<Order> orderArray = vector<Order>();
    for (int i = 0; i < numberOfOrders; i++) {
        orderArray.push_back(Order());
    }
    return orderArray;
}
