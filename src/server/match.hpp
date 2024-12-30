#pragma once

#include <plant.hpp>
#include <rapidfuzz/fuzz.hpp>

struct PlantSimilarityScore {
  int id;
  std::string common_name;
  std::string scientific_name;
  std::string family_common_name;
  std::string synonyms;
  double score;

  bool operator<(const PlantSimilarityScore &right) const;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(PlantSimilarityScore, id, common_name,
                                 synonyms, family_common_name, scientific_name,
                                 score)
};

std::vector<PlantSimilarityScore>
extract(std::string_view query, const std::vector<Garden::Plant> &choices,
        const double score_cutoff = 0.0);
