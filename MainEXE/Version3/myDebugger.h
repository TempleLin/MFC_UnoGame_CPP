#pragma once
#include <vector>
#include <iostream>
#include "card.h"
#include "players.h"

class MyDebugger
{
	friend class Players;
public:
	void printCards(std::vector<CCard> cards);
	void writeLine(std::string line);
	void printPlayersCards(Players& players);
	void playerAction(int playerIndex, std::string action);
};

