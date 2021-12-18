#ifndef TICTACTOE_H
#define TICTACTOE_H

// Standard Libraries
#include <iostream>
#include <unistd.h>

#if defined(_WIN32)
#include <windows.h>
#include <conio.h>
#define CLEAR "cls"
#elif __linux__
#include <sys/ioctl.h>
#include <termios.h>

void setup_linux_terminal(struct termios* old)
{
	if (tcgetattr(0, old) < 0)
	{
		perror("tcgetattr()");
	}
	old->c_lflag &= ~ICANON;
    old->c_lflag &= ~ECHO;
    old->c_cc[VMIN] = 1;
    old->c_cc[VTIME] = 0;
	if (tcsetattr(0, TCSANOW, old) < 0)
	{
		perror("tcsetattr ICANON");
	}
}

void restore_linux_terminal(struct termios* old)
{
	old->c_lflag |= ICANON;
    old->c_lflag |= ECHO;
	if (tcsetattr(0, TCSADRAIN, old) < 0)
	{
		perror("tcsetattr ~ICANON");
	}
}

#define CLEAR "clear"
#endif

char get_key_presses()
{
	/* FROM:
	** https://stackoverflow.com/questions/421860/capture-characters-from-standard-input-without-waiting-for-enter-to-be-pressed
	*/
	char character = 0;
	#if defined(_WIN32)
	character = _getch();
	#elif __linux__
	if (read(0, &character, 1) < 0)
	{
		perror("read()");
	}
	#endif

	return character;
}

// Terminal Handler
class TERMINAL_HANDLER
{
private:
	int terminalHeight = 0;
	int terminalWidth = 0;
	// Get Current Terminal Dimension
	void get_terminal_dimension();

public:
	// input thread status
	bool thread_exit = false;
};

void TERMINAL_HANDLER::get_terminal_dimension()
{
	/* FROM:
	** https://stackoverflow.com/questions/23369503/get-size-of-terminal-window-rows-columns
	*/
	#if defined(_WIN32)
	CONSOLE_SCREEN_BUFFER_INFO terminalWindow;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &terminalWindow);
	this->terminalWidth = terminalWindow.srWindow.Right-terminalWindow.srWindow.Left+1;
	this->terminalHeight = terminalWindow.srWindow.Bottom-terminalWindow.srWindow.Top+1;
	#elif __linux__
	struct winsize terminalWindow;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminalWindow);
	this->terminalWidth = terminalWindow.ws_col;
	this->terminalHeight = terminalWindow.ws_row;
	#endif
}

#endif // TICTACTOE_H
