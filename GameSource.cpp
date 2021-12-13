#include "include/GameNode.h"

int main( int argc, char** argv )
{
	GameNode game_node{ argc, argv };
	game_node.run();
}