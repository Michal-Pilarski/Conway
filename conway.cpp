#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>

using namespace std;

enum CellStates { DEATH, LIVE };

class Game {
private:
	char liveChar = '#';
	char deathChar = ' ';
	int gen;
	int totalCells;
	int liveCells;
	int X_DIM;
	int Y_DIM;
	int SLEEP = 100;
	int MAX_GENS = 1000;
	vector<vector<CellStates>> grid;
	HANDLE hStdOut;
	CONSOLE_CURSOR_INFO curInfo;

public:
	Game(int x, int y);
	void hideCursor();
	void initializeGrid();
	void printGrid();
	int nextLiveCells(int i, int j);

	void nextGen();
	int countLiveCells();
	void start();
};

Game::Game(int x = 20, int y = 50) {
	X_DIM = x;
	Y_DIM = y;
	totalCells = X_DIM * Y_DIM;
	gen = 0;
	liveCells = 0;
	initializeGrid();
}

void Game::hideCursor() {
	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleCursorInfo(hStdOut, &curInfo);
	curInfo.bVisible = FALSE;
	SetConsoleCursorInfo(hStdOut, &curInfo);
	return;
}

void Game::initializeGrid() {
	srand(time(0));
	grid = vector<vector<CellStates>>(X_DIM);
	for (int i = 0; i < X_DIM; i++) {
		grid[i] = vector<CellStates>(Y_DIM);
		for (int j = 0; j < Y_DIM; j++)
			grid[i][j] = CellStates(rand() % 2);
	}
	return;
}

int Game::nextLiveCells(int i, int j) {
	int lives = 0;
	for (int k = -1; k <= 1; k++) {
		for (int h = -1; h <= 1; h++) {
			if (k == 0 && h == 0)
				continue;

			int x = (k + i + X_DIM) % X_DIM;
			int y = (h + j + Y_DIM) % Y_DIM;
			lives += (int)grid[x][y];

		}
	}

	return lives;
}

void Game::nextGen() {
	vector<vector<CellStates>> tmpGrid(X_DIM);
	for (int i = 0; i < X_DIM; i++) {
		tmpGrid[i] = vector<CellStates>(Y_DIM);;

		for (int j = 0; j < Y_DIM; j++) {
			int liveCells = nextLiveCells(i, j);

			if (grid[i][j] == LIVE && liveCells == 2 || liveCells == 3)
				tmpGrid[i][j] = LIVE;
			else if (grid[i][j] == DEATH && liveCells == 3)
				tmpGrid[i][j] = LIVE;
			else
				tmpGrid[i][j] = DEATH;
		}
	}

	grid = tmpGrid;

	return;
}

void Game::printGrid() {
	system("cls");

	for (int i = 0; i < X_DIM; i++) {
		for (int j = 0; j < Y_DIM; j++) {
			if (grid[i][j] == LIVE)
				cout << liveChar;
			else
				cout << deathChar;
		}
		if (i != X_DIM - 1)
			cout << endl;
	}

	return;
}

int Game::countLiveCells() {
	int liveCells = 0;

	for (int i = 0; i < X_DIM; i++)
		for (int j = 0; j < Y_DIM; j++)
			if (grid[i][j] == LIVE)
				liveCells++;

	return liveCells;
}

void Game::start() {
	hideCursor();

	while (gen < MAX_GENS) {
		nextGen();
		gen++;
		printGrid();
		liveCells = countLiveCells();
		system(("Game of Life - Gen: " + to_string(gen) + " - Live cells: " + to_string(liveCells) + "/" + to_string(totalCells)).c_str());
		Sleep(SLEEP);
	}

	return;
}



int main() {

	Game game(30, 120);
	game.start();

	cout << endl;
	system("pause");

	return 0;
}
