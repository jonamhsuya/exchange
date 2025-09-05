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