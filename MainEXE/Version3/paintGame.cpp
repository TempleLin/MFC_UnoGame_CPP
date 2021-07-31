#include "pch.h"
#include "paintGame.h"
#include "typeIDColor.h"
#include <iostream>

// store each cards in x and y indexes.
// the x and y correspond to the image locations of x and y.
void PaintGame::storeCards(Players &players)
{
	auto setPlayersCount = [&]() {
		// last index will be for storing others cards
		for (int i = 0; i < players.players.size() + 1; i++)
			playersCardsPos.push_back(std::vector<int>());
	};
	auto setPos = [&](std::vector<int>& toSet, std::vector<CCard>& toGive) {
		for (int j = 0; j < toGive.size(); j++)
		{
			switch (static_cast<int>(toGive.at(j).type))
			{
			case TYPE_NORMAL:
				toSet.push_back(toGive.at(j).id);
				switch (static_cast<int>(toGive.at(j).color))
				{
				case RED:
					toSet.push_back(1);
					break;
				case ORANGE:
					toSet.push_back(2);
					break;
				case BLUE:
					toSet.push_back(3);
					break;
				case GREEN:
					toSet.push_back(4);
					break;
				}
				break;
			case TYPE_CHANGE:
				toSet.push_back(10);
				toSet.push_back(1);
				break;
			case TYPE_ADDTWO:
				switch (static_cast<int>(toGive.at(j).color))
				{
				case RED:
					toSet.push_back(5);
					break;
				case ORANGE:
					toSet.push_back(6);
					break;
				case BLUE:
					toSet.push_back(7);
					break;
				case GREEN:
					toSet.push_back(8);
					break;
				}
				toSet.push_back(5);
				break;
			case TYPE_ADDFOUR:
				toSet.push_back(10);
				toSet.push_back(3);
				break;
			case TYPE_SKIP:
				switch (static_cast<int>(toGive.at(j).color))
				{
				case RED:
					toSet.push_back(1);
					break;
				case ORANGE:
					toSet.push_back(2);
					break;
				case BLUE:
					toSet.push_back(3);
					break;
				case GREEN:
					toSet.push_back(4);
					break;
				}
				toSet.push_back(5);
				break;
			case TYPE_REVERSE:
				switch (static_cast<int>(toGive.at(j).color))
				{
				case RED:
					toSet.push_back(9);
					toSet.push_back(5);
					break;
				case ORANGE:
					toSet.push_back(10);
					toSet.push_back(5);
					break;
				case BLUE:
					toSet.push_back(1);
					toSet.push_back(6);
					break;
				case GREEN:
					toSet.push_back(2);
					toSet.push_back(6);
					break;
				}
				break;
			}
		}
	};
	auto setPlayersCardsPos = [&]() {
		for (int i = 0; i < playersCardsPos.size() - 1; i++)
		{
			setPos(playersCardsPos.at(i), players.players.at(i).getHoldCards());
		}
	};
	auto setOthersPos = [&]() {
		setPos(playersCardsPos.at(playersCardsPos.size() - 1), Players::PlayerCallParams::others);
	};
	playersCardsPos.clear();
	setPlayersCount();
	setPlayersCardsPos();
	setOthersPos();
}

std::vector<std::vector<int>>& PaintGame::getPlayersCardsPos()
{
	return playersCardsPos;
}

void PaintGame::setPaintGame(Players& players)
{
	storeCards(players);
}

void PaintGame::setListBoxPrint(CString toSet)
{
	listBoxToPrint = toSet;
}


CString& PaintGame::getListBoxPrint()
{
	return listBoxToPrint;
}