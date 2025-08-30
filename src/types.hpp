#pragma once
#include <chrono>
#include <cstdint>

/**
 * Denotes whether a particular order is a buy or a sell.
 */
enum class Side { BUY, SELL };

/**
 * Represents the unique, exchange-assigned sequence number for a particular
 * order.
 */
using Sequence = int64_t;

/**
 * Represents the price of an order's underlying asset in integer ticks (e.g.
 * cents).
 */
using Price = int32_t;

/**
 * Represents the fixed integer quantity of a particular order.
 */
using Quantity = int32_t;

struct ClientOrderKey {
  uint64_t clientId;
  uint64_t clientOrderId;

  ClientOrderKey() = default;

  ClientOrderKey(uint64_t clientId_, uint64_t clientOrderId_)
      : clientId(clientId_), clientOrderId(clientOrderId_) {}

  bool operator==(const ClientOrderKey &other) const noexcept {
    return clientId == other.clientId && clientOrderId == other.clientOrderId;
  }
};

struct ClientOrderKeyHash {
  std::size_t operator()(const ClientOrderKey &key) const noexcept {
    std::size_t h1 = std::hash<uint64_t>{}(key.clientId);
    std::size_t h2 = std::hash<uint64_t>{}(key.clientOrderId);
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
  Price price; // The price given for the underlying asset in integer ticks
  Quantity quantity; // The fixed integer quantity of the order
  std::chrono::steady_clock::time_point
      timestamp; // The exact timestamp when the order was received

  Order(Sequence sequence_, ClientOrderKey &clientOrderKey_, Side side_,
        Price price_, Quantity quantity_)
      : sequence(sequence_), clientOrderKey(clientOrderKey_), side(side_),
        price(price_), quantity(quantity_),
        timestamp(std::chrono::steady_clock::now()) {}
};

/**
 * Defines an interface used for creating and processing trade events.
 */
struct Trade {
  Sequence buySequence;  // The sequence number of the buy order in this trade
  Sequence sellSequence; // The sequence number of the sell order in this trade
  Price price; // The price at which the trade was executed in integer ticks
  Quantity quantity; // The quantity of assets exchanged in this trade
};

/**
 * Defines an interface used for creating and processing accept events.
 */
struct Accept {
  Sequence sequence; // The sequence number of the order that was fulfilled
  Price price; // The price at which the order was executed in integer ticks
};

/**
 * Defines an interface used for creating and processing cancel events.
 */
struct Cancel {
  Sequence sequence;    // The sequence number of the order that was canceled
  Quantity canceledQty; // The remaining quantity present in the order before
                        // cancellation
};

/**
 * Defines an interface used for creating and processing reject events.
 */
struct Reject {
  Sequence sequence;  // The sequence number of the order that was rejected
  const char *reason; // An error message describing why the order was rejected
};
