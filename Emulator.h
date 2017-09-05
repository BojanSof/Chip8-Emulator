#ifndef EMULATOR_H
#define EMULATOR_H

#include "chip8.h"
#include <SFML/Graphics.hpp>
#include <string>

class Emulator{
public:
    Emulator();
    ~Emulator();

    void drawPixel(size_t x, size_t y);
    void handleEvents();
    void loadFile(const std::string& filename);
    void run();
    void getKeyState(Chip8 &chip8);
private:
    Chip8 chip8;
    sf::RenderWindow window;
    unsigned int pixelSize;
    sf::RectangleShape pixel;
};

#endif // EMULATOR_H
