#include "pch.h"
#include "myFileManagement.h"

#include "players.h"
#include <cassert>
#include <iostream>

std::ofstream& FileManagement::operator << (std::ofstream& of, Players& players)
{
	of.open("Ranking.txt", std::ios::out | std::ios::trunc);

	for (int i = 0; i < players.ranking.size(); i++)
	{
		//assertion
		assert("player ranking space exists", players.ranking.at(i));
		of << players.playersYZUID[i] << ": ";

		switch (players.ranking.at(i))
		{

#ifdef OUT_FOUL_ERR
		case FOULRANK:
			of << "FOUL" << std::endl;
			break;
		case PLAYERROR:
			of << "ERROR" << std::endl;
			break;
		case TIMEOUT:
			of << "TIMEOUT" << std::endl;
			break;
		case ABANDONED:
			of << "ABANDONED" << std::endl;
			break;
#endif // OUT_FOUL_ERR

		default:
			of << players.ranking.at(i) << std::endl;
			break;
		}
	}

	of.close();
	return of;
}

void FileManagement::FileManager::outputRanks(Players& players)
{
#ifdef OUT_RANK

	ranksFile << players;

#endif // OUT_RANK
}

void FileManagement::FileManager::outputMoves(std::string move)
{
	*movesFile << move << std::endl;
	std::cout << move << std::endl;
	//listBoxString = move.c_str();
}

FileManagement::FileManager::FileManager()
{
	movesFile = new std::ofstream();
	movesFile->open("MovesLog.txt", std::ios::out | std::ios::trunc);
}

FileManagement::FileManager::~FileManager()
{
	movesFile->close();
	delete movesFile;
}

CString& FileManagement::FileManager::getListBoxString()
{
	return listBoxString;
}

void FileManagement::FileManager::setListBoxString(CString _listBoxString)
{
	listBoxString = _listBoxString;
}
