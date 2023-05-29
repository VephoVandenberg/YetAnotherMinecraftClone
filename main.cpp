#include "src/game_base/app/application.h"

using namespace GameNamespace;

/*
* Agenda
* Later
* - Make again your version of perlin noise with acquired knowledge
* - imGUI
* - More complex terrain generation
* - layers in the chunk
* Super Late
* - Investigate order of drawing facesr
* - Investigate the number of meshes that are alocated for chunks
* - Raycast and cube removal still don't work the way I would like it to work(VERY IMPORTANT)
* - infinite terrain generation maybe there is faster and better way of doing it
* - Fix terrain face rendering(Hope you fixed that you unbelievebly stupid ass motherfucker!)
* - Make a Cmake project
*/


int main(int argc, char** argv)
{
	Application *app = new Application;
	app->run();

	delete app;

	return 0;
} 
