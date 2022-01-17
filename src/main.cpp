// AUTHOR: Krishnendu Marathe
// PROGRAM: ticTacToe

/*
** 							TICTACTOE
** 							---------
** 	  TicTacToe Game for Terminal for Windows and Linux Platform.
**
** 					 	  	 License
** 							 -------
**
** 	  This program is free software: you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation, either version 3 of the License, or
**    (at your option) any later version.
**
**    This program is distributed in the hope that it will be useful,
**    but WITHOUT ANY WARRANTY; without even the implied warranty of
**    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
**    GNU General Public License for more details.
**
**    You should have received a copy of the GNU General Public License
**    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <thread>
#include "tictactoe.h"

void command_input(TERMINAL_HANDLER* window, char* command, int* stat)
{
	while (true)
	{
		usleep(100000);
		*command = get_key_presses();
		if (window->thread_exit || (*command == '`' && *stat == 1))
		{
			window->thread_exit = true;
			break;
		}
	}

	// In case Input Thread exits Late
	// Clear Screen
	system(CLEAR);
}

// Main Driver
int main()
{
	// Terminal Handler Instance
	TERMINAL_HANDLER*  display_handler;
	// Number of Blocks for TicTacToe = 9
	display_handler = new TERMINAL_HANDLER(9);

	#if defined(__linux__)
	struct termios terminalProps = {0};
	setup_linux_terminal(&terminalProps);
	#endif

	char input;

	// Highlight Array
	int highlight_x = 1;
	int highlight_y = 1;
	int h_array[3][3] = {
		{0, 1, 2},
		{3, 4, 5},
		{6, 7, 8}
	};

	int userID = 1; // Player One

	// Game Status
	/* 0 = idle
	** 1 = start
	** 2 = tie
	** 3 = player one wins
	** 4 = player two wins
	*/
	int game_state = 1;

	// Input Thread
	std::thread read_input (command_input, display_handler, &input, &game_state);

	// fixes a bug where ASCII color is not detected in windows
	bool OneTimeRefresh = true;
	// Program/Game Loop
	while (true)
	{
		if (OneTimeRefresh)
		{
			OneTimeRefresh = false;
			system(CLEAR);
		}
		usleep(100000);

		if (input == '`')
		{
			display_handler->thread_exit = true;
			break;
		}

		if (!display_handler->thread_exit)
		{
			display_handler->set_terminal_frame(userID, false, game_state);
			display_handler->update_blocks(h_array[highlight_y][highlight_x]);
			display_handler->draw_frame();
		}

		// Highligh Updates
		switch(input)
		{
			case 'w':
				if (highlight_y == 0)
				{
					highlight_y = 2;
				} else
				{
					highlight_y--;
				}
				break;
			case 'a':
				if (highlight_x == 0)
				{
					highlight_x = 2;
				} else
				{
					highlight_x--;
				}
				break;
			case 's':
				highlight_y = (highlight_y+1)%3;
				break;
			case 'd':
				highlight_x = (highlight_x+1)%3;
				break;
			case ' ':
				if (display_handler->set_BlockUid(h_array[highlight_y][highlight_x], userID))
				{
					if (userID == 1) { userID = 2; }
					else { userID = 1; }
				}
			default:
				break;
		}

		// Reset Input
		if (input != '`') { input = 0; }

		// check game status
		game_state = display_handler->check_game_status();

		// Game End
		if (game_state > 1)
		{
			display_handler->clear_game_data();

			while (true)
			{
				usleep(100000);

				if (input == 'f')
				{
					game_state = 1;
					break;
				}

				display_handler->set_terminal_frame(userID, true, game_state);
				display_handler->draw_frame();
			}

			highlight_x = 1;
			highlight_y = 1;
		}
	}

	// wait for input thread
	read_input.join();

	// In case Input thread finished early
    // Clear Screen
    system(CLEAR);
	// Delete Terminal Handler Instance
	display_handler->dissolve_terminal_frame();
	delete display_handler;

	#if defined(__linux__)
	restore_linux_terminal(&terminalProps);
	#endif

	return 0;
}
