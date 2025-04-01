#include <algorithm>
#include <cctype>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <limits>
#include <string>
#include <thread>
#include <vector>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#endif

using namespace std;

// Game constants
const int TTT_BOARD_SIZE = 9;
const int WIN_SCORE = 3;

// Function prototypes
void clear_input_buffer();
void clear_screen();
void cursor_hide();
void cursor_show();
void terminal_pause(const string &prompt);
void delay(int milliseconds);
int getch();

void arrow_options_animation(const vector<string> &options, int selected);
int display_options(const vector<string> &options, const string &title);

// Tic-Tac-Toe functions
void tictactoe_game_menu();
void tictactoe_game(bool vsAI = false, bool hardMode = false);
void print_tictactoe_board(const string board_mark[], int player1_score,
                           int player2_score, const string &player1_name,
                           const string &player2_name);
void tictactoe_how_to_play();
void tictactoe_developer_section();

int evaluate_board(const string board[]);
bool is_moves_left(const string board[]);
int minimax(string board[], bool isMax);
int find_best_move(string board[], bool hardMode);

// Snake and Ladder functions (stubs)
void snake_and_ladder_game_menu();
void snake_and_ladder_game();
void print_table_boarder();
void print_snake_and_ladder_board(string board_tile[]);
void snake_and_ladder_how_to_play();
void snake_and_ladder_developer_section();
void print_big_DEVELOPERS();

