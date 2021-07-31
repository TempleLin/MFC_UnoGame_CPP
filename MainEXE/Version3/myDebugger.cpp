#include "pch.h"
#include "myDebugger.h"

//class MyDebugger
//{
//public:
//	void printCards(std::vector<card> cards);
//	void writeLine(std::string line);
//};

void MyDebugger::printCards(std::vector<CCard> cards)
{
	for (std::vector<CCard>::iterator it = cards.begin(); it != cards.end(); ++it)
	{
		std::cout << "---------------------------------------------------------------" << std::endl;
		std::cout << "Index: " << it - cards.begin() << std::endl;
		std::cout << "Type: " << static_cast<int>(it->type) << std::endl;
		std::cout << "ID: " << it->id << std::endl;
		std::cout << "Color: " << static_cast<int>(it->color) << std::endl;
		std::cout << "---------------------------------------------------------------" << std::endl;
	}
}

void MyDebugger::writeLine(std::string line)
{
	std::cout << line << std::endl;
}

void MyDebugger::printPlayersCards(Players& players)
{
	writeLine("Player1:");
	printCards(players.players[0].getHoldCards());
	writeLine("Player2:");
	printCards(players.players[1].getHoldCards());
	writeLine("Player3:");
	printCards(players.players[2].getHoldCards());
	writeLine("Player4:");
	printCards(players.players[3].getHoldCards());
}

void MyDebugger::playerAction(int playerIndex, std::string action)
{
	writeLine("Player" + std::to_string(playerIndex) + "---" + action);
}