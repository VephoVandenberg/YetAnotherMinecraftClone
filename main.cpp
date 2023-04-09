#include "src/game_base/application.h"

using namespace GameNamespace;

/*
* Agenda
* - Texture cubes
* Later
* - Texture arrays
* - noise
* - terrain generation
* - add more blocks 
* - layers in the chunk
* 
* Super Later
* - Investigate order of drawing faces
*/


int main(int argc, char** argv)
{
	Application *app = new Application;
	app->run();

	delete app;

	return 0;
}
