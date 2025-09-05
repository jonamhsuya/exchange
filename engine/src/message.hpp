#include "types.hpp"
#include <cstdint>

/**
 * Defines an interface used for receiving order information from clients.
 */
#pragma pack(push, 1)
struct OrderMessage {
  uint16_t length;   // Length of type + payload + checksum
  Side side;         // Whether the order is a buy or a sell
  uint8_t padding_a; // Padding
  Quantity quantity; // The fixed integer quantity of the order
  Price price; // The price given for the underlying asset in integer ticks
  uint8_t padding_b; // Padding
  uint8_t padding_c; // Padding
  uint8_t padding_d; // Padding
  uint8_t checksum;
};
#pragma pack(pop)

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
