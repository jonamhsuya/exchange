#pragma once
#include "../matching_engine.hpp"

class Server {
public:
  Server() : port_(9999), sockfd_(-1) {}
  ~Server() { stop(); }

  void start();
  void stop();

private:
  int port_;
  int sockfd_;
  MatchingEngine engine_;

  //   void handleClient(int clientFd);
};