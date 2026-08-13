#include "pch.h"
#include <ctime>
#include "Game.h"
#include <iostream>


void StartHeapControl();
void DumpMemoryLeaks();

int SDL_main(int argv, char** args)
{
	srand(static_cast<unsigned int>(time(nullptr)));

	StartHeapControl();

	Game* pGame{ nullptr };
	int returnValue{ 0 };
	try
	{
		pGame = new Game{ Window{ "GAME PROJECT, Mykhailo Kudenko - 1DAE14", utils::g_WindowSize.x , utils::g_WindowSize.y } };
		pGame->Run();

	}
	catch (const std::exception& error)
	{
		std::cerr << "Fatal error: " << error.what() << std::endl;
		returnValue = 1;
	}
	delete pGame;

	return returnValue;
}


void StartHeapControl()
{
#if defined(DEBUG) | defined(_DEBUG)
	// Notify user if heap is corrupt
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	// Report detected leaks when the program exits
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// Set a breakpoint on the specified object allocation order number
	//_CrtSetBreakAlloc( 156 );
#endif
}

void DumpMemoryLeaks()
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtDumpMemoryLeaks();
#endif
}


