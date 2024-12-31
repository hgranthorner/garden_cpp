#pragma once

#include <nlohmann/json.hpp>

namespace Garden {
struct Plant {
  int id;
  std::string common_name;
  std::string slug;
  std::string scientific_name;
  std::string family_common_name;
  std::string image_url;
  std::string synonyms;
  std::string genus;
  std::string family;
  std::string self_link;
  std::string plant_link;
  std::string genus_link;
  // year integer,
  // bibliography text,
  // author text,
  // status text,
  // rank text,
  // genus_id integer,
  //
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Plant, id, common_name, slug, scientific_name,
                                 family_common_name, image_url, synonyms, genus,
                                 family, self_link, plant_link, genus_link)
};

struct PlantSimilarityScore {

  int id;
  std::string common_name;
  std::string scientific_name;
  std::string family_common_name;
  std::string synonyms;
  std::string genus;

  static PlantSimilarityScore from_plant(const Plant &plant, double score);
  double score;

  bool operator<(const PlantSimilarityScore &right) const;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(PlantSimilarityScore, id, common_name,
                                 synonyms, family_common_name, scientific_name,
                                 genus, score)
};

} // namespace Garden
