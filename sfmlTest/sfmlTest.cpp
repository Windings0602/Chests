#include <iostream>
#include <vector>
#include <algorithm>
#include <conio.h>
#include <thread>
#include <SFML/Graphics.hpp>
#include <fstream>
using namespace std;
using namespace sf;
RenderWindow window(VideoMode(800, 800), L"Шахматы", Style::Default);
string errorMessage = "";

//получаем наше окно, в котором нужно изменить цвеt

struct Point
{
	int x;
	int y;
};
int turn = 1;
string team1;
string team2;
struct Item {
	string shape = "void";
	int turn = 0;
};

vector <vector <Item> > board(8, vector <Item>(8));
void startNew() {
	board[0][0].shape = "rock";
	board[0][1].shape = "knight";
	board[0][2].shape = "bishop";
	board[0][3].shape = "queen";
	board[0][4].shape = "king";
	board[0][5].shape = "bishop";
	board[0][6].shape = "knight";
	board[0][7].shape = "rock";
	for (int i = 0; i < 8; i++) {
		board[1][i].shape = "solder";
		board[0][i].turn = -1;
		board[1][i].turn = -1;
		board[6][i].shape = "solder";
		board[6][i].turn = 1;
		board[7][i].turn = 1;
	}
	board[7][0].shape = "rock";
	board[7][1].shape = "knight";
	board[7][2].shape = "bishop";
	board[7][3].shape = "queen";
	board[7][4].shape = "king";
	board[7][5].shape = "bishop";
	board[7][6].shape = "knight";
	board[7][7].shape = "rock";
}

void createSprite(string IMG, float x, float y, float scalex = 1.0f, float scaley = 1.0f) {
	Image cheastBoard; //создаем объект Image (изображение)
	cheastBoard.loadFromFile(IMG);//загружаем в него файл

	Texture boardtexture;//создаем объект Texture (текстура)
	boardtexture.loadFromImage(cheastBoard);//передаем в него объект Image (изображения)

	Sprite boardsprite;//создаем объект Sprite(спрайт)
	boardsprite.setTexture(boardtexture);//передаём в него объект Texture (текстуры)
	boardsprite.setPosition(x, y);//задаем начальные координаты появления спрайта
	boardsprite.setScale(scalex, scaley);
	window.draw(boardsprite);
	//window.display();
}

void printBoard() {
	for (int i = 0; i < 8; i++) {
		cout << i + 1 << " ";
		for (int k = 0; k < 8; k++) {
			if (board[i][k].shape == "rock")
				cout << "r";
			else if (board[i][k].shape == "knight")
				cout << "k";
			else if (board[i][k].shape == "bishop")
				cout << "b";
			else if (board[i][k].shape == "king")
				cout << "K";
			else if (board[i][k].shape == "queen")
				cout << "Q";
			else if (board[i][k].shape == "solder")
				cout << "s";
			else if (board[i][k].shape == "void")
				cout << "-";
			else
				cout << "?";
		}
		cout << '\n';
	}
	cout << "\n  ";
	for (char i = 'a'; i <= 'h'; i++)
		cout << i;
	cout << '\n';
}

void drawBoard()
{
	int factor = 100;
	int shift = 12;
	string teamColor = "";
	for (int i = 0; i < 8; i++) {
		for (int k = 0; k < 8; k++) {
			if (board[i][k].turn == 1)
				teamColor = "white";
			else if (board[i][k].turn == -1)
				teamColor = "black";
			if (board[i][k].shape == "rock")
				createSprite("img/" + teamColor + "Rook.png", k * factor + shift, i * factor + shift, 0.6f, 0.6f);
			else if (board[i][k].shape == "knight")
				createSprite("img/" + teamColor + "Knight.png", k * factor + shift, i * factor + shift, 0.6f, 0.6f);
			else if (board[i][k].shape == "bishop")
				createSprite("img/" + teamColor + "Bishop.png", k * factor + shift, i * factor + shift, 0.6f, 0.6f);
			else if (board[i][k].shape == "king")
				createSprite("img/" + teamColor + "King.png", k * factor + shift, i * factor + shift, 0.6f, 0.6f);
			else if (board[i][k].shape == "queen")
				createSprite("img/" + teamColor + "Queen.png", k * factor + shift, i * factor + shift, 0.6f, 0.6f);
			else if (board[i][k].shape == "solder")
				createSprite("img/" + teamColor + "Pawn.png", k * factor + shift, i * factor + shift, 0.6f, 0.6f);
		}
	}
}

