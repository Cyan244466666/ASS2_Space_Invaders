#pragma once
#include <SDL.h>
#include "GameObject.h"
#include "Game.h"
// This class contains the barricades that the player can
// use for cover

class Barricade : public GameObject
{
private:
	int m_Health;
public:

	// Constructor:
	Barricade() 
	{
		m_Health = 80;
	}
	// Destructor:
	~Barricade() override = default;

	// Getter
	int GetHealth() { return m_Health; }
	// Setter
	void SetHealth(int health)
	{
		m_Health = health;
	}
	//
};