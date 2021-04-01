#pragma once
#include <SDL.h>
#include "GameObject.h"
#include "Game.h"

class Ship : public GameObject
{
private:
	int m_NumberOfLives; // How many lives the player has, once 0, the game ends.
	bool m_HasShot; // This will be an input, if true, fire a bullet, then set false again.
public:
	// Constructor:
	Ship();
	// Destructor:
	~Ship() override;

	// Getters:
	int GetNumberOfLives();
	bool GetHasShot();

	// Setters:
	void SetNumberOfLives(int _numberOfLives);
	void SetHasShot(bool _hasShot);
};