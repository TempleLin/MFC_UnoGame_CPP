#pragma once
#include "typeIDColor.h"

//class card
//{
//public:
//	int type;
//	int id;
//	int color;
//};
//
//inline bool operator == (card& card1, card& card2)
//{
//	return card1.id == card2.id && card1.color == card2.color && card1.type == card2.type;
//}

enum class Type { normal = 1, skip = 2, reverse = 3, drawtwo = 4, wild = 5, wild4 = 6 };
enum class Color { black = 0, red = 1, orange = 2, blue = 3, green = 4 };

class CCard
{
	friend class CDeck;
private:

public:
	Type type;
	int id;
	Color color;

	CCard() {}
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