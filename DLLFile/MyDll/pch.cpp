// pch.cpp: source file corresponding to the pre-compiled header

#include "pch.h"
#include "card.h"
#include <vector>
#include <string.h>
#include <iostream>
#include <time.h>

void replyCard(CCard& _reply, CCard& _card, int& command, bool& repliesCard)
{
	command = 1;
	repliesCard = true;
	_reply = _card;
}

void thrownCardPrint(CCard& _reply)
{
	std::cout << "Player Throws Card: " << "[" << (int)_reply.type << "," << _reply.id << "," << (int)_reply.color << "]" << std::endl;
}

void setExtra(int& extra, int* colorsCount)
{
	extra = 0;
	for (int i = 0; i < 5; i++)
	{
		extra = (extra < colorsCount[i]) ? i : extra;
	}
	if (!extra)
	{
		srand(time(NULL));
		int randomColor = rand() % 4 + 1;
		extra = randomColor;
	}
}

 //When you are using pre-compiled headers, this source file is necessary for compilation to succeed.
extern "C"
{

	int tempCount{ 0 };

	bool repliesCard{ false };

	// last index will be highest number
	int colorsCount[5]{ 0,0,0,0,0 };

	int changColorIndex{ -1 };


	_declspec (dllexport) void Play(int state, std::vector<CCard> cards, CCard one, 
		std::vector<CCard> others, int& command, CCard& reply, int& extra)
	{
#define OTHERSLAST others.at(others.size() - 1)

		std::cout << "cards size: " << cards.size() << std::endl;
		std::cout << "others size: " << others.size() << std::endl;

		std::cout << "others last type:" << (int)OTHERSLAST.type << std::endl;
		std::cout << "others last id:" << OTHERSLAST.id << std::endl;
		std::cout << "others last color:" << (int)OTHERSLAST.color << std::endl;

		std::cout << "Have Cards:";
		for (std::vector<CCard>::iterator it = cards.begin(); it != cards.end() && !repliesCard; ++it)
		{
			std::cout << "[" << (int)it->type << "," << it->id << "," << (int)it->color << "]";
		}

		std::cout << std::endl;

		if (state == 2)
		{
			if (((int)OTHERSLAST.type == TYPE_ADDFOUR || (int)OTHERSLAST.type == TYPE_CHANGE) && (int)OTHERSLAST.color == BLACK)
			{
				replyCard(reply, cards.at(cards.size() - 1), command, repliesCard);
			}

			for (std::vector<CCard>::iterator it = cards.begin(); it != cards.end() && !repliesCard; ++it)
			{
				colorsCount[(int)(it->color)]++;

				switch ((int)it->type)
				{
				case TYPE_NORMAL:
					if (it->color == OTHERSLAST.color || it->id == OTHERSLAST.id)
					{
						replyCard(reply, *it, command, repliesCard);
						thrownCardPrint(reply);
					}
					break;
				case TYPE_SKIP:
				case TYPE_REVERSE:
				case TYPE_ADDTWO:
					if (it->color == OTHERSLAST.color)
					{
						replyCard(reply, *it, command, repliesCard);
						thrownCardPrint(reply);
					}
					break;

				case TYPE_CHANGE:
				case TYPE_ADDFOUR:
					changColorIndex = it - cards.begin();
					break;
				}
				std::cout << "Tmmp Counter: " << tempCount++ << std::endl;
				std::cout << "colorsCount: " << colorsCount[(int)(it->color)] << std::endl;
			}

			if (!repliesCard && changColorIndex != -1)
			{
				setExtra(extra, colorsCount);
				replyCard(reply, cards.at(changColorIndex), command, repliesCard);
				std::cout << "Throws change color or add four card" << std::endl;
			}
			else if (!repliesCard && changColorIndex == -1)
			{
				std::cout << "No card to throw, start command 2" << std::endl;
				command = 2;
			}
		}
		else if (state == 3)
		{
			switch ((int)one.type)
			{
			case TYPE_CHANGE:
			case TYPE_ADDFOUR:
				setExtra(extra, colorsCount);
			case TYPE_NORMAL:
				if (one.id != OTHERSLAST.id && one.color != OTHERSLAST.color)
					break;
			case TYPE_SKIP:
			case TYPE_REVERSE:
			case TYPE_ADDTWO:
				if (one.color != OTHERSLAST.color)
					break;

				repliesCard = true;
				command = 3;
				break;
			}

			if (!repliesCard)
			{
				command = 2;
				std::cout << "Still no card to throw, continues command 2" << std::endl;
			}
		}

		std::cout << "reply type: " << (int)reply.type << std::endl;
		std::cout << "reply id: " << reply.id << std::endl;
		std::cout << "reply color: " << (int)reply.color << std::endl;
		std::cout << "one type: " << (int)reply.type << std::endl;
		std::cout << "one id: " << reply.id << std::endl;
		std::cout << "one color: " << (int)reply.color << std::endl;
		std::cout << "state: " << state << std::endl;
		std::cout << "command: " << command << std::endl;
		return;
	}
}