bool rockOk(Point start, Point finish) {
	if (finish.x >= 8 || finish.y >= 8) {
		errorMessage = "Ваша фигура улетела в стратосферу";
		return false;
	}
	if (start.x == finish.x && start.y != finish.y)
		return true;
	else if (start.x != finish.x && start.y == finish.y)
		return true;
	else {
		errorMessage = "Roock Ход не возможен";
		return false;
	}
}
bool bishopOk(Point start, Point finish) {
	if (finish.x >= 8 || finish.y >= 8) {
		errorMessage = "Ваша фигура улетела в стратосферу";
		return false;
	}
	if (start.x - start.y == finish.x - finish.y || start.x + start.y == finish.x + finish.y) {
		return true;
	}
	else {
		errorMessage = "Bishop Ход не возможен";
		return false;
	}

}
bool knightOk(Point start, Point finish) {
	if (finish.x >= 8 || finish.y >= 8) {
		errorMessage = "Ваша фигура улетела в стратосферу";
		return false;
	}
	if (start.x == finish.x - 2 && start.y == finish.y - 1)
		return true;
	else if (start.x == finish.x - 1 && start.y == finish.y - 2)
		return true;
	else if (start.x == finish.x + 1 && start.y == finish.y + 2)
		return true;
	else if (start.x == finish.x + 2 && start.y == finish.y + 1)
		return true;
	else if (start.x == finish.x + 1 && start.y == finish.y - 2)
		return true;
	else if (start.x == finish.x - 2 && start.y == finish.y + 1)
		return true;
	else if (start.x == finish.x - 1 && start.y == finish.y + 2)
		return true;
	else if (start.x == finish.x + 2 && start.y == finish.y - 1)
		return true;
	else {
		errorMessage = "Knight Ход не возможен";
		return false;
	}
}
bool solderOk(Point start, Point finish) {
	if (finish.y >= 8 || finish.x >= 8) {
		errorMessage = "Ваша фигура улетела в стратосферу";
		return false;
	}
	if (start.y != finish.y) {
		errorMessage = "SolderWhite Ход не возможен";
		return false;
	}
	if (start.x == 6) {
		if (finish.x - start.x == -2 || finish.x - start.x == -1)
			return true;
		else {
			errorMessage = "SolderWhite Ход на две клетки не возможен";
			return false;
		}
	}
	else if (finish.x - start.x == -1)
		return true;
	else {
		cout << "SolderWhite Ход не возможен" << " " << start.x << " " << start.y << " " << finish.x << " " << finish.y;
		return false;
	}
}
bool solderOk2(Point start, Point finish) {
	if (finish.x >= 8 || finish.y >= 8) {
		errorMessage = "Ваша фигура улетела в стратосферу";
		return false;
	}
	if (start.y != finish.y) {
		errorMessage = "SolderBlack Ход не возможен";
		return false;
	}

	if (start.x == 1) {
		if (start.x - finish.x == -2 || start.x - finish.x == -1)
			return true;
		else {
			errorMessage = "SolderBlack Ход на две лкетки не возможен";
			return false;
		}
	}
	else if (start.x - finish.x == -1)
		return true;
	else {
		errorMessage = "SolderBlack Ход не возможен";
		return false;
	}
}
bool kingOk(Point start, Point finish) {
	if (finish.x >= 8 || finish.y >= 8) {
		errorMessage = "Ваша фигура улетела в стратосферу";
		return false;
	}
	if (abs(finish.x - start.x) <= 1 && abs(finish.y - start.y) <= 1)
		return true;
	else {
		errorMessage = "King Ход не возможен";
		return false;
	}
}
bool queenOk(Point start, Point finish) {
	if (finish.x >= 8 || finish.y >= 8) {
		errorMessage = "Ваша фигура улетела в стратосферу";
		return false;
	}
	if (start.x - start.y == finish.x - finish.y || start.x + start.y == finish.x + finish.y)
		return true;
	else if (start.x == finish.x && start.y != finish.y)
		return true;
	else if (start.x != finish.x && start.y == finish.y)
		return true;
	else {
		errorMessage = "Queen Ход не возможен";
		return false;
	}
}

bool solderFood(Point start, Point finish) {
	if (board[start.x][start.y].turn == -1 && board[finish.x][finish.y].turn == 1) {
		if (start.x + 1 == finish.x && abs(start.y - finish.y) == 1)
			return true;
		else
			return false;
	}
	else if (board[start.x][start.y].turn == 1 && board[finish.x][finish.y].turn == -1) {
		if (start.x - 1 == finish.x && abs(start.y - finish.y) == 1)
			return true;
		else
			return false;
	}
	else
		return false;
}

