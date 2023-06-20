#include "src/game_base/app/application.h"

/*
* Agenda
* - Again annoying chunk face render problem
* - Multithreading makes me sad(Fix it fucker, only if it will be needed)
* Later
* - Entitiy Component System(Do I need that really?) Just read about it
* - More blocks
* - Serialisation system
* - Investigate the number of meshes that are alocated for chunks
* - Skybox
* - Player physics
* - More complex terrain generation(Perlin3D)
* Super Late
* - Investigate order of drawing faces
* - Mouse control(Do not think about it for now beacuse it doesn't matter at least, now)
* - Make a Cmake project
* - Maybe we'll got some UI?
*/

int main(int argc, char** argv)
{
	GameNamespace::Application *app = new GameNamespace::Application;
	app->run();

	delete app;

	return 0;
} 
