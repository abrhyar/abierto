#include "ui.h"

#include <ncurses.h>
#include <locale.h>

void initUI() {
    setlocale(LC_ALL, "");

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    start_color();
    use_default_colors();

    init_pair(1, COLOR_CYAN, -1);
    init_pair(2, COLOR_WHITE, -1);
    init_pair(3, COLOR_BLACK, COLOR_CYAN);
}

void shutdownUI() {
    endwin();
}

void drawMenu(
    const std::vector<MenuItem>& menu,
    int selected,
    const std::string& title)   {
    clear();

    int height, width;
    getmaxyx(stdscr, height, width);

    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(1, 2, "ABIERTO");
    attroff(COLOR_PAIR(1) | A_BOLD);

    attron(COLOR_PAIR(2));
    mvprintw(1, width - title.length() - 2, "%s", title.c_str());
    attroff(COLOR_PAIR(2));

    mvhline(2, 1, ACS_HLINE, width - 2);

    int menu_y = 4;

    for (int i = 0; i < static_cast<int>(menu.size()); i++) {
        if (i == selected) {
            attron(COLOR_PAIR(3) | A_BOLD);
        }

        mvprintw(menu_y + i, 4, "%s  %s",
                 menu[i].icon.c_str(),
                 menu[i].name.c_str());

        if (i == selected) {
            attroff(COLOR_PAIR(3) | A_BOLD);
        }
    }

    if (height >= 12) {
        mvhline(height - 3, 1, ACS_HLINE, width - 2);

        attron(COLOR_PAIR(2));
        mvprintw(height - 2, 2,
                 "↑↓ Mover    Enter Seleccionar    Esc Salir");
        attroff(COLOR_PAIR(2));
    }

    refresh();
}