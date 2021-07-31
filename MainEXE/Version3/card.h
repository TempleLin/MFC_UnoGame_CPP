#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include "typeIDColor.h"

//{0, 0, 0} = empty card
enum class Type { normal = 1, skip = 2, reverse = 3, drawtwo = 4, wild = 5, wild4 = 6 };
enum class Color { black = 0, red = 1, orange = 2, blue = 3, green = 4 };

class CCard
{
public:
	Type type;
	int id;
	Color color;

	CCard(){}
	CCard(Type type, int id, Color color)
	{
		this->type = type;
		this->id = id;
		this->color = color;
	}
	virtual ~CCard() {}
	bool operator<(const CCard& other)
	{
		if (static_cast<int>(this->color) < static_cast<int>(other.color))
			return true;
		else if (static_cast<int>(this->color) == static_cast<int>(other.color))
		{
			if (this->id < other.id)
				return true;
			else if (this->id == other.id)
			{
				if (static_cast<int>(this->type) < static_cast<int>(other.type))
					return true;
				else if (static_cast<int>(this->type) == static_cast<int>(other.type))
					return false;
				else
					return false;
			}
			else
				return false;
		}
		else
			return false;
	}

};

bool operator < (CCard& card1, CCard& card2);

class CardEdit
{
private:
	void initCard(CCard& _card, int type, int id, int color);
public:
	void initTable(std::vector<CCard>& cards);
	void shuffleCards(std::vector<CCard>& cards);
	void sortCards(std::vector<CCard>& cards);
};

bool operator==(CCard& _card, CCard& _card2);
bool operator!=(CCard& _card, CCard& _card2);