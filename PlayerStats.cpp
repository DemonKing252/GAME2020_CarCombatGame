#include "PlayerStats.h"
vector <float> PlayerStats::m_vHighestTimes;
vector <float> PlayerStats::m_vAccuracy;

int PlayerStats::accuracy_size = 0;
int PlayerStats::time_size = 0;

int PlayerStats::kills = 0;
int PlayerStats::deaths = 0;

PlayerStats::PlayerStats()
{
}


PlayerStats::~PlayerStats()
{
}


ostream& operator<<(ostream& stream, PlayerStats)
{
	// TODO: insert return statement here
	stream << "SIZE: " << PlayerStats::m_vHighestTimes.size() << endl << endl
		   << "------------------" << endl;

	for (const auto& iter : PlayerStats::m_vHighestTimes)
		stream << iter << endl;

	stream << "SIZE: " << PlayerStats::m_vAccuracy.size() << endl << endl
		<< "------------------" << endl;

	for (const auto& iter : PlayerStats::m_vAccuracy)
		stream << iter << endl;

	stream << "Kills: " << PlayerStats::kills << " " << endl
		   << "Deaths: " << PlayerStats::deaths << " " << endl;
	return stream;
}
