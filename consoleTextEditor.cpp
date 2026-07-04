#include <filesystem>
#include <string>
#include <iostream>
#include <fstream>

#include <windows.h>
#include <conio.h>
#include "GapBuffer.h"

namespace fs = std::filesystem;

using string = std::string;

int main(int argc, char* argv[])
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(console, &mode);
    SetConsoleMode(console, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    
    string path;
    // Interactive mod, enter file system path for .txt file
    if (argc == 1) {
        std::cout << "Enter a path to file:" << std::endl;
        std::getline(std::cin, path);
    }
    else {
        path = string{ argv[1] };
    }

    fs::path filePath{ path };

    std::ifstream file{ filePath };

    //if (filePath.extension() != ".txt") {
    //    std::cout << "File must be a .txt!" << std::endl;
    //    return 1;
    //}

    if (!file.is_open()) {
        std::cout << "Failed to open file: " << filePath << std::endl;
        return 1;
    }

    GapBuffer editor;
    char ch;
    while (file.get(ch)) {
        editor.insert(ch);
    }

    file.close();

    std::cout << "\x1b[?1049h" << std::endl;
    volatile bool running = true;
    while (running) {
        // Render
        std::cout << "\x1b[2J" << std::endl;
        std::cout << "\x1b[1;1H";

        string text = editor.getText();
        std::cout << text;

        int row = 1;
        int column = 1;

        for (int i = 0; i < editor.getCursorPos(); ++i) {
            if (text[i] == '\n') {
                ++row;
                column = 1;
            }
            else
                ++column;
        }

        std::cout << "\x1b[" << row << ";" << column << "H";

        // Input
        int ch = _getch();

        switch (ch) {
        case 0:
        case 224: {
            int key = _getch();
            // TODO: Up/Down
            
            switch (key) {
            case 75:
                editor.left();
                break;

            case 77:
                editor.right();
                break;

            case 83:
                editor.remove_forward();
                break;
            }

            break;
        }
        case 27:    // ESC
            running = false;
            break;

        case 127:
            editor.remove_forward();
            break;

        case 8:     // Backspace
            editor.remove();
            break;

        case 13:    // Enter (Carriage Return)
            editor.insert('\n');
            break;

        default:
            editor.insert(ch);
            break;
        }

        // Update
    }
    std::cout << "\x1b[?1049l" << std::endl;

    return 0;
}