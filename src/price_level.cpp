#include "price_level.hpp"

void PriceLevel::addOrder(const Order& order) {
    orders.push_back(order);
    totalQuantity += order.quantity;
}

void PriceLevel::removeOrder(list<Order>::iterator& it) {
    totalQuantity -= it->quantity;
    orders.erase(it);
}
