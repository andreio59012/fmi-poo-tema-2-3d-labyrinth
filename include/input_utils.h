#pragma once

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>

inline char getch() {
	termios oldt{};
	tcgetattr(STDIN_FILENO, &oldt);
	termios newt = oldt;
	newt.c_lflag &= ~static_cast<tcflag_t>(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	const char ch = static_cast<char>(getchar());
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return ch;
}
#endif