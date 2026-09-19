#include "menu.h"
#include "ui.h"
#include "photos.h"

#include <ncurses.h>
#include <vector>
#include <string>

int main() {
    initUI();

    std::vector<MenuItem> mainMenu = getMainMenu();
    std::vector<MenuItem> gamesMenu = getGamesMenu();

    bool running = true;
    bool inGames = false;
    bool inPhotos = false;

    int selected = 0;

    while (running) {

        // =========================
        // MENU DE FOTOS
        // =========================

        if (inPhotos) {

            std::vector<MenuItem> photosMenu = getPhotos();

            if (photosMenu.empty()) {

                clear();

                int height, width;
                getmaxyx(stdscr, height, width);

                attron(A_BOLD);
                mvprintw(1, 2, "FOTOS");
                attroff(A_BOLD);

                mvprintw(
                    height / 2,
                    (width - 22) / 2,
                    "No hay imagenes."
                );

                mvprintw(
                    height - 2,
                    2,
                    "ESC  Volver"
                );

                refresh();

                int key = getch();

                if (key == 27) {
                    inPhotos = false;
                    selected = 0;
                }

                continue;
            }

            if (selected >= static_cast<int>(photosMenu.size())) {
                selected = photosMenu.size() - 1;
            }

            drawMenu(
                photosMenu,
                selected,
                "Fotos"
            );

            int key = getch();

            switch (key) {

                case KEY_UP:
                    selected--;

                    if (selected < 0) {
                        selected = photosMenu.size() - 1;
                    }

                    break;

                case KEY_DOWN:
                    selected++;

                    if (selected >= static_cast<int>(photosMenu.size())) {
                        selected = 0;
                    }

                    break;

                case '\n':
                case KEY_ENTER:
                    openPhoto(photosMenu[selected].name);
                    break;

                case 27:
                    inPhotos = false;
                    selected = 0;
                    break;
            }

            continue;
        }

        // =========================
        // MENU DE JUEGOS
        // =========================

        if (inGames) {

            drawMenu(
                gamesMenu,
                selected,
                "Juegos"
            );

        } else {

            // =========================
            // MENU PRINCIPAL
            // =========================

            drawMenu(
                mainMenu,
                selected,
                "Launcher principal"
            );
        }

        int key = getch();

        switch (key) {

            case KEY_UP:

                selected--;

                if (selected < 0) {
                    selected = inGames
                        ? gamesMenu.size() - 1
                        : mainMenu.size() - 1;
                }

                break;

            case KEY_DOWN:

                selected++;

                if (inGames &&
                    selected >= static_cast<int>(gamesMenu.size())) {
                    selected = 0;
                }

                if (!inGames &&
                    selected >= static_cast<int>(mainMenu.size())) {
                    selected = 0;
                }

                break;

            case '\n':
            case KEY_ENTER:

                if (!inGames) {

                    if (mainMenu[selected].name == "Apagar") {
                    clear();

                    int height, width;
                    getmaxyx(stdscr, height, width);

                    mvprintw(
                        height / 2 - 1,
                        (width - 25) / 2,
                        "¿Seguro que quieres apagar?"
                    );

                    mvprintw(
                        height / 2 + 1,
                        (width - 39) / 2,
                        "[ Enter ] Confirmar    [ Esc ] Cancelar"
                    );

                    refresh();

                    int confirmKey = getch();

                if (confirmKey == '\n' || confirmKey == KEY_ENTER) {
                system("sudo poweroff");
                }

    selected = 0;
}

                    else if (mainMenu[selected].name == "Juegos") {
                        inGames = true;
                        selected = 0;
                    }

                    else if (mainMenu[selected].name == "Fotos") {
                        inPhotos = true;
                        selected = 0;
                    }

                    else {
                        clear();

                        int height, width;
                        getmaxyx(stdscr, height, width);

                        std::string message =
                            mainMenu[selected].name +
                            " seleccionado";

                        mvprintw(
                            height / 2,
                            (width - message.length()) / 2,
                            "%s",
                            message.c_str()
                        );

                        mvprintw(
                            height / 2 + 2,
                            (width - 20) / 2,
                            "Pulsa una tecla..."
                        );

                        refresh();
                        getch();
                    }

                } else {

                    if (gamesMenu[selected].name == "Volver") {

                        inGames = false;
                        selected = 0;

                    } else {

                        clear();

                        int height, width;
                        getmaxyx(stdscr, height, width);

                        std::string message =
                            gamesMenu[selected].name +
                            " seleccionado";

                        mvprintw(
                            height / 2,
                            (width - message.length()) / 2,
                            "%s",
                            message.c_str()
                        );

                        mvprintw(
                            height / 2 + 2,
                            (width - 20) / 2,
                            "Pulsa una tecla..."
                        );

                        refresh();
                        getch();
                    }
                }

                break;

            case 27:

                if (inGames) {
                    inGames = false;
                    selected = 0;
                } else {
                    running = false;
                }

                break;
        }
    }

    shutdownUI();

    return 0;
}