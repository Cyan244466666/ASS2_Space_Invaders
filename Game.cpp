#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <time.h>
#include "GameObject.h"
#include "Game.h"
#include "Bullet.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>



bool changeSprite = false;


// Constructors:
Game::Game()
{
	m_Window = NULL;
	m_Renderer = NULL;
	m_IsRunning = true;
	m_InMenu = true;
	m_ticksCount = NULL;
	barrierTop = { 0, 0, 1024, 150 };
	barrierBottom = { 0, 718, 1024, 5 };
	barrierRight = { 904, 0, 220, 768 };
	barrierLeft = {0, 0, 135, 768};
	
	hasShotTimer = 0;

	// Sprites:
	squidImage = nullptr;
	crabImage = nullptr;
	octopusImage = nullptr;
	UFOImage = nullptr;
	AlienDeathImage = nullptr;
	touchWall = false;
	titleImage = nullptr;
	ship = nullptr;

	playerBulletImage = nullptr;

	// Music
	backgroundMusic = nullptr;

	// SFX:
	barrier_destroyed = nullptr;
	barrier_hit = nullptr;
	bullet_collision = nullptr;
	crab_death = nullptr;
	menu_press = nullptr;
	octopus_death = nullptr;
	player_shoot = nullptr;
	player_death = nullptr;
	squid_death = nullptr;
	ufo_death = nullptr;
	ufo_spawn = nullptr;
	wave_complete = nullptr;
	alien_move_speed1 = nullptr;
	alien_move_speed2 = nullptr;
	alien_move_speed3 = nullptr;
	alien_move_speed4 = nullptr;
	gameover = nullptr;
	JoystickImage = nullptr;
	ButtomImage = nullptr;
	AlienBulletImage = nullptr;
	alienBulletCounter = 0.0f;
	BackImage = nullptr;
	arcade_mode = nullptr;
	default_mode = nullptr;
	insight_mode = nullptr;
	round_over = nullptr;
	player1 = nullptr;
	player_lives = nullptr;
	barricadeImage = nullptr;
	
	//TTF:
	font = nullptr;
	white = nullptr;
	score = nullptr;
	scoreImage = nullptr;
	scoreNumb = nullptr;
	lastRoundScore = nullptr;
	waveNumb = nullptr;

}
// Destructor
Game::~Game() {}

// This function will initialise SDL, the Window, and the Renderer,
// when the game is launched. It will return 'true' if successful,
// and false otherwise:
bool Game::MenuInitialise()
{
	/// Step 1: Initialise SDL - if successful, returns 0, otherwise print error:
	int sdlResult = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialise SDL: %s", SDL_GetError());
		return 1;
	}
	//------------------------------------------
	/// Step 2: Initialise Window - returns false is unsuccessful.
	m_Window = SDL_CreateWindow(
		"Space Invaders", // Window title
		100, // x-coordinate
		100, // y-coordinate
		1024, // width
		768, // height
		0 // tags
	);
	if (!m_Window)
	{
		SDL_Log("Unable to initialise Window: %s", SDL_GetError());
		return 1;
	}
	//------------------------------------------
	/// Step 3: Initialise Renderer - returns false is unsuccessful.
	m_Renderer = SDL_CreateRenderer(
		m_Window, // The window attached to the renderer
		-1, // Index
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC // Renderer uses hardware acceleration && present is synchronised with refresh rate.
	);
	if (!m_Renderer)
	{
		SDL_Log("Unable to initialise Renderer: %s", SDL_GetError());
		return 1;
	}

	//------------------------------------------
	/// Step 4: Initialise SDL_image
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		SDL_Log("Unable to initialise IMG: %s", SDL_GetError());
		return 1;
	}
	return 0;

	/// Step 5: Initialise SDL_TTF
	if (TTF_Init() < 0)
	{
		SDL_Log("Unable to initialise SDL_TTF: %s", SDL_GetError());
		return 1;
	}
	return 0;
}
void Game::RunGameLoop() // This will run the game each from, calling all three helper Functions:
{
	while (m_IsRunning)
	{		
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}
void Game::RunMenu()
{
	while (m_InMenu)
	{
		MenuProcessInput();
		MenuUpdateGame();
		MenuGenerateOutput();

		// Reset Game Assets
		player.SetNumberOfLives(3);
		alienBulletVector.clear();
		alienVector.clear();
		bulletVector.clear();

		UFOVector.clear();
		m_IsRunning = true;
		player.SetStatus(Alive);
		Mix_VolumeMusic(20);
	}
}

// Menu Helper Function
void Game::MenuProcessInput()
{
	SDL_Event event; // An event is any action of input (usually a key press).
	while (SDL_PollEvent(&event)) // While events are still being processed.
	{

		switch (event.type)
		{
		case SDL_QUIT:
			Game::Shutdown();
			break;
		}

	}
	// Get the state of the keyboard:
	const Uint8* state = SDL_GetKeyboardState(NULL);
	/*const Uint32* mousestate = SDL_GetMouseState(nullptr, nullptr);*/
	// If the player hits "escape", end the game loop.
	if (state[SDL_SCANCODE_ESCAPE])
	{
		Game::Shutdown();
	}
	player.SetDirection(0);
	int newDirection = 0;
	// Movement for the Player's ship. If the player pressed
	// the 'D' key, move the player right:
	if (state[SDL_SCANCODE_SPACE])
	{
		m_InMenu = false;
	}
	if (state[SDL_SCANCODE_E])
	{
		modePress = true;
	}
}
void Game::MenuUpdateGame()
{
	Mix_VolumeMusic(1);
	modeDelay += (1.0f * 0.6f);
	if (gameMode > 2)
	{
		gameMode = 0;
	}

	if (modeDelay > 20.0f && modePress)
	{
		gameMode++;
		modeDelay = 0;
	}
	modePress = false;

	// Handle HighScore and Current Score
	if (!m_gameWon)
	{
		if (m_lastScore < player.GetScore())
		{
			m_lastScore = player.GetScore();
		}
		player.SetScore(0);
		// If player loses, they go back to wave 0.
		m_CurrentWave = 0;
	}
	else
	{
		// If player wins, they go move up a wave.
		if (m_lastScore < player.GetScore())
		{
			m_lastScore = player.GetScore();
		}
	}
	// If players current high score is less than last round's score, set high score:
	
}
void Game::MenuGenerateOutput()
{
	/// Set Background
	// Clear the back buffer by specifying a colour (black):
	SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 255);
	// Clears the back buffer to black:
	SDL_RenderClear(m_Renderer);

	/// Display Ship Art
	Vector2 shipPos{ 100, 90 };
	SDL_Rect ship
	{
		(int)shipPos.x,
		(int)shipPos.y,
		340,
		250
	};

	// Load the Ship's image:
	SDL_Texture* shipImage = IMG_LoadTexture(m_Renderer, "images/shipArt_2.png");
	SDL_RenderCopy(m_Renderer, shipImage, nullptr, &ship);
	// Release Memory:
	SDL_DestroyTexture(shipImage);

	/// Show "Play Player <1>:
	player1Delay += (1 * 0.9);
	if (player1Delay > 49)
	{
		Vector2 player1Pos{ 370, 350 };
		SDL_Rect player1Rect
		{
			(int)player1Pos.x,
			(int)player1Pos.y,
			260,
			110
		};
		// Load the Ship's image:
		player1 = IMG_LoadTexture(m_Renderer, "images/player1.png");
		SDL_RenderCopy(m_Renderer, player1, nullptr, &player1Rect);
		// Release Memory:
		SDL_DestroyTexture(player1);
	}

	if(player1Delay > 100)
	{
		player1Delay = 0;
	}

	/// Display Game Mode

	Vector2 modePos{ 418, 650 };
	SDL_Rect mode
	{
		(int)modePos.x,
		(int)modePos.y,
		150,
		100
	};
	if (gameMode == 0)
	{
		default_mode = IMG_LoadTexture(m_Renderer, "images/default_mode.png");
		SDL_RenderCopy(m_Renderer, default_mode, nullptr, &mode);
		// Release Memory:
		SDL_DestroyTexture(default_mode);
	}
	else if (gameMode == 1)
	{

		arcade_mode = IMG_LoadTexture(m_Renderer, "images/arcade_mode.png");
		SDL_RenderCopy(m_Renderer, default_mode, nullptr, &mode);
		// Release Memory:
		SDL_DestroyTexture(arcade_mode);
	}
	else if (gameMode == 2)
	{
		insight_mode = IMG_LoadTexture(m_Renderer, "images/insight_mode.png");
		SDL_RenderCopy(m_Renderer, insight_mode, nullptr, &mode);
		// Release Memory:
		SDL_DestroyTexture(insight_mode);
	}
	// Load the Game Mode's image:
	

	/// Display Title
	Vector2 titlePos{ 350, 100 };
	SDL_Rect title
	{
		(int)titlePos.x,
		(int)titlePos.y,
		300,
		150
	};
	// Load the Ship's image:
	SDL_Texture* titleImage = IMG_LoadTexture(m_Renderer, "images/title.png");

	SDL_RenderCopy(m_Renderer, titleImage, nullptr, &title);
	// Release Memory:
	SDL_DestroyTexture(titleImage);

	

	/// Present Changes:

	SDL_RenderPresent(m_Renderer);
}

