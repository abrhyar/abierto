#include <ncurses.h>
#include <locale.h>
#include <vector>
#include <string>
#include <cwchar>

struct MenuItem {
    std::string icon;
    std::string name;
};

int main() {
    // Iniciar ncurses
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    // Colores
    start_color();
    use_default_colors();

    init_pair(1, COLOR_CYAN, -1);   // Títulos
    init_pair(2, COLOR_WHITE, -1);  // Texto
    init_pair(3, COLOR_BLACK, COLOR_CYAN); // Selección

    std::vector<MenuItem> menu = {
        {"🎮", "Juegos"},
        {"🎵", "Música"},
        {"🎬", "Vídeos"},
        {"🖼", "Fotos"},
        {"📷", "Cámara"},
        {"📡", "Bluetooth"},
        {"📶", "Wi-Fi"},
        {"⚙", "Configuración"},
        {"⏻", "Salir"}
    };

    int selected = 0;
    bool running = true;

    while (running) {
        clear();

        int height, width;
        getmaxyx(stdscr, height, width);

        // Título
        attron(COLOR_PAIR(1) | A_BOLD);
        mvprintw(1, 2, "ABIERTO");
        attroff(COLOR_PAIR(1) | A_BOLD);

        // Información
        attron(COLOR_PAIR(2));
        mvprintw(1, width - 18, "Launcher principal");
        attroff(COLOR_PAIR(2));

        // Línea superior
        mvhline(2, 1, ACS_HLINE, width - 2);

        // Menú
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

        // Ayuda inferior
        if (height >= 12) {
            mvhline(height - 3, 1, ACS_HLINE, width - 2);

            attron(COLOR_PAIR(2));
            mvprintw(height - 2, 2,
                     "↑↓ Mover    Enter Seleccionar    Esc Salir");
            attroff(COLOR_PAIR(2));
        }

        refresh();

        int key = getch();

        switch (key) {
            case KEY_UP:
                selected--;

                if (selected < 0)
                    selected = menu.size() - 1;

                break;

            case KEY_DOWN:
                selected++;

                if (selected >= static_cast<int>(menu.size()))
                    selected = 0;

                break;

            case '\n':
            case KEY_ENTER:
                if (menu[selected].name == "Salir") {
                    running = false;
                } else {
                    clear();

                    mvprintw(height / 2,
                             (width - menu[selected].name.length() - 10) / 2,
                             "%s seleccionado",
                             menu[selected].name.c_str());

                    mvprintw(height / 2 + 2,
                             (width - 20) / 2,
                             "Pulsa una tecla...");

                    refresh();
                    getch();
                }

                break;

            case 27: // ESC
                running = false;
                break;
        }
    }

    endwin();

    return 0;
}