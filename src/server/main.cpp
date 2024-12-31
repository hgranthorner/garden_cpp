#include "crow/http_request.h"
#include "crow/json.h"
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

constexpr uint32_t MAX_LENGTH = 10;

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

  std::sort(most_similar.rbegin(), most_similar.rend());
  uint32_t len =
      std::min(MAX_LENGTH, static_cast<uint32_t>(most_similar.size()));
  results.reserve(len);
  std::copy_n(most_similar.begin(), len, std::back_inserter(results));
  return results;
}

int main() {
  crow::SimpleApp app;

  CROW_ROUTE(app, "/api/health")([]() { return "Hello, World!"; });
  CROW_ROUTE(app, "/api/search")([](const crow::request &req) {
    std::string_view query = req.url_params.get("q");
    std::vector<PlantSimilarityScore> scores = search_for_plant(query);
    json result = scores;
    crow::json::wvalue body = crow::json::load(result.dump());
    return body;
  });
  CROW_ROUTE(app, "/api/plants")([]() {
    Plant plant = test_db_query();
    json json = plant;
    crow::response res;
    res.write(json.dump());
    res.set_header("Content-Type", "application/json");
    return res;
  });
  CROW_ROUTE(app, "/api/plants/<int>")([](crow::response &res, int plant_id) {
    auto plant = get_plant_by_id(plant_id);
    crow::json::wvalue body;
    if (plant.has_value()) {
      json json = plant.value();
      res.write(json.dump());
      res.set_header("Content-Type", "application/json");
    } else {
      res.code = crow::status::NOT_FOUND;
    }
    res.end();
  });

  CROW_ROUTE(app, "/<path>")
  ([](const std::string &path) {
    // Handle the request automatically with Crow's static handler
    crow::response res;
    auto file_extension = path.substr(path.size() - 3);
    std::cout << file_extension << std::endl;
    // TODO: Disable on non release builds
    if (file_extension == "cpp" || file_extension == "hpp") {
      res.set_static_file_info(path);
    } else if (path.find("homebrew") != std::string::npos) {
      res.set_static_file_info(path);
    } else {
      res.set_static_file_info("build/client/" + path);
    }
    return res;
  });

  app.add_static_dir();
  app.port(18080).multithreaded().run();
}
