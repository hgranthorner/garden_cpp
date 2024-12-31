#import "plant.hpp"

using namespace Garden;

bool PlantSimilarityScore::operator<(const PlantSimilarityScore &right) const {
  return score < right.score;
}

PlantSimilarityScore PlantSimilarityScore::from_plant(const Plant &plant,
                                                      double score) {
  return PlantSimilarityScore{
      .id = plant.id,
      .common_name = plant.common_name,
      .scientific_name = plant.scientific_name,
      .family_common_name = plant.family_common_name,
      .synonyms = plant.synonyms,
      .genus = plant.genus,
      .score = score,
  };
}
