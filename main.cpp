#include "src/game_base/application.h"

using namespace GameNamespace;

/*
* Agenda
* Later
* - Texture arrays
* - Raycast && cube removal
* - noise
* - terrain generation
* - add more blocks 
* - layers in the chunk
* Super Later
* - Investigate order of drawing faces
* - Investigate the way cubemaps are drawn
*/


int main(int argc, char** argv)
{
	Application *app = new Application;
	app->run();

	delete app;

	return 0;
}
