#include "src/game_base/application.h"

using namespace GameNamespace;

/*
* Agenda
* Later
* - Meshes work strange still
* - Investigate order of drawing faces
* - Atlas wrapper
*/


int main(int argc, char** argv)
{
	Application *app = new Application;
	app->run();

	delete app;

	return 0;
}
