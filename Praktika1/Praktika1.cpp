#include "pch.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <windows.h>
#include <conio.h>
#include <random>

using namespace std;

const char CDfv[] = "Monsters.txt";
const char CDfr[] = "Map.txt";
const char CDfw[] = "Weapons.txt";
const char CDfa[] = "Armor.txt";

char MAP[999][999];
int dmg = 0;
int Potions[7] = {10,5,20,0,50,0};
int Inventory = 5;
int HPMAX = 10;
int sshop[11] = { 0,0,0,0,0,0,0,0,0,0 };
int LvL = 1;
int buff = 0;
int ACH[6] = { 0,0,0,0,0 };
int esc = 0;

void Movements(struct Monsters m[], struct Items t[]);
void Fight(struct Monsters m[], int b);
void Shop(struct Items t[], struct Monsters m[], COORD c);
void LVL(struct Monsters m[]);

struct Monsters
{
	char Name[25];
	int Damage;
	int HP;
	int Gold;
	int XP;
}m[19];

struct Items
{	
	char Armor[20];
	char Weapons[20];
	int HPbuff;
	int Dmgbuff;
}t[10];

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

void Achievements()
{
	if (ACH[1]==200) {
		m[0].HP += 5;
		m[0].Damage += 2;
		dmg += 2;
	}
	else if (ACH[2] == 10){
		m[0].Gold += 2500;
		m[0].XP += 1000;
	}
	else if (ACH[3]==50) {
		m[0].Gold += 200;
		m[0].XP += 100;
	}
	else if (ACH[4] == 10000) {
		m[0].Damage += 12;
		m[0].XP += 3000;
	}
}

void Item(struct Items t[])
{
	int a;
	ifstream fw(CDfw);
	ifstream fa(CDfa);
	fw >> a;
	for (int i = 1; i <= a; i++) {
		fw >> t[i].Weapons >> t[i].Dmgbuff;
		fa >> t[i].Armor >> t[i].HPbuff;
	}
}

void Enemies(struct Monsters m[])
{
	int n;
	ifstream fd(CDfv);
	fd >> n;
	cout << "Enter your Nickname: ";
	cin >> m[0].Name;
	m[0].Damage = 3;
	m[0].HP = 10;
	m[0].Gold = 0;
	m[0].XP = 0;
	for (int i = 1; i <= n; i++) {
		fd >> m[i].Name >> m[i].HP >> m[i].Damage >> m[i].Gold >> m[i].XP;
	}
	system("cls");
}

void MAPSuk()
{
	ifstream fr(CDfr);
	for (int i = 1; i < 50; i++) {
		for (int j = 1; j < 79; j++) {
			fr >> MAP[i][j];
		}
	}
}

void MAPSpau(COORD c, struct Monsters m[])
{
	for (int i = 1; i <= 40; i++) {
		for (int j = 1; j <= 78; j++) {
				if (c.Y == i && c.X == j) {
					MAP[i + 1][j + 1] = char('X');
				}
			cout << MAP[i][j];
		}cout << endl;
	}
	cout << m[0].Name << endl;  
	cout << "LVL: " << LvL << endl;
	cout << "Xp: " << 100 * LvL <<" / "<< m[0].XP; if (m[0].XP != 0) { cout << " --- " << m[0].XP / LvL << "%"; }
	cout << endl;
	cout << "Hp: " << m[0].HP << endl;
	cout << "Dmg: " << m[0].Damage << endl;
	cout << "Gold: " << m[0].Gold << endl;
	gotoXY(32, 40); cout << "'v' - Safe zone";
	gotoXY(32, 41); cout << "'r' - Rock, you can't go through";
	gotoXY(32, 42); cout << "'f' - 1-2 lvl zone";
	gotoXY(32, 43); cout << "'h' - 3-4 lvl zone";
	gotoXY(32, 44); cout << "'n' - 5-6 lvl zone";
	gotoXY(32, 45); cout << "'o' - 7-8 lvl zone";
	gotoXY(32, 46); cout << "'d' - 9-10 lvl zone";
	gotoXY(32, 47); cout << "Spacebar - open shop, work only in 'v' zone";
	gotoXY(32, 48); cout << "ESC - Main menu, 'Start' will continue the game";
	gotoXY(32, 49); cout << "Arrows keys to move and Enter to action";
	gotoXY(5, 47); cout << "Achievements";
	gotoXY(0, 48); cout << "Kill 200 monsters:  " << 200 << " / " << ACH[1];
	gotoXY(0, 49); cout << "Kill 50 Lone-Wolfs: " << 50 << " / " << ACH[3];
	gotoXY(0, 50); cout << "Earn 10000 gold:    " << 10000 << " / " << ACH[4];
	gotoXY(0, 51); cout << "Kill 10 Dragons:    " << 10 << " / " << ACH[2];
}

