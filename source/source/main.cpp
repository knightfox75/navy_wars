/*
-------------------------------------------------

	Navy Wars DS

	Requiere DevkitARM R28
	Requiere NightFox's Lib

	Codigo por NightFox
	http://blogs.gamefilia.com/knightfox
	Inicio 22 de Junio del 2009

	(c)2009 - 2010 NightFox & Co.

-------------------------------------------------
*/





/*
-------------------------------------------------
	Includes
-------------------------------------------------
*/

// Includes c/c++
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Includes propietarios NDS
#include <nds.h>


// Includes librerias propias
#include <nf_lib.h>

// Includes propios del juego
#include "navywars.h"





/*
-------------------------------------------------
	Main() - Bloque general del programa
-------------------------------------------------
*/

int main(void) {

	// Define variables de control del juego
	u8 players = 0;		// Numero de jugadores

	// Inicializa el engine del juego
	BootGame();

	// Pantallas de presentacion del juego
	SplashScreens();

	// Bucle (repite para siempre)
	while(1) {

		// Menu Principal - Muestra el menu principal y obten el numero de jugadores.
		players = MainMenu();

		// Inicializa la partida dependiendo del numero de jugadores.
		switch (players) {

			case 1:	// Un jugador
				NavySetup(1);
				ComputerSetup();
				PlayBattle();
				GameOver();
				break;

			case 2:	// Dos jugadores
				TwoPlayers();
				break;

			case 3:	// Creditos
				Credits();
				break;
		}
	
		swiWaitForVBlank();		// Espera al sincronismo vertical
	
	}

	return 0; 

}
