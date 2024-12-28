#pragma once

#include <rapidfuzz/fuzz.hpp>

template <typename Sentence1, typename Iterable,
          typename Sentence2 = typename Iterable::value_type>
std::vector<std::pair<Sentence2, double>>
extract(const Sentence1 &query, const Iterable &choices,
        const double score_cutoff = 0.0);
