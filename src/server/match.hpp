#pragma once

#include <plant.hpp>
#include <rapidfuzz/fuzz.hpp>

std::vector<Garden::PlantSimilarityScore>
extract(std::string_view query, const std::vector<Garden::Plant> &choices,
        const double score_cutoff = 0.0);
