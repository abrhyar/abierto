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

    // Negro de la terminal como fondo
    init_pair(1, COLOR_WHITE, -1);  // Texto normal
    init_pair(2, COLOR_CYAN, -1);   // Indicador >
    init_pair(3, COLOR_WHITE, -1);  // Titulo
    init_pair(4, COLOR_WHITE, -1);  // MX
}

void shutdownUI() {
    endwin();
}

void drawMenu(
    const std::vector<MenuItem>& menu,
    int selected,
    const std::string& title
) {
    clear();

    int height, width;
    getmaxyx(stdscr, height, width);

    // =========================
    // CABECERA
    // =========================

    attron(COLOR_PAIR(3) | A_BOLD);
    mvprintw(1, 2, "ABIERTO");
    attroff(COLOR_PAIR(3) | A_BOLD);

    attron(COLOR_PAIR(4) | A_BOLD);
    mvprintw(1, width - 4, "MX");
    attroff(COLOR_PAIR(4) | A_BOLD);

    // Línea superior
    attron(COLOR_PAIR(1));
    mvhline(2, 1, '-', width - 2);
    attroff(COLOR_PAIR(1));

    // =========================
    // MENU
    // =========================

    int menu_y = 4;

    for (int i = 0; i < static_cast<int>(menu.size()); i++) {

        if (i == selected) {
            // Flecha cyan
            attron(COLOR_PAIR(2) | A_BOLD);
            mvprintw(menu_y + i, 3, ">");
            attroff(COLOR_PAIR(2) | A_BOLD);

            // Texto seleccionado blanco y negrita
            attron(COLOR_PAIR(1) | A_BOLD);
            mvprintw(menu_y + i, 6, "%s", menu[i].name.c_str());
            attroff(COLOR_PAIR(1) | A_BOLD);

        } else {
            // Texto normal
            attron(COLOR_PAIR(1));
            mvprintw(menu_y + i, 6, "%s", menu[i].name.c_str());
            attroff(COLOR_PAIR(1));
        }
    }

    // =========================
    // PIE
    // =========================

    if (height >= 12) {
        attron(COLOR_PAIR(1));
        mvhline(height - 3, 1, '-', width - 2);

        mvprintw(
            height - 2,
            2,
            "UP/DOWN  Mover    ENTER  Seleccionar    ESC  Volver"
        );

        attroff(COLOR_PAIR(1));
    }

    refresh();
}