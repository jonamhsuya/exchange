#include "matching_engine.hpp"
#include "network/server.hpp"

void MatchingEngine::submitOrder(uint32_t clientId, uint32_t clientOrderId,
                                 Side side, Quantity quantity, Price price) {
  ClientOrderKey clientOrderKey = ClientOrderKey(clientId, clientOrderId);
  sequence_.fetch_add(1, std::memory_order_relaxed);
  Order order = Order(sequence_, clientOrderKey, side, quantity, price);
  orderBook_.clientToSequence[clientOrderKey] = sequence_;
  orderBook_.addOrder(std::move(order));
  server_->onAccept(order);
  if ((order.side == Side::BUY && order.price >= orderBook_.getBestAsk()) ||
      (order.side == Side::SELL && order.price <= orderBook_.getBestBid())) {
    // A bid crossed the best ask, or vice versa; match the orders
    match();
  }
}

bool MatchingEngine::cancelOrder(uint32_t clientId, uint32_t clientOrderId) {
  ClientOrderKey clientOrderKey = ClientOrderKey(clientId, clientOrderId);
  auto it = orderBook_.clientToSequence.find(clientOrderKey);
  if (it == orderBook_.clientToSequence.end()) {
    return false;
  }
  Order order = orderBook_.removeOrder(it->second);
  server_->onCancel(order);
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

    // Construct new orders to represent the executed trades
    Order finalBestBid(bestBid.sequence, bestBid.clientOrderKey, bestBid.side,
                       executionSize, executionPrice);
    Order finalBestAsk(bestAsk.sequence, bestAsk.clientOrderKey, bestAsk.side,
                       executionSize, executionPrice);

    if (bestBid.quantity == 0) {
      orderBook_.removeOrder(bestBid.sequence);
    }
    if (bestAsk.quantity == 0) {
      orderBook_.removeOrder(bestAsk.sequence);
    }

    std::cout << finalBestBid.clientOrderKey.clientOrderId << " and "
              << finalBestAsk.clientOrderKey.clientOrderId << " traded "
              << executionSize << " at " << executionPrice << "\n";

    server_->onTrade(finalBestBid);
    server_->onTrade(finalBestAsk);
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
