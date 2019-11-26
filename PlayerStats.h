#pragma once
#include <vector>
#include <iostream>		// For ostream and istream
using namespace std;
class PlayerStats
{
public:
	static int kills;
	static int deaths;

	static int time_size;
	static int accuracy_size;
	static vector <float> m_vHighestTimes;
	static vector <float> m_vAccuracy;

	friend ostream& operator<<(ostream&, PlayerStats);

	PlayerStats();
	~PlayerStats();
};