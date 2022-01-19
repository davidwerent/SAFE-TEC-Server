#pragma once
#include <Windows.h>
#define RED SetConsoleTextAttribute(hConsole, FOREGROUND_RED)
#define WHITE SetConsoleTextAttribute(hConsole, FOREGROUND_RED   | FOREGROUND_GREEN | FOREGROUND_BLUE)
#define YELLOW SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN)
#define GREEN SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN)
#define CYAN SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE)
void change_color(const int color_flags);
class Interface
{
};

