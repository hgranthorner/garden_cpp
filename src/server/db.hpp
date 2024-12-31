#pragma once

#include <SQLiteCpp/SQLiteCpp.h>
#include <plant.hpp>

std::vector<Garden::Plant> get_plants(int count);
std::optional<Garden::Plant> get_plant_by_id(int plant_id);
