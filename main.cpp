
#include "src/game_base/application.h"

using namespace GameNamespace;

/*
* Agenda
* - Draw a cube
*/


int main(int argc, char** argv)
{
	Application *app = new Application;
	app->run();

	delete app;

	return 0;
}
