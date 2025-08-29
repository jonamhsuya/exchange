#pragma once
#include <map>
#include <unordered_map>
#include <iostream>
#include "order.hpp"
#include "price_level.hpp"

class OrderBook {
public:
    map<double, PriceLevel, greater<double>> bidLadder;
    map<double, PriceLevel, less<double>> askLadder;
    unordered_map<int, list<Order>::iterator> orderIdMap;

    void addOrder(const Order& order);
    void removeOrder(int orderId);
    void matchOrders();
    void printBook();
};
