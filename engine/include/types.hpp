#pragma once
#include <cstdint>

/**
 * Denotes whether a particular order is a buy or a sell.
 */
enum class Side : uint8_t { BUY, SELL };

/**
 * Represents the unique, exchange-assigned sequence number for a particular
 * order.
 */
using Sequence = uint64_t;

/**
 * Represents an order ID sent from a client.
 */
using OrderId = uint32_t;

/**
 * Represents the fixed integer quantity of a particular order.
 */
using Quantity = uint32_t;

/**
 * Represents the price of an order's underlying asset in integer ticks (e.g.
 * cents).
 */
using Price = uint32_t;
