#pragma once
#include <list>
#include "order.hpp"

class PriceLevel {
public:
    list<Order> orders;
    long totalQuantity = 0;

    void addOrder(const Order& order);
    void removeOrder(list<Order>::iterator& it);
};
