#include "chip8.h"
#include <SFML/Graphics.hpp>

const int PixelSize = 13;

void getKeyState(Chip8& chip8){
    chip8.setKeyState(0x1, sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1));
    chip8.setKeyState(0x2, sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2));
    chip8.setKeyState(0x3, sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3));
    chip8.setKeyState(0xC, sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4));

    chip8.setKeyState(0x4, sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q));
    chip8.setKeyState(0x5, sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W));
    chip8.setKeyState(0x6, sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E));
    chip8.setKeyState(0xD, sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R));

    chip8.setKeyState(0x7, sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A));
    chip8.setKeyState(0x8, sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S));
    chip8.setKeyState(0x9, sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D));
    chip8.setKeyState(0xE, sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F));

    chip8.setKeyState(0xA, sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z));
    chip8.setKeyState(0x0, sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X));
    chip8.setKeyState(0xB, sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C));
    chip8.setKeyState(0xF, sf::Keyboard::isKeyPressed(sf::Keyboard::Key::V));
}

int main(int argc, char* argv[])
{
    Chip8 chip8;
    if(argc != 2)
        chip8.loadFile("D:\\Projects\\C++\\Chip8\\c8games\\INVADERS");
    else
        chip8.loadFile(argv[1]);
    sf::RenderWindow window(sf::VideoMode(832,416), "CHIP-8 Emulator");
    window.setFramerateLimit(20);
    sf::RectangleShape rectangle(sf::Vector2f(PixelSize,PixelSize));
    while(window.isOpen()){
        //Handle Events
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
                window.close();
        }
        //Update window
        getKeyState(chip8);
        chip8.emulate();
        if(chip8.displayFlag){
            window.clear();
            auto display = chip8.getDisplay();
            for(size_t y=0; y < 32; ++y){
                for(size_t x=0; x < 64; ++x){
                    if(display[y][x]){
                        rectangle.setPosition((float)(x*PixelSize), (float)(y*PixelSize));
                        window.draw(rectangle);
                    }
                }
            }
        //Render Window
        window.display();
        chip8.displayFlag = false;
        }
    }
}

