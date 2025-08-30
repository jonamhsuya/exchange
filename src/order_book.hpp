#pragma once
#include "types.hpp"
#include <list>
#include <map>
#include <unordered_map>

struct PriceLevel {
  std::list<Order> orders;
  Quantity totalQuantity{0};

  void addOrder(const Order &order) {
    orders.push_back(order);
    totalQuantity += order.quantity;
  }

  void removeOrder(std::list<Order>::iterator &it) {
    totalQuantity -= it->quantity;
    orders.erase(it);
  }

  bool empty() const { return orders.empty(); }
  Order &front() { return orders.front(); }
};

struct OrderBook {
  std::map<Price, PriceLevel, std::greater<Price>> bidLadder;
  std::map<Price, PriceLevel, std::less<Price>> askLadder;
  std::unordered_map<ClientOrderKey, Sequence, ClientOrderKeyHash> clientToSequence;
  std::unordered_map<Sequence, std::list<Order>::iterator> sequenceToIterator;

  std::list<Order>::iterator addOrder(const Order &order);
  bool removeOrder(Sequence orderId);
  void printBook() const;

  Price getBestBid() const {
    return bidLadder.empty() ? Price{} : bidLadder.begin()->first;
  }
  Price getBestAsk() const {
    return askLadder.empty() ? Price{} : askLadder.begin()->first;
  }
  PriceLevel &getBestBidLevel() { return bidLadder.begin()->second; }
  PriceLevel &getBestAskLevel() { return askLadder.begin()->second; }
};
