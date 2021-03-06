#include <filesystem>
#include <iostream>
#include <random>
#include <ctime>

#include "Game.h"

Game game;

int main(int argc, char** argv) {
    srand(time(NULL));
    if (argc < 2 || !std::filesystem::exists(argv[1])) {
        std::cout << "Please, provide a valid level file directory\n";
        return 0;
    }

    game = Game();

    int window_width = 1000;
    int window_height = 1000;
    game.init(window_width, window_height, "Trabalho 3D", &argc, argv);
    game.run();
}