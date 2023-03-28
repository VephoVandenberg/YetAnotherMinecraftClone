#include "src/game_base/application.h"

using namespace GameNamespace;

/*
* Agenda
* Later
* - Hello mesh
* - Chunks
* - Don't draw things that can't be seen, take in consideration tile maps
* - Atlas wrapper
*/


int main(int argc, char** argv)
{
	Application *app = new Application;
	app->run();

	delete app;

	return 0;
}
