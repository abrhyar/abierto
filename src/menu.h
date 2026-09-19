#pragma once

#include <string>
#include <vector>

struct MenuItem {
    std::string icon;
    std::string name;
};

std::vector<MenuItem> getMainMenu();
std::vector<MenuItem> getGamesMenu();