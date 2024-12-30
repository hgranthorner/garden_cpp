#include "crow/http_request.h"
#include "db.hpp"
#include "match.hpp"
#include <algorithm>
#include <asio.hpp>
#include <crow/app.h>
#include <iostream>
#include <iterator>
#include <nlohmann/json.hpp>
#include <plant.hpp>
#include <rapidfuzz/fuzz.hpp>

using namespace Garden;
using json = nlohmann::json;

Plant test_db_query() {
  Plant plant;
  try {
    plant = get_plants(1)[0];
  } catch (std::exception &e) {
    std::cout << "exception: " << e.what() << std::endl;
  }
  return plant;
}

std::vector<PlantSimilarityScore>
search_for_plant(const std::string_view plant_query) {
  std::vector<PlantSimilarityScore> most_similar;
  try {
    auto plants = get_plants(1000000000);

    most_similar = extract(plant_query, plants, 75.0);

  } catch (std::exception &e) {
    std::cout << "exception: " << e.what() << std::endl;
  }

  std::vector<PlantSimilarityScore> results;
  if (most_similar.empty()) {
    return results;
  }

  const uint32_t MAX_LENGTH = 5;
  std::sort(most_similar.rbegin(), most_similar.rend());
  uint32_t len = std::min(MAX_LENGTH, (uint32_t)most_similar.size());
  results.resize(len);
  std::copy_n(most_similar.begin(), len, std::back_inserter(results));
  return results;
}

int main() {
  crow::SimpleApp app;

  CROW_ROUTE(app, "/api/health")([]() { return "Hello, World!"; });
  CROW_ROUTE(app,
             "/api/search")([](const crow::request &req, crow::response &res) {
    std::string_view query = req.url_params.get("q");
    json results = search_for_plant(query);
    res.write(results.dump());
    res.end();
  });
  CROW_ROUTE(app, "/api/plants")([]() {
    Plant plant = test_db_query();
    json json = plant;
    crow::response res;
    res.write(json.dump());
    res.set_header("Content-Type", "application/json");
    return res;
  });

  CROW_ROUTE(app, "/<path>")
  ([](const crow::request &req, const std::string &path) {
    // Handle the request automatically with Crow's static handler
    crow::response res;
    res.set_static_file_info("build/client/" + path);
    return res;
  });

  app.add_static_dir();
  app.port(18080).multithreaded().run();
}
