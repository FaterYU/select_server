#ifndef SELECT__SELECT_HPP
#define SELECT__SELECT_HPP

#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>

namespace select_srv {

class Select {
 public:
  explicit Select(int max_clients, int max_buffer_size, int port);
  ~Select();
  void spin();

 private:
  void init();

  int sockfd_;
  int maxfd_;
  fd_set allfds_;

  std::unique_ptr<char[]> buff_;

  int max_clients_;
  int max_buffer_size_;
  int port_;
};
}  // namespace select_srv

#endif  // SELECT__SELECT_HPP