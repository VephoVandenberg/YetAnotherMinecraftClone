#include "src/game_base/app/application.h"

using namespace GameNamespace;

/*
* Agenda
* - one more time look at the perlin noise generation octaves and amplitudes
* Later
* - infinite terrain generation
* - imGUI
* - add more blocks 
* - layers in the chunk
* Super Later
* - Investigate order of drawing faces
* - Investigate the way cubemaps are drawn
* - Investigate the number of meshes that are alocated for chunks
* - Raycast and cube removal still don't work the way I would like it to work(VERY IMPORTANT)
*/


int main(int argc, char** argv)
{
	Application *app = new Application;
	app->run();

	delete app;

	return 0;
}