void LVL(struct Monsters m[])
{
	int lvlup = 100 * LvL;
	if (m[0].XP >= lvlup) {
		LvL += 1;
		m[0].XP -= lvlup;
		m[0].HP += 5;
		HPMAX += 5;
		buff += 1;
	}
	if (buff == 2) {
		m[0].Damage += 1;
		buff = 0;
	}
}

void MAP_Ejimas(COORD c)
{
	for (int i = 1; i <= 40; i++) {
		for (int j = 1; j <= 78; j++) {
			if (c.Y == i && c.X == j) {
				if (c.Y < 9 && c.X < 12) {
					MAP[c.Y + 1][c.X + 1] = char('v');
				}
				else if (c.Y > 0 && c.X < 26 && c.Y <29) {
					MAP[c.Y + 1][c.X + 1] = char('f');
				}
				else if (c.X < 44 && c.Y <29) {
					MAP[c.Y + 1][c.X + 1] = char('h');
				}
				else if (c.X < 61 && c.Y < 29) {
					MAP[c.Y + 1][c.X + 1] = char('n');
				}
				else if (c.X < 78 && c.Y < 29) {
					MAP[c.Y + 1][c.X + 1] = char('o');
				}
				else if (c.Y == 29 && c.X > 66 && c.X < 78) {
					MAP[c.Y + 1][c.X + 1] = char('-');

				}
				else if (c.Y > 33 && c.Y < 40 && c.X > 2 && c.X < 5) {
					MAP[c.Y + 1][c.X + 1] = char('B');
				}
				else {
					MAP[c.Y + 1][c.X + 1] = char('d');
				}
			}
		}
	}
}

void EmenyCha(struct Monsters m[], COORD c)
{
	random_device rd;
	mt19937 gen(rd());
	if (c.Y == 29 && c.X > 66 && c.X < 78) {
		system("cls");
		cout << "Fly you fools";
		Sleep(500);
		system("cls");
	}
	else {
		uniform_int_distribution<> dis(1, 100);
			int d = dis(gen);
			if ((d > 0 && d < 16) || (d > 24 && d < 45) || (d > 57 && d < 84)) {
				if (MAP[c.Y][c.X] == char('f')) {
					uniform_int_distribution<> dis(1, 3);
					int b = dis(gen);
					Fight(m, b);
				}
				else if (MAP[c.Y][c.X] == char('h')) {
					uniform_int_distribution<> dis(4, 6);
					int b = dis(gen);
					Fight(m, b);
				}
				else if (MAP[c.Y][c.X] == char('n')) {
					uniform_int_distribution<> dis(7, 9);
					int b = dis(gen);
					Fight(m, b);
				}
				else if (MAP[c.Y][c.X] == char('o')) {
					uniform_int_distribution<> dis(10, 13);
					int b = dis(gen);
					Fight(m, b);
				}
				else if (MAP[c.Y][c.X] == char('d')) {
					uniform_int_distribution<> dis(14, 16);
					int b = dis(gen);
					Fight(m, b);
				}
				else if (MAP[c.Y][c.X] == char('B')) {
					int b = 17;
					cout << "B" << b;
					Fight(m, b);
				}
			}

	}
	
}

