#pragma once
#include "../matching_engine.hpp"
#include "message.hpp"
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <map>
#include <memory>
#include <mutex>
#include <random>

class Server {
public:
  Server(MatchingEngine &engine, int port = 9999)
      : engine_(engine), port_(port) {}
  ~Server() { stop(); }

  void start();
  void stop();

  void onAccept(Order &order);
  void onCancel(Order &order);
  void onTrade(Order &order);

private:
  using ws_server = websocketpp::server<websocketpp::config::asio>;
  using connection_hdl = websocketpp::connection_hdl;

  void handleClient(const std::string &payload);

  MatchingEngine &engine_;
  int port_;
  ws_server ws_;
  std::mt19937 gen_{std::random_device{}()};
  std::map<uint32_t, connection_hdl> clients_;
  std::mutex mtx_;
};
