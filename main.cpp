
#include "src/game_base/application.h"

using namespace GAME_NAMESPACE;

/*
Agenda
 - Draw a quad
*/

int main(int argc, char** argv)
{
	Application *app = new Application;
	app->run();

	delete app;

	return 0;
}
