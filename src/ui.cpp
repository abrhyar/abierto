#include "ui.h"

#include <ncurses.h>
#include <locale.h>

#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <sys/utsname.h>
#include <unistd.h>
#include <vector>

static std::string getOSName() {
    std::ifstream file("/etc/os-release");

    if (!file.is_open()) {
        return "Linux";
    }

    std::string line;

    while (std::getline(file, line)) {
        if (line.rfind("PRETTY_NAME=", 0) == 0) {
            std::string value = line.substr(13);

            if (!value.empty() &&
                value.front() == '"' &&
                value.back() == '"') {
                value = value.substr(1, value.size() - 2);
            }

            return value;
        }
    }

    return "Linux";
}

static std::string getHostname() {
    char hostname[256] = {};

    if (gethostname(hostname, sizeof(hostname)) == 0) {
        return hostname;
    }

    return "abierto";
}

static std::string getKernel() {
    struct utsname info {};

    if (uname(&info) == 0) {
        return info.release;
    }

    return "desconocido";
}

static std::string getBattery() {
    const std::vector<std::string> batteries = {
        "/sys/class/power_supply/BAT0/capacity",
        "/sys/class/power_supply/BAT1/capacity"
    };

    for (const auto& path : batteries) {
        std::ifstream file(path);

        if (file.is_open()) {
            std::string value;

            std::getline(file, value);

            if (!value.empty()) {
                return value + "%";
            }
        }
    }

    return "--";
}

static std::string getConnection() {
    if (access("/sys/class/net/wlan0", F_OK) == 0) {
        return "Wi-Fi";
    }

    if (access("/sys/class/net/eth0", F_OK) == 0) {
        return "Ethernet";
    }

    return "Desconectado";
}

static std::string getDescription(const std::string& name) {
    if (name == "Juegos")
        return "Emuladores, juegos y más";

    if (name == "Música")
        return "Reproductor de audio y archivos";

    if (name == "Vídeos")
        return "Reproductor de vídeos";

    if (name == "Fotos")
        return "Visor de imágenes";

    if (name == "Cámara")
        return "Usar cámara USB";

    if (name == "Bluetooth")
        return "Dispositivos y conexiones";

    if (name == "Wi-Fi")
        return "Redes inalámbricas";

    if (name == "Navegador")
        return "Explorador web";

    if (name == "Configuración")
        return "Sistema y preferencias";

    if (name == "Salir")
        return "Cerrar Abierto";

    return "";
}

static void drawBox(int y, int x, int height, int width) {
    attron(COLOR_PAIR(1));

    mvaddch(y, x, ACS_ULCORNER);
    mvhline(y, x + 1, ACS_HLINE, width - 2);
    mvaddch(y, x + width - 1, ACS_URCORNER);

    for (int i = 1; i < height - 1; i++) {
        mvaddch(y + i, x, ACS_VLINE);
        mvaddch(y + i, x + width - 1, ACS_VLINE);
    }

    mvaddch(y + height - 1, x, ACS_LLCORNER);
    mvhline(y + height - 1, x + 1, ACS_HLINE, width - 2);
    mvaddch(y + height - 1, x + width - 1, ACS_LRCORNER);

    attroff(COLOR_PAIR(1));
}

