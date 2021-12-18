#ifndef TICTACTOE_H
#define TICTACTOE_H

// Standard Libraries
#include <iostream>
#include <unistd.h>

#if defined(_WIN32)
#include <windows.h>
#elif __linux__
#include <sys/ioctl.h>
#endif

// Terminal Handler
class TERMINAL_HANDLER
{
private:
	//

public:
	int terminalHeight = 0;
	int terminalWidth = 0;
	// Get Current Terminal Dimension
	void get_terminal_dimension();
};

void TERMINAL_HANDLER::get_terminal_dimension()
{
	#if defined(_WIN32)
	CONSOLE_SCREEN_BUFFER_INFO terminalWindow;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &terminalWindow);
	this->terminalWidth = terminalWindow.srWindow.Right-terminalWindow.srWindow.Left+1;
	this->terminalHeight = terminalWindow.srWindow.Bottom-terminalWindow.srWindow.Top+1;
	#elif __linux__
	struct winsize terminalWindow;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminalWindow);
	this->terminalWidth = terminalWindow.ws_col;
	this->terminalHeight = terminalHeight.ws_row;
	#endif
}

#endif // TICTACTOE_H
