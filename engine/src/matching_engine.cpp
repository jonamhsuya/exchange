#include "matching_engine.hpp"

void MatchingEngine::submitOrder(uint64_t clientId, uint64_t clientOrderId,
                                 Side side, Price price, Quantity quantity) {
  ClientOrderKey clientOrderKey = ClientOrderKey(clientId, clientOrderId);
  sequence_.fetch_add(1, std::memory_order_relaxed);
  Order order = Order(sequence_, clientOrderKey, side, price, quantity);
  orderBook_.clientToSequence[clientOrderKey] = sequence_;
  orderBook_.addOrder(order);
  if ((order.side == Side::BUY && order.price >= orderBook_.getBestAsk()) ||
      (order.side == Side::SELL && order.price <= orderBook_.getBestBid())) {
    match();
  }
}

bool MatchingEngine::cancelOrder(uint64_t clientId, uint64_t clientOrderId) {
  ClientOrderKey clientOrderKey = ClientOrderKey(clientId, clientOrderId);
  auto it = orderBook_.clientToSequence.find(clientOrderKey);
  if (it == orderBook_.clientToSequence.end()) {
    return false;
  }
  orderBook_.removeOrder(it->second);
  return true;
}

void MatchingEngine::match() {
  auto &bidLadder = orderBook_.bidLadder;
  auto &askLadder = orderBook_.askLadder;

  while (!bidLadder.empty() && !askLadder.empty() &&
         orderBook_.getBestBid() >= orderBook_.getBestAsk()) {
    PriceLevel &bidLevel = orderBook_.getBestBidLevel();
    PriceLevel &askLevel = orderBook_.getBestAskLevel();
    Order &bestBid = bidLevel.orders.front();
    Order &bestAsk = askLevel.orders.front();

    long executionSize = std::min(bestBid.quantity, bestAsk.quantity);
    Price executionPrice = getRestingPrice(bestBid, bestAsk);

    bestBid.quantity -= executionSize;
    bestAsk.quantity -= executionSize;
    bidLevel.totalQuantity -= executionSize;
    askLevel.totalQuantity -= executionSize;

    if (bestBid.quantity == 0) {
      orderBook_.removeOrder(bestBid.sequence);
    }
    if (bestAsk.quantity == 0) {
      orderBook_.removeOrder(bestAsk.sequence);
    }
  }
}

Price MatchingEngine::getRestingPrice(const Order &a, const Order &b) const {
  if (a.timestamp == b.timestamp) {
    if (a.sequence < b.sequence) {
      return a.price;
    }
    return b.price;
  } else if (a.timestamp < b.timestamp) {
    return a.price;
  }
  return b.price;
}
