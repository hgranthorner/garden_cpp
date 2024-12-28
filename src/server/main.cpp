#include <SQLiteCpp/SQLiteCpp.h>
#include <asio.hpp>
#include <crow/app.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <rapidfuzz/fuzz.hpp>

struct Plant {
  int id;
  std::string common_name;
  std::string slug;
  std::string scientific_name;
  // year integer,
  // bibliography text,
  // author text,
  // status text,
  // rank text,
  // family_common_name text,
  // genus_id integer,
  // image_url text,
  // synonyms text,
  // genus text,
  // family text,
  std::string self_link;
  // plant_link text,
  // genus_link text
  //
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Plant, id, common_name, slug, scientific_name)
};

Plant test_db_query() {
  Plant plant;
  try {
    // Open a database file
    SQLite::Database db("db.sqlite3");

    // Compile a SQL query, containing one parameter (index 1)
    SQLite::Statement query(
        db,
        "SELECT id, common_name, slug, scientific_name FROM plants limit 1");

    // Loop to execute the query step by step, to get rows of result
    while (query.executeStep()) {
      // Demonstrate how to get some typed column value
      plant.id = query.getColumn(0);
      plant.common_name = query.getColumn(1).getString();
      plant.slug = query.getColumn(2).getString();
      plant.scientific_name = query.getColumn(3).getString();
    }
  } catch (std::exception &e) {
    std::cout << "exception: " << e.what() << std::endl;
  }
  return plant;
}

int main() {
  crow::SimpleApp app;

  CROW_ROUTE(app, "/api/health")([]() { return "Hello, World!"; });
  CROW_ROUTE(app, "/api/plants")([]() {
    Plant plant = test_db_query();
    nlohmann::json json = plant;
    crow::response res;
    res.write(json.dump());
    res.set_header("Content-Type", "application/json");
    return res;
  });

  CROW_ROUTE(app, "/<path>")
  ([](const crow::request &req, const std::string &path) {
    // Handle the request automatically with Crow's static handler
    std::cout << "here: " << path << "\n";
    crow::response res;
    res.set_static_file_info("build/client/" + path);
    return res;
  });

  app.add_static_dir();
  app.port(18080).multithreaded().run();
}
