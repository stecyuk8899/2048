#include "Game2048.h"

int main()
{
	char command[100];
	string name;
	sprintf(command, "mode con: cols=%d lines=%d", scr_width, scr_height);
	system(command);
	bool exit = false;
	char value;
	cout << "Enter name : ";
	cin >> name;
	system("cls");
	Game2048 game2048;
	cout << "Your name : " << name << endl;
	game2048.player.SetName(name);
	while (!exit)
	{
		cout << "Enter option: \n1) Start game;\n2) Change color;\n3) Best results;\n0) Exit.\nEnter : ";
		cin >> value;
		switch(value)
		{ 
			case '1':
				while (1)
				{
					game2048.Work();
					game2048.Show();
					if (GetKeyState(VK_ESCAPE) < 0)
						break;
					Sleep(10);
				}
				break;
			case '2':
				ChangeColor();
				break;
			case '3':
				game2048.player.Show(game2048.player.ReadFromFile());
				cout << "Press any key to exit" << endl;
				_getch();
				break;
			case '0':
			default:
				if (game2048.player.GetResult() > 0)
					WriteToFileSerice(game2048.player);
				exit = true;
				break;
		}
		system("cls");
	}
	return 0;
}