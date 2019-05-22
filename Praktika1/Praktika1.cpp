#include "pch.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <windows.h>
#include <conio.h>
#include <time.h> 

using namespace std;

const char CDfv[] = "Monsters.txt";
const char CDfr[] = "Map.txt";

char MAP[999][999];
int Load = 0;

void Movements(struct Monsters m[]);
int main();

struct Monsters
{
	char Name[20];
	int Damage;
	int HP;
}m[5];

void SetColor(int value)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), value);
}

void gotoXY(int x, int y)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE); // used for goto
	COORD CursorPosition;
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console, CursorPosition);
}

void Enemies(struct Monsters m[])
{
	ifstream fd(CDfv);
	int n;
	fd >> n;
	cout << "Enter your Nickname: ";
	cin >> m[0].Name;
	m[0].Damage = 3;
	m[0].HP = 20;
	for (int i = 1; i <= n; i++) {
		fd >> m[i].Name >> m[i].HP >> m[i].Damage;
	}
	system("cls");
}

void Items(struct Monsters m[])
{

}

void MAPSuk()
{
	ifstream fd(CDfr);
	for (int i = 1; i < 50; i++) {
		for (int j = 1; j < 79; j++) {
			fd >> MAP[i][j];
		}
	}
}

void MAPSpau(COORD c)
{
	for (int i = 1; i <= 40; i++) {
		for (int j = 1; j <= 78; j++) {
			if (MAP[i][j] != '|' || MAP[i][j] != '=') {
				if (c.Y == i && c.X == j) {
					MAP[i + 1][j + 1] = char('X');
				}
			}
			cout << MAP[i][j];
		}cout << endl;
	}
}

void MAP_Ejimas(COORD c)
{
	for (int i = 1; i <= 40; i++) {
		for (int j = 1; j <= 78; j++) {
			if (c.Y == i && c.X == j) {
				MAP[c.Y + 1][c.X + 1] = char('o');
			}
			//cout << MAP[i][j];
		}//cout << endl;
	}
}

void Fight(struct Monsters m[])
{
	/*
	gotoXY(70, 7); cout << "                         ";*/
	srand(time(0));
	system("cls");
	int c = rand() % 100 + 1;
	if (c > 65) {
		int b = rand() % 3 + 1;
		int Hp = m[b].HP;
		int Escape = 0;
		int x = 13, fight = 0;
		gotoXY(48, 13); cout << "->";
		while (Hp > 0 || m[0].HP > 0) {
			gotoXY(20, 5); cout << "You";
			gotoXY(50, 5); cout << " VS ";
			gotoXY(70, 5); cout << m[b].Name;
			gotoXY(20, 7); cout << "                                                                      ";//delete previouse line kile "your were damaged by: " 
			gotoXY(70, 7); cout << "                                                                      "; //enemy
			gotoXY(20, 7); cout << "HP:  " << m[0].HP;
			gotoXY(70, 7); cout << "HP:  " << Hp; //enemy
			gotoXY(20, 8); cout << "Dmg: " << "0-" << m[0].Damage;
			gotoXY(70, 8); cout << "Dmg: " << "0-" << m[b].Damage;
			gotoXY(50, 13); cout << "Attack";
			gotoXY(50, 14); cout << "Use Potion";
			gotoXY(50, 15); cout << "Run";
			system("pause>nul");
			if (GetAsyncKeyState(VK_DOWN) && x != 15) //down button pressed
			{
				gotoXY(48, x); cout << "  ";
				x++;
				gotoXY(48, x); cout << "->";
				fight++;
			}
			if (GetAsyncKeyState(VK_UP) && x != 13) //up button pressed
			{
				gotoXY(48, x); cout << "  ";
				x--;
				gotoXY(48, x); cout << "->";
				fight--;
			}
			if (GetAsyncKeyState(VK_RETURN)) { // Enter key pressed
				switch (fight) {
				case 0: {//Attack
					int a = rand() % m[0].Damage + 0;
					gotoXY(55, 17); cout << "you damaged enemy by: " << a;
					/*gotoXY(55, 25);  cout << a;*/
					Hp = Hp - a;
					int e = rand() % m[b].Damage + 0;
					gotoXY(55, 18); cout << "you were damaged by: " << e;
					/*gotoXY(55, 26); cout << e;*/
					m[0].HP = m[0].HP - e;
					break;
				}
				case 1: {//Use Potion
					if (m[0].HP != 20) {
						m[0].HP = m[0].HP + 10;
						if (m[0].HP > 20) {
							m[0].HP = 20;
						}
						gotoXY(50, 17); cout << "                                                                      ";
						gotoXY(55, 17); cout << "you healed yourself";
						int e = rand() % m[b].Damage + 0;
						m[0].HP = m[0].HP - e;
						gotoXY(55, 18); cout << "you were damaged by: " << e;
					}
					else {
						gotoXY(50, 17); cout << "                                                                      ";
						gotoXY(50, 18); cout << "																       ";
						gotoXY(50, 17); cout << "Your HP already full";
					}  
					break;
				}
				case 2: {//Run
					int r = rand() % 10 - 1;
					if (r < 4) { 
						gotoXY(50, 17); cout << "                                                                      ";
						gotoXY(50, 17); cout << "You successfully escaped";
						Escape++;
						Sleep(1000);
					}
					else if (r >= 4 && r <= 7) {
						int e = rand() % m[b].Damage + 0;
						m[0].HP = m[0].HP - (rand() % m[b].Damage + 0);
						gotoXY(50, 17); cout << "                                                                      ";
						gotoXY(50, 17); cout << "During escape you were damaged by: "<<e;
						Escape++;
						Sleep(1000);
					}
					else {
						m[0].HP = m[0].HP - m[b].Damage;
						gotoXY(50, 17); cout << "You did not manage to escape and you were damaged by: "<< m[b].Damage;
						Sleep(100);        
					}
					break;
				}
				}
			}
			if (Hp < 1 || m[0].HP < 1 || Escape == 1) {
				break;
			}
		}
		system("cls");
	}
}

