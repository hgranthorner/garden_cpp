#include <emscripten/bind.h>
#include <emscripten/em_js.h>
#include <emscripten/eventloop.h>
#include <emscripten/fetch.h>
#include <emscripten/html5.h>
#include <emscripten/val.h>
#include <iostream>
#include <plant.hpp>
#include <string>

#include "document.hpp"
#include <fmt/core.h>

using val = emscripten::val;
using json = nlohmann::json;

constexpr char plant_list_id[] = "plant-list";

namespace Garden {

void make_get_request(std::string_view url,
                      void (*onsuccess)(struct emscripten_fetch_t *)) {
  emscripten_fetch_attr_t attr;
  emscripten_fetch_attr_init(&attr);
  strcpy(attr.requestMethod, "GET");
  attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
  attr.onsuccess = onsuccess;
  attr.onerror = [](emscripten_fetch_t *fetch) {
    std::cout << "Failed to make request to " << fetch->url << std::endl;
    emscripten_fetch_close(fetch);
  };

  emscripten_fetch(&attr, url.data());
}

void show_plant(const Plant &plant) {
  auto img = Document::create_element("img");
  img.set("src", plant.image_url);
  auto div = Document::get_element_by_id("main-content-container");
  div.set("innerHTML", "");
  div.call<void>("appendChild", img);
}

void add_list_item(const PlantSimilarityScore &plant) {
  auto plant_list = Document::get_element_by_id(plant_list_id);

  auto new_plant = Document::create_element("li");

  std::array<std::string, 6> fields = {
      "Common Name",         plant.common_name, "Scientific Name",
      plant.scientific_name, "Genus",           plant.genus,
  };

  for (unsigned long i = 0; i < fields.size(); i += 2) {
    const auto &label = fields[i];
    const auto &value = fields[i + 1];
    auto div = Document::create_element("div");
    div.set("innerText", fmt::format("{}: {}", label, value));
    new_plant.call<void>("appendChild", div);
  }

  new_plant["dataset"].set("id", plant.id);

  new_plant.set("style", R"(
	border: gray;
	padding: 2px;
	border-radius: 10px;
	border-style: solid;
	border-width: thin;)");

  plant_list.call<void>("appendChild", new_plant);

  em_mouse_callback_func cb = [](int, const EmscriptenMouseEvent *,
                                 void *user_data) {
    // TODO: Make get request to get the plant and dispay it on the page
    int plant_id = reinterpret_cast<int>(user_data);

    make_get_request(fmt::format("/api/plants/{}", plant_id),
                     [](struct emscripten_fetch_t *fetch) {
                       std::cout << "Fetched " << fetch->numBytes << " bytes\n";
                       std::string_view data(
                           fetch->data, static_cast<uint32_t>(fetch->numBytes));
                       Plant plant = json::parse(data);
                       show_plant(plant);
                       emscripten_fetch_close(fetch);
                     });
    return true;
  };

  emscripten_set_click_callback(fmt::format("[data-id='{}']", plant.id).c_str(),
                                reinterpret_cast<void *>(plant.id), false, cb);
}

void populate_plant_list(std::string query) {
  make_get_request(
      std::string("/api/search?q=") + query, [](emscripten_fetch_t *fetch) {
        std::cout << "Fetched " << fetch->numBytes << " bytes!\n";
        std::string_view data(fetch->data,
                              static_cast<uint32_t>(fetch->numBytes));
        std::cout << "Fetched " << data << "\n";
        std::vector<PlantSimilarityScore> plants =
            json::parse(data, nullptr, false);
        auto plant_list = Document::get_element_by_id(plant_list_id);
        plant_list.set("innerHTML", "");
        for (const auto &plant : plants) {
          add_list_item(plant);
        }
        emscripten_fetch_close(fetch);
      });
}

void get_plants() {
  make_get_request("/api/plants", [](emscripten_fetch_t *fetch) {
    std::cout << "Fetched " << fetch->numBytes << " bytes!\n";
    emscripten_fetch_close(fetch);
  });
}

} // namespace Garden

void foo(emscripten::val) { std::cout << "Here!\n"; }

EMSCRIPTEN_BINDINGS(main) { emscripten::function("foo", &foo); }

static long debounce_timer_id = -1;

static auto plant_input =
    Garden::Document::get_element_by_id("search-plant-input");

int main() {

  em_mouse_callback_func fetch_cb = [](int, const EmscriptenMouseEvent *,
                                       void *) {
    Garden::get_plants();
    return true;
  };

  em_key_callback_func plant_search_debounce_cb =
      [](int, const EmscriptenKeyboardEvent *, void *) {
        if (debounce_timer_id != -1) {
          emscripten_clear_timeout(debounce_timer_id);
        }
        debounce_timer_id = emscripten_set_timeout(
            [](void *) {
              auto query = plant_input["value"].call<std::string>("toString");
              Garden::populate_plant_list(query);
            },
            500, nullptr);
        return false;
      };

  emscripten_set_keyup_callback("#search-plant-input", nullptr, false,
                                plant_search_debounce_cb);
  emscripten_set_click_callback("#click", nullptr, false, fetch_cb);
}
