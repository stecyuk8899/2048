#include "Game2048.h"

vector<Player> Player::ReadFromFile()
{
	ifstream in;
	int size = 0;
	in.open(filename);
	if (in.is_open())
		in >> size;
	vector<Player> players;
	string name = "";
	int result = 0;
	for(int i = 0; i < size; i++)
	{
		in >> name >> result;
		players.push_back(Player(name, result));
	}
	in.close();
	return players;
}

void Player::Show(vector<Player> players)
{
	cout << "Results : " << endl;
	for (Player player : players)
		cout << player.name << setw(20 - player.name.length()) << " :" << setw(6) << player.result << endl;
}

vector<Player> Player::Sort(vector<Player>& players)
{
		sort(players.begin(), players.end(), [](Player& a, Player& b) {
		return a.result > b.result;});
		return players;
}

void Player::WriteOnFile(vector<Player> players)
{
	ofstream out;
	out.open(filename);
	if (out.is_open())
	{
		out << players.size() << endl;
		for (int i = 0; i < players.size(); i++)
			out << players[i].name << " " << players[i].result << endl;
	}
	out.close();
}

void Player::SetResult(int result) { this->result = result; }

int Player::GetResult() { return result; }

void Player::SetName(string name) { this->name = name; }

string Player::GetName() { return name; }