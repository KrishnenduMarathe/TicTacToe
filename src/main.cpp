#include <thread>
#include "tictactoe.h"

// Main Driver
int main()
{
	// Terminal Handler Instance
	TERMINAL_HANDLER*  display_handler;
	display_handler = new TERMINAL_HANDLER();

	while (true)
	{
		display_handler->get_terminal_dimension();
		std::cout << "Terminal: h = " << display_handler->terminalHeight << ", w = " << display_handler->terminalWidth << std::endl;
	}

	// Delete Terminal Handler Instance
	delete display_handler;

	return 0;
}
