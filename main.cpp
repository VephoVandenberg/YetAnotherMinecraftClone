#include "src/game_base/app/application.h"

using namespace GameNamespace;

/*
* Agenda
* Later
* - Entitiy Component System(Do I need that really?) Just read about it
* - Some debug drawing
* - More blocks
* - Investigate the number of meshes that are alocated for chunks
* - Skybox
* - Player physics
* - More complex terrain generation(Perlin3D)
* Super Late
* - Investigate order of drawing faces
* - Mouse control(Do not think about it for now beacuse it doesn't matter at least, now)
* - Make a Cmake project
* - I hope now std::async works the way it should(If something happens keep track of it)
* - Maybe we'll got some UI?
*/


int main(int argc, char** argv)
{
	Application *app = new Application;
	app->run();

	delete app;

	return 0;
} 
