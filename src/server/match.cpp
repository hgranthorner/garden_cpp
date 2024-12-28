#include <rapidfuzz/fuzz.hpp>

template <typename Sentence1, typename Iterable,
          typename Sentence2 = typename Iterable::value_type>
std::vector<std::pair<Sentence2, double>>
extract(const Sentence1 &query, const Iterable &choices,
        const double score_cutoff = 0.0) {
  std::vector<std::pair<Sentence2, double>> results;

  rapidfuzz::fuzz::CachedRatio<typename Sentence1::value_type> scorer(query);

  for (const auto &choice : choices) {
    double score = scorer.similarity(choice, score_cutoff);

    if (score >= score_cutoff) {
      results.emplace_back(choice, score);
    }
  }

  return results;
}
