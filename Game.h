#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
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
	// This vector holds all UFO aliens spawned in-game:
	std::vector <Alien*> UFOVector;
	// This vector holds all bullets spawned in-game:
	std::vector <Bullet*> bulletVector;
	// This vector hold all alien bullets spawned in-game:
	std::vector <Bullet*> alienBulletVector;
	// These are time counters used to time the execution of some code (e.g. alien movement and shooting bullets):
	float movementTimer = 0.0f;
	float shootingDelay = 0.0f;
	float alienShootingDelay = 0.0f;
	bool activateRespawnTimer = false;
	float respawnTimer = 0.0f;
	float UFOTimer = 0.0f;

	int musicPlaying1 = 0;
	int musicPlaying2 = 0;

	// Background Music:
	Mix_Music* backgroundMusic;
	

	// SFX:
	Mix_Chunk* player_shoot;
	Mix_Chunk* barrier_destroyed;
	Mix_Chunk* barrier_hit;
	Mix_Chunk* bullet_collision;
	Mix_Chunk* crab_death;
	Mix_Chunk* menu_press;
	Mix_Chunk* octopus_death;
	Mix_Chunk* player_death;
	Mix_Chunk* squid_death;
	Mix_Chunk* ufo_death;
	Mix_Chunk* ufo_spawn;
	Mix_Chunk* wave_complete;
	Mix_Chunk* alien_move_speed1;
	Mix_Chunk* alien_move_speed2;
	Mix_Chunk* alien_move_speed3;
	Mix_Chunk* alien_move_speed4;


	// This bool is used to tell the program that the alien has already moved down a row and doesn't
	// need to do it again:
	bool touchWall;
	// Dictates alien movement speed:
	float movementSpeed = 0.0f;
	// Dictates firing speed of aliens:
	float firingSpeed = 3.0f;
	// Variables to hold the alien sprited:
	SDL_Texture* squidImage;
	SDL_Texture* crabImage;
	SDL_Texture* octopusImage;
	SDL_Texture* UFOImage;
	SDL_Texture* AlienDeathImage;
	int deathAnimationCounter = 0;
	// Variable to hold player bullet sprite:
	SDL_Texture* playerBulletImage;




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