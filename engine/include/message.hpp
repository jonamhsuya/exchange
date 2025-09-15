#include "types.hpp"
#include <cstdint>

/**
 * Defines an interface used for receiving order information from clients.
 */
#pragma pack(push, 1)
struct OrderMessage {
  OrderId clientId;      // The ID of the client that sent the order
  OrderId clientOrderId; // The client-assigned ID for the order
  Quantity quantity;     // The fixed integer quantity of the order
  Price price;      // The price given for the underlying asset in integer ticks
  Side side;        // Whether the order is a buy or a sell
  uint8_t checksum; // A simple checksum for validating message integrity
};

struct CancelMessage {
  OrderId clientId;      // The ID of the client that sent the order
  OrderId clientOrderId; // The client-assigned ID for the order
  uint8_t checksum;      // A simple checksum for validating message integrity
};

/**
 * Denotes the event type of a particular event message.
 */
enum class EventType : uint8_t { ID_ASSIGNMENT, ACCEPT, CANCEL, TRADE };

/**
 * Defines an interface used for notifying clients of their assigned client IDs.
 */
struct IdAssignment {
  uint32_t clientId; // The exchange-assigned client ID
  EventType event;   // The event type of this event (ID_ASSIGNMENT)
  uint8_t checksum;  // A simple checksum for validating message integrity
};

/**
 * Defines an interface used for creating and processing order status
 * messages.
 */
struct Status {
  OrderId clientOrderId; // The client-assigned ID for the order
  EventType event;       // The event type of this event
  uint8_t checksum;      // A simple checksum for validating message integrity
};

/**
 * Defines an interface used for creating and processing trade events.
 */
struct Trade {
  OrderId clientOrderId; // The client-assigned ID for the order
  Quantity quantity;     // The quantity of assets exchanged in this trade
  Price price;     // The price at which the trade was executed in integer ticks
  EventType event; // The event type of this event (TRADE)
  uint8_t checksum; // A simple checksum for validating message integrity
};

#pragma pack(pop)