void initUI() {
    setlocale(LC_ALL, "");

    initscr();
    cbreak();
    noecho();

    keypad(stdscr, TRUE);
    curs_set(0);

    start_color();
    use_default_colors();

    // 1 = bordes / texto secundario
    init_pair(1, COLOR_CYAN, -1);

    // 2 = selección
    init_pair(2, COLOR_WHITE, COLOR_BLUE);

    // 3 = título
    init_pair(3, COLOR_CYAN, -1);

    // 4 = texto normal
    init_pair(4, COLOR_WHITE, -1);

    // 5 = texto destacado
    init_pair(5, COLOR_CYAN, -1);
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

    int height;
    int width;

    getmaxyx(stdscr, height, width);

    /*
     * ==========================================================
     * DIMENSIONES
     * ==========================================================
     */

    const int margin = 1;
    const int topBarY = 1;
    const int contentY = 4;
    const int bottomBarHeight = 4;
    const int bottomBarY = height - bottomBarHeight - 1;

    int contentHeight = bottomBarY - contentY;

    if (contentHeight < 5) {
        contentHeight = 5;
    }

    /*
     * ==========================================================
     * BARRA SUPERIOR
     * ==========================================================
     */

    drawBox(topBarY, margin, 2, width - 2);

    attron(COLOR_PAIR(4) | A_BOLD);
    mvprintw(topBarY, 3, "ABIERTO v0.2.1");
    attroff(COLOR_PAIR(4) | A_BOLD);

    attron(COLOR_PAIR(3) | A_BOLD);

    int titleX = (width - static_cast<int>(title.length())) / 2;

    if (titleX < 20) {
        titleX = 20;
    }

    mvprintw(topBarY, titleX, "%s", title.c_str());

    attroff(COLOR_PAIR(3) | A_BOLD);

    attron(COLOR_PAIR(4));

    mvprintw(
        topBarY,
        width - 25,
        "MX"
    );

    attroff(COLOR_PAIR(4));

    /*
     * ==========================================================
     * PANEL PRINCIPAL
     * ==========================================================
     */

    int leftWidth = static_cast<int>(width * 0.68);

    if (leftWidth < 35) {
        leftWidth = width - 22;
    }

    if (leftWidth > width - 18) {
        leftWidth = width - 18;
    }

    int rightX = leftWidth + 2;
    int rightWidth = width - rightX - 1;

    if (rightWidth < 18) {
        rightWidth = 18;
        leftWidth = width - rightWidth - 3;
        rightX = leftWidth + 2;
    }

    drawBox(
        contentY,
        margin,
        contentHeight,
        leftWidth
    );

    drawBox(
        contentY,
        rightX,
        contentHeight,
        rightWidth
    );

    /*
     * ==========================================================
     * LISTA
     * ==========================================================
     */

    const int listX = margin + 2;
    const int nameX = listX + 10;
    const int descriptionX = listX + 24;

    int visibleItems = contentHeight - 3;

    int firstVisible = 0;

    if (selected >= visibleItems) {
        firstVisible = selected - visibleItems + 1;
    }

    for (
        int i = firstVisible;
        i < static_cast<int>(menu.size()) &&
        i < firstVisible + visibleItems;
        i++
    ) {
        int row = contentY + 2 + (i - firstVisible);

        bool isSelected = (i == selected);

        /*
         * Fondo de selección
         */

        if (isSelected) {
            attron(COLOR_PAIR(2));

            mvhline(
                row,
                margin + 1,
                ' ',
                leftWidth - 2
            );

            attroff(COLOR_PAIR(2));
        }

        /*
         * Nombre
         */

        if (isSelected) {
            attron(COLOR_PAIR(2) | A_BOLD);

            mvprintw(
                row,
                listX,
                "> %s",
                menu[i].name.c_str()
            );

            attroff(COLOR_PAIR(2) | A_BOLD);
        } else {
            attron(COLOR_PAIR(4));

            mvprintw(
                row,
                listX,
                "  %s",
                menu[i].name.c_str()
            );

            attroff(COLOR_PAIR(4));
        }

        /*
         * Descripción
         */

        std::string description =
            getDescription(menu[i].name);

        if (!description.empty() &&
            descriptionX < rightX - 5) {

            if (isSelected) {
                attron(COLOR_PAIR(2));
            } else {
                attron(COLOR_PAIR(5));
            }

            mvprintw(
                row,
                descriptionX,
                "%s",
                description.c_str()
            );

            if (isSelected) {
                attroff(COLOR_PAIR(2));
            } else {
                attroff(COLOR_PAIR(5));
            }
        }

        /*
         * Flecha derecha
         */

        if (isSelected) {
            attron(COLOR_PAIR(2) | A_BOLD);

            mvprintw(
                row,
                leftWidth - 3,
                ">"
            );

            attroff(COLOR_PAIR(2) | A_BOLD);
        }
    }

    /*
     * ==========================================================
     * PANEL DERECHO
     * ==========================================================
     */

    int panelX = rightX + 2;

    attron(COLOR_PAIR(3) | A_BOLD);

    mvprintw(
        contentY + 2,
        panelX,
        "      /\\"
    );

    mvprintw(
        contentY + 3,
        panelX,
        "     /  \\"
    );

    mvprintw(
        contentY + 4,
        panelX,
        "    / /\\ \\"
    );

    mvprintw(
        contentY + 5,
        panelX,
        "   /_/  \\_\\"
    );

    attroff(COLOR_PAIR(3) | A_BOLD);

    attron(COLOR_PAIR(3) | A_BOLD);

    mvprintw(
        contentY + 4,
        panelX + 12,
        "ABIERTO"
    );

    attroff(COLOR_PAIR(3) | A_BOLD);

    /*
     * Línea
     */

    attron(COLOR_PAIR(1));

    mvhline(
        contentY + 8,
        panelX,
        ACS_HLINE,
        rightWidth - 4
    );

    attroff(COLOR_PAIR(1));

    /*
     * Descripción general
     */

    attron(COLOR_PAIR(5) | A_BOLD);

    mvprintw(
        contentY + 11,
        panelX,
        "Tu dispositivo. Tus apps."
    );

    mvprintw(
        contentY + 12,
        panelX,
        "Tu mundo."
    );

    attroff(COLOR_PAIR(5) | A_BOLD);

    /*
     * Información del sistema
     */

    std::string os = getOSName();
    std::string hostname = getHostname();
    std::string kernel = getKernel();
    std::string connection = getConnection();
    std::string battery = getBattery();

    int infoY = contentY + 16;

    attron(COLOR_PAIR(4));

    mvprintw(infoY, panelX, "Sistema:");
    mvprintw(infoY + 1, panelX, "Hostname:");
    mvprintw(infoY + 2, panelX, "Kernel:");
    mvprintw(infoY + 3, panelX, "Conexion:");
    mvprintw(infoY + 4, panelX, "Bateria:");

    attroff(COLOR_PAIR(4));

    attron(COLOR_PAIR(5));

    mvprintw(infoY, panelX + 11, "%s", os.c_str());
    mvprintw(infoY + 1, panelX + 11, "%s", hostname.c_str());
    mvprintw(infoY + 2, panelX + 11, "%s", kernel.c_str());
    mvprintw(infoY + 3, panelX + 11, "%s", connection.c_str());
    mvprintw(infoY + 4, panelX + 11, "%s", battery.c_str());

    attroff(COLOR_PAIR(5));

    /*
     * ==========================================================
     * BARRA INFERIOR
     * ==========================================================
     */

    if (height >= 12) {

        drawBox(
            bottomBarY,
            margin,
            bottomBarHeight,
            width - 2
        );

        int x = 3;

        /*
         * UP / DOWN
         */

        attron(COLOR_PAIR(2) | A_BOLD);

        mvprintw(bottomBarY + 1, x, "↑↓");

        attroff(COLOR_PAIR(2) | A_BOLD);

        attron(COLOR_PAIR(4));

        mvprintw(
            bottomBarY + 1,
            x + 5,
            "Navegar"
        );

        attroff(COLOR_PAIR(4));

        x += 19;

        /*
         * A
         */

        attron(COLOR_PAIR(2) | A_BOLD);

        mvprintw(bottomBarY + 1, x, "[ A ]");

        attroff(COLOR_PAIR(2) | A_BOLD);

        attron(COLOR_PAIR(4));

        mvprintw(
            bottomBarY + 1,
            x + 7,
            "Seleccionar"
        );

        attroff(COLOR_PAIR(4));

        x += 25;

        /*
         * B
         */

        attron(COLOR_PAIR(2) | A_BOLD);

        mvprintw(bottomBarY + 1, x, "[ B ]");

        attroff(COLOR_PAIR(2) | A_BOLD);

        attron(COLOR_PAIR(4));

        mvprintw(
            bottomBarY + 1,
            x + 7,
            "Atras / Menu"
        );

        attroff(COLOR_PAIR(4));

        x += 25;

        /*
         * START
         */

        attron(COLOR_PAIR(2) | A_BOLD);

        mvprintw(bottomBarY + 1, x, "[START]");

        attroff(COLOR_PAIR(2) | A_BOLD);

        attron(COLOR_PAIR(4));

        mvprintw(
            bottomBarY + 1,
            x + 9,
            "Menu rapido"
        );

        attroff(COLOR_PAIR(4));

        /*
         * Segunda línea
         */

        attron(COLOR_PAIR(1));

        mvprintw(
            bottomBarY + 2,
            3,
            "SELECT  Opciones"
        );

        attroff(COLOR_PAIR(1));
    }

    refresh();
}