#pragma once
#include "types.hpp"
#include <chrono>
#include <functional>
#include <iostream>

struct ClientOrderKey {
  uint32_t clientId;      // The ID of the client that sent the order
  uint32_t clientOrderId; // The client-assigned ID for the order

  ClientOrderKey() = default;

  ClientOrderKey(uint32_t clientId_, uint32_t clientOrderId_)
      : clientId(clientId_), clientOrderId(clientOrderId_) {}

  bool operator==(const ClientOrderKey &other) const noexcept {
    return clientId == other.clientId && clientOrderId == other.clientOrderId;
  }
};

struct ClientOrderKeyHash {
  std::size_t operator()(const ClientOrderKey &key) const noexcept {
    std::size_t h1 = std::hash<uint32_t>{}(key.clientId);
    std::size_t h2 = std::hash<uint32_t>{}(key.clientOrderId);
    std::size_t seed = h1;
    seed ^= h2 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    return seed;
  }
};

/**
 * Defines an interface used for creating and processing orders and order data.
 */
struct Order {
  Sequence sequence; // The exchange-assigned sequence number used to maintain
                     // ordering among orders
  ClientOrderKey clientOrderKey; // Contains the client ID and client order ID
  Side side;                     // Whether the order is a buy or a sell
  Quantity quantity;             // The fixed integer quantity of the order
  Price price; // The price given for the underlying asset in integer ticks
  std::chrono::steady_clock::time_point
      timestamp; // The exact timestamp when the order was received

  Order(Sequence sequence_, const ClientOrderKey &clientOrderKey_, Side side_,
        Quantity quantity_, Price price_)
      : sequence(sequence_), clientOrderKey(clientOrderKey_), side(side_),
        quantity(quantity_), price(price_),
        timestamp(std::chrono::steady_clock::now()) {}
};
