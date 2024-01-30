#include "select/select.hpp"

int main() {
  int max_clients = 10;
  int max_buffer_size = 1024;
  int port = 8080;
  select_srv::Select srv(max_clients, max_buffer_size, port);
  srv.spin();
  return 0;
}