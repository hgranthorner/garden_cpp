#include "db.hpp"

using namespace Garden;

static int64_t num_plants = -1;
static std::vector<Garden::Plant> all_plants = std::vector<Garden::Plant>();

Plant plant_from_query(const SQLite::Statement &query) {
  Plant plant;
  plant.id = query.getColumn(0);
  plant.common_name = std::move(query.getColumn(1).getString());
  plant.slug = std::move(query.getColumn(2).getString());
  plant.scientific_name = std::move(query.getColumn(3).getString());
  plant.family_common_name = std::move(query.getColumn(4).getString());
  return plant;
}

const std::string PLANT_QUERY =
    "SELECT id, common_name, slug, scientific_name, family_common_name FROM "
    "plants";

std::vector<Garden::Plant> get_plants(int count) {
  SQLite::Database db("db.sqlite3");

  if (num_plants == -1) {
    SQLite::Statement count_query(db, "SELECT count(1) FROM plants");
    count_query.executeStep();
    num_plants = count_query.getColumn(0);

    SQLite::Statement all_plants_query(db, PLANT_QUERY);

    while (all_plants_query.executeStep()) {
      Plant plant = plant_from_query(all_plants_query);
      all_plants.push_back(std::move(plant));
    }
  }

  if (count >= num_plants) {
    return all_plants;
  }

  SQLite::Statement query(db, PLANT_QUERY + " limit ?");

  query.bind(1, count);

  std::vector<Plant> plants;
  while (query.executeStep()) {
    Plant plant = plant_from_query(query);
    plants.push_back(std::move(plant));
  }

  return plants;
}
