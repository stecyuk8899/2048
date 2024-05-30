#include "Game2048.h"

void SetCurPos(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void SetColor(int text, int bk)
{
	char command[100];
	sprintf(command, "color %d%d", bk, text);
	system(command);
}

bool KeyDownOnce(char c)
{
	if (GetKeyState(c) < 0)
	{
		while (GetKeyState(c) < 0);
		return true;
	}
	return false;
}

void Screen::Clear() { memset(scr, ' ', sizeof(scr)); }

void Screen::Show()
{
	SetCurPos(0, 0);
	SetEnd();
	cout << scr[0];
}

void Screen::DisplayScore(int result)
{
	char resultBuf[80];
	sprintf(resultBuf, "Result: %d", result);
	size_t len = strlen(resultBuf);
	for (int i = 0; i < len; ++i)
		scr[0][i] = resultBuf[i];
	sprintf(resultBuf, "New game: Enter N");
	len = strlen(resultBuf);
	for (int i = 0; i < len; ++i)
		scr[1][i] = resultBuf[i];
}

void Screen::SetEnd() { scr[scr_height - 1][scr_width - 1] = '\0'; }

void Cell::Init(int x, int y, int value)
{
	this->value = value;
	pos = { x,y };
}

void Cell::Put(ScreenMap scr)
{
	for (int i = 0; i < cell_width; i++)
	{
		for (int j = 0; j < cell_height; j++)
		{
			scr[pos.y + j][pos.x + i] = (i == 0 || i == cell_width - 1 ||
				j == 0 || j == cell_height - 1) ? '*' : ' ';
		}
	}
	if (value == 0) return;
	char buf[10];
	sprintf(buf, "%d", value);
	size_t len = strlen(buf);
	size_t posX = (cell_width - len) / 2;
	size_t posY = (cell_height - 1) / 2;
	for (int i = 0; i < len; i++)
		scr[pos.y + posY][pos.x + i + posX] = buf[i];
}

void Game2048::Init()
{
	const int dx = 2;
	const int dy = 2;
	srand(time(NULL));
	for (unsigned int i = 0; i < fld_width; i++)
		for (unsigned int j = 0; j < fld_height; j++)
			cell[j][i].Init(dx + i * (cell_width - 1), dy + j * (cell_height - 1), 0);
	GenNewRandNum();
	GenNewRandNum();
}

void Game2048::Work()
{
	if (KeyDownOnce('W')) Move(0, -1);
	if (KeyDownOnce('S')) Move(0, 1);
	if (KeyDownOnce('A')) Move(-1, 0);
	if (KeyDownOnce('D')) Move(1, 0);
	if (KeyDownOnce(VK_UP)) Move(0, -1);
	if (KeyDownOnce(VK_DOWN)) Move(0, 1);
	if (KeyDownOnce(VK_LEFT)) Move(-1, 0);
	if (KeyDownOnce(VK_RIGHT)) Move(1, 0);
	if (KeyDownOnce('N'))
	{
		if (player.GetResult() > 0)
		{
			WriteToFileSerice(player);
			player.SetResult(0);
		}
		Init();
	}

}

void Game2048::Show()
{
	screen.Clear();
	screen.DisplayScore(player.GetResult());
	for (int i = 0; i < fld_width; i++)
		for (int j = 0; j < fld_height; j++)
			cell[i][j].Put(screen.scr);
	screen.Show();
}

bool Game2048::MoveValInArray(Cell* valArr[], int cnt)
{
	bool moved = false;
	int lastX = 0;
	for (int i = 1; i < cnt; i++)
	{
		if (valArr[i]->value != 0)
		{
			if (valArr[lastX]->value == 0)
			{
				moved = true;
				valArr[lastX]->value = valArr[i]->value;
				valArr[i]->value = 0;
			}
			else if (valArr[lastX]->value == valArr[i]->value)
			{
				moved = true;
				valArr[lastX]->value += valArr[i]->value;
				player.SetResult(player.GetResult() + valArr[lastX]->value);
				valArr[i]->value = 0;
				lastX++;
			}
			else if (valArr[lastX]->value != valArr[i]->value)
			{
				lastX++;
				if (lastX != i)
				{
					moved = true;
					valArr[lastX]->value = valArr[i]->value;
					valArr[i]->value = 0;
				}
			}
		}
	}
	return moved;
}

void Game2048::Move(int dx, int dy)
{
	bool moved = false;
	if (dx != 0)
	{
		for (int j = 0; j < fld_height; j++)
		{
			Cell* valArr[fld_width];
			for (int i = 0; i < fld_width; i++)
			{
				int x = (dx < 0) ? i : fld_width - i - 1;
				valArr[i] = &cell[j][x];
			}
			if (MoveValInArray(valArr, fld_width))
				moved = true;
		}
	}
	if (dy != 0)
	{
		for (int i = 0; i < fld_width; i++)
		{
			Cell* valArr[fld_height];
			for (int j = 0; j < fld_height; j++)
			{
				int y = (dy < 0) ? j : fld_height - j - 1;
				valArr[j] = &cell[y][i];
			}
			if (MoveValInArray(valArr, fld_height))
				moved = true;
		}
	}
	if (CheckEndGame())
	{
		WriteToFileSerice(player);
		Init();
		player.SetResult(0);
	}
	else
		if (moved)
			GenNewRandNum();
}

void Game2048::GenNewRandNum()
{
	if (GetFreeCellCnt() == 0) return;
	int cnt = 1;
	while (cnt > 0)
	{
		int x = rand() % fld_width;
		int y = rand() % fld_height;
		if (cell[y][x].value == 0)
			cell[y][x].value = (rand() % 10 == 0) ? 4 : 2, cnt--;
	}
}

int Game2048::GetFreeCellCnt()
{
	int count = 0;
	for (int i = 0; i < fld_width * fld_height; i++)
	{
		if (cell[0][i].value == 0)
			count++;
	}
	return count;
}

bool Game2048::CheckEndGame()
{
	if (GetFreeCellCnt() > 0)
		return false;
	for (int i = 0; i < fld_width; i++)
	{
		for (int j = 0; j < fld_height; j++)
		{
			if ((j < fld_height - 1 && cell[j][i].value == cell[j + 1][i].value) ||
				(i < fld_width - 1 && cell[j][i].value == cell[j][i + 1].value))
				return false;
		}
	}
	return true;
}

void ChangeColor()
{
	int chooseT, chooseB;
	cout << "Choose the colors :\n0) Black;\n1) Blue;\n2) Green;\n3) Cyan;\n4) Red;\n5) Violet;\n6) Yellow;\n7) White;\n8) Grey.\nEnter text and background colors : ";
	cin >> chooseT >> chooseB;
	SetColor(chooseT, chooseB);
}

void WriteToFileSerice(Player player)
{
	vector<Player> players = player.ReadFromFile();
	players.push_back(player);
	player.WriteOnFile(player.Sort(players));
}
