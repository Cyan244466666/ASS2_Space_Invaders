#include <SDL.h>
#include <SDL_image.h>
#include <time.h>
#include "GameObject.h"
#include "Game.h"
#include "Bullet.h"
#include <vector>
#include <iostream>
#include <algorithm>



bool changeSprite = false;


// Constructors:
Game::Game()
{
	m_Window = NULL;
	m_Renderer = NULL;
	m_IsRunning = true;
	m_ticksCount = NULL;
	barrierTop = { 0, 0, 1024, 150 };
	barrierBottom = { 0, 718, 1024, 5 };
	barrierRight = { 904, 0, 220, 768 };
	barrierLeft = {0, 0, 135, 768};
	
	// Sprites:
	squidImage = nullptr;
	crabImage = nullptr;
	octopusImage = nullptr;
	UFOImage = nullptr;
	touchWall = false;

	playerBulletImage = nullptr;
}
// Destructor
Game::~Game() {}

// This function will initialise SDL, the Window, and the Renderer,
// when the game is launched. It will return 'true' if successful,
// and false otherwise:
bool Game::Initialise()
{
	// Step 1: Initialise SDL - if successful, returns 0, otherwise print error:
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialise SDL: %s", SDL_GetError());
		return 1;
	}
	//------------------------------------------
	// Step 2: Initialise Window - returns false is unsuccessful.
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
	// Step 3: Initialise Renderer - returns false is unsuccessful.
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
	// Step 4: 
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		SDL_Log("Unable to initialise IMG: %s", SDL_GetError());
		return 1;
	}

	// If all initialisation is successful, return true:


	//------------------------------------------
	// Step 5: Initialise the Aliens' start position:
	// First, we set the Position of the space
	// in which the aliens will spawn:
	float spaceX = 230;
	float spaceY = 140;

	
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
		yPos = ((i / 11) * ( alienScale.y + 20 )) + spaceY;

		// Set index and column number:
		alien->SetIndex(i);
		alien->SetColumn(i % 11);
		
		
	    // Finally, set the Alien's position vector to
		// the created coordinates:
		Vector2 spawnPosition{ xPos, yPos };
		alien->SetPosition(spawnPosition);
		alienVector.push_back(alien);
		Game::GenerateOutput();
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
	/// Step 4: Calculate Alien Movement:
	// This code determines the alien movement speed based on how many aliens are alive:
	
	int aliensAlive = 0;
	// Check how many aliens are alive:
	for (auto* alien : alienVector)
	{
		if (alien->GetStatus() == Alive)
		{
			aliensAlive += 1;
		}
	}
	// End game if there are no aliens:
	if(aliensAlive == 0)
	{
		Game::Shutdown();
	}
	// Calculate Movement Speed:
	if(aliensAlive == 1) { movementSpeed = 12.0f;}
	if(aliensAlive == 2) { movementSpeed = 9.8f;}
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
	/// Step 5: Check if player has fired a bullet, if true,
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
			shootingDelay = 0.0f;
		}
	}
	player.SetHasShot(false);

	//------------------------------------------
	/// Step 6: Move Player's bullets:
	for (auto& bullet : bulletVector)
	{
		bullet->MoveBullet();
	}

	//------------------------------------------

	/// Step 7: Bullet Collision for all aliens:
	
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
				alienVector[i]->SetStatus(Dead);
				bullet->SetHasHit(true);
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
	/// Step 8: Check which aliens are in the front row:
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
		bullet->SetPosition(shooterAlien->GetPosition());
		alienBulletVector.push_back(bullet);

		alienShootingDelay = 0.0f;
	}

	for (auto alienBullets : alienBulletVector)
	{
		alienBullets->MoveAlienBullet();
	}


	//------------------------------------------
	/// Step 9: Ship/Player Bullet Collision:
	// This code checks whether the player has been hit by an alien bullet:
	for (auto alienBullet : alienBulletVector)
	{
		bool playerHit = false;
		playerHit = player.BulletCollision(alienBullet->GetPosition(), alienBullet->GetScale());

		if (playerHit)
		{
			player.SetStatus(Dead);
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

	/// Step 10: Respawn Player/Check if GameOver:
	// This code checks if the player has lost all 3 lives - if yes - the game ends.
	if (player.GetNumberOfLives() == 0)
	{
		Game::Shutdown();
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

	/// Step 11: Randomly spawn UFO
	// This code sets up a timer and a random number to spawn the UFO at,
	// if the timer is exceeds the spawn number, the UFO spawns: 
	UFOTimer += (2 * deltaTime);
	static int UFOSpawnTime = (rand() % 60) + 30;
	if (UFOTimer > (float)UFOSpawnTime)
	{
		Alien* UFO = new Alien();
		int UFOSpawnSide = (rand() % 2) + 1;
		if (UFOSpawnSide == 1)
		{
			// Spawn UFO top-left
			Vector2 UFOSpawnPos{ 30, 50 };
			UFO->SetPosition(UFOSpawnPos);
			UFO->SetDirection(1);
			UFOTimer = 0.0f;
			UFOVector.push_back(UFO);
		}
		if (UFOSpawnSide == 2)
		{
			// Spawn UFO top-left
			Vector2 UFOSpawnPos{ 1000, 50 };
			UFO->SetPosition(UFOSpawnPos);
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

	
	//------------------------------------------

	/// Step 5: Render the Aliens:
	// Set Render colour to white:
	SDL_SetRenderDrawColor(m_Renderer, 255, 255, 255, 255);

	// Loop through all aliens in the vector,
	// creating a Rect for each and filling them:
	for (int i = 0; i < (int)alienVector.size(); i++)
	{
		// Get the scale and pos of current alien:
		Vector2 currentPos = alienVector[i]->GetPosition();
		Vector2 currentScale = alienVector[i]->GetScale();
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
	/// Step 6: Render Alien Bullets:
	for (auto& alienBullet : alienBulletVector)
	{
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
			(int)currentBulletScale.x,
			(int)currentBulletScale.y
		};

		SDL_RenderFillRect(m_Renderer, &alienBullet);

	}

	//------------------------------------------
	/// Step 7: Render UFO:
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

	SDL_DestroyWindow(m_Window); // Destroys Window
	SDL_DestroyRenderer(m_Renderer); // Destroys Renderer
	SDL_Quit(); // Ends the program.

}