void delay(int milliseconds)
{
  std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

int getch()
{
#ifdef _WIN32
  return _getch();
#else
  struct termios oldt, newt;
  int ch;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  ch = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  return ch;
#endif
}

int main()
{
#ifdef _WIN32
  SetConsoleOutputCP(65001);
#endif
  srand(time(0)); // Add this line for random number generation

  vector<string> game_options = {" Tictactoe ❌🔵", " Snake and Ladder 🪜🐍"};

  while (true)
  {
    int game_selected = display_options(game_options, "Please Select a Game");

    switch (game_selected)
    {
    case 0:
      tictactoe_game_menu();
      break;
    case 1:
      snake_and_ladder_game_menu();
      break;
    }
  }

  return 0;
}

void clear_input_buffer()
{
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void clear_screen()
{
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

void cursor_hide()
{
#ifdef _WIN32
  HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO info;
  info.dwSize = 100;
  info.bVisible = FALSE;
  SetConsoleCursorInfo(consoleHandle, &info);
#else
  cout << "\033[?25l";
#endif
}

void cursor_show()
{
#ifdef _WIN32
  HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO info;
  info.dwSize = 100;
  info.bVisible = TRUE;
  SetConsoleCursorInfo(consoleHandle, &info);
#else
  cout << "\033[?25h";
#endif
}



void terminal_pause(const string &prompt)
{
  cout << prompt;
  cin.clear();
  cin.get();
}

void arrow_options_animation(const vector<string> &options, int selected)
{
  for (size_t i = 0; i < options.size(); i++)
  {
    cout << (i == selected ? ">" : " ") << options[i] << endl;
  }
}

int display_options(const vector<string> &options, const string &title)
{
  int selected = 0;
  int key;

  while (true)
  {
    clear_screen();
    cursor_hide();

    // Print title with border
    int title_length = title.length();
    cout << string(title_length + 16, '*') << "\n"
         << string(8, ' ') << title << "\n"
         << string(title_length + 16, '*') << "\n\n";

    arrow_options_animation(options, selected);
    key = getch();

#ifdef _WIN32
    if (key == 0 || key == 0xE0)
    {
      key = getch();
      if (key == 72)
      { // Up arrow
        selected = (selected - 1 + options.size()) % options.size();
      }
      else if (key == 80)
      { // Down arrow
        selected = (selected + 1) % options.size();
      }
    }
#else
    if (key == 27 && getch() == 91)
    {
      key = getch();
      if (key == 65)
      { // Up arrow
        selected = (selected - 1 + options.size()) % options.size();
      }
      else if (key == 66)
      { // Down arrow
        selected = (selected + 1) % options.size();
      }
    }
#endif
    else if (key == '\r' || key == '\n')
    {
      break;
    }
  }

  return selected;
}

void tictactoe_game_menu()
{
  clear_screen();
  cout << "*****************************************\n"
       << "    👋 WELCOME TO ❌TICTACTOE GAME🔵\n"
       << "*****************************************\n";

  terminal_pause("Press Enter to Continue...");

  while (true)
  {
    clear_screen();
    vector<string> main_menu_options = {" Play Game 🕹️", " How to Play ❔",
                                        " Developers ⌨️", " Exit ➡️"};

    int menu_decision =
        display_options(main_menu_options, "❌ TICTACTOE GAME 🔵");

    switch (menu_decision)
    {
    case 0:
    {
      vector<string> mode_options = {" Player vs Player 👥",
                                     " Player vs AI 🤖 (Easy)",
                                     " Player vs AI 🤖 (Hard)"};

      int mode_choice = display_options(mode_options, "Select Game Mode");

      switch (mode_choice)
      {
      case 0:
        tictactoe_game(false);
        break;
      case 1:
        tictactoe_game(true, false);
        break;
      case 2:
        tictactoe_game(true, true);
        break;
      }
      break;
    }
    case 1:
      tictactoe_how_to_play();
      break;
    case 2:
      tictactoe_developer_section();
      break;
    case 3:
      clear_screen();
      cout << "\n\n\tTHANK YOU FOR PLAYING OUR GAME 🙏💖" << endl;
      terminal_pause("\n\tPRESS ENTER TO GO BACK TO GAME SELECTION MENU...");
      return;
    }
  }
}

void tictactoe_game(bool vsAI, bool hardMode)
{
  string board_mark[TTT_BOARD_SIZE] = {" 1", " 2", " 3", " 4", " 5",
                                       " 6", " 7", " 8", " 9"};
  string player1_name, player2_name = vsAI ? "AI" : "";
  int player1_score = 0, player2_score = 0;

  // Game instructions
  char read_instruction_response;
  do
  {
    clear_screen();
    cout << "**********************************************************\n"
         << "        PLEASE READ THE HOW TO PLAY MENU FIRST\n"
         << "**********************************************************\n"
         << "Reading this game instructions before playing is a must if \n"
         << "you already read the instructions press 'p' to proceed and  \n"
         << "       if not press 'r' to read the instructions\n"
         << "\n\n\tProceed [p]\t\tRead Instructions [r]\n";

    read_instruction_response = tolower(getch());

    if (read_instruction_response == 'r')
    {
      tictactoe_how_to_play();
    }
    else if (read_instruction_response != 'p')
    {
      terminal_pause("Invalid input please read the prompt carefully,\nPress "
                     "enter to continue...");
    }
  } while (read_instruction_response != 'p');

  // Player registration
  clear_screen();
  cout << "---Before starting the game lets register each player's name "
          "first---\n\n";
  cursor_show();

  cout << "    Enter the 1st player name: ";
  cin >> player1_name;

  if (!vsAI)
  {
    cout << "\n    Enter the 2nd player name: ";
    cin >> player2_name;
  }
  cursor_hide();

  clear_screen();
  clear_input_buffer();
  cout << "Welcome " << player1_name << (vsAI ? "" : " and " + player2_name)
       << " please enjoy the game 😊\n";
  cout << endl << player1_name << " will take the first move. The player who reach 3 points first, will win 🏆" << endl;
  terminal_pause("Press ENTER and the game will begin...");

  // Main game loop
  while (player1_score < WIN_SCORE && player2_score < WIN_SCORE)
  {
    int current_move = 0;
    bool game_over = false;

    // Reset board
    for (int i = 0; i < TTT_BOARD_SIZE; i++)
    {
      board_mark[i] = " " + to_string(i + 1);
    }

    // Round loop
    while (!game_over && current_move < TTT_BOARD_SIZE)
    {
      // Player 1's move
      int move;
      bool valid_move = false;

      print_tictactoe_board(board_mark, player1_score, player2_score,
                            player1_name, player2_name);

      do
      {
        // cout << player1_name << "'s move (1-9): ";
        cout << player1_name << "'s move ❌ (1-9): ";

        if (cin >> move)
        {
          if (move < 1 || move > 9)
          {
            cout << "Invalid input. Please choose a number between 1-9.\n";
          }
          else if (board_mark[move - 1] == "❌" || board_mark[move - 1] == "🔵")
          {
            cout << "That position is already taken. Please choose another.\n";
          }
          else
          {
            valid_move = true;
          }
        }
        else
        {
          cout << "Invalid input. Please enter a number.\n";
          cin.clear();
          clear_input_buffer();
        }
      } while (!valid_move);

      board_mark[move - 1] = "❌";
      current_move++;

      // Check if player 1 won
      if (evaluate_board(board_mark))
      {
        print_tictactoe_board(board_mark, player1_score, player2_score,
                              player1_name, player2_name);
        cout << "CONGRATULATIONS!!! " << player1_name << " Wins!" << endl;
        player1_score++;
        clear_input_buffer();
        terminal_pause("Press enter to proceed to next round...");
        game_over = true;
        break;
      }

      if (current_move == TTT_BOARD_SIZE)
        break;

      // Player 2's move (either human or AI)
      if (vsAI)
      {
        // AI's turn

        print_tictactoe_board(board_mark, player1_score, player2_score,
                              player1_name, player2_name);

        cout << "AI is thinking";
        for (int i = 0; i < 5; i++)
        {
          cout << ".";
          cout.flush();
          delay(250); // 0.5 second delay
        }

        int ai_move = find_best_move(board_mark, hardMode);
        board_mark[ai_move] = "🔵";
        current_move++;

        // cout << "AI chose position " << (ai_move + 1) << endl;

        print_tictactoe_board(board_mark, player1_score, player2_score,
                              player1_name, player2_name);

        cout << "AI chose position " << (ai_move + 1) << endl;

        delay(900);

        // Check if AI won
        if (evaluate_board(board_mark))
        {
          cout << endl << "AI Wins! Better luck next time!" << endl;
          player2_score++;
          clear_input_buffer();
          terminal_pause("Press enter to proceed to next round...");
          game_over = true;
        }
      }
      else
      {
        // Human player 2's turn
        print_tictactoe_board(board_mark, player1_score, player2_score,
                              player1_name, player2_name);

        valid_move = false;
        do
        {
          // cout << player2_name << "'s move (1-9): ";
          cout << player2_name << "'s move 🔵 (1-9): ";
          if (cin >> move)
          {
            if (move < 1 || move > 9)
            {
              cout << "Invalid input. Please choose a number between 1-9.\n";
            }
            else if (board_mark[move - 1] == "❌" ||
                     board_mark[move - 1] == "🔵")
            {
              cout
                  << "That position is already taken. Please choose another.\n";
            }
            else
            {
              valid_move = true;
            }
          }
          else
          {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            clear_input_buffer();
          }
        } while (!valid_move);

        board_mark[move - 1] = "🔵";
        current_move++;

        // Check if player 2 won
        if (evaluate_board(board_mark))
        {
          print_tictactoe_board(board_mark, player1_score, player2_score,
                                player1_name, player2_name);
          cout << "CONGRATULATIONS!!! " << player2_name << " Wins!" << endl;
          player2_score++;
          clear_input_buffer();
          terminal_pause("Press enter to proceed to next round...");
          game_over = true;
        }
      }
    }

    // Check for draw
    if (!game_over && current_move == TTT_BOARD_SIZE)
    {
      print_tictactoe_board(board_mark, player1_score, player2_score,
                            player1_name, player2_name);
      clear_input_buffer();
      cout << "⚠️ It's a DRAW!!!" << endl;
      terminal_pause("\nPress enter to start a new round..");
    }

    clear_screen();
    print_tictactoe_board(board_mark, player1_score, player2_score,
    player1_name, player2_name);
    char exit_decision, exit_decision_confirmation;

    cout << "Press E [e] if you want end the game and go back to game menu if not just press any other key...";
    exit_decision = _getch();

    if(exit_decision == 'e'){
      clear_screen();
      cout << "Are you sure you want to EXIT? Press Y [y] if yes and if no press any other key";
      exit_decision_confirmation = _getch();

      if(exit_decision_confirmation == 'y'){
        tictactoe_game_menu();
      }
    }
  }

  // Game over - determine winner
  clear_screen();
  if (player1_score == WIN_SCORE)
  {
    cout << endl
         << "Congratulations!🎊 " << player1_name
         << " You Won!🎉, You OUTSMARTED " << (vsAI ? "The AI" : player2_name)
         << " 🍾\n";
  }
  else
  {
    cout << endl
         << "Congratulations!🎊 " << (vsAI ? "AI" : player2_name)
         << " You Won!🎉, You OUTSMARTED " << player1_name << " 🍾\n";
  }

  terminal_pause("Press ENTER to go back to TICTACTOE main menu...");
}

void print_tictactoe_board(const string board_mark[], int player1_score,
                           int player2_score, const string &player1_name,
                           const string &player2_name)
{
  clear_screen();

  cout << "   " << board_mark[0] << " | " << board_mark[1] << " | "
       << board_mark[2] << "\t\t\t===========================\n"
       << "  ----+----+----" << "\t\t      🔢 SCORE BOARD 🔢\n"
       << "   " << board_mark[3] << " | " << board_mark[4] << " | "
       << board_mark[5] << "\t\t\t===========================\n"
       << "  ----+----+----" << "\t\t" << player1_name << ": " << player1_score
       << "\n"
       << "   " << board_mark[6] << " | " << board_mark[7] << " | "
       << board_mark[8] << "\t\t\t" << player2_name << ": " << player2_score
       << "\n\n";
}

// AI functions implementation
int evaluate_board(const string board[])
{
  // Check all possible winning combinations
  const int win_patterns[8][3] = {
      {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, // rows
      {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, // columns
      {0, 4, 8}, {2, 4, 6}             // diagonals
  };

  for (auto &pattern : win_patterns)
  {
    if (board[pattern[0]] == board[pattern[1]] &&
        board[pattern[0]] == board[pattern[2]])
    {
      if (board[pattern[0]] == "❌")
        return -10;
      if (board[pattern[0]] == "🔵")
        return 10;
    }
  }
  return 0; // No winner yet
}

bool is_moves_left(const string board[])
{
  for (int i = 0; i < TTT_BOARD_SIZE; i++)
  {
    if (board[i] != "❌" && board[i] != "🔵")
    {
      return true;
    }
  }
  return false;
}

int minimax(string board[], bool isMax)
{
  int score = evaluate_board(board);

  // If maximizer or minimizer has won
  if (score == 10)
    return score;
  if (score == -10)
    return score;

  // If no moves left (draw)
  if (!is_moves_left(board))
    return 0;

  // If maximizing player's turn (AI)
  if (isMax)
  {
    int best = -1000;
    for (int i = 0; i < TTT_BOARD_SIZE; i++)
    {
      if (board[i] != "❌" && board[i] != "🔵")
      {
        string original = board[i];
        board[i] = "🔵"; // AI's move
        best = max(best, minimax(board, !isMax));
        board[i] = original;
      }
    }
    return best;
  }
  // If minimizing player's turn (human)
  else
  {
    int best = 1000;
    for (int i = 0; i < TTT_BOARD_SIZE; i++)
    {
      if (board[i] != "❌" && board[i] != "🔵")
      {
        string original = board[i];
        board[i] = "❌"; // Human's move
        best = min(best, minimax(board, !isMax));
        board[i] = original;
      }
    }
    return best;
  }
}

int find_best_move(string board[], bool hardMode)
{
  if (!hardMode)
  {
    // Easy mode - make random moves
    vector<int> available;
    for (int i = 0; i < TTT_BOARD_SIZE; i++)
    {
      if (board[i] != "❌" && board[i] != "🔵")
      {
        available.push_back(i);
      }
    }
    if (!available.empty())
    {
      return available[rand() % available.size()];
    }
    return -1;
  }
  else
  {
    // Hard mode - use minimax for optimal play
    int best_val = -1000;
    int best_move = -1;

    for (int i = 0; i < TTT_BOARD_SIZE; i++)
    {
      if (board[i] != "❌" && board[i] != "🔵")
      {
        string original = board[i];
        board[i] = "🔵";
        int move_val = minimax(board, false);
        board[i] = original;

        if (move_val > best_val)
        {
          best_move = i;
          best_val = move_val;
        }
      }
    }
    return best_move;
  }
}

void tictactoe_how_to_play()
{
  clear_screen();
  cout << "\nMechanics in playing TicTacToe Game: \n"
       << "\n1. **Players**: Two players are required to play (or play against "
          "AI). \n"
       << "2. **Game Grid**: The game is played on a 3x3 grid, making up 9 "
          "spaces in total.\n"
       << "3. **Turn Order**: Players take turns marking an empty space on the "
          "grid.\n"
       << "4. **Objective**: The goal is to be the first player to get three "
          "of their symbols in a row, column, or diagonal.\n"
       << "5. **Winning Condition**: A player wins if they have three of their "
          "symbols in a horizontal, vertical, or diagonal line.\n"
       << "6. **Tie Condition**: The game ends in a draw if all 9 spaces are "
          "filled and no player has won.\n"
       << "7. **Valid Move**: A player can only mark an empty square. If a "
          "square is already filled, they must choose another one.\n"
       << "8. **Game Start**: The game begins with player 1 taking the first "
          "move.\n"
       << "9. **End of Game**: The game ends when either a player wins or all "
          "spaces on the grid are filled.\n"
       << "10. **Game Modes**:\n"
       << "    - Player vs Player: Two human players compete\n"
       << "    - Player vs AI (Easy): AI makes random moves\n"
       << "    - Player vs AI (Hard): AI uses advanced strategy\n";

  char instruction_decision;
  do
  {
    cout << "\n\nPress [m] to go back to main menu 📃; \t [p] if you want to "
            "play the game 🎮";
    instruction_decision = tolower(getch());

    if (instruction_decision != 'm' && instruction_decision != 'p')
    {
      cout << "\n\nInvalid Input, Just input [m] or [p]!" << endl;
    }
  } while (instruction_decision != 'm' && instruction_decision != 'p');

  if (instruction_decision == 'm')
  {
    tictactoe_game_menu();
  }
  else
  {
    vector<string> mode_options = {" Player vs Player 👥",
                                   " Player vs AI 🤖 (Easy)",
                                   " Player vs AI 🤖 (Hard)"};

    int mode_choice = display_options(mode_options, "Select Game Mode");
    tictactoe_game(mode_choice > 0, mode_choice == 2);
  }
}

void tictactoe_developer_section()
{
	clear_screen();
    print_big_DEVELOPERS();
    cout <<"\n\n";
    // Print the developer names
	cout << "Andrie Timothy R. Cabuguas\n";
	cout << "- Talk is cheap. Show me the code.\n\n";
    cout << "Asdi V. Amamence\n";
	cout << "- Computers are fast; developers keep them slow.\n\n";
    cout << "John Kennith A. Madera\n";
	cout << "- It's not a BUG, it's a FEATURE.\n\n";
    cout << "Krisna Hojland\n";
	cout << "- If debugging is the process of removing bugs, then programming must be the process of putting them in.\n";


	terminal_pause("\nPress ENTER to go back to main menu...");
}

// Snake and Ladder stubs (unchanged)
void snake_and_ladder_game_menu()
{
  int menu_decision = 0;
  clear_screen();
  cout << "*******************************************\n"
       << "   👋 WELCOME TO SNAKE 🐍 AND LADDER 🪜\n"
       << "*******************************************\n";
  terminal_pause("Press Enter to Continue...");

  vector<string> main_menu_options = {" Play Game 🕹️", " How to Play ❔",
                                      " Developers ⌨️", " Exit ➡️"};

  menu_decision = display_options(main_menu_options, "🐍 SNAKE AND LADDER 🪜");

  switch (menu_decision)
  {
  case 0:
    snake_and_ladder_game();
    break;
  case 1:
    snake_and_ladder_how_to_play();
    break;
  case 2:
    snake_and_ladder_developer_section();
    break;
  case 3:
    clear_screen();
    cout << "\n\n\tTHANK YOU FOR PLAYING OUR GAME 🙏💖" << endl;
    terminal_pause("\n\tPRESS ENTER TO GO BACK TO GAME SELECTION MENU...");
    break;
  }
}

void snake_and_ladder_game() {
  string easy_board_tile [101] = {"00",
        " 01", " 02", " 03", " 04", " 05", " 06", " 07"," 🪜", " 09", " 10",
        " 11", " 12", " 13", " 14", " 15", " 16", " 17"," 🪜", " 19", " 20",
        " 21", " 22", " 23", " 🐍", " 25", " 26", " 27", " 28", " 29", " 30",
        " 31", " 32", " 33", " 34", " 35", " 36", " 37", " 38", " 39", " 40",
        " 41", " 42", " 43", " 44", " 🐍", " 46", " 47", " 48", " 49", " 50",
        " 51", " 52", " 53", " 🪜", " 55", " 56", " 57", " 58", " 59", " 60",
        " 🪜", " 62", " 63", " 64", " 65", " 66", " 67", " 68", " 69", " 70",
        " 71", " 72", " 73", " 74", " 75", " 🐍", " 77", " 78", " 79", " 80",
        " 81", " 🪜", " 83", " 84", " 85", " 86", " 87", " 88", " 89",  " 🐍",
        " 91", " 92", " 93", " 94", " 95", " 96", " 97", " 98", " 99", "100"};
  

    print_snake_and_ladder_board(easy_board_tile);
  

}
void print_table_boarder(){
  cout << "+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+" << endl;
}

void print_snake_and_ladder_board(string board_tile[]) {
  clear_screen();
  cout<< "                 **************************\n" 
      << "                      SNAKE AND LADDER\n"
      << "                 **************************\n";


  int tile_end = 100;

  for(int i = 1; i <= 21  ; i++){
    if(i % 2 == 1){
        print_table_boarder();
    }else if(i % 2 == 0 && i % 4 != 0){
        for(int j = tile_end; j > (tile_end - 10); j--){
            cout << "| " << board_tile[j] << " ";
        }
        cout << "|\n";
        tile_end -= 10;

    }else{
        for(int j = (tile_end - 9); j <= tile_end; j++){
            cout << "| " << board_tile[j] << " ";
        }
        cout << "|\n";
        tile_end -= 10;

    }
}

cin.clear();
cin.get();
  
}
void snake_and_ladder_how_to_play() {
  clear_screen();
  cout <<"OBJECTIVE OF THE GAME: \n\n";
	  cout << "The objective of the game is to be the first player to reach the\n"
		  "end of the board (square 100) by rolling a die\n and moving "
		  "your game piece accordingly.\n\n";

cout << "BASIC SET-UP:\n\n";
	  cout << "1. The game is played on a board with numbered squares from 1 to "
		  "100.\n";
	  cout << "2. Each player has a game piece that starts at square 1.\n";
	  cout << "3. Players take turns rolling a die to determine how many squares "
		  "to move.\n\n";

cout << "GAME PLAY:\n\n";
	  cout << "1. On your turn, roll the die and move your game piece forward the "
		  "number of squares rolled.\n";
	  cout << "2. If you land on a square with the bottom of a ladder, you can "
		  "climb up to the top of the ladder.\n";
	  cout << "3. If you land on a square with the head of a snake, you must slide "
		  "down to the tail of the snake.\n";
	  cout << "4. The first player to reach square 100 wins the game.\n\n";

cout << "WINNING THE GAME: \n\n";
	  cout << "1. The first player to reach square 100 wins the game.\n";
	  cout << "2. If a player rolls a number that would take them past square "
		  "100, they must stay on their current square until they roll the exact number to land on the square 100.\n\n";
cout << "ADDITIONAL RULES:\n\n";
	  cout << "1. Each players have power ups or skills.\n";
	  cout << "2. If a player use a skill it will cooldown for 6 rounds.\n";
	  cout << "3. Players can choose which player will be affected of the skill he/she will use.\n\n";

	  terminal_pause("Press ENTER to go back to main menu...");
}
void print_big_DEVELOPERS() {
    string developers[] = {
        "DDDD   EEEEE  V   V  EEEEE  L       OOO   PPPP   EEEEE  RRRR    SSSS",
        "D   D  E      V   V  E      L      O   O  P   P  E      R   R  S    ",
        "D   D  EEEE   V   V  EEEE   L      O   O  PPPP   EEEE   RRRR    SSS ",
        "D   D  E      V   V  E      L      O   O  P      E      R  R      S  ",
        "DDDD   EEEEE   VVV   EEEEE  LLLLL   OOO   P      EEEEE  R   R   SSSS",
    };

    // Print each row of letters for "DEVELOPERS"
    for (int i = 0; i < 5; ++i) {
        cout << developers[i] << endl;
    }
}

// Developers information
void snake_and_ladder_developer_section() {
	clear_screen();
    print_big_DEVELOPERS();
    cout <<"\n\n";
    // Print the developer names
    cout << "Andrie Timothy R. Cabuguas\n";
	cout << "- Talk is cheap. Show me the code.\n\n";
    cout << "Asdi V. Amamence\n";
	cout << "- Computers are fast; developers keep them slow.\n\n";
    cout << "John Kennith A. Madera\n";
	cout << "- It's not a BUG, it's a FEATURE.\n\n";
    cout << "Krisna Hojland\n";
	cout << "- If debugging is the process of removing bugs, then programming must be the process of putting them in.\n";


	terminal_pause("\nPress ENTER to go back to main menu...");
}
