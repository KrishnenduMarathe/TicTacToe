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

// Ansi Color Codes
/*FROM:
** https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797
*/
namespace COLOR
{
	const std::string reset = "\u001b[0m";

	// Default Colors
	const std::string fg_default = "39";
	const std::string bg_default = "49";

	// Foreground Colors
	const std::string fg_black = "30";
	const std::string fg_red = "31";
	const std::string fg_green = "32";
	const std::string fg_yellow = "33";
	const std::string fg_blue = "34";
	const std::string fg_magenta = "35";
	const std::string fg_cyan = "36";
	const std::string fg_white = "37";

	const std::string fg_black_b = "90";
	const std::string fg_red_b = "91";
	const std::string fg_green_b = "92";
	const std::string fg_yellow_b = "93";
	const std::string fg_blue_b = "94";
	const std::string fg_magenta_b = "95";
	const std::string fg_cyan_b = "96";
	const std::string fg_white_b = "97";

	// Background Colors
	const std::string bg_black = "40";
	const std::string bg_red = "41";
	const std::string bg_green = "42";
	const std::string bg_yellow = "43";
	const std::string bg_blue = "44";
	const std::string bg_magenta = "45";
	const std::string bg_cyan = "46";
	const std::string bg_white = "47";

	const std::string bg_black_b = "100";
	const std::string bg_red_b = "101";
	const std::string bg_green_b = "102";
	const std::string bg_yellow_b = "103";
	const std::string bg_blue_b = "104";
	const std::string bg_magenta_b = "105";
	const std::string bg_cyan_b = "106";
	const std::string bg_white_b = "107";

	// Unicode Sequences
	const std::string unicode_escape = "\u001b[";
	const std::string separator = ";";
	const std::string unicode_end = "m";

	// Other Decorations
	const std::string reset_code = "0";
	const std::string bold_on = "1";
	const std::string faint_on = "2";
	const std::string italic_on = "3";
	const std::string underline_on = "4";
	const std::string blinking_on = "5";
	const std::string inverse_on = "7";
	const std::string hidden_on = "8";
	const std::string strikethrough_on = "9";
	const std::string bold_off = "22";
	const std::string faint_off = "22";
	const std::string italic_off = "23";
	const std::string underline_off = "24";
	const std::string blinking_off = "25";
	const std::string inverse_off = "27";
	const std::string hidden_off = "28";
	const std::string strikethrough_off = "29";

}

// Terminal Custom Pixel Definiton
class PIXEL
{
private:
	int X = -1, Y = -1;
	std::string color_codes = "NULL";

public:
	char holder = '\0';

	// Get X Coordinate
	int get_X() { return this->X; }

	// Get Y Coordinate
	int get_Y() { return this->Y; }

	// Set X and Y Coordinate
	void set_coordinate(int x, int y)
	{
		this->X = x;
		this->Y = y;
	}

	// Get Color Codes
	std::string get_ascii() { return this->color_codes; }

	// Set Color Codes
	void set_ascii(const std::string code) { this->color_codes.assign(code); }
};

// Terminal UI BOX Pixel Group
class BOX
{
public:
	// Block Dimensions
	int X1 = 0, Y1 = 0;
	int X2 = 0, Y2 = 0;
	int X3 = 0, Y3 = 0;
	int X4 = 0, Y4 = 0;

	// User Possession
	/* 0 = None
	** 1 = Player One
	** 2 = Player Two
	*/
	int uid = 0;

	// Update Block Dimensions
	void update_dimension(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
	{
		this->X1 = x1;
		this->Y1 = y1;
		this->X2 = x2;
		this->Y2 = y2;
		this->X3 = x3;
		this->Y3 = y3;
		this->X4 = x4;
		this->Y4 = y4;
	}
};

// Terminal Handler
class TERMINAL_HANDLER
{
private:
	int terminalHeight = 0;
	int terminalWidth = 0;
	// Get Current Terminal Dimension
	void get_terminal_dimension();

	// Display Frame
	PIXEL** TerminalFrame;

