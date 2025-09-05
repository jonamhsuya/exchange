#include "order_book.hpp"
#include <iostream>

std::list<Order>::iterator OrderBook::addOrder(const Order &order) {
  PriceLevel &level = (order.side == Side::BUY) ? bidLadder[order.price]
                                                : askLadder[order.price];
  level.addOrder(order);
  auto it = std::prev(level.orders.end());
  sequenceToIterator[order.sequence] = it;
  return it;
}

bool OrderBook::removeOrder(Sequence sequence) {
  auto f = sequenceToIterator.find(sequence);
  if (f == sequenceToIterator.end()) {
    return false;
  }

  auto &it = f->second;
  ClientOrderKey &client = it->clientOrderKey;
  double price = it->price;

  if (it->side == Side::BUY) {
    PriceLevel &level = bidLadder[price];
    level.removeOrder(it);
    if (level.orders.empty())
      bidLadder.erase(price);
  } else {
    PriceLevel &level = askLadder[price];
    level.removeOrder(it);
    if (level.orders.empty())
      askLadder.erase(price);
  }

  clientToSequence.erase(client);
  // TODO: Deliver event to client about cancellation of order
  sequenceToIterator.erase(sequence);
  return true;
}

void OrderBook::printBook() const {
  std::cout << "Bids:\n";
  for (auto it = bidLadder.rbegin(); it != bidLadder.rend(); ++it) {
    std::cout << it->first << " : " << it->second.totalQuantity << "\n";
  }

  std::cout << "\nAsks:\n";
  for (auto it = askLadder.begin(); it != askLadder.end(); ++it) {
    std::cout << it->first << " : " << it->second.totalQuantity << "\n";
  }
}
