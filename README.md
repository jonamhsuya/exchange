# exchange

A high-performance trading exchange engine built in **C++** with a **React/Next.js frontend**. The system is designed for low-latency order handling, efficient order book management, and real-time updates over WebSockets.

## Features

- **C++ Matching Engine**
  - Handles limit and market orders with millisecond-level latency.
  - Implements price-time priority matching.
  - Maintains a fast in-memory order book.
  - WebSocket++ server for client communication.
- **Frontend (React + Next.js)**
  - Live updating order book view.
  - Order placement and cancellation.
  - Trade history and status tracking.
- **Networking**
  - Direct WebSocket++ integration in C++ for low-latency client connections.
- **Performance**
  - Optimized with efficient data structures (`unordered_map`, price ladders).
  - Stress tested with up to millions of orders.

## Architecture

```
+----------------+        +----------------+        +------------------+
|   Frontend     | <----> |   WebSocket++  | <----> | Matching Engine   |
|  (React/Next)  |        |   (C++ server) |        |   (C++ orderbook) |
+----------------+        +----------------+        +------------------+
```

- **Frontend** communicates with the backend via WebSockets.  
- **WebSocket++ server** processes client connections and relays requests.  
- **Matching Engine** maintains the order book, validates orders, and executes trades.  

## Getting Started

### Prerequisites
- C++17 or newer
- Boost
- WebSocket++
- React/Next.js (for frontend)  

### Build & Run (C++ Engine + WebSocket Server)

```bash
# Clone repository
git clone https://github.com/yourname/exchange.git
cd exchange

# Build
cd engine
make

# Run exchange server
./main
```

### Run Frontend

```bash
cd client
npm install
npm run dev
```

The frontend will be available at [http://localhost:3000](http://localhost:3000).  

## Development

- **C++**: core engine and networking.  
- **React/Next.js**: order book and trade UI.
