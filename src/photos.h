#pragma once

#include "menu.h"

#include <string>
#include <vector>

std::vector<MenuItem> getPhotos();
bool openPhoto(const std::string& path);