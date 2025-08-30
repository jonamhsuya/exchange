#include <chrono>
#include <iostream>
#include <random>
#include <vector>

#include "matching_engine.hpp"

using namespace std;
using namespace chrono;

void stressTests();

int main() {
  MatchingEngine engine;

  cout << "===== Adding Orders =====\n";
  engine.submitOrder(1, 1, Side::BUY, 100, 10);
  engine.submitOrder(1, 2, Side::BUY, 101, 5);
  engine.submitOrder(2, 3, Side::SELL, 102, 7);
  engine.submitOrder(2, 4, Side::SELL, 99, 3);

  cout << "\n===== Order Book After Submissions =====\n";
  engine.book().printBook();

  cout << "\n===== Adding More Orders =====\n";
  engine.submitOrder(3, 5, Side::SELL, 101, 10);
  engine.submitOrder(1, 6, Side::BUY, 101, 7);

  cout << "\n===== Order Book After More Submissions =====\n";
  engine.book().printBook();

  cout << "\n===== Testing Cancellations =====\n";
  engine.cancelOrder(1, 1); // Cancel a remaining buy order
  engine.cancelOrder(2, 3); // Cancel a sell order

  cout << "\n===== Order Book After Cancellations =====\n";
  engine.book().printBook();

  cout << "\n";

  stressTests();
  return 0;
}

void stressTests() {
  MatchingEngine engine;

  const int numOrders = 1'000'000;
  vector<Order> orders;
  orders.reserve(numOrders);

  mt19937 rng(42);
  uniform_int_distribution<int> priceDist(1, 100'000); // cents
  uniform_int_distribution<int> sideDist(0, 1);
  uniform_int_distribution<int> qtyDist(1, 100'000);

  auto start = high_resolution_clock::now();

  for (int i = 0; i < numOrders; ++i) {
    Side side = static_cast<Side>(sideDist(rng));
    int priceTicks = priceDist(rng); // cents
    int qty = qtyDist(rng);

    engine.submitOrder(1, i + numOrders, side, priceTicks, qty);
  }

  auto end = high_resolution_clock::now();
  auto totalDuration = duration_cast<nanoseconds>(end - start).count();

  // engine.book().printBook();

  cout << "Total duration for " << numOrders << " orders: " << totalDuration
       << " ns\n";
  cout << "Average latency per order: "
       << totalDuration / static_cast<double>(numOrders) << " ns\n";
}
