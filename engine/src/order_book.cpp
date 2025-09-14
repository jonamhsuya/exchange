#include "order_book.hpp"
#include <iomanip>
#include <iostream>

std::list<Order>::iterator OrderBook::addOrder(Order &&order) {
  PriceLevel &level = (order.side == Side::BUY) ? bidLadder[order.price]
                                                : askLadder[order.price];
  level.addOrder(std::move(order));
  auto it = std::prev(level.orders.end());
  sequenceToIterator[order.sequence] = it;
  return it;
}

Order OrderBook::removeOrder(Sequence sequence) {
  auto &it = sequenceToIterator[sequence];
  Order order = *it;

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
  sequenceToIterator.erase(sequence);
  return order;
}

void OrderBook::printBook() const {
  constexpr int widthPrice = 12;
  constexpr int widthQty = 15;

  std::cout << "===== ORDER BOOK =====\n\n";

  std::cout << " BIDS (Buy Orders):\n";
  std::cout << std::left << std::setw(widthPrice) << "Price"
            << std::setw(widthQty) << "Total Quantity" << "\n";
  std::cout << std::string(widthPrice + widthQty, '-') << "\n";
  for (auto it = bidLadder.rbegin(); it != bidLadder.rend(); ++it) {
    std::cout << std::left << std::setw(widthPrice)
              << it->first / 100.0
              << std::setw(widthQty) << it->second.totalQuantity << "\n";
  }

  std::cout << "\n";

  std::cout << " ASKS (Sell Orders):\n";
  std::cout << std::left << std::setw(widthPrice) << "Price"
            << std::setw(widthQty) << "Total Quantity" << "\n";
  std::cout << std::string(widthPrice + widthQty, '-') << "\n";
  for (auto it = askLadder.begin(); it != askLadder.end(); ++it) {
    std::cout << std::left << std::setw(widthPrice)
              << it->first / 100.0
              << std::setw(widthQty) << it->second.totalQuantity << "\n";
  }

  std::cout << "\n======================\n";
}
