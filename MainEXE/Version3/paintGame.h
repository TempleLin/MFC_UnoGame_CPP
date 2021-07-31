#pragma once
#include "players.h"
#include <vector>
class PaintGame
{
private:
	//saves player's card x and y starting point.
	//there will be ints, placed by player order.
	std::vector<std::vector<int>> playersCardsPos;
	//std::vector<int> cardDetails;
	void storeCards(Players& players);
	CString listBoxToPrint;
public:
	//std::vector<int>& getCardDetails();
	std::vector<std::vector<int>>& getPlayersCardsPos();
	void setPaintGame(Players& players);
	void setListBoxPrint(CString toSet);
	CString& getListBoxPrint();
};

