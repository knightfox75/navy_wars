#ifdef __cplusplus
extern "C" {
#endif





	// Navy Wars DS - Funciones comunes
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

	// Includes propietarios NDS
	#include <nds.h>

	// Includes NightFox's Lib
	#include <nf_lib.h>

	// Includes del juego
	#include "nw_common.h"
	#include "nw_system.h"





	// Variables Globales del juego //

	// Control del fondo, pantalla inferior
	s16 SCREEN_BG_X[2];		// Coordenada X
	s16 SCREEN_BG_Y[2];		// Coordenada Y

	// Control de la animacion del agua
	u8 WATER_SPEED[2];	// [pantalla]
	u8 WATER_FRAME[2][9][9];

	// Control de posiciones
	u8 GRID[2][8][8];		// [player][x][y]
	u8 BKGRID[2][8][8];		// [player][x][y]
	u8 TMPGRID [2][8][8];	// [player][x][y]

	// Barra de seleccion de los menus
	s8 MENU_SELECT;
	s16 MENU_ORIGIN;
	s16 MENU_DESTINATION;
	u16 MENU_SPEED;
	u8 MENU_LAST;
	s8 MENU_DIRECTION;
	bool MENU_MOVE;

	// Control de navios
	ship_info SHIP[2][6];
	shipspr_info SHIP_SPRITE[2][16];

	// Animacion de los navios (comun a todos y en ambas pantallas
	u8 SHIP_ANIM_FRAME;		// Frame de la animacion
	u8 SHIP_ANIM_DELAY;		// Retardo de la animacion
	s8 SHIP_ANIM_NEXT;		// Direccion de la animacion

	// Animacion del Humo
	u8 SMOKE_ANIM_FRAME;	// Frame de la animacion
	u8 SMOKE_ANIM_DELAY;	// Retardo en cambiar de frame

	// Animacion del borrado de las cuadriculas
	bool SQUARE_DELETE;		// Flag de animacion
	u8 SQUARE_FRAME;		// Frame de la animacion
	u8 SQUARE_DELAY;		// Retardo en la animacion
	u8 SQUARE_X;			// Coordenada X
	u8 SQUARE_Y;			// Coordenada Y

	// Animacion del marcado de las casillas CPU/Player 2
	bool MARK_FLAG;			// Flag de animacion
	u8 MARK_FRAME;			// Frame de la animacion
	u8 MARK_DELAY;			// Retardo en la animacion
	u8 MARK_X;				// Coordenada X
	u8 MARK_Y;				// Coordenada Y

	// Movimiento Autoscroll de la CPU en la pantalla superior
	bool TOPBG_FLAG;		// Flag de movimiento
	s16 TOPBG_X;			// Coordenada X de destino
	s16 TOPBG_Y;			// Coordenada Y de destino

	// Animacion de las explosiones
	explosion_info EXPLOSION[2];		// [Screen]
	explosion_info FAILURE[2];			// [Screen]

	// Datos de la IA
	u8 IA_GRID[2][64];		// [real/respaldo] [casilla]
	u8 GRID_REST[2];		// Cuantas casillas quedan por destapar ? [Player]
	ia_info IA;				// Parametros de la IA

	// Control de daños, unidades restantes
	u8 UNITS_LEFT[2];	// Unidades restantes [player]

	// Escape - Abortar partida
	bool GAME_ABORTED;

	// Caja de texto [pantalla]
	textbox_info TEXTBOX[2];





	// Funcion MoveScreen();
	void MoveScreen(u8 screen) {

		// Calcula el movimiento del fondo
		if ((KEYHELD.right || KEYHELD.a) && (!KEYHELD.left && !KEYHELD.y)) {
			SCREEN_BG_X[screen] += 2;
			if (SCREEN_BG_X[screen] > MAP_RIGHT) SCREEN_BG_X[screen] = MAP_RIGHT;
		}
		if ((KEYHELD.left || KEYHELD.y) && (!KEYHELD.right && !KEYHELD.a)) {
			SCREEN_BG_X[screen] -= 2;
			if (SCREEN_BG_X[screen] < MAP_LEFT) SCREEN_BG_X[screen] = MAP_LEFT;
		}
		if ((KEYHELD.up || KEYHELD.x) && (!KEYHELD.down && !KEYHELD.b)) {
			SCREEN_BG_Y[screen] -=2;
			if (SCREEN_BG_Y[screen] < MAP_TOP) SCREEN_BG_Y[screen] = MAP_TOP;
		}
		if ((KEYHELD.down || KEYHELD.b) && (!KEYHELD.up && !KEYHELD.x)) {
			SCREEN_BG_Y[screen] += 2;
			if (SCREEN_BG_Y[screen] > MAP_BOTTOM) SCREEN_BG_Y[screen] = MAP_BOTTOM;
		}

	}




	// Funcion FillWithTiles();
	void FillWithTiles(u8 screen, u8 layer, u16 tile_x, u16 tile_y, u16 size_x, u16 size_y, u16 tile) {

		// Variables
		u16 x = 0;
		u16 y = 0;
		u16 max_x = (tile_x + size_x);
		u16 max_y = (tile_y + size_y);

		// Cambia los tiles solicitados
		for (y = tile_y; y < max_y; y ++) {
			for (x = tile_x; x < max_x; x ++) {
				NF_SetTileOfMap(screen, layer, x, y, tile);
			}
		}

	}



	// Funcion GetGridSquareX();
	s8 GetGridX(void) {

		if (
			(TOUCHPAD.pos_x >= 8)
			&&
			(TOUCHPAD.pos_x <= 247)
			&&
			(TOUCHPAD.pos_y >= 8)
			&&
			(TOUCHPAD.pos_y <= 183)
			) {

			// Variables locales
			s16 x = 0;

			// Calcula el diferencal entre la posicion del fondo y el touchscreen
			x = TOUCHPAD.pos_x + SCREEN_BG_X[1];

			// Segun el resultado
			if ((x >= GRID_LEFT) && (x <= GRID_RIGHT)) {	// Si esta en rango, devuelve el numero de casilla
				return ((x - GRID_LEFT) >> 5);		// (X / 32)
			} else {						
				return -1;					// Si esta fuera de rango, devuelve -1
			}

		} else {

			// Si estas fuera de rango
			return -1;

		}

	}



	// Funcion GetGridSquareY();
	s8 GetGridY(void) {

		if (
			(TOUCHPAD.pos_x >= 8)
			&&
			(TOUCHPAD.pos_x <= 247)
			&&
			(TOUCHPAD.pos_y >= 8)
			&&
			(TOUCHPAD.pos_y <= 183)
			) {

			// Variables locales
			s16 y = 0;

			// Calcula el diferencal entre la posicion del fondo y el touchscreen
			y = TOUCHPAD.pos_y + SCREEN_BG_Y[1];

			// Segun el resultado
			if ((y >= GRID_TOP) && (y <= GRID_BOTTOM)) {	// Si esta en rango, devuelve el numero de casilla
				return ((y - GRID_TOP) >> 5);		// (Y / 32)
			} else {						
				return -1;					// Si esta fuera de rango, devuelve -1
			}

		} else {

			// Si estas fuera de rango
			return -1;

		}

	}



	// Funcion AnimateWater();
	void AnimateWater(u8 screen, u8 layer, u8 delay) {

		// Calcula el movimiento
		WATER_SPEED[screen] ++;

		// Si debes cambiar de frame...
		if (WATER_SPEED[screen] >= delay) {

			// Reset del contador de espera
			WATER_SPEED[screen] = 0;

			// Variables temporales
			u8 x = 0;
			u8 y = 0;
			u8 tile_x = 0;
			u8 tile_y = 0;
			u8 start_x = 0;
			u8 end_x = 0;
			u8 start_y = 0;
			u8 end_y = 0;
			u8 n = 0;

			// Cambia los tiles necesarios para realizar la animacion
			for (y = 0; y < 9; y ++) {
				for (x = 0; x < 9; x ++) {
					// Calcula el area a rellenar en este ciclo
					start_x = (x << 2);
					end_x = start_x + 4;
					start_y = (y << 2);
					end_y = start_y +4;
					// Y el frame a ejecutar
					WATER_FRAME[screen][x][y] ++;
					if (WATER_FRAME[screen][x][y] > WATER_FRAMES) WATER_FRAME[screen][x][y] = 0;
					if (WATER_FRAME[screen][x][y] <= 10) {
						// Desplazamiento
						n = 0;
						// Rellena la cuadricula
						for (tile_y = start_y; tile_y < end_y; tile_y ++) {
							for (tile_x = start_x; tile_x < end_x; tile_x ++) {
								NF_SetTileOfMap(screen, layer, tile_x, tile_y, ((WATER_FRAME[screen][x][y] << 4) + n));
								n ++;
							}
						}
					}
				}
			}

			// Actualiza el mapa en la VRAM
			NF_UpdateVramMap(screen, layer);
				
		}

	}



	// Funcion UpdateScreenGrid();
	void UpdateScreenGrid(u8 player, u8 screen, u8 layer) {

		// Variables locales
		u8 x = 0;
		u8 y = 0;
		bool update = false;
		u8 tile = 0;

		// Actualiza la parrilla
		for (y = 0; y < 8; y ++) {
			for (x = 0; x < 8; x ++) {
				// Verifica si se tiene que realizar algun cambio
				if (GRID[player][x][y] != BKGRID[player][x][y]) {
					// Valor por defecto
					tile = SOLID;
					// Y modificalo si es necesario
					if (GRID[player][x][y] == BLUE) tile = BLUE;
					if (GRID[player][x][y] == GREEN) tile = GREEN;
					if (GRID[player][x][y] == RED) tile = RED;
					if ((GRID[player][x][y] >= 10) && (GRID[player][x][y] <= 15)) tile = BLUE;
					if (GRID[player][x][y] == 255) tile = SOLID;
					// Cambia los tiles
					FillWithTiles(screen, layer, ((x << 2) + 2), ((y << 2) + 2), 4, 4, tile);
					// Actualiza el backup del array
					BKGRID[player][x][y] = GRID[player][x][y];
					// Flag de actualizacion
					update = true;
				}
			}
		}

		// Actualiza el mapa en VRAM
		if (update) NF_UpdateVramMap(screen, layer);

	}



	// Funcion GridBackup();
	void GridBackup(u8 player) {
		
		// Variables locales
		u8 x = 0;
		u8 y = 0;

		// Copia el array GRID al BKGRID
		for (y = 0; y < 8; y ++) {
			for (x = 0; x < 8; x ++) {
				BKGRID[player][x][y] = GRID[player][x][y];
			}
		}

	}



	// Funcion GridTmp();
	void GridTmp(u8 player, bool tmp) {
		
		// Variables locales
		u8 x = 0;
		u8 y = 0;

		// Copia el array GRID al BKGRID
		for (y = 0; y < 8; y ++) {
			for (x = 0; x < 8; x ++) {
				if (tmp) {
					TMPGRID[player][x][y] = GRID[player][x][y];
				} else {
					GRID[player][x][y] = TMPGRID[player][x][y];
				}
			}
		}

	}



	// Funcion CreateTextBox();
	void CreateTextBox(u8 screen, u8 layer, u16 start_x, u16 start_y, u16 end_x, u16 end_y, const char* text) {

		// Guarda los datos
		sprintf(TEXTBOX[screen].data, "%s", text);		// Datos
		TEXTBOX[screen].start_x = start_x;				// Medidas de la caja
		TEXTBOX[screen].start_y = start_y;
		TEXTBOX[screen].end_x = end_x;
		TEXTBOX[screen].end_y = end_y;
		TEXTBOX[screen].pos_x = start_x;				// Posicion inicial
		TEXTBOX[screen].pos_y = start_y;
		TEXTBOX[screen].layer = layer;					// Capa de destino
		TEXTBOX[screen].delay = 0;						// Contador de retraso
		TEXTBOX[screen].size = strlen(text);			// Tamaño del texto
		TEXTBOX[screen].character = 0;					// Caracter actual

	}



	// Funcion WriteTextBox();
	u8 WriteTextBox(u8 screen, u8 delay) {

		// Valor a devolver
		u8 status = 0;

		if (TEXTBOX[screen].character < TEXTBOX[screen].size) {		// Si no se ha escrito todo el texto...

			// Estado si aun queda por escribir
			status = 1;

			// Contador de retardo
			TEXTBOX[screen].delay ++;
			if (TEXTBOX[screen].delay >= delay) {
				// Valor si toca escribir un caracter
				status = 2;
				// Resetea el contador
				TEXTBOX[screen].delay = 0;
				// Escribe el caracter que toque
				char text[2];
				text[0] = TEXTBOX[screen].data[TEXTBOX[screen].character];
				text[1] = '\0';
				// Si es el caracter de cambio de linea...
				if (strcmp(text, "|") == 0) {
					status = 1;
					TEXTBOX[screen].character ++;
					TEXTBOX[screen].pos_x = TEXTBOX[screen].start_x;
					TEXTBOX[screen].pos_y ++;
					if (TEXTBOX[screen].pos_y > TEXTBOX[screen].end_y) {
						TEXTBOX[screen].pos_y = TEXTBOX[screen].start_y;
					}
				} else {
					// Si es un espacio en blanco, no hay sonido
					if (strcmp(text, " ") == 0) status = 1;
					// Escribe el caracter
					NF_WriteText(screen, TEXTBOX[screen].layer, TEXTBOX[screen].pos_x, TEXTBOX[screen].pos_y, text);
					// Calcula la siguiente posicion y caracter
					TEXTBOX[screen].character ++;
					TEXTBOX[screen].pos_x ++;
					if (TEXTBOX[screen].pos_x > TEXTBOX[screen].end_x) {
						TEXTBOX[screen].pos_x = TEXTBOX[screen].start_x;
						TEXTBOX[screen].pos_y ++;
						if (TEXTBOX[screen].pos_y > TEXTBOX[screen].end_y) {
							TEXTBOX[screen].pos_y = TEXTBOX[screen].start_y;
						}
					}
				}
			}

		}

		// Devuelve el estado
		return status;

	}





#ifdef __cplusplus
}
#endif
