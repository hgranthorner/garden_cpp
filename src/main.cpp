#include "nlohmann/json_fwd.hpp"
#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using namespace emscripten;

class Document {
public:
  static val query_selector(const std::string &target) {
    return _document.call<val>("querySelector", target);
  }

private:
  inline static val _document = val::global("document");
};

std::string print(std::string json_object) {
  using json = nlohmann::json;
  auto data = json::parse(json_object);
  std::string s = data["foo"];
  std::cout << s << std::endl;
  return s;
}

void print_event_target_value(val event) {
  std::cout << event["detail"].call<std::string>("toString") << std::endl;
}

// This is the extra code you need to write to expose your function to JS
EMSCRIPTEN_BINDINGS(main) {
  function("print", &print);
  function("print_event_target_value", &print_event_target_value);
}

int main() {
  std::vector<int> ns(3);
  for (auto &n : ns) {
    n = 5;
  }
  for (auto &n : ns) {
    std::cout << n << std::endl;
  }
  std::cout << "Hello world\n";
  auto document = val::global("document");
  auto div = Document::query_selector("div");
  std::cout << div.call<std::string>("toString") << std::endl;
}
