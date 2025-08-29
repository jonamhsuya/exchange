#include "order.hpp"

atomic<uint64_t> sequenceGenerator{0};

Order::Order(int id_, Side side_, double price_, int quantity_)
    : id(id_), side(side_), price(price_), quantity(quantity_) {
    timestamp = chrono::system_clock::now();
    sequence = sequenceGenerator.fetch_add(1, memory_order_relaxed);
}

bool Order::operator<(const Order& other) const {
    if (price == other.price) {
        if (timestamp == other.timestamp) {
            return sequence < other.sequence;
        }
        return timestamp < other.timestamp;
    }
    return price < other.price;
}
