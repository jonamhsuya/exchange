#pragma once
#include <atomic>
#include <chrono>
using namespace std;

enum Side { BUY, SELL };

class Order {
public:
    int id;
    Side side;
    double price;
    int quantity;
    chrono::system_clock::time_point timestamp;
    uint64_t sequence;

    Order(int id_, Side side_, double price_, int quantity_);
    bool operator<(const Order& other) const;
};
