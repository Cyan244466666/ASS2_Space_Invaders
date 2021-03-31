#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "GameObject.h"
#include "Ship.h"
#include "Barrier.h"
#include "Alien.h"
#include "Bullet.h"

class Game
{
private:
	SDL_Window* m_Window; // This is the game window.
	SDL_Renderer* m_Renderer; // This will render the game to the window.
	bool m_IsRunning; // This bool will continue to run the game, until it becomes false.
	Uint32 m_ticksCount; // This will be used when calculating deltaTime.
	Ship player; // This is the player's ship.
	Vector2 updatePlayerPos{ 512, 650 };
	// This vector will hold all Aliens that'll be spawned into the game:
	std::vector <Alien*> alienVector;
	// This vector holds all bullets spawned in-game:
	std::vector <Bullet*> bulletVector;
	// This vector hold all alien bullets spawned in-game:
	std::vector <Bullet*> alienBulletVector;
	// These are time counters used to time the execution of some code (e.g. alien movement and shooting bullets):
	float movementTimer = 0;
	float shootingDelay = 0;
	float alienShootingDelay = 0;
	// This bool is used to tell the program that the alien has already moved down a row and doesn't
	// need to do it again:
	bool touchWall;
	// Variables to hold the alien sprited:
	SDL_Texture* squidImage;
	SDL_Texture* crabImage;
	SDL_Texture* octopusImage;
	// Variable to hold player bullet sprite:
	SDL_Texture* playerBulletImage;

	// Columns of aliens:
	std::vector<Alien*> column0;
	std::vector<Alien*> column1;
	std::vector<Alien*> column2;
	std::vector<Alien*> column3;
	std::vector<Alien*> column4;
	std::vector<Alien*> column5;
	std::vector<Alien*> column6;
	std::vector<Alien*> column7;
	std::vector<Alien*> column8;
	std::vector<Alien*> column9;
	std::vector<Alien*> column10;

	std::vector < std::vector<Alien*>> allColumns;


	// Barriers:
	Barrier barrierTop;
	Barrier barrierBottom;
	Barrier barrierRight;
	Barrier barrierLeft;
	

	
public:
	// Constructors:
	Game();
	// Destructor
	~Game();

	// This function will initialise SDL, the Window, and the Renderer,
	// when the game is launched. It will return 'true' if successful,
	// and false otherwise:
	bool Initialise();

	void RunGameLoop(); // This will run the game each from, calling all three helper Functions:
	// Helper Functions:
	void ProcessInput(); // Processes all user input.
	void UpdateGame(); // Makes changed to the game via an update.
	void GenerateOutput(); // Present the changes made in the update to the screen.


	void Shutdown(); // Shuts the game down once the player quits.
};