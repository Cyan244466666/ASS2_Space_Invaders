#include <SDL.h>
#include "GameObject.h"
#include "Game.h"
#include "Ship.h"

	// Constructor:
	Ship::Ship() : GameObject()
	{
		Vector2 Position{ 512, 650 };
		SetPosition(Position);
		Vector2 Velocity{ 10, 10 };
		SetVelocity(Velocity);
		Vector2 Scale{ 25,15 };
		SetScale( Scale );
		SetDirection(NULL);
		SetStatus(Alive);
		m_NumberOfLives = 8;
		m_HasShot = false;
	}

	// Destructor:
	Ship::~Ship() = default;
	
	// Getters:
	int Ship::GetNumberOfLives() { return m_NumberOfLives; }
	bool Ship::GetHasShot() { return m_HasShot; }

	// Setters:
	void Ship::SetNumberOfLives(int _numberOfLives) { m_NumberOfLives = _numberOfLives; }
	void Ship::SetHasShot(bool _hasShot) { m_HasShot = _hasShot; }

	


