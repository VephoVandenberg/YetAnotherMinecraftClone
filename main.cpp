#include "src/game_base/app/application.h"

using namespace GameNamespace;

/*
* Agenda
* Later
* - Entitiy Component System(Do I need that really)
* - Some debug drawing
* Super Late
* - Investigate order of drawing faces
* - Raycast and cube removal still don't work the way I would like it to work(VERY IMPORTANT)
* - Investigate the number of meshes that are alocated for chunks
* - infinite terrain generation maybe there is faster and better way of doing it
* - More complex terrain generation
* - Mouse control(That thing doesn't seem to work now)
* - Make a Cmake project
* - I hope now std::async works the way it should(If something happens keep track of it)
*/


int main(int argc, char** argv)
{
	Application *app = new Application;
	app->run();

	delete app;

	return 0;
} 
