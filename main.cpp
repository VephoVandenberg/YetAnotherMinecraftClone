#include "src/game_base/app/application.h"

using namespace GameNamespace;

/*
* Agenda
* - Fix terrain face rendering
* Later
* - one more time look at the perlin noise generation octaves and amplitudes
* - imGUI
* - Investigate order of drawing faces
* - layers in the chunk
* Super Later
* - Investigate the number of meshes that are alocated for chunks
* - Raycast and cube removal still don't work the way I would like it to work(VERY IMPORTANT)
* - infinite terrain generation maybe there is faster and better way of doing it
*/


int main(int argc, char** argv)
{
	Application *app = new Application;
	app->run();

	delete app;

	return 0;
} 
