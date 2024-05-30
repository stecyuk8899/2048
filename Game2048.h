#pragma once
#pragma warning(disable : 4996)
#include <iostream>
#include <windows.h>
#include <cmath>
#include <conio.h>
#include <iomanip>
#include <cstdio>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

const int scr_width = 80;
const int scr_height = 25;
const int cell_width = 8;
const int cell_height = 5;
const int fld_width = 4;
const int fld_height = 4;
const string filename = "results.txt";

void SetCurPos(int x, int y);
void SetColor(int text, int bk);
void ChangeColor();
bool CheckColor(int color);

typedef char ScreenMap[scr_height][scr_width];

class Screen
{
public:
	ScreenMap scr;
	Screen() { Clear(); }
	void Clear();
	void Show();
	void DisplayScore(int result);
private:
	void SetEnd();
};

class Cell
{
public:
	int value;
	POINT pos;
	Cell() { Init(0, 0, 0); }
	void Init(int x, int y, int value);
	void Put(ScreenMap scr);
};

class Player
{
public:
	Player() : name(""), result(0) {}
	Player(string name, int result) : name(name), result(result) {}
	vector<Player> ReadFromFile();
	void Show(vector<Player> players);
	vector<Player> Sort(vector<Player>& players);
	void WriteOnFile(vector<Player> players);
	void SetResult(int result);
	int GetResult();
	void SetName(string name);
	string GetName();

private:
	string name;
	int result = 0;
};

class Game2048
{
public:
	Game2048() { Init(); }
	void Init();
	void Work();
	void Show();
	Player player;
private:
	Screen screen;
	Cell cell[fld_height][fld_width];
	bool MoveValInArray(Cell* valArr[], int cnt);
	void Move(int dx, int dy);
	void GenNewRandNum();
	int GetFreeCellCnt();
	bool CheckEndGame();
};

void WriteToFileSerice(Player player);