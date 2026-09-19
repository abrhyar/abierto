#pragma once

#include "menu.h"

void initUI();
void shutdownUI();

void drawMenu(
    const std::vector<MenuItem>& menu,
    int selected,
    const std::string& title
);