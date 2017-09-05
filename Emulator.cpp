#include "Emulator.h"

Emulator::Emulator() : chip8(), window(sf::VideoMode(640,320), "Chip8 Emulator"), pixelSize(10), pixel(sf::Vector2f(pixelSize, pixelSize)) {
    window.setFramerateLimit(60);
}

Emulator::~Emulator(){
    window.close();
}

void Emulator::drawPixel(size_t x, size_t y){
    pixel.setPosition(float(x*pixelSize), float(y*pixelSize));
    window.draw(pixel);
}

void Emulator::getKeyState(Chip8& chip8){
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

void Emulator::handleEvents(){
    sf::Event event;
    while(window.pollEvent(event)){
        switch(event.type){
        case sf::Event::Closed:
            window.close();
        break;
        case sf::Event::KeyPressed:
            if(event.key.code == sf::Keyboard::Escape)
                window.close();
        break;
        }
    }
}

void Emulator::loadFile(const std::string& filename){
    chip8.loadFile(filename);
}

void Emulator::run(){
    while(window.isOpen()){
        handleEvents();

        chip8.emulateCycle();
        if(chip8.displayFlag){
            window.clear();
            auto display = chip8.getDisplay();
            for(size_t y = 0; y < 32; ++y)
                for(size_t x = 0; x < 64; ++x)
                    if(display[y][x])
                        drawPixel(x, y);

            chip8.displayFlag = false;
            window.display();
        }
        getKeyState(chip8);
    }
}
