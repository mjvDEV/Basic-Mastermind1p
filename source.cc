#include <iostream>
#include <cstdlib>		//This and time.h used for random number generation.
#include <time.h>
#include <cstring>
using namespace std;

////////////////////////////////////////////////////////////////////
// Algorithm getGuess()
// 	safely collects a guess from the console.
//
// Pre: guesses :: 2D int array. The player's guesses.
//		answer :: int. Stores the solution, for comparison against the guess.
//		turn :: int. keeps track of which turn it is.
//		diff :: int. keeps track of the user selected difficulty.
// Post: guess stored in guesses[turn].
// Return: nothing.
////////////////////////////////////////////////////////////////////

void getGuess(int guesses[8][6], int turn, int diff)
{
	char x;
	bool check = false, invalid;
	while (!check)
	{
		invalid = false;
		cout << "Enter a guess: ";
		for (int c = 0; c < diff; c++)
		{
			cin >> ws;
			cin.get(x);
			if (isdigit(x)) {
				guesses[turn][c] = x - 48;
				check = true;
			}
			else {
				invalid = true;
				break;
			}
		}
		if (invalid) {
			cout << "Invalid input. Please try again.\n";
			check = false;
		}
		cin.sync();
	}
}

////////////////////////////////////////////////////////////////////
// Algorithm showResults(rW, rP, turn) 
// shows the score from a guess.
//
// Pre: rW :: 1D int array. The number of wholly correct digits in each guess.
//		rP :: 1D int array. The number of partially correct digits in each guess.
//		turn :: int. The correct turn.
// Post: results printed to console.
// Return: nothing.
////////////////////////////////////////////////////////////////////

void showResults(int rW[], int rP[], int turn)
{
	for (int c = 0; c < rW[turn]; c++) {
		cout << "X ";
	}
	for (int d = 0; d < rP[turn]; d++) {
		cout << "O ";
	}
	cout << endl;
}

////////////////////////////////////////////////////////////////////
// Algorithm Display(guesses, turn, diff, rW, rP, player)
// outputs to the console all guesses made so far and their scores.
//
// Pre: guesses :: 2D int array. The player's guesses.
//		turn :: int. keeps track of which turn it is, so the function knows which row to stop
//				outputting data from "guesses".	
//		diff :: int. keeps track of the user selected difficulty, so the function knows which
//				column to stop outputting data from "guesses".
//		rW :: 1D int array. The number of wholly correct digits in each guess.
//		rP :: 1D int array. The number of partially correct digits in each guess.
//		player :: cstring. Name of the current player.
// Post: nothing.
// Return: nothing.
////////////////////////////////////////////////////////////////////

void Display(int guesses[8][6], int turn, int diff, int rW[], int rP[], char* player)
{
	cout << "\n********************************************" << endl
		<< player << ", here are your guesses so far: " << endl;
	for (int a = 0; a < turn; a++)
	{
		cout << a + 1 << ".  ";
		for (int b = 0; b < diff; b++)
		{
			cout << guesses[a][b] << " ";
		}
		cout << " ---  Scored: ";
		showResults(rW, rP, a);
	}
	cout << "********************************************" << endl;
}

////////////////////////////////////////////////////////////////////
// Algorithm analyze(guesses, answer, turn, diff, rW, rP) 
//	determines the result of a player's guess.
//
// Pre: guesses :: 2D int array. The player's guesses.
//		answer :: int. Stores the solution, for comparison against the guess.
//		turn :: int. keeps track of which turn it is.
//		diff :: int. keeps track of the user selected difficulty.
//		rW :: 1D int array. The number of wholly correct digits in each guess.
//		rP :: 1D int array. The number of partially correct digits in each guess.
// Post: rP and rW incremented at offset [turn] according to result.
// Return: nothing.
////////////////////////////////////////////////////////////////////

void analyze(int guesses[8][6], int answer[], int turn, int diff, int rW[], int rP[])
{
	// Check for wholly correct digits first.
	// go through this turn's guess one digit at a time.
	for (int x = 0; x < diff; x++)
	{
		if (guesses[turn][x] >= 0 && guesses[turn][x] < 10)
		{
			// Check whether the current digit is wholly correct (correct digit, correct spot)
			if (guesses[turn][x] == answer[x])
			{
				rW[turn]++;
				// change answer and guess digits so that they won't return false results later this turn. 
				answer[x] += 10;
				guesses[turn][x] += 10;
			}
		}
	}
	// Now check for partially correct digits.
	// go through this turn's guess one digit at a time.
	for (int a = 0; a < diff; a++)
	{
		bool found = false; // used to exit loop once digit is found to prevent doubling of results.
		// Only check against solution if the digit is within the acceptable bounds (0 through 9).
		if (guesses[turn][a] >= 0 && guesses[turn][a] < 10)
		{
			for (int b = 0; (b < diff && !found); b++)
			{
				if (guesses[turn][a] == answer[b])
				{
					rP[turn]++;
					// Change answer digit so it won't return false results later.
					answer[b] += 10;
					found = true;
				}
			}
		}
	}
	// Reset all digits for use again next turn / in main.
	for (int c = 0; c < diff; c++)
	{
		if (answer[c] > 9)
		{
			answer[c] -= 10;
		}
		if (guesses[turn][c] > 9)
		{
			guesses[turn][c] -= 10;
		}
	}
}

