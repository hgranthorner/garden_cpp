#include <emscripten/bind.h>
#include <emscripten/fetch.h>
#include <emscripten/html5.h>
#include <emscripten/val.h>
#include <iostream>
#include <string>

using val = emscripten::val;

namespace Garden {

// import Document;

class Document {
public:
  static val query_selector(const std::string &target) {
    return _document.call<val>("querySelector", target);
  }

  static val get_element_by_id(const std::string &id) {
    return _document.call<val>("getElementById", id);
  }

private:
  inline static val _document = val::global("document");
};

void print_event_target_value(val event) {
  std::cout << event["detail"].call<std::string>("toString") << std::endl;
}

void print_event_target_value_test() {
  emscripten_fetch_attr_t attr;
  emscripten_fetch_attr_init(&attr);
  strcpy(attr.requestMethod, "GET");
  attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
  attr.onsuccess = [](emscripten_fetch_t *fetch) {
    std::cout << "Fetched " << fetch->numBytes << " bytes!\n";
  };
  attr.onerror = [](emscripten_fetch_t *fetch) {
    std::cout << "Failed to get data!\n";
  };
  emscripten_fetch(&attr, "http://localhost:8000/emscripten_probe.html");
}

EMSCRIPTEN_BINDINGS(main) {
  emscripten::function("print_event_target_value", &print_event_target_value);
  emscripten::function("print_event_target_value_test",
                       &print_event_target_value_test);
}

} // namespace Garden
//
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
  auto div = Garden::Document::query_selector("div");
  std::cout << div.call<std::string>("toString") << std::endl;

  em_mouse_callback_func cb =
      [](int event_type, const EmscriptenMouseEvent *event, void *user_data) {
        Garden::print_event_target_value_test();
        return true;
      };
}
