#include "trabalhoGA/Game.h"

// Fun��o MAIN
int main()
{
	Game game = Game();
	game.initialize();
	game.start();
}


/*
	Como mover?
	1 - pegar sprite do personagem como variavel global e no key_callback mudar a posicao
	

	Colisão. Como fazer?
	- armazenar altura e largura da sprite
*/