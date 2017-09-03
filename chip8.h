#ifndef CHIP8_H
#define CHIP8_H

#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

const uint8_t chip8_characters[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, //0
    0x20, 0x60, 0x20, 0x20, 0x70, //1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
    0x90, 0x90, 0xF0, 0x10, 0x10, //4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
    0xF0, 0x10, 0x20, 0x40, 0x40, //7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
    0xF0, 0x90, 0xF0, 0x90, 0x90, //A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
    0xF0, 0x80, 0x80, 0x80, 0xF0, //C
    0xE0, 0x90, 0x90, 0x90, 0xE0, //D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
    0xF0, 0x80, 0xF0, 0x80, 0x80  //F
};

class Chip8{
public:
    Chip8();
    ~Chip8();

    bool displayFlag;

    void emulate();
    uint8_t (&getDisplay())[32][64];
    bool loadGame();
    void loadFile(const std::string& filename);
    void setKeyState(uint8_t key, bool state);
private:
    uint8_t memory[4096]; //Chip8 has 4KB of RAM
    uint8_t display[32][64]; //Chip8 used 64*32 (64 pixels height and 32 pixels width) monochrome display
    uint8_t V[16]; //Chip8 has 16 8-bit registers named from V0 to VF
    uint16_t I; //Chip8 has 1 16 bits address register named I (Index Register)
    uint8_t delay_timer; //Timer used for timing the events of games
    uint8_t sound_timer; //Timer used for sound effects. Beeps when its value is nonzero
    uint16_t opcode; //Used for storing the current opcode - Chip8 has total 35 opcodes which are 2 bytes long
    uint16_t pc; //Chip8 has 16-bit program counter used to store the currently executing address
    uint16_t stack[16]; //Chip8' stack is an array of 16 16-bit values used to store the address that the interpreter should return to when finished with a subroutine
    uint8_t stack_pointer; //Chip8 has 8-bit stack pointer which points to the topmost level of the stack
    uint8_t keys[16]; //Chip8 has 16-key hexadecimal keypad, this array hold their state (pressed or not pressed)
};

#endif // CHIP8_H
