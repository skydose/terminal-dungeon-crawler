#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <iostream>
// Include the correct OS libraries automatically
#ifdef _WIN32
    #include <conio.h>
#else
    #include <termios.h>
    #include <unistd.h>
#endif

class InputManager {
public:
    // Captures a single keystroke instantly (No 'Enter' required)
    static inline char getChar() {
#ifdef _WIN32
        char ch;
        do {
            ch = _getch();
        } while (ch == '\n' || ch == '\r'); // Ignore lingering 'Enter' keys
        return ch;
#else
        char buf = 0;
        struct termios old_tio, new_tio;

        // Get current terminal settings
        tcgetattr(STDIN_FILENO, &old_tio);
        new_tio = old_tio;
        
        // Disable canonical mode (buffered i/o) and local echo (hides the pressed key)
        new_tio.c_lflag &=(~ICANON & ~ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

        // Read a single character, looping if it's an accidental 'Enter' key
        do {
            read(STDIN_FILENO, &buf, 1);
        } while (buf == '\n' || buf == '\r');

        // Restore old terminal settings
        tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);

        return buf;
#endif
    }
    
    // A replacement for the clunky "Press Enter to continue" cin.ignore() logic
    static inline void pause() {
        std::cout << "\nPress any key to continue..." << std::flush;
        getChar(); 
    }
};

#endif