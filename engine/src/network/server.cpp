#include "network/server.hpp"
#include <algorithm>
#include <cstring>
#include <iostream>
#include <random>

void Server::start() {
  ws_.init_asio();

  ws_.set_open_handler([this](connection_hdl hdl) {
    std::lock_guard<std::mutex> lock(mtx_);
    uint32_t clientId =
        std::uniform_int_distribution<uint32_t>(1, 0xFFFFFFFF)(gen_);
    while (clients_.find(clientId) != clients_.end()) {
      clientId = std::uniform_int_distribution<uint32_t>(1, 0xFFFFFFFF)(gen_);
    }
    clients_[clientId] = hdl;
    std::cout << "New WebSocket client connected\n";

    IdAssignment idAssignment{clientId, EventType::ID_ASSIGNMENT, 0};
    uint8_t checksum = 0;
    for (int i = 0; i < (int)sizeof(idAssignment) - 1; i++) {
      checksum ^= ((uint8_t *)&idAssignment)[i];
    }
    idAssignment.checksum = checksum;

    websocketpp::lib::error_code ec;
    ws_.send(hdl, reinterpret_cast<char *>(&idAssignment), sizeof(IdAssignment),
             websocketpp::frame::opcode::binary, ec);
    if (ec) {
      std::cerr << "Error sending status: " << ec.message() << "\n";
    }
  });

  ws_.set_close_handler([this](connection_hdl hdl) {
    std::lock_guard<std::mutex> lock(mtx_);

    for (auto it = clients_.begin(); it != clients_.end();) {
      if (!it->second.owner_before(hdl) && !hdl.owner_before(it->second)) {
        it = clients_.erase(it);
      } else {
        ++it;
      }
    }

    std::cout << "Client disconnected\n";
  });

  ws_.set_message_handler(
      [this](connection_hdl hdl, ws_server::message_ptr msg) {
        handleClient(msg->get_payload());
      });

  ws_.listen(port_);
  ws_.start_accept();
  std::cout << "WebSocket server listening on port " << port_ << std::endl;

  ws_.run();
}

void Server::stop() {
  ws_.stop_listening();

  std::lock_guard<std::mutex> lock(mtx_);
  for (auto &[clientId, hdl] : clients_) {
    websocketpp::lib::error_code ec;
    ws_.close(hdl, websocketpp::close::status::going_away, "", ec);
    if (ec) {
      std::cerr << "Error closing connection: " << ec.message() << "\n";
    }
  }
  clients_.clear();

  ws_.stop();
}

void Server::handleClient(const std::string &payload) {
  if (payload.size() < sizeof(OrderMessage)) {
    std::cout << "Invalid message size\n";
    return;
  }

  OrderMessage msg{};
  memcpy(&msg, payload.data(), sizeof(OrderMessage));

  uint8_t checksum = 0;
  for (int i = 0; i < (int)sizeof(OrderMessage) - 1; i++) {
    checksum ^= ((uint8_t *)&msg)[i];
  }
  if (checksum != msg.checksum) {
    std::cout << "Checksum mismatch!\n";
    return;
  }

  std::cout << "\nReceived order: clientId=" << msg.clientId
            << " clientOrderId=" << msg.clientOrderId
            << " side=" << (msg.side == Side::BUY ? "BUY" : "SELL")
            << " quantity=" << msg.quantity << " price=" << msg.price
            << " checksum=" << (int)msg.checksum << "\n\n";

  engine_.submitOrder(msg.clientId, msg.clientOrderId, msg.side, msg.quantity,
                      msg.price);

  std::cout << "\nOrder book:\n";
  engine_.book().printBook();
  std::cout << "\n";
}

void Server::onAccept(Order &order) {
  std::cout << "Notifying client of order acceptance\n";
  Status accept{order.clientOrderKey.clientOrderId, EventType::ACCEPT, 0};
  uint8_t checksum = 0;
  for (int i = 0; i < (int)sizeof(Status) - 1; i++) {
    checksum ^= ((uint8_t *)&accept)[i];
  }
  accept.checksum = checksum;

  connection_hdl hdl;
  {
    std::lock_guard<std::mutex> lock(mtx_);
    auto it = clients_.find(order.clientOrderKey.clientId);
    if (it == clients_.end()) {
      std::cerr << "Client ID not found: " << order.clientOrderKey.clientId
                << "\n";
      return;
    }
    hdl = it->second;
  }

  websocketpp::lib::error_code ec;
  ws_.send(hdl, reinterpret_cast<char *>(&accept), sizeof(Status),
           websocketpp::frame::opcode::binary, ec);
  if (ec) {
    std::cerr << "Error sending accept: " << ec.message() << "\n";
  }
}

void Server::onCancel(Order &order) {
  std::cout << "Notifying client of order cancellation\n";
  Status cancel{order.clientOrderKey.clientOrderId, EventType::CANCEL, 0};
  uint8_t checksum = 0;
  for (int i = 0; i < (int)sizeof(Status) - 1; i++) {
    checksum ^= ((uint8_t *)&cancel)[i];
  }
  cancel.checksum = checksum;

  connection_hdl hdl;
  {
    std::lock_guard<std::mutex> lock(mtx_);
    auto it = clients_.find(order.clientOrderKey.clientId);
    if (it == clients_.end()) {
      std::cerr << "Client ID not found: " << order.clientOrderKey.clientId
                << "\n";
      return;
    }
    hdl = it->second;
  }

  websocketpp::lib::error_code ec;
  ws_.send(hdl, reinterpret_cast<char *>(&cancel), sizeof(Status),
           websocketpp::frame::opcode::binary, ec);
  if (ec) {
    std::cerr << "Error sending cancel: " << ec.message() << "\n";
  }
}

void Server::onTrade(Order &order) {
  std::cout << "Notifying client of trade execution\n";
  Trade trade{order.clientOrderKey.clientOrderId, order.quantity, order.price,
              EventType::TRADE, 0};
  uint8_t checksum = 0;
  for (int i = 0; i < (int)sizeof(Trade) - 1; i++) {
    checksum ^= ((uint8_t *)&trade)[i];
  }
  trade.checksum = checksum;

  connection_hdl hdl;
  {
    std::lock_guard<std::mutex> lock(mtx_);
    auto it = clients_.find(order.clientOrderKey.clientId);
    if (it == clients_.end()) {
      std::cerr << "Client ID not found: " << order.clientOrderKey.clientId
                << "\n";
      return;
    }
    hdl = it->second;
  }

  websocketpp::lib::error_code ec;
  try {
    ws_.send(hdl, reinterpret_cast<char *>(&trade), sizeof(Trade),
             websocketpp::frame::opcode::binary, ec);
    if (ec) {
      std::cerr << "Error sending trade: " << ec.message() << "\n";
    }
  } catch (const websocketpp::exception &e) {
    std::cerr << "WebSocket exception: " << e.what() << "\n";
  }
}
