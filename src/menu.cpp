#include "menu.h"

std::vector<MenuItem> getMainMenu() {
    return {
        {"Juegos"},
        {"Música"},
        {"Vídeos"},
        {"Fotos"},
        {"Cámara"},
        {"Bluetooth"},
        {"Wi-Fi"},
        {"Configuración"},
        {"Apagar"}
    };
}

std::vector<MenuItem> getGamesMenu() {
    return {
        {"Emuladores"},
        {"Juegos"},
        {"Configuración"},
        {"Volver"}
    };
}