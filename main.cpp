#include "src/game_base/app/application.h"

using namespace GameNamespace;

/*
* Agenda
* - Raycast && cube removal
* Later
* - noise
* - imGUI
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