void Fight(struct Monsters m[], int b)
{
	srand(time(0));
	system("cls");
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
			gotoXY(20, 9); cout << "Potion +10: " << Potions[1];
			gotoXY(20, 10); cout << "Potion +50: " << Potions[3];
			gotoXY(20, 11); cout << "Potion +20: " << Potions[5];
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
					int a = rand() % m[0].Damage + dmg;
					gotoXY(55, 17); cout << "you damaged enemy by: " << a;
					Hp = Hp - a;
					int e = rand() % m[b].Damage + 0;
					gotoXY(55, 18); cout << "you were damaged by: " << e;
					m[0].HP = m[0].HP - e;
					break;
				}
				case 1: {//Use Potion
					bool a = true;
					int p = 0;
					int x = 19;
					int Potion = 0;
					gotoXY(55, 17); cout << "                                                                      ";
					gotoXY(55, 18); cout << "																       ";
					gotoXY(48, 19); cout << "->";
					while (a) {
						gotoXY(50, 19); cout << "Potion +10";
						gotoXY(50, 20); cout << "Potion +20";
						gotoXY(50, 21); cout << "Potion +50";
						system("pause>nul");
						if (GetAsyncKeyState(VK_DOWN) && x != 21) //down button pressed
						{
							gotoXY(48, x); cout << "  ";
							x++;
							gotoXY(48, x); cout << "->";
							Potion++;
						}
						if (GetAsyncKeyState(VK_UP) && x != 19) //up button pressed
						{
							gotoXY(48, x); cout << "  ";
							x--;
							gotoXY(48, x); cout << "->";
							Potion--;
						}
						if (GetAsyncKeyState(VK_RETURN)) {
							switch (Potion)
							{
							case 0: {
								if (m[0].HP != HPMAX && Potions[1] > 0) {
									m[0].HP = m[0].HP + 10;
									if (m[0].HP > HPMAX) {
										m[0].HP = HPMAX;
									}
									Inventory--;
									Potions[1] -= 1;
									gotoXY(55, 17); cout << "you healed yourself";
									int e = rand() % m[b].Damage + 0;
									m[0].HP = m[0].HP - e;
									gotoXY(55, 18); cout << "you were damaged by: " << e;
								}
								else {
									gotoXY(55, 17); cout << "Your HP is already full";
								}
								p++;
								break;
							}
							case 1: {
								if (m[0].HP != HPMAX && Potions[3] > 0) {
									m[0].HP = m[0].HP + 20;
									if (m[0].HP > HPMAX) {
										m[0].HP = HPMAX;
									}
									Inventory--;
									Potions[3] -= 1;
									gotoXY(55, 17); cout << "you healed yourself";
									int e = rand() % m[b].Damage + 0;
									m[0].HP = m[0].HP - e;
									gotoXY(55, 18); cout << "you were damaged by: " << e;
								}
								else {
									gotoXY(55, 17); cout << "Your HP is already full";
								}
								p++;
								break;
							}
							case 2: {
								if (m[0].HP != HPMAX && Potions[5]>0) {
									m[0].HP = m[0].HP + 50;
									if (m[0].HP > HPMAX) {
										m[0].HP = HPMAX;
									}
									Inventory--;
									Potions[5] -= 1;
									gotoXY(55, 17); cout << "you healed yourself";
									int e = rand() % m[b].Damage + 0;
									m[0].HP = m[0].HP - e;
									gotoXY(55, 18); cout << "you were damaged by: " << e;
								}
								else {
									gotoXY(55, 17); cout << "Your HP is already full";
								}
								p++;
								break;
							}
							}
						}
						if (p == 1) {
							a = false;
							gotoXY(48, 19); cout << "                                   ";
							gotoXY(48, 20); cout << "                                   ";
							gotoXY(48, 21); cout << "                                   ";
						}
					}
					break;
				}
				case 2: {//Run
					int r = rand() % 10 - 1;
					gotoXY(50, 17); cout << "                                                                      ";
					gotoXY(50, 18); cout << "                                                                      ";
					if (r < 4) {
						gotoXY(55, 17); cout << "You successfully escaped";
						Escape++;
						Sleep(500);
					}
					else if (r >= 4 && r <= 7) {
						int e = rand() % m[b].Damage + 0;
						m[0].HP = m[0].HP - (rand() % m[b].Damage + 0);
						gotoXY(55, 17); cout << "During escape you were damaged by: " << e;
						Escape++;
						Sleep(500);
					}
					else {
						m[0].HP = m[0].HP - m[b].Damage;
						gotoXY(55, 17); cout << "You did not manage to escape and you were damaged by: " << m[b].Damage;
						Sleep(500);
					}
					break;
				}
				}
			}
			if (Hp < 1)
			{
				gotoXY(50, 17); cout << "                                                                      ";
				gotoXY(50, 18); cout << "                                                                      ";
				gotoXY(55, 17); cout << "You won";
				Sleep(500);
				m[0].Gold += m[b].Gold;
				m[0].XP += m[b].XP;
				
				ACH[4] += m[b].Gold;
				ACH[1] += 1;
				if (b == 16) {
					ACH[2] +=1;
				}
				if (b == 2) {
					ACH[3] += 1;
				}
				Achievements();
				LVL(m);
				break;
			}
			if(m[0].HP < 1 || Escape == 1) {
				system("cls");
				cout << "Death: You failed you mission";
				Sleep(500);
				exit(0);
			}
		}
		system("cls");
}

