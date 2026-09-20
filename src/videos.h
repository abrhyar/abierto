#pragma once

#include "menu.h"

#include <string>
#include <vector>

std::vector<MenuItem> getVideos();
bool openVideo(const std::string& path);