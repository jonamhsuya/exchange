#include "order_book.hpp"

void OrderBook::addOrder(const Order& order) {
    if (order.side == BUY) {
        bidLadder[order.price].addOrder(order);
        orderIdMap[order.id] = prev(bidLadder[order.price].orders.end());
    } else {
        askLadder[order.price].addOrder(order);
        orderIdMap[order.id] = prev(askLadder[order.price].orders.end());
    }
}

void OrderBook::removeOrder(int orderId) {
    auto it = orderIdMap.find(orderId);
    if (it == orderIdMap.end()) return;

    auto& listIt = it->second;
    double price = listIt->price;
    if (listIt->side == BUY) {
        PriceLevel& level = bidLadder[price];
        level.removeOrder(listIt);
        if (level.orders.empty()) bidLadder.erase(price);
    } else {
        PriceLevel& level = askLadder[price];
        level.removeOrder(listIt);
        if (level.orders.empty()) askLadder.erase(price);
    }
    orderIdMap.erase(orderId);
}

const Order& getRestingOrder(const Order& a, const Order& b) {
    if (a.timestamp == b.timestamp) {
        if (a.sequence < b.sequence) {
            return a;
        }
        return b;
    } else if (a.timestamp < b.timestamp) {
        return a;
    }
    return b;
}


void OrderBook::matchOrders() {
    while (!bidLadder.empty() && !askLadder.empty() &&
           bidLadder.begin()->first >= askLadder.begin()->first) {
        PriceLevel& bidLevel = bidLadder.begin()->second;
        PriceLevel& askLevel = askLadder.begin()->second;
        Order& bestBid = bidLevel.orders.front();
        Order& bestAsk = askLevel.orders.front();

        long executionSize = min(bestBid.quantity, bestAsk.quantity);
        double executionPrice = getRestingOrder(bestBid, bestAsk).price;

        bestBid.quantity -= executionSize;
        bestAsk.quantity -= executionSize;
        bidLevel.totalQuantity -= executionSize;
        askLevel.totalQuantity -= executionSize;

        // cout << "Trade: " << executionSize << " @ " << executionPrice
        //      << " (Bid " << bestBid.id << " / Ask " << bestAsk.id << ")\n";

        if (bestBid.quantity == 0) removeOrder(bestBid.id);
        if (bestAsk.quantity == 0) removeOrder(bestAsk.id);
    }
}

void OrderBook::printBook() {
    cout << "Bids:\n";
    for (auto& [price, level] : bidLadder) {
        cout << price << ": ";
        for (Order& order : level.orders) cout << order.quantity << " ";
        cout << "\n";
    }
    cout << "\nAsks:\n";
    for (auto& [price, level] : askLadder) {
        cout << price << ": ";
        for (Order& order : level.orders) cout << order.quantity << " ";
        cout << "\n";
    }
}
