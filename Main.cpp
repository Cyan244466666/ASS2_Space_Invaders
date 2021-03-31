#include <SDL.h>
#include "GameObject.h"
#include "Game.h"
#include "Barrier.h"


int main(int argc, char** argv)
{
	Game SpaceInvaders;
	SpaceInvaders.Initialise();
	SpaceInvaders.RunGameLoop();
	
	SpaceInvaders.Shutdown();
	return 0;
}