void Menu()
{
	int menu = 0, x = 7;
	gotoXY(18, 5); cout << "Main Menu";
	gotoXY(18, 7); cout << "->";
	while (true)
	{
		gotoXY(20, 7);  cout << "Start";
		gotoXY(20, 8);  cout << "Leaderboard";
		gotoXY(20, 9); cout << "Exit";
		system("pause>nul"); // the >nul bit causes it the print no message
		if (GetAsyncKeyState(VK_DOWN) && x != 9) //down button pressed
		{
			gotoXY(18, x); cout << "  ";
			x++;
			gotoXY(18, x); cout << "->";
			menu++;
		}
		if (GetAsyncKeyState(VK_UP) && x != 7) //up button pressed
		{
			gotoXY(18, x); cout << "  ";
			x--;
			gotoXY(18, x); cout << "->";
			menu--;
		}
		if (GetAsyncKeyState(VK_RETURN)) { // Enter key pressed
			switch (menu) {
			case 0: {//Start 
				//if (Load == 1) {
					system("cls");
					//Movements(m);
				//}
				return;
				break;
			}
			case 1: {//Leaderboard
				system("cls");
				break;
			}
			case 2: {//Exit
				exit(0);
			}
			}
		}
	}
}
void Movements(struct Monsters m[])
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { 30, 30 };
	MAPSpau({ 30,30 });
	SetConsoleCursorPosition(h, c);
	while (1)
	{
		int k = _getch();
		if (k == 0 || k == 224)  k = _getch();
		switch (k)
		{
		case 72: //72,75,77,80 - код для клавиш стрелок на клавиатуре
			if (c.Y > 1) {
				system("cls");
				MAP_Ejimas(c);
				c.Y--;
				//if (c.Y > 6) {
				Fight(m);
				//}
				MAPSpau(c);

			}
			break;
		case 75:
			if (c.X > 1) {
				system("cls");
				MAP_Ejimas(c);
				c.X--;
				//if (c.X > 8) {
				Fight(m);
				//}
				MAPSpau(c);
			}
			break;
		case 77:
			if (c.X < 76) {
				system("cls");
				MAP_Ejimas(c);
				c.X++;
				Fight(m);
				MAPSpau(c);

			}
			break;
		case 80:
			if (c.Y < 38) {
				system("cls");
				MAP_Ejimas(c);
				c.Y++;
				Fight(m);
				MAPSpau(c);

			}
			break;
		case 27:
			system("cls");
			Load = 1;
			Menu();
			break;
		//default: return 0;
		}
		SetConsoleCursorPosition(h, c);
	}
}
int main()
{
	SetColor(7);
	Menu();
	system("cls");
	Monsters m[6];
	Enemies(m);
	MAPSuk();
	Movements(m);
	return 0;
}