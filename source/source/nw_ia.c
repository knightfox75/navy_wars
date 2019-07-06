#ifdef __cplusplus
extern "C" {
#endif





	// Navy Wars DS - Funciones de Game
	// Requiere DevkitARM R26
	// Requiere NightFox's Lib
	// Requiere EFS Lib
	// Codigo por NightFox
	// http://blogs.gamefilia.com/knightfox
	// Inicio 22 de Junio del 2009



	// Includes c/c++
	#include <stdio.h>
	#include <string.h>
	#include <unistd.h>
	#include <time.h>

	// Includes propietarios NDS
	#include <nds.h>

	// Includes NightFox's Lib
	#include <nf_lib.h>

	// Includes del juego
	#include "nw_ia.h"
	#include "nw_system.h"
	#include "nw_common.h"






	// Funcion CpuIa();
	u8 CpuIa(void) {

		// Variables locales
		u8 index = 0;		// Indice a devolver
		s8 grid_x = 0;		// Coordenada X
		s8 grid_y = 0;		// Coordenada Y
		u8 value = 0;		// Valor de la casilla
		
		u8 n = 0;

		// Ejecuta el Thread de la IA seleccionado
		switch (IA.thread) {

			case IA_DO_RANDOM:		// Ejecuta un Random
				if (GRID_REST[PLAYER_2] > 1) {					// Si queda mas de una casilla...
					index = (rand() % GRID_REST[PLAYER_2]);		// Random de las casillas libres
				} else {										// Pero si solo queda una libre,
					index = 0;									// Asignala
				}
				break;

			case IA_DO_TRY_NEXT:	// Muevete a la siguiente casilla, a ver si hay suerte
				if (IA.next_move == MOVE_UP) {		// Si el siguiente movimiento es arriba...
					grid_x = IA.first_x;
					grid_y = (IA.first_y - 1);
				}
				if (IA.next_move == MOVE_DOWN) {	// Si el siguiente movimiento es abajo...
					grid_x = IA.first_x;
					grid_y = (IA.first_y + 1);
				}
				if (IA.next_move == MOVE_LEFT) {	// Si el siguiente movimiento es a la izquierda...
					grid_x = (IA.first_x - 1);
					grid_y = IA.first_y;
				}
				if (IA.next_move == MOVE_RIGHT) {	// Si el siguiente movimiento es a la derecha...
					grid_x = (IA.first_x + 1);
					grid_y = IA.first_y;
				}
				// Ahora calcula el valor de la casilla a buscar
				value = grid_x + (grid_y << 3);
				// Buscala en el array y devuelve el indice donde esta
				for (n = 0; n < GRID_REST[PLAYER_2]; n ++) {
					if (IA_GRID[0][n] == value) {
						index = n;
						n = GRID_REST[PLAYER_2];
					}
				}
				// Y para finalizar, registra las coordenadas
				IA.last_x = grid_x;
				IA.last_y = grid_y;
				break;

			case IA_DO_CONTINUE:	// Continua en la misma direccion del anterior disparo
				if (IA.next_move == MOVE_UP) {		// Si el siguiente movimiento es arriba...
					grid_x = IA.last_x;
					grid_y = (IA.last_y - 1);
				}
				if (IA.next_move == MOVE_DOWN) {	// Si el siguiente movimiento es abajo...
					grid_x = IA.last_x;
					grid_y = (IA.last_y + 1);
				}
				if (IA.next_move == MOVE_LEFT) {	// Si el siguiente movimiento es a la izquierda...
					grid_x = (IA.last_x - 1);
					grid_y = IA.last_y;
				}
				if (IA.next_move == MOVE_RIGHT) {	// Si el siguiente movimiento es a la derecha...
					grid_x = (IA.last_x + 1);
					grid_y = IA.last_y;
				}
				// Ahora calcula el valor de la casilla a buscar
				value = grid_x + (grid_y << 3);
				// Buscala en el array y devuelve el indice donde esta
				for (n = 0; n < GRID_REST[PLAYER_2]; n ++) {
					if (IA_GRID[0][n] == value) {
						index = n;
						n = GRID_REST[PLAYER_2];
					}
				}
				// Y para finalizar, registra las coordenadas
				IA.last_x = grid_x;
				IA.last_y = grid_y;
				break;

		}

		// Devuelve el resultado
		return index;

	}



	// Funcion InitCpuIa();
	void InitCpuIa(void) {

		// Variables locales
		u8 n = 0;

		// Matriz de casillas no destapadas
		for (n = 0; n < 64; n ++) {
			IA_GRID[0][n] = n;
			IA_GRID[1][n] = n;
		}

		// Inicializa parametros de la IA
		IA.thread = 0;
		IA.first_x = 0;			
		IA.first_y = 0;
		IA.last_x = 0;
		IA.last_y = 0;
		IA.hits = 0;
		IA.next_move = 0;
		IA.up = true;
		IA.down = true;
		IA.left = true;
		IA.right = true;
		IA.flag = false;

	}



	// Funcion IaCheckArround
	void IaCheckArround(s8 x, s8 y) {

		// Izquierda
		if (x > 0) {
			if (TMPGRID[PLAYER_1][(x - 1)][y] == 255) {
				IA.left = true;
			} else {
				IA.left = false;
			}
		} else {
			IA.left = false;
		}
		// Derecha
		if (x < 7) {
			if (TMPGRID[PLAYER_1][(x + 1)][y] == 255) {
				IA.right = true;
			} else {
				IA.right = false;
			}
		} else {
			IA.right = false;
		}
		// Arriba
		if (y > 0) {
			if (TMPGRID[PLAYER_1][x][(y - 1)] == 255) {
				IA.up = true;
			} else {
				IA.up = false;
			}
		} else {
			IA.up = false;
		}
		// Abajo
		if (y < 7) {
			if (TMPGRID[PLAYER_1][x][(y + 1)] == 255) {
				IA.down = true;
			} else {
				IA.down = false;
			}
		} else {
			IA.down = false;
		}

	}



	// Funcion IaNextAction();
	void IaNextAction(bool hit) {

		// Variables locales
		u8 z = 0;

		// Si has acertado el blanco...
		if (hit) {

			// Antes de todo, verifica si este disparo a undido el navio
			z = GRID[PLAYER_1][IA.first_x][IA.first_y];

			// Si a este barco, lo ha hundido este disparo...
			if ((z >= 0) && (z <= 5) && (SHIP[PLAYER_1][z].live == 0)) {

				// Busca si hay algun otro navio parcialmente descubierto
				if (IaScanForShips()) {		// Si lo hay...
					// Contabiliza el acierto
					IA.hits ++;
					// Decide el siguiente movimiento
					IaDecideNextMove();
				} else {	// Si no lo hay
					// Busca desde el principio
					IA.hits = 0;
					IA.thread = IA_DO_RANDOM;
				}

			} else {

				// Si es el primer acierto...
				if (IA.hits == 0) {

					// Contabiliza el acierto
					IA.hits ++;
					// Decide el siguiente movimiento
					IaDecideNextMove();

				} else {	// Si has acertado, y llevas uno mas...

					// Incrementa el contador de aciertos
					IA.hits ++;
					// Consulta las casillas a tu alrededor
					IaCheckArround(IA.last_x, IA.last_y);
					// Si te estas moviendo arriba y esta la casilla disponible...
					if (IA.next_move == MOVE_UP) {
						if (IA.up) {
							IA.thread = IA_DO_CONTINUE;
						} else {
							IA.thread = IA_DO_TRY_NEXT;
						}
					}
					// Si te estas moviendo abajo y esta la casilla disponible...
					if (IA.next_move == MOVE_DOWN) {
						if (IA.down) {
							IA.thread = IA_DO_CONTINUE;
						} else {
							IA.thread = IA_DO_TRY_NEXT;
						}
					}
					// Si te estas moviendo a la izquierda y esta la casilla disponible...
					if (IA.next_move == MOVE_LEFT) {
						if (IA.left) {
							IA.thread = IA_DO_CONTINUE;
						} else {
							IA.thread = IA_DO_TRY_NEXT;
						}
					}
					// Si te estas moviendo arriba y esta la casilla disponible...
					if (IA.next_move == MOVE_RIGHT) {
						if (IA.right) {
							IA.thread = IA_DO_CONTINUE;
						} else {
							IA.thread = IA_DO_TRY_NEXT;
						}
					}
				}

			}

		} else {	// Pero si has fallado...

			// Y no llevavas ningun acierto
			if (IA.hits == 0) {

				// Continua en random
				IA.thread = IA_DO_RANDOM;

			} else {	// Si habias acertado antes y ahora has fallado...

				// Decide el siguiente movimiento a partir de la referencia del primer impacto
				IaDecideNextMove();

			}

		}

	}



	// Funciona IaScanForShips();
	bool IaScanForShips(void) {

		// Variables locales
		u8 x = 0;
		u8 y = 0;
		u8 ship = 0;
		u8 visible = 0;
		u8 grid_x = 0;
		u8 grid_y = 0;
		bool found = false;

		// Scanea el tablero
		for (y = 0; y < 8; y ++) {
			for (x = 0; x < 8; x ++) {

				// Si la casilla contiene un navio descubierto que no esta hundido
				visible = TMPGRID[PLAYER_1][x][y];
				ship = GRID[PLAYER_1][x][y];
				if (
					// Si el navio no esta hundido
					((ship >= 0) && (ship <= 5) && (SHIP[PLAYER_1][ship].live > 0))
					&&
					// Y es visible
					(visible == 0)
					) {
						// Registralo y sal
						grid_x = x;
						grid_y = y;
						x = 8;
						y = 8;
						found = true;
				}

			}
		}

		// Manda los valores a la IA
		IA.hits = 0;
		IA.first_x = grid_x;
		IA.first_y = grid_y;

		// Y devuelve si has encontrado alguno
		return found;

	}



	// Funcion IaDecideNextMove();
	void IaDecideNextMove(void) {

		// Variables locales
		bool repeat = true;
		u8 random = 0;

		// Consulta las casillas a tu alrededor
		IaCheckArround(IA.first_x, IA.first_y);

		// Si hay alguna casilla libre alrededor...
		if (IA.up || IA.down || IA.left || IA.right) {

			// Reinicia el seed del random
			srand(time(NULL));

			// Ahora busca una casilla libre alrededor
			while (repeat) {

				random = ((rand() % 4) + 1);	// Calcula entre 1 y 4

				// Despues del random, verifica que esa casilla esta libre y asignala
				switch (random) {
					case MOVE_UP:
						if (IA.up) {
							IA.next_move = random;
							repeat = false;
						}
						break;
					case MOVE_DOWN:
						if (IA.down) {
							IA.next_move = random;
							repeat = false;
						}
						break;
					case MOVE_LEFT:
						if (IA.left) {
							IA.next_move = random;
							repeat = false;
						}
						break;
					case MOVE_RIGHT:
						if (IA.right) {
							IA.next_move = random;
							repeat = false;
						}
						break;
					}
				}

				// Ahora asigna el Thread correspondiente, dispara a la casilla contigua escogida
				IA.thread = IA_DO_TRY_NEXT;

			} else {	// Si no queda ninguna, algo pasa, ejecuta el random de nuevo

				IA.hits = 0;
				IA.thread = IA_DO_RANDOM;

			}

	}





#ifdef __cplusplus
}
#endif