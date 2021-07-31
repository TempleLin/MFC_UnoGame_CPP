#include "pch.h"
#include "card.h"
#include "players.h"
#include <random>

/*
* initialization of a single card
*/
void CardEdit::initCard(CCard& _card, int type, int id, int color)
{
	_card.type = (Type)type;
	_card.id = id;
	_card.color = (Color)color;
}


bool operator < (CCard& card1, CCard& card2)
{
	if (card1.color < card2.color) return true;
	else if (card1.color == card2.color)
	{
		if (card1.id < card2.id) return true;
		else return false;
	}
	else
	{
		return false;
	}
}

/*
* initialization of all cards at start of game
*/
void CardEdit::initTable(std::vector<CCard>& cards)
{
	for (std::vector<CCard>::iterator it = cards.begin(); it != cards.end(); ++it)
	{
		int currentIndex = it - cards.begin();

		//for assigning id and color of card
		static int idAssign = ID_ONE;
		static int colorAssign = RED;

		//initialize numbered cards first
		if (currentIndex < 72)
		{
			initCard(cards[currentIndex], TYPE_NORMAL, idAssign, colorAssign);

			idAssign++;
			if (idAssign == 10)
				idAssign = 1;
			if (!((currentIndex + 1) % 18))
			{
				colorAssign++;
			}
		}
		//initialize special cards
		else if (72 <= currentIndex < 104)
		{
			int tempType;

			//lambda for assigning color to special cards (used in for loop)
			auto assignSpecialColor = [&]() {
				colorAssign = currentIndex % 4 + 1;
				if (tempType == TYPE_CHANGE || tempType == TYPE_ADDFOUR)
					colorAssign = BLACK;
			};

			//first initialize skip cards
			if (currentIndex <= 79)
				tempType = TYPE_SKIP;
			//next initialize reverse cards
			else if (currentIndex > 79 && currentIndex < 88)
				tempType = TYPE_REVERSE;
			//next initialize change color cards
			//color of the change color card doesn't matter
			else if (currentIndex >= 88 && currentIndex <= 95)
				tempType = TYPE_CHANGE;
			//next initialize add two cards
			else if (currentIndex > 95 && currentIndex < 100)
				tempType = TYPE_ADDTWO;
			//next initialize add four cards
			//color of the add four card doesn't matter
			else
				tempType = TYPE_ADDFOUR;


			assignSpecialColor();

			initCard(cards[currentIndex], tempType, ID_ZERO, colorAssign);
		}

	}

	srand(time(NULL));
	Players::PlayerCallParams::others.push_back(cards.at(rand() % cards.size()));
	if (Players::PlayerCallParams::others.at(0).color == static_cast<Color>(BLACK))
	{
		Players::PlayerCallParams::others.at(0).color = static_cast<Color>(rand() % 4 + 1);
	}
}
/*
* for shuffling cards
*/
void CardEdit::shuffleCards(std::vector<CCard>& cards)
{
	std::random_device rd;
	std::shuffle(cards.begin(), cards.end(), rd);
}

void CardEdit::sortCards(std::vector<CCard>& cards)
{
	std::sort(cards.begin(), cards.end());
}

bool operator==(CCard& _card, CCard& _card2)
{
	return _card.type == _card2.type && _card.id == _card2.id 
		&& _card.color == _card2.color;
}

bool operator!=(CCard& _card, CCard& _card2)
{
	return _card.type != _card2.type || _card.id != _card2.id
		|| _card.color != _card2.color;
}