	// BOX Array
	BOX* BlockArray;
	int numberOfBlocks = 0;

public:
	// Constructor
	TERMINAL_HANDLER(int num)
	{
		this->numberOfBlocks = num;
		this->BlockArray = new BOX[num];
	}

	// PLayer Names
	std::string player_one = "Player One";
	std::string player_two = "Player Two";

	// input thread status
	bool thread_exit = false;

	// In-Terminal Borders
	int width_one = 0, width_two = 0;
	int height_one = 0, height_two = 0;

	// Game Win Combinations
	int wins[8][3] = {
		{0, 1, 2}, {3, 4, 5}, {6, 7, 8},
		{0, 3, 6}, {1, 4, 7}, {2, 5, 8},
		{0, 4, 8}, {2, 4, 6}
	};

	// Setup Terminal Frame
	void set_terminal_frame(int uID, bool textMode, int status);

	// Dissolve Terminal Frame
	void dissolve_terminal_frame();

	// Draw Frame
	void draw_frame();

	// Get Pixel's Block Position
	int get_block_position(int xPos, int yPos);

	// Update BOX Dimensions with each refresh
	void update_blocks(int highlight_block);

	// Set Block User Possession
	bool set_BlockUid(int blockNum, int user_id);

	// Check Game Status
	int check_game_status();

