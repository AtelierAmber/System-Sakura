#include "BaseGame.h"

#ifdef _DEBUG
#include <vld.h>
#endif

int main(int argc, char** argv){
	BaseGame game("Game", (int)(1280), (int)(760), Sakura::WindowFlags::BORDERLESS , -100, -100);
	return game.run();
}