void Menu(struct Monsters m[], struct  Items t[])
{
	int menu = 0, x = 7;
	gotoXY(18, 5); cout << "Main Menu";
	gotoXY(18, 7); cout << "->";
	while (true)
	{
		if (esc == 1) {
			gotoXY(20, 7);  cout << "Continue";
		}
		else {
			gotoXY(20, 7);  cout << "Start";
		}
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
				if (esc == 1) {
					esc = 0;
					system("cls");
					Movements(m, t);
				}
				else {
					system("cls");
					return;
				}
				break;
			}
			case 1: {//Leaderboard
				ofstream fr;
				//fr << "<html> <head> </head><body><table><tr><th>score</th><th>xp</th><tr><td>"<<m[0].Name<<"</td><td>"<<"GOLD</td><tr><td>pers2</td><td>coin2</td></table></body>";
				break;
			}
			case 2: {//Exit
				exit(0);
			}
			}
		}
	}
}
void Movements(struct Monsters m[], struct  Items t[])
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { 5, 5 };
	MAPSpau({ 5,5 },m);
	SetConsoleCursorPosition(h, c);
	while (1)
	{
		int k = _getch();
		if (k == 0 || k == 224)  k = _getch();
		switch (k)
		{
		case 72: //72,75,77,80 - arrow keys
			if (c.Y > 1) {
				if (c.Y != 34) {
					system("cls");
					MAP_Ejimas(c);
					c.Y--;
					EmenyCha(m, c);
					MAPSpau(c,m);
				}
				else if (c.X >= 67) {
					system("cls");
					MAP_Ejimas(c);
					c.Y--;
					EmenyCha(m, c);
					MAPSpau(c, m);
				}
			}
			break;
		case 75:
			if (c.X > 1) {
				system("cls");
				MAP_Ejimas(c);
				c.X--;
				EmenyCha(m, c);
				MAPSpau(c, m);
			}
			break;
		case 77:
			if (c.X < 76) {
				system("cls");
				MAP_Ejimas(c);
				c.X++;
				EmenyCha(m, c);
				MAPSpau(c, m);
			}
			break;
		case 80:
			if (c.Y < 38) {
				if (c.Y != 28) {
					system("cls");
					MAP_Ejimas(c);
					c.Y++;
					EmenyCha(m, c);
					MAPSpau(c, m);
				}
				else if (c.X >= 67) {
					system("cls");
					MAP_Ejimas(c);
					c.Y++;
					EmenyCha(m, c);
					MAPSpau(c, m);
				}
			}
			break;
		case 27:
			system("cls");
			esc += 1;
			Menu(m,t);
			break;
		case 0x20:

			if (c.Y < 9 && c.X < 12) {
				system("cls");
				Shop(t, m, c);
			}
		break;
		}
		SetConsoleCursorPosition(h, c);
	}
}


