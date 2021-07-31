#pragma once
#include <fstream>
//#include "myDebugger.h"

class Players;

#define OUT_RANK
#define OUT_FOUL_ERR

namespace FileManagement
{
	class FileManager
	{
	private:
		std::ofstream ranksFile;
		std::ofstream* movesFile;
		CString listBoxString;

	public:
		friend std::ofstream& operator << (std::ofstream& of, Players& players);

		void outputRanks(Players& players);
		void outputMoves(std::string move);
		CString& getListBoxString();
		void setListBoxString(CString _listBoxString);

		FileManager();
		~FileManager();
		//void clearFiles();
	};
}
