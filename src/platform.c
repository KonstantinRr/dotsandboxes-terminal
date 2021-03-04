/// MIT License
/// 
/// Copyright (c) 2021 Konstantin Rolf
/// 
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
/// 
/// The above copyright notice and this permission notice shall be included in all
/// copies or substantial portions of the Software.
/// 
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.

#include <platform.h>

#include <stdio.h>
#include <stdlib.h>

#if defined(WIN32) || defined(WIN64)

#include <windows.h>
#include <conio.h> // defines _getch

void initTerminal()
{
    SetConsoleOutputCP(65001U);
}

int getChar(void)
{
    return _getch();
}

void clearScreen(void)
{
    system("cls");
}

#elif __unix__ || __linux__

#include <termios.h>
#include <stdio.h>

static struct termios old, current;

void initTerminal()
{

}

static void initTermios(int echo)
{
    tcgetattr(0, &old);
    current = old;
    current.c_lflag &= ~ICANON;
    if (echo)
    {
        current.c_lflag |= ECHO;
    }
    else
    {
        current.c_lflag &= ~ECHO;
    }
    tcsetattr(0, TCSANOW, &current);
}

static void resetTermios(void)
{
    tcsetattr(0, TCSANOW, &old);
}

static char getch_(int echo)
{
    char ch;
    initTermios(echo);
    ch = getchar();
    resetTermios();
    return ch;
}

int getChar(void)
{
    return (int)getch_(0);
}

void clearScreen(void)
{
    system("clear");
}

#endif

void pause(void)
{
    printf("Press any key to continue!\n");
    getChar();
}