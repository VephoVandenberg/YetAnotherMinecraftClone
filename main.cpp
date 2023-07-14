#include "src/game_base/app/application.h"

/*
* TO DO
* - More complex terrain generation(Perlin3D)
* - Entitiy Component System(Do I need that really?) Just read about it
* - Serialisation system
* - Investigate the number of meshes that are alocated for chunks
* - Skybox
* - Player physics
* - Water?
* TO DO LATER
* - The way multithreading works is not perfect could be changed in the future
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
