#include <emscripten/bind.h>
#include <emscripten/fetch.h>
#include <emscripten/html5.h>
#include <emscripten/val.h>
#include <iostream>
#include <string>

#include "document.hpp"

using val = emscripten::val;

namespace Garden {

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
  emscripten_fetch(&attr, "/api/health");
}

void add_list_item() {
  auto plant_list = Document::get_element_by_id("plant-list");
  auto new_plant = Document::create_element("div");
  new_plant.set("innerText", "foo");
  plant_list.call<void>("appendChild", new_plant);
}

} // namespace Garden

EMSCRIPTEN_BINDINGS(main) {
  emscripten::function("print_event_target_value_test",
                       &Garden::print_event_target_value_test);
  emscripten::function("add_list_item", &Garden::add_list_item);
}

int main() {
  em_mouse_callback_func cb =
      [](int event_type, const EmscriptenMouseEvent *event, void *user_data) {
        Garden::add_list_item();
        return true;
      };

  em_mouse_callback_func fetch_cb =
      [](int event_type, const EmscriptenMouseEvent *event, void *user_data) {
        Garden::print_event_target_value_test();
        return true;
      };

  emscripten_set_click_callback("#click", nullptr, false, cb);
  emscripten_set_click_callback("#click", nullptr, false, fetch_cb);
}
