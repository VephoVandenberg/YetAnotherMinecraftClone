#include "src/game_base/app/application.h"

using namespace GameNamespace;

/*
* Agenda
* Later
* - Simple ambient Lighting
* - imGUI(Framerate + any other stuff that I will probablt need)
* - Mouse control
* - deletion of the std::async
* - Entitiy Component System
* - Some debug drawing
* Super Late
* - Investigate order of drawing faces
* - Raycast and cube removal still don't work the way I would like it to work(VERY IMPORTANT)
* - Investigate the number of meshes that are alocated for chunks
* - infinite terrain generation maybe there is faster and better way of doing it
* - More complex terrain generation
* - Make a Cmake project
*/


int main(int argc, char** argv)
{
	Application *app = new Application;
	app->run();

	delete app;

	return 0;
} 
