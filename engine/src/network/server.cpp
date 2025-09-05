#include "server.hpp"
#include "../message.hpp"
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

void Server::start() {
  sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd_ < 0) {
    std::cerr << "Error creating socket\n";
    return;
  }

  sockaddr_in servaddr{};
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(port_);
  servaddr.sin_addr.s_addr = INADDR_ANY;

  if (bind(sockfd_, (sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    std::cerr << "Bind failed\n";
    return;
  }

  if (listen(sockfd_, SOMAXCONN) < 0) {
    std::cerr << "Listen failed\n";
    return;
  }

  int client_fd = accept(sockfd_, nullptr, nullptr);
  std::cout << "Client connected on port " << port_ << std::endl;

  std::vector<uint8_t> recv_buffer;

  while (true) {
    uint8_t temp[1024];
    ssize_t n = recv(client_fd, temp, sizeof(temp), 0);
    if (n <= 0) {
      std::cout << "Connection closed\n";
      break;
    }

    recv_buffer.insert(recv_buffer.end(), temp, temp + n);

    while (recv_buffer.size() >= sizeof(OrderMessage)) {
      OrderMessage msg;
      memcpy(&msg, recv_buffer.data(), sizeof(OrderMessage));
      recv_buffer.erase(recv_buffer.begin(),
                        recv_buffer.begin() + sizeof(OrderMessage));

      uint8_t checksum = 0;
      for (int i = 0; i < 15; i++) {
        checksum ^= ((uint8_t *)&msg)[i];
      }
      if (checksum != msg.checksum) {
        std::cout << "Checksum mismatch!\n";
      }

      std::cout << "Received Order:\n";
      std::cout << "  Side: " << (msg.side == Side::BUY ? "BUY" : "SELL")
                << "\n";
      std::cout << "  Quantity: " << msg.quantity << "\n";
      std::cout << "  Price: " << msg.price / 100.0 << "\n";
    }
  }

  close(client_fd);
  close(sockfd_);
}

void Server::stop() {
  if (sockfd_ != -1) {
    close(sockfd_);
    sockfd_ = -1;
  }
}