#include "Interface.h"
void change_color(const int color_flags)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color_flags);
}