void go(Point start, Point finish)
{
	board[finish.x][finish.y].shape = board[start.x][start.y].shape;
	board[finish.x][finish.y].turn = board[start.x][start.y].turn;
	board[start.x][start.y].shape = "void";
	board[start.x][start.y].turn = 0;
}

bool move(Point start, Point finish) {
	if (board[start.x][start.y].shape == "void") {
		errorMessage = "Сдесь нет фгуры";
		return false;
	}
	if (board[start.x][start.y].shape == "rock") {
		if (rockOk(start, finish) == true) {
			go(start, finish);
			return true;
		}
		else
			return false;
	}
	if (board[start.x][start.y].shape == "knight") {
		if (knightOk(start, finish) == true) {
			go(start, finish);
			return true;
		}
		else
			return false;
	}
	if (board[start.x][start.y].shape == "bishop") {
		if (bishopOk(start, finish) == true) {
			go(start, finish);
			return true;
		}
		else
			return false;
	}
	if (board[start.x][start.y].shape == "king") {
		if (kingOk(start, finish) == true) {
			go(start, finish);
			return true;
		}
		else
			return false;
	}
	if (board[start.x][start.y].shape == "queen") {
		if (queenOk(start, finish) == true) {
			go(start, finish);
			return true;
		}
		else
			return false;
	}
	if (board[start.x][start.y].shape == "solder" && board[start.x][start.y].turn == -1) {
		if (solderOk2(start, finish) == true && board[finish.x][finish.y].shape == "void") {
			go(start, finish);
			return true;
		}
		else if (solderFood(start, finish) == true && board[finish.x][finish.y].shape != "void") {
			go(start, finish);
			return true;
		}
		else
			return false;
	}
	if (board[start.x][start.y].shape == "solder" && board[start.x][start.y].turn == 1) {
		if (solderOk(start, finish) == true && board[finish.x][finish.y].shape == "void") {
			go(start, finish);
			return true;
		}
		else if (solderFood(start, finish) == true && board[finish.x][finish.y].shape != "void") {
			go(start, finish);
			return true;
		}
		else
			return false;
	}
	return false;
}

void save() {
	ofstream file("save.txt");
	file << turn << '\n';
	for (int i = 0; i < board.size(); i++) {
		for (int k = 0; k < board.size(); k++) {
			file << board[i][k].shape <<'\n';
		}
	}
	file << '\n';
	for (int i = 0; i < board.size(); i++) {
		for (int k = 0; k < board.size(); k++) {
			file << board[i][k].turn;
		}
	}
	file.close();

}
void load() {
	ifstream file("save.txt");
	file >> turn;
	for (int i = 0; i < board.size(); i++) {
		for (int k = 0; k < board.size(); k++) {
			file >> board[i][k].shape;
		}
	}
	for (int i = 0; i < board.size(); i++) {
		for (int k = 0; k < board.size(); k++) {
			file >> board[i][k].turn;
		}
	}
	file.close();
}

bool getPoint(Point& p) {
	string coord;
	cin >> coord;
	if (coord == "save") {
		errorMessage = "Игра сохранена";
		save();
		return false;
	}
	if (coord == "load") {
		load();
		errorMessage = "Сохранение загруженно, приятной игры";
		return false;
	}
	if (coord.size() != 2 || (coord[1] - '1') < 0 || (coord[1] - '1') > 7 || (coord[0] - 'a') < 0 || (coord[0] - 'a') > 7) {
		errorMessage = "Идея интересная, но бред";
		return false;
	}
	p.y = coord[0] - 'a';
	p.x = coord[1] - '1';
	return true;
}

bool game() {
	printBoard();
	cout << errorMessage << "\n";
	if (turn == 1)
		cout << "Ход белых\n";
	else if (turn == -1)
		cout << "Ход черных\n";
	Point start, finish;
	if (getPoint(start) == true && getPoint(finish) == true && turn == board[start.x][start.y].turn) {
		string finishShape = board[finish.x][finish.y].shape;
		if (move(start, finish) == true) {
			if (finishShape == "king") {
				if (turn == 1)
					cout << "Синие победили";
				else
					cout << "Красные победили";
				return false;
			}
			turn *= -1;
			errorMessage = "";
		}
	}
	system("CLS");
	return game();
}

void draw()
{
	window.setVerticalSyncEnabled(true);

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}


		window.clear(Color::White);
		createSprite("img/ChestBoard.png", -25, -25, 3.78f, 3.78f);
		drawBoard();
		window.display();
	}
}

int main()
{
	setlocale(0, "");

	startNew();

	thread thr(game);

	draw();

	thr.join();
}