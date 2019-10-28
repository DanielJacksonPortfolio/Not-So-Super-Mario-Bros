

//#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )

#include "Game.h"

int main(int argc, char* args[])
{

	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Game* mario = new Game(argc, args);

	_CrtDumpMemoryLeaks();
	return 0;
}