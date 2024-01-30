#include "select/select.hpp"

namespace select_srv {

Select::Select(int max_clients, int max_buffer_size, int port)
    : max_clients_(max_clients),
      max_buffer_size_(max_buffer_size),
      port_(port) {
  init();
}

Select::~Select() { close(sockfd_); }

void Select::init() {
  struct sockaddr_in server_addr;
  buff_ = std::make_unique<char[]>(max_buffer_size_);
  sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (-1 == sockfd_) {
    perror("socket");
    exit(1);
  }
  int opt = 1;
  if (setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
    perror("setsockopt");
    exit(1);
  }
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port_);
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  if (-1 ==
      bind(sockfd_, (struct sockaddr*)&server_addr, sizeof(server_addr))) {
    perror("bind");
    exit(1);
  }
  if (-1 == listen(sockfd_, max_clients_)) {
    perror("listen");
    exit(1);
  }
  std::cout << "Server is listening on port " << port_ << std::endl;
  FD_ZERO(&allfds_);
  FD_SET(sockfd_, &allfds_);
  maxfd_ = sockfd_;
}

void Select::spin() {
  while (true) {
    fd_set readfds = allfds_;
    fd_set writefds = allfds_;
    fd_set exceptfds = allfds_;
    auto ret = select(maxfd_ + 1, &readfds, &writefds, &exceptfds, NULL);
    if (ret < 0) {
      perror("select");
      exit(1);
    }
    for (int fd = 0; fd <= maxfd_; ++fd) {
      if (fd == sockfd_) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        auto clientfd =
            accept(sockfd_, (struct sockaddr*)&client_addr, &client_len);
        if (clientfd < 0) {
          perror("accept");
          exit(1);
        }
        FD_SET(clientfd, &allfds_);
        maxfd_ = std::max(maxfd_, clientfd);
      } else if (FD_ISSET(fd, &readfds)) {
        memset(buff_.get(), 0, max_buffer_size_);
        auto n = read(fd, buff_.get(), max_buffer_size_);
        if (n < 0) {
          perror("read");
          exit(1);
        } else if (n == 0) {
          close(fd);
          FD_CLR(fd, &allfds_);
        } else {
          auto n = write(fd, buff_.get(), strlen(buff_.get()));
          if (n < 0) {
            perror("write");
            exit(1);
          }
        }
      }
    }
  }
}

}  // namespace select_srv