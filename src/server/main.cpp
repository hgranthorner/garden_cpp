#include "crow/app.h"
#include <asio.hpp>

int main() {
  crow::SimpleApp app;

  CROW_ROUTE(app, "/")([]() { return "Hello, World!"; });

  app.port(18080).multithreaded().run();
}
