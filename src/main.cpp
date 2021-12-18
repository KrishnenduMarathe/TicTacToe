#include <thread>
#include "tictactoe.h"

void command_input(TERMINAL_HANDLER* window, char* command)
{
	while (true)
	{
		usleep(100000);
		*command = 0;
		*command = get_key_presses();
		if (window->thread_exit || *command == '`')
		{
			window->thread_exit = true;
			break;
		}
	}

	// In case Input Thread exits Late
	// Clear Screen
	system(CLEAR);

	// DEBUG
	//std::cout << "Thread Exited\n";
}

// Main Driver
int main()
{
	// Terminal Handler Instance
	TERMINAL_HANDLER*  display_handler;
	display_handler = new TERMINAL_HANDLER();

	#if defined(__linux__)
	struct termios terminalProps = {0};
	setup_linux_terminal(&terminalProps);
	#endif

	char input;
	// Input Thread
	std::thread read_input (command_input, display_handler, &input);

	// Program/Game Loop
	while (true)
	{
		usleep(100000);

		// DEBUG
		//std::cout << "Key: " << input << std::endl;

		if (input == '`')
		{
			// In case input thread exits early
			system(CLEAR);

			// DEBUG
			//std::cout << "Main Loop Exiting\n";
			break;
		}

		if (!display_handler->thread_exit)
		{
			display_handler->set_terminal_frame();
			display_handler->draw_frame();
		}

		std::flush(std::cout);
	}

	// wait for input thread
	read_input.join();

	// Delete Terminal Handler Instance
	display_handler->dissolve_terminal_frame();
	delete display_handler;

	#if defined(__linux__)
	restore_linux_terminal(&terminalProps);
	#endif

	return 0;
}
