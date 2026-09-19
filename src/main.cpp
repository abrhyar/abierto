#include "menu.h"
#include "ui.h"

#include <ncurses.h>
#include <vector>
#include <string>

int main() {
    initUI();

    std::vector<MenuItem> mainMenu = getMainMenu();
    std::vector<MenuItem> gamesMenu = getGamesMenu();

    bool running = true;
    bool inGames = false;

    int selected = 0;

    while (running) {

        if (inGames) {
            drawMenu(gamesMenu, selected, "Juegos");
        } else {
            drawMenu(mainMenu, selected, "Launcher principal");
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

                    if (mainMenu[selected].name == "Salir") {
                        running = false;
                    }

                    else if (mainMenu[selected].name == "Juegos") {
                        inGames = true;
                        selected = 0;
                    }

                    else {
                        clear();

                        int height, width;
                        getmaxyx(stdscr, height, width);

                        std::string message =
                            mainMenu[selected].name + " seleccionado";

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
                    }

                    else {
                        clear();

                        int height, width;
                        getmaxyx(stdscr, height, width);

                        std::string message =
                            gamesMenu[selected].name + " seleccionado";

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

            case 27: // ESC

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