void Game::Initialise()
{
	// If player has quit, don't run this code:
	if (m_IsRunning)
	{
		// If all initialisation is successful, return true:
		//------------------------------------------
		/// Step 1: Initialise SDL mixer:
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
		menu_press = Mix_LoadWAV("sound_effects/menu_press.wav");

		backgroundMusic = Mix_LoadMUS("music/invader.mp3");
		player_shoot = Mix_LoadWAV("sound_effects/player_shoot.wav");
		barrier_destroyed = Mix_LoadWAV("sound_effects/barrier_destroyed.wav");
		barrier_hit = Mix_LoadWAV("sound_effects/barrier_hit.wav");
		bullet_collision = Mix_LoadWAV("sound_effects/bullet_collision.wav");
		crab_death = Mix_LoadWAV("sound_effects/crab_death.wav");
		octopus_death = Mix_LoadWAV("sound_effects/octopus_death.wav");
		player_death = Mix_LoadWAV("sound_effects/player_death.wav");
		squid_death = Mix_LoadWAV("sound_effects/squid_death.wav");
		ufo_death = Mix_LoadWAV("sound_effects/ufo_death.wav");
		ufo_spawn = Mix_LoadWAV("sound_effects/ufo_spawn.wav");
		wave_complete = Mix_LoadWAV("sound_effects/wave_complete.wav");
		alien_move_speed1 = Mix_LoadWAV("sound_effects/alien_move1.wav");
		alien_move_speed2 = Mix_LoadWAV("sound_effects/alien_move2.wav");
		alien_move_speed3 = Mix_LoadWAV("sound_effects/alien_move3.wav");
		alien_move_speed4 = Mix_LoadWAV("sound_effects/alien_move4.wav");
		gameover = Mix_LoadWAV("sound_effects/gameover.wav");
		

		if(TTF_Init()==-1)
		{
			SDL_Log("Unable to initialise SDL_TTF: %s", SDL_GetError());
		}
		/// Step 2: Play background music in a loop:

		// allocate 16 mixing channels
		Mix_AllocateChannels(16);
		if (m_IsRunning)
		{
			Mix_PlayMusic(backgroundMusic, -1);
		}
		// Set Volume for channels:
		Mix_VolumeMusic(10);
		Mix_Volume(-1, 15);
		Mix_Volume(2, 5);
		Mix_Volume(4, 30);
		Mix_Volume(5, 6);
		Mix_Volume(6, 10);
		Mix_Volume(6, 20);
		Mix_VolumeChunk(wave_complete, 50);

		// Free Music Memory:
		/*Mix_FreeMusic(backgroundMusic);*/

		//------------------------------------------
		/// Step 3: Initialise the Aliens' start position:
		// First, we set the Position of the space
		// in which the aliens will spawn:
		float spaceX = 230;
		float spaceY = 70;

		// Second, work out where each alien will be
		// placed within these dimensions:
		float xPos, yPos;
		for (int i = 0; i < 55; i++)
		{
			auto* alien = new Alien();
			Vector2 alienScale = alien->GetScale();
			Vector2 updatePos = alien->GetPosition();

			// Here, find the xPosition for the alien.
			xPos = ((i % 11) * (alienScale.x + 30)) + spaceX;
			yPos = ((i / 11) * (alienScale.y + 20)) + spaceY;


			// Set index and column number:
			alien->SetIndex(i);
			alien->SetColumn(i % 11);

			// Set the alien's point value:
			if (alien->GetIndex() >= 0 && alien->GetIndex() < 11)
			{
				alien->SetPointValue(30);
			}
			if (alien->GetIndex() >= 11 && alien->GetIndex() < 33)
			{
				alien->SetPointValue(20);
			}
			if (alien->GetIndex() >= 33 && alien->GetIndex() < 55)
			{
				alien->SetPointValue(10);
			}

			// Finally, set the Alien's position vector to
			// the created coordinates:
			Vector2 spawnPosition{ xPos, yPos };
			alien->SetPosition(spawnPosition);
			alienVector.push_back(alien);
			Game::GenerateOutput();
		}

		// Set alien type
		for (auto* alien : alienVector)
		{
			if (alien->GetIndex() >= 0 && alien->GetIndex() < 11)
			{
				alien->SetAlienType(Squid);
			}
			else if (alien->GetIndex() > 10 && alien->GetIndex() < 33)
			{
				alien->SetAlienType(Crab);
			}
			else
			{
				alien->SetAlienType(Octopus);
			}
		}
	}


	/// Step 4: Initialise the Barricades start position:
	for (int i = 0; i < 4; i++)
	{
		auto* barricade = new Barricade();
		Vector2 barricadePos{ 110.0f + (i * 230.0f), 520.0f };
		Vector2 barricadeScale{ 90, 70 };
		barricade->SetPosition(barricadePos);
		barricade->SetScale(barricadeScale);
		barricadeVector.push_back(barricade);
	}

	/// Update wave numb
	if (m_gameWon)
	{
		m_gameWon = false;
		m_CurrentWave++;
	}
	
}
// Helper Functions:
void Game::ProcessInput()
{
	
	SDL_Event event; // An event is any action of input (usually a key press).
	while(SDL_PollEvent(&event)) // While events are still being processed.
	{

		switch (event.type)
		{
		case SDL_QUIT:
			m_IsRunning = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			player.SetHasShot(true);
			break;

		}
	
	}
	// Get the state of the keyboard:
	const Uint8* state = SDL_GetKeyboardState(NULL);
	/*const Uint32* mousestate = SDL_GetMouseState(nullptr, nullptr);*/
	// If the player hits "escape", end the game loop.
	if (state[SDL_SCANCODE_ESCAPE])
	{
		m_IsRunning = false;
	}
	player.SetDirection(0);
	int newDirection = 0;
	// Movement for the Player's ship. If the player pressed
	// the 'D' key, move the player right:
	if (state[SDL_SCANCODE_D])
	{
		newDirection += 1;
		player.SetDirection(newDirection);
	}
	//If the player pressed the 'A' key, move the player left:
	if (state[SDL_SCANCODE_A])
	{
		newDirection -= 1;
		player.SetDirection(newDirection);
	}
	// If the player pressed 'Spacebar', set 'HasShot' to true:
	if (state[SDL_SCANCODE_SPACE])
	{
		player.SetHasShot(true);
	}
	// If the player pressed 'Mouse Button', set 'HasShot' to true:
	
}
void Game::UpdateGame()
{
	srand(time(NULL));

	//------------------------------------------
	/// Step 1: Setup deltaTime:
	// Frame Limiting, this line of code waits until 16m/s has passed until
	// calculating the new deltaTime:
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), m_ticksCount + 16)) {}
	// Calculates deltaTime, calculating how many seconds have passed since
	// the last frame was processed:
	float deltaTime = (SDL_GetTicks() - m_ticksCount) / 1000.0f;
	// Clamp maximum deltaTime (prevent lag):
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	// Assign current ticks passed:
	m_ticksCount = SDL_GetTicks();
	//------------------------------------------
	/// Step 2: Update Player's Position based on user input:
	// if the player's direction is NOT stationary: 
	if (player.GetDirection() != 0)
	{
		// Calculate Movement:
		updatePlayerPos.x += player.GetDirection() * 300.0f * deltaTime;
		player.SetPosition(updatePlayerPos);
	}
	//------------------------------------------
	// Step 3: Set bounds for the movement of the Player:
	// Left Boundry:
	Vector2 playerScale = player.GetScale();
	if (updatePlayerPos.x < barrierLeft.GetScaleX())
	{
		updatePlayerPos.x = barrierLeft.GetScaleX() + 3.0f;
		player.SetPosition(updatePlayerPos);
	}
	// Right Boundry:
	if (updatePlayerPos.x > barrierRight.GetPosX() - 25.0f)
	{
		updatePlayerPos.x = barrierRight.GetPosX() - 25.0f;
		player.SetPosition(updatePlayerPos);
	}

	//------------------------------------------
	/// Step 3: Calculate Alien Movement:
	// This code determines the alien movement speed based on how many aliens are alive:
	
	int aliensAlive = 0;
	// Check how many aliens are alive:
	for (auto* alien : alienVector)
	{
		if (alien->GetStatus() == Alive)
		{
			aliensAlive += 1;
		}
		if (alien->GetPosition().y > 630)
		{
			Mix_PlayChannel(7, gameover, 0);
			m_IsRunning = false;
			m_InMenu = true;
			m_gameWon = false;
		}
	}
	// End game if there are no aliens:
	if(aliensAlive == 0)
	{
		m_IsRunning = false;
		Mix_PlayChannel(6, wave_complete, 0);
		m_InMenu = true;
		m_gameWon = true;
	}
	// Calculate Movement Speed:
	if(aliensAlive == 1) { movementSpeed = 17.0f;}
	if(aliensAlive == 2) { movementSpeed = 10.8f;}
	if (aliensAlive < 5 && aliensAlive > 2) { movementSpeed = 9.0f; }
	if (aliensAlive > 4 && aliensAlive < 9) { movementSpeed = 8.0f; }
	if (aliensAlive > 8 && aliensAlive < 16) { movementSpeed = 6.0f; }
	if (aliensAlive > 15 && aliensAlive < 21) { movementSpeed = 4.5f; }
	if (aliensAlive > 20 && aliensAlive < 27) { movementSpeed = 3.8f; }
	if (aliensAlive > 26) { movementSpeed = 3.0f; }

	

	// Set up a time counter that increments, when it reached a certain
	// number, the aliens will move:
	movementTimer += (movementSpeed * deltaTime);
	bool moveDown = false;

	// Test if any alien is touching a barricade:
	if (movementTimer > 1.4f)
	{
		for (auto& alien : alienVector)
		{
			if (alien->GetStatus() == Dead)
			{
				continue;
			}
			Vector2 currentPos = alien->GetPosition();
			if (currentPos.x >= 925)
			{
				moveDown = true;
			}
			if (currentPos.x <= 125)
			{
				moveDown = true;
			}
		}
	}

	// If an alien is touching a barricade:
	if (moveDown && !touchWall && movementTimer > 1.5f)
	{
		for (auto& alien : alienVector)
		{
			Vector2 newPosition = alien->GetPosition();
			Vector2 alienScale = alien->GetScale();
			// Move all aliens down a row:
			newPosition.y += alienScale.y;
			alien->SetPosition(newPosition);


			// Change direction:
			int newDirection = alien->GetDirection();
			newDirection *= -1;
			alien->SetDirection(newDirection);
			moveDown = false;
			touchWall = true;
		}
		movementTimer = 0;
	}
	// Move the aliens according to their direction:
	if (movementTimer > 1.5f)
	{
		for (auto& alien : alienVector)
		{
			if (movementSpeed < 5.0f && movementSpeed > 0.0f)
			{
				
			}
			if (movementSpeed < 7.0f && movementSpeed > 5.0f)
			{
				Mix_PlayChannel(4, alien_move_speed2, 0);
			}
			if (movementSpeed < 9.0f && movementSpeed > 7.0f)
			{
				Mix_PlayChannel(4, alien_move_speed3, 0);
			}
			if (movementSpeed < 20.0f && movementSpeed > 9.0f)
			{
				Mix_PlayChannel(4, alien_move_speed4, 0);
			}
			
			alien->MoveAlien(25);
			if (alien->GetSpriteNumber() == 1)
			{
				alien->SetSpriteNumber(2);
			}
			else if (alien->GetSpriteNumber() == 2)
			{
				alien->SetSpriteNumber(1);
			}
		}
		touchWall = false;
		//Reset Timer:
		movementTimer = 0;
		changeSprite = true;
	}

	//------------------------------------------
	/// Step 4: Check if player has fired a bullet, if true,
	// Check if player is alive:
	
	if (player.GetStatus() != Dead)
	{
		// spawn a bullet into the 'bulletVector':
		Vector2 shipCurrentPos = player.GetPosition();
		// There needs to be a 'delay' so that the player can't spam bullets:
		shootingDelay += (2.0f * (2 * deltaTime));

		// If the player has shot, and the delay timer is up:
		if (player.GetHasShot() == true && shootingDelay > 2.8f)
		{
			// Spawn bullet on player position:
			auto* bullet = new Bullet();
			Vector2 spawnPosition{ shipCurrentPos.x + 5, shipCurrentPos.y };
			bullet->SetPosition(spawnPosition);
			bulletVector.push_back(bullet);
			hasShotTimer = 10;
			shootingDelay = 0.0f;

			// Play SFX:
			Mix_PlayChannel(0, player_shoot, 0);
		}
	}
	player.SetHasShot(false);

	//------------------------------------------

	/// Step 5: Move Player's bullets:
	for (auto& bullet : bulletVector)
	{
		bullet->MoveBullet();
	}

	//------------------------------------------

	/// Step 6: Bullet Collision for all aliens:
	
	for (int i = 0; i < (int)alienVector.size(); i++)
	{
		// Check alien against all bullets.
		bool beenShot = false;
		for (auto& bullet : bulletVector)
		{
			if (alienVector[i]->GetStatus() == Dead)
			{
				continue;
			}
			beenShot = alienVector[i]->BulletCollision(bullet->GetPosition(), bullet->GetScale());
			// Break the check if the alien has been shot:
			if (beenShot)
			{
				
				// Delete the Alien:
				// Play SFX:
				if (alienVector[i]->GetAlienType() == Squid)
				{
					int updatePlayerScore = player.GetScore();
					updatePlayerScore += alienVector[i]->GetPointValue();
					player.SetScore(updatePlayerScore);
					Mix_PlayChannel(2, squid_death, 0);
				}
				if (alienVector[i]->GetAlienType() == Crab)
				{
					int updatePlayerScore = player.GetScore();
					updatePlayerScore += alienVector[i]->GetPointValue();
					player.SetScore(updatePlayerScore);
					Mix_PlayChannel(2, crab_death, 0);
				}
				if (alienVector[i]->GetAlienType() == Octopus)
				{
					int updatePlayerScore = player.GetScore();
					updatePlayerScore += alienVector[i]->GetPointValue();
					player.SetScore(updatePlayerScore);
					Mix_PlayChannel(2, octopus_death, 0);
				}
				
				alienVector[i]->SetStatus(JustKilled);
				bullet->SetHasHit(true);
			}
			for (int i = 0; i < (int)bulletVector.size(); i++)
			{
				// Delete Bullet (if it hit):
				if (bulletVector[i]->GetHasHit() || bulletVector[i]->GetPosition().y < 0)
				{
					// Delete the Bullet:
					std::vector<Bullet*>::iterator it2;
					it2 = bulletVector.begin() + i;
					bulletVector.erase(it2);
				}
			}
		}
	}
	for (int i = 0; i < (int)UFOVector.size(); i++)
	{
		// Check alien against all bullets.
		bool beenShot = false;
		for (auto& bullet : bulletVector)
		{
			beenShot = UFOVector[i]->BulletCollision(bullet->GetPosition(), bullet->GetScale());
			// Break the check if the alien has been shot:
			if (beenShot)
			{

				// Play SFX:
				Mix_PlayChannel(3, ufo_death, 0);
				UFOVector[i]->SetStatus(Dead);
			}
			for (int i = 0; i < (int)bulletVector.size(); i++)
			{
				// Delete Bullet (if it hit):
				if (bulletVector[i]->GetHasHit())
				{
					// Delete the Bullet:
					std::vector<Bullet*>::iterator it2;
					it2 = bulletVector.begin() + i;
					bulletVector.erase(it2);
				}
			}
		}
	}

	//------------------------------------------

	/// Step 7: Check which aliens are in the front row:
	// Set bool values to ensure only one alien is marked for each column.
	alienShootingDelay += (firingSpeed * deltaTime);
	if (alienShootingDelay > 2.0f)
	{
		Alien* frontRow[11]{};
		for (int i = 0; i < (int)alienVector.size(); ++i)
		{
			Alien* curAlien = alienVector[i];
			if (curAlien->GetStatus() == Dead) { continue; }
			// Find the column the the alien belongs in. (the column)
			if (frontRow[curAlien->GetColumn()] != nullptr)
			{
				// if the alien exist in the front column
				if (i > frontRow[curAlien->GetColumn()]->GetIndex())
				{
					frontRow[curAlien->GetColumn()] = curAlien;
					continue;
				}
			}
			else
			{
				frontRow[curAlien->GetColumn()] = curAlien;
			}
		}

		// This code randomly assigns an alien from a column to shoot,
		// it checks if there is an alien in that column, if true, that
		// alien fires a bullet, otherwise it continues to look in another
		// column.
		bool hasFoundAlien = false;
		Alien* shooterAlien = nullptr;
		if (player.GetStatus() != Dead)
		{
			while (!hasFoundAlien)
			{
				int numb = (rand() % 11);
				shooterAlien = frontRow[numb];
				if (shooterAlien != nullptr)
				{
					hasFoundAlien = true;
				}
			}
			auto* bullet = new Bullet();
			// Set what type of alien the bullet has come from,
			// this will determine how to render the bullet.
			bullet->SetPosition(shooterAlien->GetPosition());
			alienBulletVector.push_back(bullet);

			alienShootingDelay = 0.0f;
		}
	}

	for (auto alienBullets : alienBulletVector)
	{
		alienBullets->MoveAlienBullet();
	}


	//------------------------------------------
	/// Step 8: Ship/Player Bullet Collision:
	// This code checks whether the player has been hit by an alien bullet:
	for (auto alienBullet : alienBulletVector)
	{
		bool playerHit = false;
		playerHit = player.BulletCollision(alienBullet->GetPosition(), alienBullet->GetScale());

		if (playerHit)
		{
			player.SetStatus(Dead);
			// Play SFX:
			Mix_PlayChannel(0, player_death, 0);
			alienBullet->SetStatus(Dead);
			// Remove a life:
			int lives = player.GetNumberOfLives();
			lives -= 1;
			player.SetNumberOfLives(lives);
			// Start Respawn Timer:
			activateRespawnTimer = true;
		}
		// Remove Dead alien bullet:
		for (int i = 0; i < (int)alienBulletVector.size(); i++)
		{
			// Delete Bullet (if it hit):
			if (alienBulletVector[i]->GetStatus() == Dead)
			{
				// Delete the Bullet:
				std::vector<Bullet*>::iterator it;
				it = alienBulletVector.begin() + i;
				alienBulletVector.erase(it);
			}
		}
	}

	for (auto* bullet : bulletVector)
	{
		for (int i = 0; i < (int)bulletVector.size(); i++)
		{
			// Delete Bullet (if it hit):
			if (bulletVector[i]->GetStatus() == Dead)
			{
				// Delete the Bullet:
				std::vector<Bullet*>::iterator it;
				it = bulletVector.begin() + i;
				bulletVector.erase(it);
			}
		}
	}

	//------------------------------------------
	/// Step 9: Respawn Player/Check if GameOver:
	// This code checks if the player has lost all 3 lives - if yes - the game ends.
	if (player.GetNumberOfLives() == 0)
	{
		Mix_PlayChannel(6, gameover, 0);
		m_IsRunning = false;
		m_InMenu = true;
		m_gameWon = false;
	}
	// This code checks if the player is dead, if yes they respawn.
	if (activateRespawnTimer)
	{
		respawnTimer += (2 * deltaTime);
		if (respawnTimer > 1.5f)
		{
			player.SetStatus(Alive);
			activateRespawnTimer = false;
			respawnTimer = 0.0f;
		}
	}

	//------------------------------------------
	/// Step 10: Randomly spawn UFO
	// This code sets up a timer and a random number to spawn the UFO at,
	// if the timer is exceeds the spawn number, the UFO spawns: 
	UFOTimer += (2.4f * deltaTime);
	static int UFOSpawnTime = (rand() % 60) + 30;
	if (UFOTimer > (float)UFOSpawnTime)
	{
		Alien* UFO = new Alien();
		int UFOSpawnSide = (rand() % 2) + 1;
		Mix_PlayChannel(3, ufo_spawn, 0);
		// Play spawn music:
		if (UFOSpawnSide == 1)
		{

			// Spawn UFO top-left
			Vector2 UFOSpawnPos{ 30, 50 };
			Vector2 UFOSpawnScale{ 50, 25 };
			UFO->SetPosition(UFOSpawnPos);
			UFO->SetScale(UFOSpawnScale);
			UFO->SetDirection(1);
			UFOTimer = 0.0f;
			UFOVector.push_back(UFO);
		}
		if (UFOSpawnSide == 2)
		{

			// Spawn UFO top-left
			Vector2 UFOSpawnPos{ 1000, 50 };
			Vector2 UFOSpawnScale{ 50, 25 };
			UFO->SetPosition(UFOSpawnPos);
			UFO->SetScale(UFOSpawnScale);
			UFO->SetDirection(-1);
			UFOTimer = 0.0f;
			UFOVector.push_back(UFO);

		}
		
	}
	
	// Move any UFO's:
	for (auto* UFO : UFOVector)
	{
		if (UFO->GetStatus() == Dead)
		{
			continue;
		}
		UFO->MoveAlien(2);

		// Check if UFO goes out of bounds:
		if (UFO->GetPosition().x > 1400)
		{
			UFO->SetStatus(Dead);
		}
		else if (UFO->GetPosition().x < -200)
		{
			UFO->SetStatus(Dead);
		}
	}
	// Delete UFO if it's dead:
	for (auto* UFO : UFOVector)
	{
	
		for (int i = 0; i < (int)UFOVector.size(); i++)
		{
			if (UFOVector[i]->GetStatus() == Dead)
			{
				// Give player points, randomly either 50, 100, or 150:
				int randNumb = (rand() % 3) + 1;
				if (randNumb == 1)
				{
					int updatePlayerScore = player.GetScore();
					updatePlayerScore += 50;
					player.SetScore(updatePlayerScore);
				}
				else if (randNumb == 2)
				{
					int updatePlayerScore = player.GetScore();
					updatePlayerScore += 100;
					player.SetScore(updatePlayerScore);
				}
				else
				{
					int updatePlayerScore = player.GetScore();
					updatePlayerScore += 150;
					player.SetScore(updatePlayerScore);
				}
				// Delete the Barricade:
				std::vector<Alien*>::iterator it;
				it = UFOVector.begin() + i;
				UFOVector.erase(it);
			}
		}
		
	}
	// If game loop ends, delete UFO's:
	// If barricade health becomes 0, then it's dead
	if (!m_IsRunning)
	{
		for (int i = 0; i < (int)UFOVector.size(); i++)
		{
			// Delete the Barricade:
			std::vector<Alien*>::iterator it;
			it = UFOVector.begin() + i;
			UFOVector.erase(it);
				
		}	
	}

	/// Collision Check for Barricades:
	barrierHitDelay += (1 * 0.6f);
	// Check collision against alien bullets:
	// Delete barricades if game loop ends:
	if (!m_IsRunning)
	{
		for (int i = 0; i < (int)barricadeVector.size(); i++)
		{
			// Delete the Barricade:
			std::vector<Barricade*>::iterator it;
			it = barricadeVector.begin() + i;
			barricadeVector.erase(it);

		}
	}

	for (int i = 0; i < (int)alienBulletVector.size(); i++)
	{
		for (auto* barricade : barricadeVector)
		{
			if (barricade->BulletCollision(alienBulletVector[i]->GetPosition(), alienBulletVector[i]->GetScale()))
			{
				Mix_PlayChannel(5, barrier_hit, 0);
				int changeHealth = barricade->GetHealth();
				changeHealth -= 7;
				barricade->SetHealth(changeHealth);
				alienBulletVector[i]->SetStatus(Dead);
				barrierHitDelay = 0;
				}
				else
				{
					continue;
				}
			}
		}

		// Check collision against player bullets:
		for (auto* bullet : bulletVector)
		{
			for (auto* barricade : barricadeVector)
			{
				if (barricade->BulletCollision(bullet->GetPosition(), bullet->GetScale()))
				{
					Mix_PlayChannel(5, barrier_hit, 0);
					int changeHealth = barricade->GetHealth();
					changeHealth -= 7;
					barricade->SetHealth(changeHealth);
					bullet->SetStatus(Dead);
					barrierHitDelay = 0;
				}
				else
				{
					continue;
				}
			}
		}

		// If barricade health becomes 0, then it's dead
		for (auto* barricade : barricadeVector)
		{
			for (int i = 0; i < (int)barricadeVector.size(); i++)
			{
				if (barricadeVector[i]->GetHealth() <= 0)
				{
					Mix_PlayChannel(3, barrier_destroyed, 0);
					// Delete the Barricade:
					std::vector<Barricade*>::iterator it;
					it = barricadeVector.begin() + i;
					barricadeVector.erase(it);
				}
			}
		}

}
void Game::GenerateOutput()
{
	/// Step 1: Set up background:
	// Clear the back buffer by specifying a colour (black):
	SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 255);
	// Clears the back buffer to black:
	SDL_RenderClear(m_Renderer);
	//------------------------------------------

	/// Step 2: Render Player's Ship:
	// Set Draw Colour to bright green:
	SDL_SetRenderDrawColor(m_Renderer, 22, 195, 22, 255);
	if (player.GetStatus() != Dead)
	{
		// Set up Player's Rect:
		Vector2 playerPos = player.GetPosition();
		Vector2 playerScale = player.GetScale();
		int width = (int)playerScale.x;
		int height = (int)playerScale.y;
		// This is the player's hitbox:
		SDL_Rect player
		{
			(int)playerPos.x,
			(int)playerPos.y,
			(int)playerScale.x,
			(int)playerScale.y
		};
		// Load the Ship's image:
		SDL_Texture* playerImg = IMG_LoadTexture(m_Renderer, "images/Ship.png");
		SDL_QueryTexture(playerImg, nullptr, nullptr, &width, &height);
		SDL_RenderCopy(m_Renderer, playerImg, nullptr, &player);
		// Release Memory:
		SDL_DestroyTexture(playerImg);
	}

	//------------------------------------------

	/// Step 3: Render the Player's bullets:
	for (auto& bullet : bulletVector)
	{
		// Set up bullet's hitbox:
		Vector2 currentBulletPos = bullet->GetPosition();
		Vector2 currentBulletScale = bullet->GetScale();
		int bulletWidth = (int)currentBulletScale.x;
		int bulletHeight = (int)currentBulletScale.y;

		// This is the bullet's hitbox:
		SDL_Rect playerBullet
		{
			(int)currentBulletPos.x,
			(int)currentBulletPos.y,
			(int)currentBulletScale.x,
			(int)currentBulletScale.y
		};

		SDL_RenderFillRect(m_Renderer, &playerBullet);
	}

	//------------------------------------------

	/// Step 4: Render the Aliens:
	// Set Render colour to white:
	SDL_SetRenderDrawColor(m_Renderer, 255, 255, 255, 255);

	// Loop through all aliens in the vector,
	// creating a Rect for each and filling them:
	for (int i = 0; i < (int)alienVector.size(); i++)
	{
		// Get the scale and pos of current alien:
		Vector2 currentPos = alienVector[i]->GetPosition();
		Vector2 currentScale = alienVector[i]->GetScale();
		if (alienVector[i]->GetStatus() == JustKilled)
		{
			deathAnimationCounter += 1;
			SDL_Rect alien{
			(int)currentPos.x,
			(int)currentPos.y,
			30,
			30
			};
			int alienW = (int)currentScale.x;
			int alienH = (int)currentScale.y;
			AlienDeathImage = IMG_LoadTexture(m_Renderer, "images/Alien_Death.png");
			SDL_QueryTexture(AlienDeathImage, nullptr, nullptr, &alienW, &alienH);
			SDL_RenderCopy(m_Renderer, AlienDeathImage, nullptr, &alien);
			SDL_DestroyTexture(AlienDeathImage);
			if (deathAnimationCounter > 7)
			{
				alienVector[i]->SetStatus(Dead);
				deathAnimationCounter = 0;
			}
			else
			{
				continue;
			}
		}

		if (alienVector[i]->GetStatus() == Dead)
		{
			continue;
		}
		
		// Load Squid Sprites:
		if (i >= 0 && i < 11)
		{
			SDL_Rect alien{
			(int)currentPos.x,
			(int)currentPos.y,
			(int)currentScale.x,
			(int)currentScale.y
			};
			int squidWidth = (int)currentScale.x;
			int squidHeigh = (int)currentScale.y;
			// Set sprite:
			// Set which sprite should be rendered:
			if(alienVector[i]->GetSpriteNumber() == 1)
			{ 
				squidImage = IMG_LoadTexture(m_Renderer, "images/Squid_1.png");
			}
			if (alienVector[i]->GetSpriteNumber() == 2)
			{
				squidImage = IMG_LoadTexture(m_Renderer, "images/Squid_2.png");
			}
			// Render Squid sprite:
			SDL_QueryTexture(squidImage, nullptr, nullptr, &squidWidth, &squidHeigh);
			SDL_RenderCopy(m_Renderer, squidImage, nullptr, &alien);
			SDL_DestroyTexture(squidImage);
			
		}
		// Load Crab Sprites:
		if (i >= 11 && i < 33)
		{
			SDL_Rect alien{
			(int)currentPos.x,
			(int)currentPos.y,
			(int)currentScale.x + 5,
			(int)currentScale.y
			};
			int crabWidth = (int)currentScale.x;
			int crabHeigh = (int)currentScale.y;
			// Set which sprite should be rendered:
			if (alienVector[i]->GetSpriteNumber() == 1)
			{
				crabImage = IMG_LoadTexture(m_Renderer, "images/Crab_1.png");
			}
			if (alienVector[i]->GetSpriteNumber() == 2)
			{
				crabImage = IMG_LoadTexture(m_Renderer, "images/Crab_2.png");
			}
			// Render Crab Sprite:
			SDL_QueryTexture(crabImage, nullptr, nullptr, &crabWidth, &crabHeigh);
			SDL_RenderCopy(m_Renderer, crabImage, nullptr, &alien);
			SDL_DestroyTexture(crabImage);
		}
		// Load Octopus Sprites:
		if (i >= 33 && i < 55)
		{
			SDL_Rect alien{
			(int)currentPos.x,
			(int)currentPos.y,
			(int)currentScale.x + 8,
			(int)currentScale.y
			};
			int octopusWidth = (int)currentScale.x;
			int octopusHeigh = (int)currentScale.y;
			// Set which sprite should be rendered:
			if (alienVector[i]->GetSpriteNumber() == 1)
			{
				octopusImage = IMG_LoadTexture(m_Renderer, "images/Octopus_1.png");
			}
			if (alienVector[i]->GetSpriteNumber() == 2)
			{
				octopusImage = IMG_LoadTexture(m_Renderer, "images/Octopus_2.png");
			}
			// Render Octopus Sprite:
			SDL_QueryTexture(octopusImage, nullptr, nullptr, &octopusWidth, &octopusHeigh);
			SDL_RenderCopy(m_Renderer, octopusImage, nullptr, &alien);
			SDL_DestroyTexture(octopusImage);
		}
	}

	//------------------------------------------
	/// Step 5: Render Alien Bullets:
	alienBulletCounter += (2 * 0.60f);
	for (auto& alienBullet : alienBulletVector)
	{
		// This check makes sure that bullets aren't rendered when outside the window,
		// preventing memory leaks:
		if (alienBullet->GetPosition().y < 760)
		{
			// Reset Timer:
			if (alienBulletCounter > 80)
			{
				alienBulletCounter = 0;
			}

			// This code will render alien bullets white (if before barricades), green if after:
			if (alienBulletCounter > 0 && alienBulletCounter < 21)
			{
				if (alienBullet->GetPosition().y > 550)
				{
					AlienBulletImage = IMG_LoadTexture(m_Renderer, "images/Alien_Bullet_1_1g.png");
				}
				else
				{
					AlienBulletImage = IMG_LoadTexture(m_Renderer, "images/Alien_Bullet_1_1.png");
				}
			}
			if (alienBulletCounter > 20 && alienBulletCounter < 41)
			{
				if (alienBullet->GetPosition().y > 550)
				{
					AlienBulletImage = IMG_LoadTexture(m_Renderer, "images/Alien_Bullet_1_2g.png");
				}
				else
				{
					AlienBulletImage = IMG_LoadTexture(m_Renderer, "images/Alien_Bullet_1_2.png");
				}

			}
			if (alienBulletCounter > 40 && alienBulletCounter < 61)
			{
				if (alienBullet->GetPosition().y > 550)
				{
					AlienBulletImage = IMG_LoadTexture(m_Renderer, "images/Alien_Bullet_1_3g.png");
				}
				else
				{
					AlienBulletImage = IMG_LoadTexture(m_Renderer, "images/Alien_Bullet_1_3.png");
				}
			}
			if (alienBulletCounter > 60 && alienBulletCounter < 80)
			{
				if (alienBullet->GetPosition().y > 550)
				{
					AlienBulletImage = IMG_LoadTexture(m_Renderer, "images/Alien_Bullet_1_4g.png");
				}
				else
				{
					AlienBulletImage = IMG_LoadTexture(m_Renderer, "images/Alien_Bullet_1_4.png");
				}

			}

			// Set up bullet's hitbox:
			Vector2 currentBulletPos = alienBullet->GetPosition();
			Vector2 currentBulletScale = alienBullet->GetScale();
			int bulletWidth = (int)currentBulletScale.x;
			int bulletHeight = (int)currentBulletScale.y;

			// This is the bullet's hitbox:
			SDL_Rect alienBullet
			{
				(int)currentBulletPos.x,
				(int)currentBulletPos.y,
				6,
				20
			};
			SDL_QueryTexture(AlienBulletImage, nullptr, nullptr, &bulletWidth, &bulletHeight);
			SDL_RenderCopy(m_Renderer, AlienBulletImage, nullptr, &alienBullet);
			SDL_DestroyTexture(AlienBulletImage);
		}
		
	}

	//------------------------------------------
	/// Step 6: Render UFO:
	for (auto* UFO : UFOVector)
	{
		if (UFO->GetStatus() == Dead)
		{
			continue;
		}
		Vector2 currentPos = UFO->GetPosition();
		Vector2 currentScale = UFO->GetScale();
		SDL_Rect UFORect{
			(int)currentPos.x,
			(int)currentPos.y,
			(int)currentScale.x,
			(int)currentScale.y
		};
		UFOImage = IMG_LoadTexture(m_Renderer, "images/UFO_2.png");
		SDL_QueryTexture(UFOImage, nullptr, nullptr, &UFORect.w, &UFORect.h);
		SDL_RenderCopy(m_Renderer, UFOImage, nullptr, &UFORect);
		SDL_DestroyTexture(UFOImage);
	}

	//------------------------------------------
	/// Step 7: Render UI:
	// Render ArcadeMode Joystick
	if (gameMode == 1)
	{
		SDL_Rect Joystick{
				200,
				670,
				60,
				60
		};
		if (player.GetDirection() < 0)
		{
			JoystickImage = IMG_LoadTexture(m_Renderer, "images/joystick_left.png");
		}
		else if (player.GetDirection() > 0)
		{
			JoystickImage = IMG_LoadTexture(m_Renderer, "images/joystick_right.png");
		}
		else
		{
			JoystickImage = IMG_LoadTexture(m_Renderer, "images/joystick.png");
		}
		SDL_QueryTexture(JoystickImage, nullptr, nullptr, &Joystick.w, &Joystick.h);
		SDL_RenderCopy(m_Renderer, JoystickImage, nullptr, &Joystick);
		SDL_DestroyTexture(JoystickImage);

		// Render ArcadeMode Button:
		SDL_Rect Button{
				150,
				720,
				60,
				60
		};
		if (hasShotTimer > 0)
		{
			ButtomImage = IMG_LoadTexture(m_Renderer, "images/button_down.png");
			hasShotTimer -= 1;
		}
		else
		{
			ButtomImage = IMG_LoadTexture(m_Renderer, "images/button_up.png");
		}
		SDL_QueryTexture(ButtomImage, nullptr, nullptr, &Button.w, &Button.h);
		SDL_RenderCopy(m_Renderer, ButtomImage, nullptr, &Button);
		SDL_DestroyTexture(ButtomImage);
	}

	SDL_Rect Back{
			0,
			760,
			1024,
			60
	};
	BackImage = IMG_LoadTexture(m_Renderer, "images/Back_Area.png");
	SDL_QueryTexture(BackImage, nullptr, nullptr, &Back.w, &Back.h);
	SDL_RenderCopy(m_Renderer, BackImage, nullptr, &Back);
	SDL_DestroyTexture(BackImage);

	//------------------------------------------
	/// Display player lives:
	SDL_Rect playerLivesRect{
			   800,
			   10,
			   200,
			   30
	};
	if (player.GetNumberOfLives() == 3)
	{
		player_lives = IMG_LoadTexture(m_Renderer, "images/lives_3.png");
	}
	else if (player.GetNumberOfLives() == 2)
	{
		player_lives = IMG_LoadTexture(m_Renderer, "images/lives_2.png");
	}
	else
	{
		player_lives = IMG_LoadTexture(m_Renderer, "images/lives_1.png");
	}
	SDL_RenderCopy(m_Renderer, player_lives, nullptr, &playerLivesRect);
	SDL_DestroyTexture(player_lives);



	/// Render Barricades:
	// Set Draw Colour to bright green:
	SDL_SetRenderDrawColor(m_Renderer, 0, 252, 0, 255);
	for (auto* barricade : barricadeVector)
	{
		if (barricade->GetStatus() == Dead)
		{
			continue;
		}
		SDL_Rect barricadeRect{
			(int)barricade->GetPosition().x,
			(int)barricade->GetPosition().y,
			90,
			70
		};
		if (barricade->GetHealth() > 60)
		{
			barricadeImage = IMG_LoadTexture(m_Renderer, "images/barricade_1.png");
		}
		else if (barricade->GetHealth() > 45 && barricade->GetHealth() < 61)
		{
			barricadeImage = IMG_LoadTexture(m_Renderer, "images/barricade_2.png");
		}
		else if (barricade->GetHealth() > 20 && barricade->GetHealth() < 46)
		{
			barricadeImage = IMG_LoadTexture(m_Renderer, "images/barricade_3.png");
		}
		else if (barricade->GetHealth() > 10 && barricade->GetHealth() < 47)
		{
			barricadeImage = IMG_LoadTexture(m_Renderer, "images/barricade_4.png");
		}
		else
		{
			barricadeImage = IMG_LoadTexture(m_Renderer, "images/barricade_5.png");
		}
		
		SDL_RenderCopy(m_Renderer, barricadeImage, nullptr, &barricadeRect);
		SDL_DestroyTexture(barricadeImage);
	}

	/// Render player's score:
	// Initialise font:
	font = TTF_OpenFont("font/font.ttf", 20);
	if (font == NULL)
	{
		SDL_Log("Unable to initialise SDL_TTF Font: %s", SDL_GetError());
	}
	// Set Colour:
	SDL_Color white{ 255, 255, 255, 255 };
	SDL_Color green{ 0, 248, 0, 255 };


	// Setup Surface:
	score = TTF_RenderText_Solid(font, "SCORE: ", white);
	// Setup Texture:
	scoreImage = SDL_CreateTextureFromSurface(m_Renderer, score);
	SDL_FreeSurface(score);

	// Set up score number surface:
	std::string playerScoreNumb = std::to_string(player.GetScore());
	score = TTF_RenderText_Solid(font, playerScoreNumb.c_str(), green);
	// Set up score number texture:
	scoreNumb = SDL_CreateTextureFromSurface(m_Renderer, score);
	SDL_FreeSurface(score);

	// Set up lastRoundScore Surface:
	std::string lastRoundScoreStr = "HIGH SCORE:   " + std::to_string(m_lastScore);
	score = TTF_RenderText_Solid(font, lastRoundScoreStr.c_str(), white);
	// Set up score number texture:
	lastRoundScore = SDL_CreateTextureFromSurface(m_Renderer, score);
	SDL_FreeSurface(score);

	// Set up currentWave Surface:
	std::string currentWaveStr = "CURRENT WAVE:    " + std::to_string(m_CurrentWave);
	score = TTF_RenderText_Solid(font, currentWaveStr.c_str(), white);
	// Set up currentWave texture:
	waveNumb = SDL_CreateTextureFromSurface(m_Renderer, score);
	SDL_FreeSurface(score);


	// Create Rect for score letters:
	SDL_Rect scoreRect{
		30,
		15,
		85,
		25
	};

	// Create Rect for score numbers:
	SDL_Rect scoreNumbRect{
		150,
		15,
		40,
		25
	};

	// Create Rect for Last Round Score:
	SDL_Rect lastRoundScoreRect{
		260,
		15,
		190,
		25
	};

	// Create Rect for currentWave:
	SDL_Rect currentWaveRect{
		500,
		15,
		190,
		25
	};

	// Render:
	// Score text:
	SDL_RenderCopy(m_Renderer, scoreImage, NULL, &scoreRect);
	// Score Number
	SDL_QueryTexture(scoreNumb, nullptr, nullptr, &scoreNumbRect.w, &scoreNumbRect.h);
	SDL_RenderCopy(m_Renderer, scoreNumb, NULL, &scoreNumbRect);
	// Last Round Score
	SDL_RenderCopy(m_Renderer, lastRoundScore, NULL, &lastRoundScoreRect);
	SDL_QueryTexture(lastRoundScore, nullptr, nullptr, &lastRoundScoreRect.w, &lastRoundScoreRect.h);
	// Current Wave Numb:
	SDL_RenderCopy(m_Renderer, waveNumb, NULL, &currentWaveRect);
	SDL_QueryTexture(waveNumb, nullptr, nullptr, &currentWaveRect.w, &currentWaveRect.h);

	// Close Font after use:
	TTF_CloseFont(font);
	// Destroy Texture:
	SDL_DestroyTexture(scoreImage);
	SDL_DestroyTexture(scoreNumb);
	SDL_DestroyTexture(lastRoundScore);
	SDL_DestroyTexture(waveNumb);


	/// Display Insight Mode content:
	// Initialise font:
	font = TTF_OpenFont("font/font.ttf", 20);



	/// Final Step: Render all changes:
	// Present all Render changes to the window:
	SDL_RenderPresent(m_Renderer);
}

void Game::Shutdown()
{
	// Clear alien vector memory:
	for (auto &alien : alienVector)
	{
		delete alien;
	}
	for (auto& barricade : barricadeVector)
	{
		delete barricade;
	}

	SDL_DestroyWindow(m_Window); // Destroys Window
	SDL_DestroyRenderer(m_Renderer); // Destroys Renderer
	SDL_Quit(); // Ends the program.

}