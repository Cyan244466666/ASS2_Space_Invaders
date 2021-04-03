#pragma once
#include "SDL/SDL/include/SDL.h"
#include "SDL/SDL2_image-2.0.5/include/SDL_image.h"
#include "SDL//SDL2_mixer-devel-2.0.4-VC/SDL2_mixer-2.0.4/include/SDL_mixer.h"
#include "SDL/SDL2_ttf-devel-2.0.15-VC/SDL2_ttf-2.0.15/include/SDL_ttf.h"
#include <vector>
#include <string>
#include "GameObject.h"
#include "Ship.h"
#include "Barrier.h"
#include "Barricade.h"
#include "Alien.h"
#include "Bullet.h"

class Game
{
private:
	SDL_Window* m_Window; // This is the game window.
	SDL_Renderer* m_Renderer; // This will render the game to the window.
	bool m_IsRunning; // This bool will continue to run the game, until it becomes false.
	bool m_InMenu; // This bool will continue to run the menu, until it becomes false.
	int hasShotTimer; // This int tells all functions if the player has shot this frame.
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
	// This vector hold all barricades spawned in-game:
	std::vector <Barricade*> barricadeVector;
	// This keeps track if the round is over or not:
	bool roundOver = false;

	// These are time counters used to time the execution of some code (e.g. alien movement and shooting bullets):
	float movementTimer = 0.0f;
	float shootingDelay = 0.0f;
	float alienShootingDelay = 0.0f;
	bool activateRespawnTimer = false;
	float respawnTimer = 0.0f;
	float destroyedBulletTimer = 0.0f;
	float UFOTimer = 0.0f;
	int deathAnimationCounter = 0;
	float modeDelay = 0.0f;
	float roundOverDelay = 0.0f;
	float barrierHitDelay = 0.0f;
	double player1Delay = 0.0;
	bool modePress = false;
	int playerScore = 0; // Keeps track of the player's score:
	int gameMode = 0; // This int changes depending on the game mode selected.
	// 0 = Default Mode
	// 1 = Arcade Mode
	// 2 = Insight Mode

	// Background Music:
	Mix_Music* backgroundMusic;
	Mix_Music* backgroundMusic2;
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
	Mix_Chunk* gameover;


	// This bool is used to tell the program that the alien has already moved down a row and doesn't
	// need to do it again:
	bool touchWall;
	// Dictates alien movement speed:
	float movementSpeed = 0.0f;
	// Dictates firing speed of aliens:
	float firingSpeed = 3.0f;

	bool bulletDestroyed = false;

	// Menu Sprites:
	SDL_Texture* titleImage;
	SDL_Texture* ship;

	// Game Loop Sprites:
	SDL_Texture* squidImage;
	SDL_Texture* crabImage;
	SDL_Texture* octopusImage;
	SDL_Texture* UFOImage;
	SDL_Texture* AlienDeathImage;
	SDL_Texture* AlienBulletImage;
	float alienBulletCounter; // This number is used to determine what frame of a bullet to display.
	SDL_Texture* JoystickImage;
	SDL_Texture* ButtomImage;
	SDL_Texture* BackImage;
	// Variable to hold player bullet sprite:
	SDL_Texture* playerBulletImage;
	SDL_Texture* arcade_mode;
	SDL_Texture* default_mode;
	SDL_Texture* insight_mode;
	SDL_Texture* lonely_driver_mode;
	SDL_Texture* round_over;
	SDL_Texture* player1;
	SDL_Texture* player_lives;
	SDL_Texture* barricadeImage;
	
	// Score Handling:
	bool m_gameWon = false; // Checks if player won the round.
	int m_lastScore = 0; // Keeps track of player's last round score.
	int m_CurrentWave = 0; //  Keeps track of player's current wave.
	//float m_WavePlayTime = 0; // Keeps track of how long the player has played in this round.
	int m_BulletsFired = 0; // Keeps track of how many bullets the player has fired in this round.
	int m_AlienBulletsFired = 0; // Keeps track of how many bullets the aliens have fired in this round.
	int barricade1, barricade2, barricade3, barricade4 = 1; // Keeps track of health for each barricade.
	// Barriers:
	Barrier barrierTop;
	Barrier barrierBottom;
	Barrier barrierRight;
	Barrier barrierLeft;
	
	// Font:
	TTF_Font* font;
	// Colour of Text:
	SDL_Color* white;
	// TTF:
	
	SDL_Surface* score;
	SDL_Texture* scoreImage;
	SDL_Texture* scoreNumb;
	SDL_Texture* lastRoundScore;
	SDL_Texture* waveNumb;
	// Insight/Debug Mode Stats
	SDL_Surface* debugSurface;

	SDL_Texture* playerXPosTexture;
	SDL_Texture* alienYPosTexture;
	SDL_Texture* totalAliensAliveTexture;
	SDL_Texture* squidsAliveTexture;
	SDL_Texture* crabsAliveTexture;
	SDL_Texture* octopusesAliveTexture;
	SDL_Texture* UFOsAliveTexture;
	SDL_Texture* alienMovementSpeedTexture;
	SDL_Texture* alienFireSpeedTexture;
	SDL_Texture* playerShotTexture;
	SDL_Texture* CurrentWavePlayeTimeTexture;
	SDL_Texture* bulletsFiredTexture;
	SDL_Texture* alienBulletsFiredTexture;
	SDL_Texture* barricadeHealthTexture1;
	SDL_Texture* barricadeHealthTexture2;
	SDL_Texture* barricadeHealthTexture3;
	SDL_Texture* barricadeHealthTexture4;
	
public:
	// Constructors:
	Game();
	// Destructor
	~Game();

	
	bool MenuInitialise();

	void RunMenu(); // This function runs the loop for the menu, once exited, the game starts.
	void MenuProcessInput(); // Processes all user input.
	void MenuUpdateGame(); // Makes changed to the game via an update.
	void MenuGenerateOutput(); // Present the changes made in the update to the screen.

	// This function initialises all assets used when a round starts.
	void Initialise();

	void RunGameLoop(); // This will run the game each from, calling all three helper Functions:
	// Helper Functions:
	void ProcessInput(); // Processes all user input.
	void UpdateGame(); // Makes changed to the game via an update.
	void GenerateOutput(); // Present the changes made in the update to the screen.

	//Getters:
	bool GetInMenu() { return m_InMenu; }
	void Shutdown(); // Shuts the game down once the player quits.
};