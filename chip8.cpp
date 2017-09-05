#include "chip8.h"

//No argument constructor initializes the system
Chip8::Chip8(){
    srand(time(NULL)); //Seed the random number generator
    reset(); //Reset the system
}

Chip8::~Chip8() {}

void Chip8::clearScreen(){
    //Clear the display
    for(size_t width = 0; width < 32; width++)
        for(size_t height = 0; height < 64; height++)
            display[width][height] = 0;
    displayFlag = true;
}

void Chip8::emulateCycle(){
    //Fetch opcode
    //Opcode is 2 bytes long so we need to merge two successive bytes of memory
    opcode = memory[pc] << 8 | memory[pc+1];
    //Decode and execute opcode
    switch(opcode & 0xF000){
    case 0x0000:
        switch(opcode & 0x000F){
        case 0x0000: //opcode 0x00E0: Clears the screen.
            clearScreen();
            pc += 2; //increase program counter
        break;
        case 0x000E: //opcode 0x00EE: Returns from a subroutine.
            pc = stack[--stack_pointer];
            pc += 2; //increase program counter
        break;
        default: std::cerr<<"Unknown opcode "<<std::hex<<opcode<<std::endl;
        break;
        }
    break;
    case 0x1000: //opcode 0x1NNN: Jumps to address NNN.
        pc = opcode & 0x0FFF;
    break;
    case 0x2000: //opcode 0x2NNN: Calls subroutine at NNN.
        stack[stack_pointer++] = pc;
        pc = opcode & 0x0FFF;
    break;
    case 0x3000: //opcode 0x3XNN: Skips the next instruction if VX equals NN. (Usually the next instruction is a jump to skip a code block)
        if(V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
            pc += 4;
        else
            pc += 2;
    break;
    case 0x4000: //opcode 0x4XNN: Skips the next instruction if VX doesn't equal NN. (Usually the next instruction is a jump to skip a code block)
        if(V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
            pc += 4;
        else
            pc += 2;
    break;
    case 0x5000: //opcode 0x5XY0: Skips the next instruction if VX equals VY. (Usually the next instruction is a jump to skip a code block)
        if(V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
            pc += 4;
        else
            pc += 2;
    break;
    case 0x6000: //opcode 0x6XNN: Sets VX to NN.
        V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
        pc += 2;
    break;
    case 0x7000: //opcode 0x7XNN: Adds NN to VX.
        V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
        pc += 2;
    break;
    case 0x8000:
        switch(opcode & 0x000F){
        case 0x0000: //opcode 0x8XY0: 	Sets VX to the value of VY.
            V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
            pc += 2;
        break;
        case 0x0001: //opcode 0x8XY1: Sets VX to VX or VY. (Bitwise OR operation)
            V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
            pc += 2;
        break;
        case 0x0002: //opcode 0x8XY2: Sets VX to VX and VY. (Bitwise AND operation)
            V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
            pc += 2;
        break;
        case 0x0003: //opcode 0x8XY3: Sets VX to VX xor VY.
            V[(opcode & 0x0F00) >> 8] ^= V[(opcode & 0x00F0) >> 4];
            pc += 2;
        break;
        case 0x0004: //opcode 0x8XY4: Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't.
            V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
            V[0xF] = (V[(opcode & 0x0F00) >> 8] > 255) ? 1 : 0;
            pc += 2;
        break;
        case 0x0005: //opcode 0x8XY5: VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
            V[0xF] = (V[(opcode & 0x00F0) >> 4] > V[(opcode & 0x0F00) >> 8]) ? 0 : 1;
            V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
            pc += 2;
        break;
        case 0x0006: //opcode 0x8XY6: Shifts VX right by one. VF is set to the value of the least significant bit of VX before the shift.
            V[0xF] = V[(opcode & 0x0F00) >> 8] & 0x1;
            V[(opcode & 0x0F00) >> 8] >>= 1;
            pc += 2;
        break;
        case 0x0007: //opcode 0x8XY7: Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
            V[0xF] = (V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4]) ? 0 : 1;
            V[(opcode & 0x0F00) >> 8] =  V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
            pc += 2;
        break;
        case 0x000E: //opcode 0x8XYE: Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift.
            V[0xF] = (V[(opcode & 0x0F00) >> 8]) >> 7;
            V[(opcode & 0x0F00) >> 8] <<= 1;
            pc += 2;
        break;
        default: std::cerr<<"Unknown opcode "<<std::hex<<opcode<<std::endl;
        break;
        }
    break;
    case 0x9000: //opcode 9XY0: Skips the next instruction if VX doesn't equal VY. (Usually the next instruction is a jump to skip a code block)
        if(V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4])
            pc += 4;
        else
            pc += 2;
    break;
    case 0xA000: //opcode ANNN: Sets I to the address NNN.
        I = opcode & 0x0FFF;
        pc += 2;
    break;
    case 0xB000: //opcode BNNN: Jumps to the address NNN plus V0.
        pc = V[0] + (opcode & 0x0FFF);
    break;
    case 0xC000: //opcode 0xCXNN: Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN.
        V[(opcode & 0x0F00) >> 8] = (rand() % 256) & (opcode & 0x00FF);
        pc += 2;
    break;
    case 0xD000:{ //opcode 0xDXYN: Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels. Each row of 8 pixels is read as bit-coded starting from memory location I; I value doesn’t change after the execution of this instruction. As described above, VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, and to 0 if that doesn’t happen
        uint16_t x = V[(opcode & 0x0F00) >> 8];
        uint16_t y = V[(opcode & 0x00F0) >> 4];
        uint16_t height = opcode & 0x000F;
        uint16_t pixel;
        V[0xF] = 0;
        for(uint8_t yline = 0; yline < height; yline++){
            pixel = memory[I + yline];
            for(uint8_t xline = 0; xline < 8; xline++){
                if(pixel & (0x80 >> xline)){
                    if(display[y + yline][x + xline])
                        V[0xF] = 1;
                    display[y + yline][x + xline] ^= 1;
                }
            }
        }
        displayFlag = true;
        pc += 2;
    }
    break;
    case 0xE000:
        switch(opcode & 0x000F){
        case 0x000E: //opcode 0xEX9E: Skips the next instruction if the key stored in VX is pressed. (Usually the next instruction is a jump to skip a code block)
            if(keys[V[(opcode & 0x0F00) >> 8]])
                pc += 4;
            else
                pc += 2;
        break;
        case 0x0001: //opcode 0xEXA1: Skips the next instruction if the key stored in VX isn't pressed. (Usually the next instruction is a jump to skip a code block)
            if(!keys[V[(opcode & 0x0F00) >> 8]])
                pc += 4;
            else
                pc += 2;
        break;
        default: std::cerr<<"Unknown opcode "<<std::hex<<opcode<<std::endl;
        break;
        }
        break;
    case 0xF000:
        switch(opcode & 0x000F){
        case 0x0007: //opcode 0xFX07: Sets VX to the value of the delay timer.
            V[(opcode & 0x0F00) >> 8] = delay_timer;
            pc += 2;
        break;
        case 0x000A:{ //opcode 0xFX0A: A key press is awaited, and then stored in VX. (Blocking Operation. All instruction halted until next key event)
            bool isPressed = false;
            for(size_t i = 0; i < 16; i++){
                if(keys[i]){
                    V[(opcode & 0x0F00) >> 8] = i;
                    isPressed = true;
                    break;
                }
            }
            if(isPressed == false) return;
            pc += 2;
        }
        break;
        case 0x0005:
            switch(opcode & 0x00F0){
            case 0x0010: //opcode 0xFX15: Sets the delay timer to VX.
                delay_timer = V[(opcode & 0x0F00) >> 8];
                pc += 2;
            break;
            case 0x0050: //opcode 0xFX55: Stores V0 to VX (including VX) in memory starting at address I.
                for(size_t i = 0; i <= ((opcode & 0x0F00) >> 8); i++)
                    memory[I+i] = V[i];
                I += ((opcode & 0x0F00) >> 8) + 1;
                pc += 2;
            break;
            case 0x0060: //opcode 0xFX65: Fills V0 to VX (including VX) with values from memory starting at address I.
                for(size_t i = 0; i <= ((opcode & 0x0F00) >> 8); i++)
                    V[i] = memory[I+i];
                I += ((opcode & 0x0F00) >> 8) + 1;
                pc += 2;
            break;
            default: std::cerr<<"Unknown opcode "<<std::hex<<opcode<<std::endl;
            break;
            }
        break;
        case 0x0008: //opcode 0xFX18: Sets the sound timer to VX.
            sound_timer = V[(opcode & 0x0F00) >> 8];
            pc += 2;
        break;
        case 0x000E: //opcode 0xFX1E: Adds VX to I.
            I += V[(opcode & 0x0F00) >> 8];
            pc += 2;
        break;
        case 0x0009: //opcode 0xFX29: Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font.
            I = V[(opcode & 0x0F00) >> 8]*5;
            pc += 2;
        break;
        case 0x0003: //opcode 0xFX33: Stores the binary-coded decimal representation of VX, with the most significant of three digits at the address in I, the middle digit at I plus 1, and the least significant digit at I plus 2. (In other words, take the decimal representation of VX, place the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2.)
            memory[I] = V[(opcode & 0x0F00) >> 8] / 100;
            memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
            memory[I + 2] = V[(opcode & 0x0F00) >> 8] % 10;
            pc += 2;
        break;
        }
    break;
    default: std::cerr<<"Unknown opcode "<<std::hex<<opcode<<std::endl;
    break;
    }
    //Check timers
    if(delay_timer > 0) delay_timer--;
    if(sound_timer > 0){
        //std::cout<<"BEEP"<<char(7)<<std::endl;
        sound_timer--;
    }
    //DEBUG
    /*std::cout<<"Program counter: "<<pc<<std::endl;
    std::cout<<"Index register: "<<I<<std::endl;
    std::cout<<"Stack Pointer: "<<stack_pointer<<std::endl;
    for(size_t i = 0; i < 16; i++){
        std::cout<<"V["<<i<<"]="<<V[i]<<std::endl;
    }
    for(size_t i = 0; i < 16; i++){
        std::cout<<"stack["<<i<<"]="<<stack[i]<<std::endl;
    }
    for(size_t i = 0; i < 16; i++){
        std::cout<<"keys["<<i<<"]="<<keys[i]<<std::endl;
    }*/

}

uint8_t (&Chip8::getDisplay())[32][64]{
    return display;
}

void Chip8::loadFile(const std::string& filename){
    std::cout<<"Loading file "<<filename<<"..."<<std::endl;
    std::ifstream file(filename, std::ios::binary | std::ios::in | std::ios::ate); //Open file in binary mode and seek to the end of it
    if(file.is_open() == false){
        std::cerr<<"Can't open "<<filename<<std::endl;
        return;
    }
    unsigned int filesize = file.tellg(); //Find file size
    if(filesize > 0xFFF-0x200){
        std::cerr<<"File size too big to feet in memory!"<<std::endl;
        return;
    }
    char* data = (char*)(&memory[0x200]); //Create pointer to memory location at 0x200
    file.seekg(0, std::ios::beg); //Seek to the beginning of file
    file.read(data, filesize); //Read the file into memory location at 0x200
    file.close(); //Close the file
    std::cout<<"Done."<<std::endl;
}

void Chip8::reset(){
     //First clear the memory
    for(size_t i = 0; i < 4096; i++)
        memory[i] = 0;
    //Now load the fontset into memory
    for(size_t i = 0; i < 80; i++)
        memory[i] = chip8_characters[i];

    pc = 0x200; //Program counter starts at 0x200
    I = 0; //Reset the Index Register
    stack_pointer = 0; //Reset the Stack Pointer
    opcode = 0; //Set current opcode to zero

    delay_timer = 0; //Reset the delay timer
    sound_timer = 0; //Reset the sound timer

    //Reset the registers
    for(size_t i = 0; i < 16; i++)
        V[i] = 0;

    //Reset the stack
    for(size_t i = 0; i < 16; i++)
        stack[i] = 0;

    //Set the keypad keys state to not pressed
    for(size_t i = 0; i < 16; i++)
        keys[i] = 0;

    //Clear screen
    clearScreen();
}

void Chip8::setKeyState(size_t key, bool state){
    if(state) keys[key] = 1;
    else      keys[key] = 0;
}
