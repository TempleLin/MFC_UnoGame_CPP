#pragma once
#include "card.h"
#include <filesystem>
#include <Windows.h>
#include <tchar.h>
#include "myFileManagement.h"

class BackProcess;
class MyDebugger;

#define FOULRANK -1
#define PLAYERROR -2
#define TIMEOUT -3
#define ABANDONED -4

#define CALL_DLL
#define JUDGE_GAME

class Player
{
private:
	int cardCount;
	std::vector<CCard> holdCards;
public:
	Player();
	Player(int& giveCardsCount, std::vector<CCard>& cards);
	int getCardCount();
	std::vector<CCard>& getHoldCards();
	void drawFromTable(int& drawCount, std::vector<CCard>& cards);
};

// pointer to player DLL's Play function
typedef void (*CallPlay)(int state, std::vector<CCard> cards, CCard one,
	std::vector<CCard> others, int& command, CCard& reply, int& extra);

class Players
{
private:
	void Players::gameJudge(int& command, CCard& reply, std::vector<CCard>& cards, int& pickCardCoroutine,
		std::vector<CCard>& others, int& extra, std::vector<CCard>& deck, bool& reverseOrder, int& state, CCard& one, int &coroutineWait, bool &endGame);
	CWinThread* pThread;
public:
	CallPlay callPlay;
	std::vector<Player> players;

	//ranking and playerID index corresponds to players vector index
	std::vector<int> ranking;

	//ID of player, index corresponds to players vector index
	std::vector<std::string> playersYZUID;

	void storePlayersID(std::string pathname, int playersCount);

	Players(int& drawCount, std::vector<CCard>& cards);
	~Players();
	void callDLLPlay(std::string pathname, std::vector<CCard>& deck);
	std::string playersCardsString();

	struct PlayerCallParams
	{
		inline static int state{ 1 };
		inline static CCard one{ (Type)0,0,(Color)0 };
		inline static std::vector<CCard> others{};
		inline static int command{ 0 };
		inline static CCard reply{ (Type)0,0,(Color)0 };
		inline static int extra{ 0 };
		inline static std::vector<CCard> cards{};
	};


	// the current player index number
	// game role turns to first player when playerIndex reaches 3
	inline static int playerIndex{ 0 };
};

