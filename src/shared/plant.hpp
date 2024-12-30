#pragma once

#include <nlohmann/json.hpp>

namespace Garden {
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
  std::string family_common_name;
  // genus_id integer,
  // image_url text,
  std::string synonyms;
  // genus text,
  // family text,
  std::string self_link;
  // plant_link text,
  // genus_link text
  //
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Plant, id, common_name, slug,
                                 family_common_name, synonyms, scientific_name)
};

} // namespace Garden
