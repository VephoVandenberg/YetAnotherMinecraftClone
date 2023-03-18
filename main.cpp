
#include "src/game_base/application.h"

using namespace GameNamespace;

/*
* Agenda
* - Camera
* - Draw a cube
* - Textures
*/


int main(int argc, char** argv)
{
	Application *app = new Application;
	app->run();

	delete app;

	return 0;
}
