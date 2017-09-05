#include "Emulator.h"

int main(int argc, char* argv[]){
    Emulator emulator;
    if(argc != 2){
        std::cout<<"Usage: chip8 <filename>"<<std::endl;
        return 0;
    }
    emulator.loadFile(argv[1]);
    emulator.run();
}
