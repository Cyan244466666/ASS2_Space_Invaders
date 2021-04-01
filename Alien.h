#pragma once
#include <SDL.h>
#include "GameObject.h"
#include "Game.h"


enum alienType
{
	Squid,
	Crab,
	Octopus
};
// This class contains an individual alien. This includes 
// the UFO:
class Alien : public GameObject
{
private:
	// This value is how many points the alien is worth when killed
	// by the player:
	int m_PointValue; 
	int m_SpriteNumber;
	// The variables track where the alien is:
	int m_Index;
	int m_Column;
	bool m_InFront;

	//Holds what type the alien is:
	alienType m_Type;

public:
	// Constructor:
	Alien() 
	{
		m_PointValue = NULL;
		Vector2 position{ 200, 300 };
		SetPosition(position);
		Vector2 scale{ 25, 25 };
		SetScale(scale);
		SetDirection(1); // By default alien travels right.
		m_SpriteNumber = 1;
		m_Index = NULL;
		m_Column = NULL;
		m_InFront = false;
		m_Type = Squid;
	}
	// Destructor:
	~Alien() override = default;

	// Getter:
	int GetPointValue() { return m_PointValue; }
	int GetSpriteNumber() { return m_SpriteNumber; }
	int GetIndex() { return m_Index; }
	int GetColumn() { return m_Column; }
	bool GetInFront() { return m_InFront; }
	alienType GetAlienType() { return m_Type; }

	// Setter:
	void SetPointValue(int pointValue)
	{
		m_PointValue = pointValue;
	}
	void SetSpriteNumber(int spriteNumber)
	{
		m_SpriteNumber = spriteNumber;
	}
	void SetIndex(int index)
	{
		m_Index = index;
	}
	void SetColumn(int column)
	{
		m_Column = column;
	}
	void SetInFront(bool inFront)
	{
		m_InFront = inFront;
	}
	void SetAlienType(alienType _type)
	{
		m_Type = _type;
	}

	// This function takes in a value and moves the alien by that
	// amount of pixels:
	void MoveAlien(int modifier)
	{
		Vector2 updatePos = this->GetPosition();
	
		
		// Check which direction the alien is travelling:
		// If alien is going 'left':
		if (this->GetDirection() < 0)
		{
			// Take away modifier from x-Pos:
			updatePos.x -= modifier;
			this->SetPosition(updatePos);
		}
		// If alien is going 'right':
		if (this->GetDirection() > 0)
		{
			// Sum modifier to x-Pos:
			updatePos.x += modifier;
			this->SetPosition(updatePos);
		}

	}

	
};