////////////////////////////////////////////////////////////////////
// Algorithm pTurn(guesses, answer, turn, diff, rW, rP, player) 
//	handles a player's turn
//
// Pre: guesses :: 2D int array. The player's guesses.
//		answer :: int. Stores the solution, for comparison against the guess.
//		turn :: int. keeps track of which turn it is.
//		diff :: int. keeps track of the user selected difficulty.
//		rW :: 1D int array. The number of wholly correct digits in each guess.
//		rP :: 1D int array. The number of partially correct digits in each guess.
//		player :: cstring. Name of the current player.
// Post: handles a single turn appropriately. 
// Return: true if the player won on this round, false otherwise.
////////////////////////////////////////////////////////////////////

bool pTurn(int guesses[8][6], int answer[], int turn, int diff, int rW[], int rP[], char* player)
{
	bool win = false;

	if (turn > 0)
	{
		Display(guesses, turn, diff, rW, rP, player);
	}
	getGuess(guesses, turn, diff);
	analyze(guesses, answer, turn, diff, rW, rP);

	if (rW[turn] == diff)
	{
		win = true;
	}
	return win;
}

////////////////////////////////////////////////////////////////////
// Algorithm getAnswer(answer, diff)
// 	collects an answer for the other player to guess. 
//
// Pre: answer :: int. Stores the solution, for comparison against the guess.
//		diff :: int. keeps track of the user selected difficulty.
// Post: answer generated randomly.
// Return: nothing.
////////////////////////////////////////////////////////////////////

void getAnswer(int answer[], int diff)
{
	for (int a = 0; a < diff; a++)
	{
		answer[a] = rand() % 10;
	}
}

////////////////////////////////////////////////////////////////////
// Algorithm main()
// 	Single Player Mastermind.
//
// Pre: nothing.
// Post: nothing.
// Return: 0.
////////////////////////////////////////////////////////////////////

int main()
{
	srand(time(NULL)); // used for random number generation.

	int guesses[8][6], answer[6], turn, diff, failSafe;
	int rW[8], rP[8];
	char test = 'y'; // used to allow players to rematch without rebooting program.
	char player[1000];
	char x; // throwaway character for safe integer input.
	bool win, check;

	cout << "Greetings, master, what is your name? ";
	cin.getline(player,1000);
	cin.sync();

	do
	{
		cout << endl << endl
			<< "*******************************" << endl
			<< "  Welcome to Basic MasterMind  " << endl
			<< "*******************************" << endl
			<< "RULES:" << endl
			<< "1. The object of the game is to correctly guess a code." << endl
			<< "    - The code will contain digits from 0 through 9." << endl
			<< "2. The code will consist of 4-6 digits, based on which difficulty is selected." << endl
			<< "3. You will have 8 turns to correctly guess the code." << endl
			<< "4. Each guess will be scored as follows: " << endl
			<< "    - X: One X for each correct digit that is also in the correct spot." << endl
			<< "    - O: One O for each correct digit that is in the wrong spot." << endl
			<< "**************" << endl
			<< "  Good luck!  " << endl
			<< "**************" << endl << endl;

		failSafe = 0;
		check = false;
		do
		{
			cout << "Select difficulty (4,5,6): ";
			cin.get(x);
			if (isdigit(x)) {
				diff = x - 48;
				if (diff >= 4 && diff <= 6) {
					check = true;
				}
				else {
					check = false;
				}
			}
			else {
				check = false;
			}
			if (!check) {
				cout << "Invalid input, please try again.\n";
			}
			cin.sync();
			failSafe += 1;
		} while (!check && failSafe < 10);
		cout << endl << endl;

		if (failSafe > 9) {
			cout << "Program failed due to input error. Application will now close.";
			return 0;
		}

		// reset variables (so game is replayable).
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				guesses[i][j] = 0;
			}
			rW[i] = 0;
			rP[i] = 0;
		}
		win = false;
			
		getAnswer(answer, diff);

		// The actual gameplay portion. Player inputs their guesses one at a time,
		// which are then analyzed and the results shown along with all previous
		// guesses and corresponding results. Loop exits after 8 turns or if the
		// player wins.
		for (turn = 0; turn < 8 && !win; turn++)
		{
				win = pTurn(guesses, answer, turn, diff, rW, rP, player);
				cout << endl;
		}

		cout << "The correct solution was: ";
		for (int e = 0; e < diff; e++)
		{
			cout << answer[e] << " ";
		}
		cout << endl << endl;

		cout << "Your final guess was: ";
		for (int f = 0; f < diff; f++)
		{
			cout << guesses[turn - 1][f] << " ";
		}
		cout << endl << endl;

		if (win)
		{
			cout << "Congratulations, " << player << "! You won!" << endl << endl;
		}
		else
		{
			cout << "Aww too bad, " << player << ", you lost." << endl << endl;
		}

		cout << "Would you like to play again? (y/n): ";
		cin.get(test);
		cin.sync();
	} while (test != 'n');
	return 0;
}

