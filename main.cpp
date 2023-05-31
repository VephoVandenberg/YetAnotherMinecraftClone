#include "src/game_base/app/application.h"

using namespace GameNamespace;

/*
* Agenda
* - Raycast and cube removal still don't work the way I would like it to work(VERY IMPORTANT)
* Later
* - imGUI
* - More complex terrain generation
* Super Late
* - Investigate order of drawing faces
* - Investigate the number of meshes that are alocated for chunks
* - infinite terrain generation maybe there is faster and better way of doing it
* - Make a Cmake project
*/


int main(int argc, char** argv)
{
	Application *app = new Application;
	app->run();

	delete app;

	return 0;
} 
