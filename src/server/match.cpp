#include "match.hpp"
#include "rapidfuzz/fuzz.hpp"

using namespace Garden;

std::vector<PlantSimilarityScore>
extract(std::string_view query, const std::vector<Garden::Plant> &choices,
        const double score_cutoff) {
  std::vector<PlantSimilarityScore> results;

  rapidfuzz::fuzz::CachedPartialRatio<char> scorer(query);

  for (const auto &choice : choices) {
    double common_name_score =
        scorer.similarity(choice.common_name, score_cutoff);
    double scientific_name_score =
        scorer.similarity(choice.scientific_name, score_cutoff);
    double family_common_name_score =
        scorer.similarity(choice.family_common_name, score_cutoff);
    double synonyms_score = scorer.similarity(choice.synonyms, score_cutoff);
    double genus_score = scorer.similarity(choice.genus, score_cutoff);

    auto score = std::max(common_name_score, scientific_name_score);
    score = std::max(score, family_common_name_score);
    score = std::max(score, synonyms_score);
    score = std::max(score, genus_score);

    if (choice.common_name.compare(query) == 0 ||
        choice.scientific_name.compare(query) == 0 ||
        choice.family_common_name.compare(query) == 0 ||
        choice.genus.compare(query) == 0) {
      score = 101.0;
    }

    if (score >= score_cutoff) {
      results.push_back(PlantSimilarityScore::from_plant(choice, score));
    }
  }

  return results;
}
