#include "db.hpp"
#include <optional>

using namespace Garden;

Plant plant_from_query(const SQLite::Statement &query) {
  Plant plant;

  plant.id = query.getColumn("id");
  plant.common_name = query.getColumn("common_name").getString();
  plant.slug = query.getColumn("slug").getString();
  plant.scientific_name = query.getColumn("scientific_name").getString();
  plant.family_common_name = query.getColumn("family_common_name").getString();
  plant.image_url = query.getColumn("image_url").getString();
  plant.synonyms = query.getColumn("synonyms").getString();
  plant.genus = query.getColumn("genus").getString();
  plant.family = query.getColumn("family").getString();
  plant.self_link = query.getColumn("self_link").getString();
  plant.plant_link = query.getColumn("plant_link").getString();
  plant.genus_link = query.getColumn("genus_link").getString();
  return plant;
}

const std::string PLANT_QUERY =
    R"(SELECT id, common_name, slug, scientific_name,
       family_common_name, image_url, synonyms, genus,
       family, self_link, plant_link, genus_link 
       from plants)";

static int64_t num_plants = -1;
static std::vector<Garden::Plant> all_plants = std::vector<Garden::Plant>();

std::vector<Garden::Plant> get_plants(int count) {
  SQLite::Database db("db.sqlite3");

  if (num_plants == -1) {
    SQLite::Statement count_query(db, "SELECT count(1) FROM plants");
    count_query.executeStep();
    num_plants = count_query.getColumn(0);

    SQLite::Statement all_plants_query(db, PLANT_QUERY);

    while (all_plants_query.executeStep()) {
      all_plants.push_back(plant_from_query(all_plants_query));
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

std::optional<Plant> get_plant_by_id(int plant_id) {
  SQLite::Database db("db.sqlite3");
  SQLite::Statement query(db, PLANT_QUERY + " where id = ? limit 1");
  query.bind(1, plant_id);
  if (query.executeStep()) {
    return plant_from_query(query);
  } else {
    return std::nullopt;
  }
}
