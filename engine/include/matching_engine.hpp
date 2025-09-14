#pragma once
#include "order_book.hpp"
#include <atomic>
#include <functional>

class Server;

class MatchingEngine {
public:
  MatchingEngine() : sequence_(0) {}
  void setServer(Server *server) { server_ = server; }

  void submitOrder(uint32_t clientId, uint32_t clientOrderId, Side side,
                   Quantity quantity, Price price);
  bool cancelOrder(uint32_t clientId, uint32_t clientOrderId);

  const OrderBook &book() const { return orderBook_; }

private:
  OrderBook orderBook_;
  std::atomic<Sequence> sequence_;
  Server *server_ = nullptr;
  void match();
  Price getRestingPrice(const Order &a, const Order &b) const;
};
