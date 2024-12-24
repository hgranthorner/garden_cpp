#include "nlohmann/json_fwd.hpp"
#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

class Document {
public:
  static emscripten::val query_selector(const std::string &target) {
    return _document.call<emscripten::val>("querySelector", target);
  }

private:
  inline static emscripten::val _document = emscripten::val::global("document");
};

std::string print(std::string json_object) {
  using json = nlohmann::json;
  auto data = json::parse(json_object);
  std::string s = data["foo"];
  std::cout << s << std::endl;
  return s;
}

// This is the extra code you need to write to expose your function to JS
EMSCRIPTEN_BINDINGS(main) { emscripten::function("print", &print); }

int main() {
  std::vector<int> ns(3);
  for (auto &n : ns) {
    n = 5;
  }
  for (auto &n : ns) {
    std::cout << n << std::endl;
  }
  std::cout << "Hello world\n";
  auto document = emscripten::val::global("document");
  auto div = Document::query_selector("div");
  std::cout << div.call<std::string>("toString") << std::endl;
}