	// Clear Game Data
	void clear_game_data();
};

void TERMINAL_HANDLER::clear_game_data()
{
	for (int b = 0; b < this->numberOfBlocks; b++)
	{
		this->BlockArray[b].uid = 0;
	}
}

int TERMINAL_HANDLER::check_game_status()
{
	int status = 1;

	for (int y = 0; y < 8; y++)
	{
		if (this->BlockArray[this->wins[y][0]].uid == this->BlockArray[this->wins[y][1]].uid && this->BlockArray[this->wins[y][1]].uid == this->BlockArray[this->wins[y][2]].uid)
		{
			if (this->BlockArray[this->wins[y][0]].uid == 1)
			{
				status = 3;
				break;
			} else
			{
				if (this->BlockArray[this->wins[y][0]].uid == 2)
				{
					status = 4;
					break;
				}
			}
		}
	}

	if (status == 1)
	{
		int noZoneNums = 0;
		for (int b = 0; b < this->numberOfBlocks; b++)
		{
			if (this->BlockArray[b].uid != 0)
			{
				noZoneNums++;
			}
		}

		if (noZoneNums == this->numberOfBlocks)
		{
			status = 2;
		}
	}

	return status;
}

bool TERMINAL_HANDLER::set_BlockUid(int blockNum, int user_id)
{
	if (this->BlockArray[blockNum].uid == 0)
	{
		this->BlockArray[blockNum].uid = user_id;
		return true;
	}
	return false;
}

void TERMINAL_HANDLER::update_blocks(int highlight_block)
{
	// Mandatory Function

	int hO = this->height_one;
	int hT = this->height_two;
	int wO = this->width_one;
	int wT = this->width_two;
	int h = this->terminalHeight;
	int w = this->terminalWidth;

	// set Block Dimensions
	this->BlockArray[0].update_dimension(2, 1, wO-1, 1, 2, hO-1, wO-1, hO-1); // BOX 1
	this->BlockArray[1].update_dimension(wO+1, 1, wT-1, 1, wO+1, hO-1, wT-1, hO-1); // BOX 2
	this->BlockArray[2].update_dimension(wT+1, 1, w-2, 1, wT+1, hO-1, w-2, hO-1); // BOX 3
	this->BlockArray[3].update_dimension(2, hO+1, wO-1, hO+1, 2, hT-1, wO-1, hT-1); // BOX 4
	this->BlockArray[4].update_dimension(wO+1, hO+1, wT-1, hO+1, wO+1, hT-1, wT-1, hT-1); // BOX 5
	this->BlockArray[5].update_dimension(wT+1, hO+1, w-2, hO+1, wT+1, hT-1, w-2, hT-1); // BOX 6
	this->BlockArray[6].update_dimension(2, hT+1, wO-1, hT+1, 2, h-1, wO-1, h-1); // BOX 7
	this->BlockArray[7].update_dimension(wO+1, hT+1, wT-1, hT+1, wO+1, h-1, wT-1, h-1); // BOX 8
	this->BlockArray[8].update_dimension(wT+1, hT+1, w-2, hT+1, wT+1, h-1, w-2, h-1); // BOX 9

	int bPos;

	int block_pos[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	char block_num[9] = { '1', '2', '3', '4', '5', '6', '7', '8', '9'};

	for (int h = 0; h < this->terminalHeight; h++)
	{
		for (int w = 0; w < this->terminalWidth; w++)
		{
			bPos = this->get_block_position(this->TerminalFrame[h][w].get_X(), this->TerminalFrame[h][w].get_Y());

			if (bPos != -1)
			{
				if (block_pos[bPos] == 0)
				{
					block_pos[bPos]++;
					this->TerminalFrame[h][w].holder = block_num[bPos];
				}
			}
			if (bPos == highlight_block)
			{
				this->TerminalFrame[h][w].set_ascii(COLOR::unicode_escape+COLOR::fg_white_b+COLOR::separator+COLOR::bold_on+COLOR::separator+COLOR::bg_magenta+COLOR::unicode_end);
			} else
			{
				if (bPos != -1)
				{
					if (this->BlockArray[bPos].uid == 1)
					{
						this->TerminalFrame[h][w].set_ascii(COLOR::unicode_escape+COLOR::fg_blue+COLOR::separator+COLOR::bg_white+COLOR::separator+COLOR::bold_on+COLOR::unicode_end);
					} else
					{
						if (this->BlockArray[bPos].uid == 2)
						{
							this->TerminalFrame[h][w].set_ascii(COLOR::unicode_escape+COLOR::fg_white_b+COLOR::separator+COLOR::bg_yellow+COLOR::separator+COLOR::bold_on+COLOR::unicode_end);
						}
					}
				}
			}
		}
	}

}

int TERMINAL_HANDLER::get_block_position(int xPos, int yPos)
{
	int pos = -1; // Default Value

	for (int b = 0; b < this->numberOfBlocks; b++)
	{
		// Since BOX is a rectangle in case of TicTacToe
		if (xPos <= this->BlockArray[b].X2 && xPos >= this->BlockArray[b].X1)
		{
			if (yPos <= this->BlockArray[b].Y3 && yPos >= this->BlockArray[b].Y1)
			{
				return b;
			}
		}
	}

	return pos;
}

void TERMINAL_HANDLER::set_terminal_frame(int uID, bool textMode, int status)
{
	// Mandatory Function

	// Free Terminal Frame
	for (int h = 0; h < this->terminalHeight; h++)
	{
		delete[] this->TerminalFrame[h];
	}
	delete[] this->TerminalFrame;

	// Get Updates
	this->get_terminal_dimension();

	// testMode = true
	std::string textToDisplay;

	int length_cnt = 0;
	int center_height = this->terminalHeight/2;
	int center_width;

	std::string contMsg = "Press f to continue >>";
	int cont_cnt = 0;
	int cont_width;

	if (textMode)
	{
		if (status == 2)
		{
			textToDisplay.assign("TIE");
		} else if (status == 3)
		{
			textToDisplay.assign(this->player_one+" Win!");
		} else if (status == 4)
		{
			textToDisplay.assign(this->player_two+" Win!");
		} else
		{
			textToDisplay.assign("NULL");
		}

		center_width = (this->terminalWidth-textToDisplay.length())/2;
		cont_width = (this->terminalWidth-contMsg.length())/2;
	} else
	{
		// Set Borders
		this->width_one = this->terminalWidth/3;
		this->width_two = this->width_one*2;
		this->height_one = this->terminalHeight/3;
		this->height_two = this->height_one*2;
	}

	char char_buffer;
	int player_char_cnt = 0;

	// Allocate Terminal Frame
	this->TerminalFrame = new PIXEL*[this->terminalHeight];
	for (int h = 0; h < this->terminalHeight; h++)
	{
		this->TerminalFrame[h] = new PIXEL[this->terminalWidth];
		for (int w = 0; w < this->terminalWidth; w++)
		{
			// set PIXEL coordinate
			this->TerminalFrame[h][w].set_coordinate(w, h);
			char_buffer = ' ';

			if (!textMode)
			{
				// In-Screen Borders
				if (w == 0 || w == (this->terminalWidth-1))
				{
					char_buffer = '#';
					this->TerminalFrame[h][w].set_ascii(COLOR::unicode_escape+COLOR::fg_red_b+COLOR::separator+COLOR::bg_black+COLOR::unicode_end);
				} else if (w == 1 || w == (this->terminalWidth-2) || h == 0 || h == (this->terminalHeight-1))
				{
					this->TerminalFrame[h][w].set_ascii(COLOR::unicode_escape+COLOR::fg_green_b+COLOR::separator+COLOR::bg_black+COLOR::unicode_end);
					if (h == 0 && w >= 2)
					{
						if (uID == 1)
						{
							if (player_char_cnt < this->player_one.length())
							{
								char_buffer = player_one[player_char_cnt];
								player_char_cnt++;
							}
						} else
						{
							if (uID == 2)
							{
								if (player_char_cnt < this->player_two.length())
								{
									char_buffer = player_two[player_char_cnt];
									player_char_cnt++;
								}
							}
						}
					}
				} else
				{
					if (h == this->height_one || h == this->height_two)
					{
						this->TerminalFrame[h][w].set_ascii(COLOR::unicode_escape+COLOR::fg_red_b+COLOR::separator+COLOR::bg_black+COLOR::unicode_end);

						if (w == this->width_one || w == this->width_two)
						{
							char_buffer = '+';
						} else
						{
							char_buffer = '-';
						}
					} else
					{
						if (w == this->width_one || w == this->width_two)
						{
							char_buffer = '|';
							this->TerminalFrame[h][w].set_ascii(COLOR::unicode_escape+COLOR::fg_red_b+COLOR::separator+COLOR::bg_black+COLOR::unicode_end);
						} else
						{
							this->TerminalFrame[h][w].set_ascii(COLOR::unicode_escape+COLOR::fg_green_b+COLOR::separator+COLOR::bg_black+COLOR::unicode_end);
						}
					}
				}

			} else
			{
				// TextMode = true
				this->TerminalFrame[h][w].set_ascii(COLOR::unicode_escape+COLOR::fg_white_b+COLOR::separator+COLOR::bg_green+COLOR::separator+COLOR::bold_on+COLOR::unicode_end);

				if (h == (center_height - 1))
				{
					if (w >= (center_width - 1) && length_cnt < textToDisplay.length())
					{
						char_buffer = textToDisplay[length_cnt];
						length_cnt++;
					}
				}

				if (h == (this->terminalHeight - 1) && cont_cnt < contMsg.length())
				{
					if (w >= cont_width)
					{
						char_buffer = contMsg[cont_cnt];
						cont_cnt++;
					}
				}
			}

			this->TerminalFrame[h][w].holder = char_buffer;
		}
	}
}

void TERMINAL_HANDLER::draw_frame()
{
	// Mandatory Function

	// Clear Screen before redraw
	system(CLEAR);

	for (int h = 0; h < this->terminalHeight; h++)
	{
		for (int w = 0; w < this->terminalWidth; w++)
		{
			std::cout << this->TerminalFrame[h][w].get_ascii()+this->TerminalFrame[h][w].holder+COLOR::reset;
		}

		if (h != (this->terminalHeight - 1))
		{
			std::cout << std::endl;
		}
	}
}

void TERMINAL_HANDLER::dissolve_terminal_frame()
{
	// Mandatory Function
	// Free Terminal Frame
	for (int h = 0; h < this->terminalHeight; h++)
	{
		delete[] this->TerminalFrame[h];
	}
	delete[] this->TerminalFrame;

	// Free Block Array
	delete[] this->BlockArray;
}

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
