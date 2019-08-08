#include "BaseGame.h"

#ifdef _DEBUG
#include <vld.h>
#endif

int main(int argc, char** argv){
	BaseGame game("Game", (int)(1280 / 1.75f), (int)(736 / 1.5f), 0, -1, -1);
	return game.run();;
}