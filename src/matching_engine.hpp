#pragma once
#include "order_book.hpp"
#include <atomic>
#include <functional>

class MatchingEngine {
public:
  // TODO
  // std::function<void(const Accept &)> onAccept;
  // std::function<void(const Trade &)> onTrade;
  // std::function<void(const Cancel &)> onCancel;
  // std::function<void(const Reject &)> onReject;

  MatchingEngine();

  void submitOrder(uint64_t clientId, uint64_t clientOrderId, Side side, Price price, Quantity quantity);
  bool cancelOrder(uint64_t clientId, uint64_t clientOrderId);

  const OrderBook &book() const { return orderBook_; }

private:
  OrderBook orderBook_;
  std::atomic<Sequence> sequence_;
  void match();
  Price getRestingPrice(const Order &a, const Order &b) const;
};
