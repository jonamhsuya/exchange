#include <chrono>
#include <iostream>
#include <vector>
#include <random>

#include "order_book.hpp"

using namespace std;
using namespace chrono;

int main() {
    OrderBook ob;

    const int numOrders = 1'000'000;
    vector<Order> orders;
    orders.reserve(numOrders);

    // Random generator for price and side
    mt19937 rng(42);
    uniform_real_distribution<double> priceDist(90.0, 110.0);
    uniform_int_distribution<int> sideDist(0, 1);
    uniform_int_distribution<int> qtyDist(1, 100);

    // Generate random orders
    for (int i = 0; i < numOrders; ++i) {
        Side side = static_cast<Side>(sideDist(rng));
        double price = priceDist(rng);
        int qty = qtyDist(rng);
        orders.emplace_back(Order(i, side, price, qty));
    }

    auto start = high_resolution_clock::now();

    // Add all orders
    for (auto& order : orders) {
        ob.addOrder(order);
    }

    // Optionally run matching
    ob.matchOrders();

    auto end = high_resolution_clock::now();
    auto totalDuration = duration_cast<nanoseconds>(end - start).count();

    cout << "Total duration for " << numOrders << " orders: " 
         << totalDuration << " ns\n";
    cout << "Average latency per order: " 
         << totalDuration / static_cast<double>(numOrders) << " ns\n";

    return 0;
}