void Shop(struct Items t[], struct Monsters m[], COORD c)
{
	int shop = 0, x = 5, Back = 0;
	gotoXY(10, 5); cout << "Swords";
	gotoXY(10, 10); cout << "Armor";
	gotoXY(18, 5); cout << "->";
	while (true)
	{
		gotoXY(20, 5); cout << "Wooden(2) 20";
		gotoXY(20, 6); cout << "Iron(5) 50";
		gotoXY(20, 7); cout << "Steel(10) 300";
		gotoXY(20, 8); cout << "Titanium(18) 1000";
		gotoXY(20, 9); cout << "Diamond(25) 10000";
		gotoXY(20, 10); cout << "Leather(10) 30";
		gotoXY(20, 11); cout << "Iron(20) 60";
		gotoXY(20, 12); cout << "Steel(30) 350";
		gotoXY(20, 13); cout << "Diamond(40) 1200";
		gotoXY(20, 14); cout << "Dragon(60) 15000";
		gotoXY(20, 15); cout << "Heal Potion 10 - 20";
		gotoXY(20, 16); cout << "Heal Potion 20 - 100";
		gotoXY(20, 17); cout << "Heal Potion 50 - 500";
		gotoXY(20, 18); cout << "Back";
		gotoXY(15, 20); cout << "Your Gold: "<< m[0].Gold;
		system("pause>nul"); // the >nul bit causes it the print no message
		gotoXY(25, 20); cout << "                                                                          ";
		gotoXY(25, 21); cout << "                                                                          ";
		if (GetAsyncKeyState(VK_DOWN) && x != 18) //down button pressed
		{
			gotoXY(18, x); cout << "  ";
			x++;
			gotoXY(18, x); cout << "->";
			shop++;
		}
		if (GetAsyncKeyState(VK_UP) && x != 5) //up button pressed
		{
			gotoXY(18, x); cout << "  ";
			x--;
			gotoXY(18, x); cout << "->";
			shop--;
		}
		if (GetAsyncKeyState(VK_RETURN)) { // Enter key pressed
			switch (shop) {
			case 0: {
				if (m[0].Gold >= 20 && sshop[0]==0) {
					m[0].Damage = m[0].Damage + 2;
					dmg = 2;
					m[0].Gold -= 20;
					sshop[0] = 1;
				}
				else {
					gotoXY(25, 21); cout << "Not enought gold or you already bought this item or better one";
				}
				break;
			}
			case 1: {
				if (m[0].Gold >= 50 && sshop[1] == 0) {
					m[0].Damage = m[0].Damage + 3;
					dmg += 3;
					m[0].Gold -= 50;
					sshop[1] = 1;
				}
				else {
					gotoXY(25, 21); cout << "Not enought gold or you already bought this item or better one";
				}
				break;
			}
			case 2: {
				if (m[0].Gold >= 300 && sshop[2] == 0) {
					m[0].Damage = m[0].Damage + 5;
					dmg += 5;
					m[0].Gold -= 300;
					sshop[2] = 1;
				}
				break;
			}
			case 3: {
				if (m[0].Gold >= 1000 && sshop[3] == 0) {
					m[0].Damage = m[0].Damage + 8;
					dmg += 8;
					m[0].Gold -= 1000;
					sshop[3] = 1;
				}
				else {
					gotoXY(25, 21); cout << "Not enought gold or you already bought this item or better one";
				}
				break;
			}
			case 4: {
				if (m[0].Gold >= 10000 && sshop[4] == 0) {
					m[0].Damage = m[0].Damage + 7;
					dmg += 7; 
					m[0].Gold -= 10000;
					sshop[4] = 1;
				}
				else {
					gotoXY(25, 21); cout << "Not enought gold or you already bought this item or better one";
				}
				break;
			}
			case 5: {
				if (m[0].Gold >= 30 && sshop[5] == 0) {
					m[0].HP = m[0].HP + 10;
					HPMAX += 10;
					m[0].Gold -= 30;
					sshop[5] = 1;
				}
				else {
					gotoXY(25, 21); cout << "Not enought gold or you already bought this item or better one";
				}
				break;
			}
			case 6: {
				if (m[0].Gold >= 60 && sshop[6] == 0) {
					m[0].HP = m[0].HP + 10;
					HPMAX += 10;
					m[0].Gold -= 60;
					sshop[6] = 1;
				}
				else {
					gotoXY(25, 21); cout << "Not enought gold or you already bought this item or better one";
				}
				break;
			}
			case 7: {
				if (m[0].Gold >= 350 && sshop[7] == 0) {
					m[0].HP = m[0].HP + 10;
					HPMAX += 10;
					m[0].Gold -= 350;
					sshop[7] = 1;
				}
				else {
					gotoXY(25, 21); cout << "Not enought gold or you already bought this item or better one";
				}
				break;
			}
			case 8: {
				if (m[0].Gold >= 1200 && sshop[8] == 0) {
					m[0].HP = m[0].HP + 10;
					HPMAX += 10;
					m[0].Gold -= 1200;
					sshop[8] = 1;
				}
				else {
					gotoXY(25, 21); cout << "Not enought gold or you already bought this item or better one";
				}
				break;
			}
			case 9: {
				if (m[0].Gold >= 15000 && sshop[9] == 0 && ACH[2] == 10 ) {
					m[0].HP = m[0].HP + 20;
					HPMAX += 20;
					m[0].Gold -= 15000;
					sshop[9] = 1;
				}
				else {
					gotoXY(25, 21); cout << "Not enought gold or you already bought this item or you haven't complited the quest ";
				}
				break;
			}
			case 10: {
				if (Inventory < 26 && m[0].Gold >=20) {
					Inventory++;
					Potions[1] += 1;
					m[0].Gold -= 20;
				}
				else {
					gotoXY(25, 21); cout << "Not enought gold or your inventory is full";
				}
				break;
			}
			case 11: {
				if (Inventory < 26 && m[0].Gold >=100) {
					Inventory++;
					Potions[3] += 1;
					m[0].Gold -= 100;
				}
				else {
					gotoXY(25, 21); cout << "Not enought gold or your inventory is full";
				}
				break;
			}
			case 12: {
				if (Inventory < 26 && m[0].Gold >= 500) {
					Inventory++;
					Potions[5] += 1;
					m[0].Gold -= 500;
					break;
				}
				else {
					gotoXY(25, 21); cout << "Not enought gold or your inventory is full";
				}
			}
			case 13: {
				Back++;
				system("cls");
			}
			}
		}
		if (Back == 1) {
			break; 
		}
	}
	MAPSpau(c, m);
}

int main()
{
	SetColor(7);
	Menu(m,t);
	cout << "It was a peaceful place, till the nightmare began, the Demon Tower appeared in the mountains" << endl;
	cout <<"and monsters started to occupy the kindgom." << endl;
	cout << "The kingdom managed to constrain distribution of monsters," << endl;
	cout << "but to destroy all of them it is necessary to defeat the Demon Lord who is living in the Tower" << endl;
	cout << "Heroes and bravest from whole kingdom, went to defeat the demon" << endl;
	cout << "and you are one of them and here you story begin..." << endl;  
	cout << "press Enter to start";
	system("pause>nul");
	system("cls");
	Monsters m[19];
	Items t[10];
	Item(t);
	Enemies(m);
	MAPSuk();
	Movements(m, t);
	return 0;
}