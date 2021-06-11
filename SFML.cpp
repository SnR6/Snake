//library<
#include <iostream> //Simple commands cout, cin...
#include <SFML/Graphics.hpp> //SFML librany
#include <thread>
#include <chrono>
#include <ctime>
#include <sstream>
#include <fstream>
//>library

//namespace<
using namespace std; //iostream namespace
using namespace sf; //SFML namespace
//>namespace

//variables<
const int	swidth = 700, //Window width
			sheight = 700; //Window height //!heigh = width!
int	width = swidth, //Window width
	height = sheight; //Window height
const string windowTitle = "Sssnake!"; //Window title

RenderWindow window(VideoMode(swidth, sheight), windowTitle, Style::None); //Window objekt
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };

const int xmap = 20, ymap = 20; //!xmap = ymap!
RectangleShape mapp[ymap][xmap];

int ntile;
int x, y; //Head
int snakex[1000], snakey[1000]; //Body
int applex, appley; //Apple

int mapSize;

string command;
int cd = 250; //Cooldown 1000 - 1seconds
bool walls = 0; //Walls
bool tmp1;
bool collision = 0; //Collision snake
eDirection dir, lastDir;

string score;
Text scoreLabel;
Font font;

bool a1 = 1;
//>variables

//function prototypes<
void logic(); //Logic in WWO
void WhileWindowOpen(); //Main cycle
void CheckEvents(); //Check events
void Setup();
void Draw();
//>functio prototypes

//functions<
int main() //main
{
	window.setFramerateLimit(60);

	srand(time(nullptr));
	font.loadFromFile("arial.ttf");
	Setup();
	WhileWindowOpen(); //WWO

	ofstream fout("Score.txt");
	fout << ntile;

	return 0; //Exit main
}

void WhileWindowOpen() //Main cycle (WWO)
{
	Clock clock;
	while (window.isOpen() && command != "Exit") //While window open:
	{
		float clocktime = clock.getElapsedTime().asMilliseconds();

		if (!tmp1) CheckEvents(); //Check all events

		window.clear(); //Clear all

		Draw();

		if (clocktime >= cd) {
			logic(); //Logic
			if (command == "Exit") return;
			clock.restart();
			tmp1 = 0;
		}

		window.display(); //Draw all
	}
}

void logic() //Main logic
{
	//Move
	if (dir != STOP) {
		for (int i = ntile - 1; i > 0; i--)
		{
			snakex[i] = snakex[i - 1];
			snakey[i] = snakey[i - 1];
		}

		snakex[0] = x;
		snakey[0] = y;

		//Collision of walls<
		if (walls) {
			if (x == 0 && dir == LEFT || y == 0 && dir == UP || x == xmap - 1 && dir == RIGHT || y == ymap - 1 && dir == DOWN) {
				command = "Exit";
				return;
			}
		}
		else {
			if (x == 0 && dir == LEFT) x = xmap;
			else if (y == 0 && dir == UP) y = ymap;
			else if (x == xmap - 1 && dir == RIGHT) x = -1;
			else if (y == ymap - 1 && dir == DOWN) y = -1;
		}
		//>Collision of walls

		if (dir == UP) y--;
		if (dir == DOWN) y++;
		if (dir == LEFT) x--;
		if (dir == RIGHT) x++;

		//Collision of snake<
		if (collision) {
			for (int i = 0; i < ntile; i++)
			{
				if (snakex[i] == x && snakey[i] == y) {
					command = "Exit";
					return;
				}
			}
		}
		//>Collision of snake
	}

	//collision of apple and apple spawn<
	if (x == applex && y == appley)
	{
		bool a = 1;
		snakex[ntile] = snakex[ntile - 1];
		snakey[ntile] = snakey[ntile - 1];
		ntile++;
		if (ntile >= xmap * ymap) {
			command = "Exit";
			return;
		}
		while (a) {
			a = 0;
			applex = rand() % xmap;
			appley = rand() % ymap;

			for (int i = 0; i < ntile; i++)
			{
				if (applex == snakex[i] && appley == snakey[i]) {
					a = 1;
					break;
				}
			}
			if (applex == x && appley == y)
				a = 1;
		}
	}

	return;
}

void Draw()
{
	mapp[appley][applex].setFillColor(Color(255,0,0));
	window.draw(mapp[appley][applex]);

	for (int i = 0; i < ntile; i++)
	{
		int clr = 255.0 - 225.0 * (((i + 1.0) / ntile) + 0.0);
		mapp[snakey[i]][snakex[i]].setFillColor(Color(0, 255, 0, clr));
		window.draw(mapp[snakey[i]][snakex[i]]);
	}

	mapp[y][x].setFillColor(Color(0, 255, 0));
	window.draw(mapp[y][x]);
	
	score = "Score: ";
	ostringstream oss;
	oss << ntile;
	score += oss.str();
	scoreLabel.setFont(font);
	scoreLabel.setString(score);
	scoreLabel.setCharacterSize(24);
	scoreLabel.setFillColor(sf::Color::White);

	window.draw(scoreLabel);

	return;
}

void CheckEvents() //Check all event
{
	Event event; //Objekt events
	while (window.pollEvent(event)) //Check all events
	{
		if (event.type == Event::Closed) //If press button "window close":
			window.close(); //Window close

		if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A)) {
			if (dir != RIGHT) dir = LEFT;
			tmp1 = 1;
		}else
		if (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W)) {
			if (dir != DOWN) dir = UP;
			tmp1 = 1;
		}else
		if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D)) {
			if (dir != LEFT) dir = RIGHT;
			tmp1 = 1;
		}else
		if (Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S)) {
			if (dir != UP) dir = DOWN;
			tmp1 = 1;
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			command = "Exit";
		}
	}

	return;
}

void Setup()
{
	dir = STOP;
	lastDir = STOP;

	x = xmap / 2, y = ymap / 2;
	ntile = 0;

	if (width / xmap < height / ymap)
		mapSize = width / xmap;
	else {
		mapSize = height / ymap;
	}

	for (int i = 0; i < ymap; i++) {
		for (int j = 0; j < xmap; j++)
		{
			mapp[i][j].setPosition(j * mapSize, i * mapSize);
			mapp[i][j].setSize(Vector2f(mapSize, mapSize));
		}
	}

	while (a1) {
		a1 = 0;
		applex = rand() % xmap;
		appley = rand() % ymap;

		for (int i = 0; i < ntile; i++)
		{
			if (applex == snakex[i] && appley == snakey[i]) {
				a1 = 1;
				break;
			}
		}
		if (applex == x && appley == y)
			a1 = 1;
	}

	window.setPosition(Vector2i(window.getPosition().x, 0));

	return;
}
//>functions