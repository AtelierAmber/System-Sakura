#include "MainGame.h"

#ifdef _DEBUG
#include <vld.h>
#endif

int main(int argc, char** argv){
	MainGame game("Main Game", (int)(1280 / 1.75f), (int)(736 / 1.5f), 0, -1, -1);
	game.run();
	return 0;
}