#ifdef __cplusplus
extern "C" {
#endif





	// Navy Wars DS - Funciones de Navy Setup
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
	#include "nw_setup.h"
	#include "nw_system.h"
	#include "nw_common.h"





	// Funcion CreateNavySetup();
	void CreateNavySetup(void) {

		// Borra todo el contenido
		ResetAll();

		// Variables locales
		u8 x = 0;
		u8 y = 0;
		u8 z = 0;

		// Pantalla en negro
		setBrightness(3, -16);
		swiWaitForVBlank();

		// Carga los fondos
		NF_LoadTiledBg("bg/common/mainframe", "mainframe", 256, 256);	// Marco pantalla
		NF_LoadTiledBg("bg/setup/deploy", "deploy", 256, 256);			// Deploy your army
		NF_LoadTiledBg("bg/setup/selector", "selector", 256, 512);		// Item selector
		NF_LoadTiledBg("bg/common/grid", "grid", 512, 512);				// Efecto parrilla
		NF_LoadTiledBg("bg/setup/submask", "submask", 512, 512);		// mascara inferior

		// Carga los sprites
		NF_LoadSpriteGfx("sprite/setup/navyrest", 0, 16, 16);	// Contador de unidades
		NF_LoadSpritePal("sprite/setup/navyrest", 0);
		NF_LoadSpriteGfx("sprite/setup/is_ok", 10, 64, 32);		// Menu "This is Ok?" Sprite 10+
		NF_LoadSpriteGfx("sprite/setup/done", 11, 64, 32);
		NF_LoadSpriteGfx("sprite/setup/redo", 12, 64, 32);
		NF_LoadSpritePal("sprite/setup/is_ok", 1);			// Paleta 1


		// Crea los fondos pantalla superior
		NF_CreateTiledBg(0, 0, "mainframe");
		NF_CreateTiledBg(0, 1, "selector");
		NF_CreateTiledBg(0, 2, "deploy");

		// Crea los fondos pantalla inferior
		NF_CreateTiledBg(1,0, "mainframe");
		NF_CreateTiledBg(1,1, "grid");
		NF_CreateTiledBg(1,2, "submask");

		// Transfiere los Sprites a la VRAM (6 copias del mismo Gfx a VRAM (Gfx. 0 a los Slots 0 - 5))
		for (x = 0; x < 6; x ++) {
			NF_VramSpriteGfx(0, 0, x, false);
		}
		// Transfiere ahora la paleta (Id. 0 al Slot nº 0)
		NF_VramSpritePal(0, 0, 0);

		// Crea los Sprites del marcador
		NF_CreateSprite(0, 0, 0, 0, 192, 88);
		NF_CreateSprite(0, 1, 1, 0, 176, 120);
		NF_CreateSprite(0, 2, 2, 0, 200, 120);
		NF_CreateSprite(0, 3, 3, 0, 160, 152);
		NF_CreateSprite(0, 4, 4, 0, 184, 152);
		NF_CreateSprite(0, 5, 5, 0, 208, 152);

		// Prioridad y frame de los Sprites
		for (x = 0; x < 6; x ++) {
			NF_SpriteFrame(0, x, 0);
			NF_SpriteLayer(0, x, 2);
		}

		// Transfiere a VRAM los datos para los Sprites "This is Ok?" (VRAM SLOT 0+)
		NF_VramSpriteGfx(1, 10, 0, false);
		NF_VramSpriteGfx(1, 11, 1, false);
		NF_VramSpriteGfx(1, 12, 2, false);
		NF_VramSpritePal(1, 1, 0);		// Slot 0 pantalla inferior

		// Crea los sprites del cartel "This is ok?"
		NF_CreateSprite(1, 0, 0, 0, 64, 40);	// "This is ok?" izquierda
		NF_SpriteLayer(1, 0, 0);
		NF_SpriteFrame(1, 0, 0);
		NF_ShowSprite(1, 0, false);
		NF_CreateSprite(1, 1, 0, 0, 128, 40);	// "This is ok? derecha
		NF_SpriteLayer(1, 1, 0);
		NF_SpriteFrame(1, 1, 1);
		NF_ShowSprite(1, 1, false);
		NF_CreateSprite(1, 2, 1, 0, 96, 88);	// "Done"
		NF_SpriteLayer(1, 2, 0);
		NF_SpriteFrame(1, 2, 1);
		NF_ShowSprite(1, 2, false);
		NF_CreateSprite(1, 3, 2, 0, 96, 120);	// "Redo"
		NF_SpriteLayer(1, 3, 0);
		NF_SpriteFrame(1, 3, 0);
		NF_ShowSprite(1, 3, false);
		
		// Habilita el canal Alpha entre los fondos 1 y 2 (Mainscreen)
		REG_BLDCNT = BLEND_ALPHA | BLEND_SRC_BG1 | (BLEND_DST_BG2 + BLEND_DST_SPRITE);
		// Nivel de Alpha (Mainscreen)
		REG_BLDALPHA = 0x05 | (0x0F << 8);


		// Habilita el canal Alpha entre los fondos 1 y 2 (Subscreen)
		REG_BLDCNT_SUB = BLEND_ALPHA | BLEND_SRC_BG1 | (BLEND_DST_BG2 + BLEND_DST_BG3);
		// Nivel de Alpha (Subscreen)
		REG_BLDALPHA_SUB = 0x0A | (0x0F << 8);

		// Actualiza los OAM's
		NF_SpriteOamSet(0);
		NF_SpriteOamSet(1);
		swiWaitForVBlank();
		oamUpdate(&oamMain);
		oamUpdate(&oamSub);

		// Inicializa las parrillas de datos de ambos jugadores
		for (y = 0; y < 8; y ++) {
			for (x = 0; x < 8; x ++) {
				GRID[PLAYER_1][x][y] = EMPTY;
				GRID[PLAYER_2][x][y] = EMPTY;
				BKGRID[PLAYER_1][x][y] = EMPTY;
				BKGRID[PLAYER_2][x][y] = EMPTY;
				TMPGRID[PLAYER_1][x][y] = EMPTY;
				TMPGRID[PLAYER_2][x][y] = EMPTY;
			}
		}

		// Inicializa las variables de los navios
		for (x = 0; x < 2; x ++) {
			for (y = 0; y < 6; y ++) {
				SHIP[x][y].in_position = false;
				SHIP[x][y].vertical = false;
				if (y == 0) z = 4;
				if ((y >= 1) && (y <= 2)) z = 3;
				if (y >= 3) z = 2;
				SHIP[x][y].live = z;
				SHIP[x][y].size = z;
				SHIP[x][y].x = -1;
				SHIP[x][y].y = -1;
			}
		}

		// Reinicia las posiciones de todos los scrolls
		SCREEN_BG_X[0] = 0;
		SCREEN_BG_Y[0] = 0;
		SCREEN_BG_X[1] = 0;
		SCREEN_BG_Y[1] = 0;

		// Inicializa la barra de seleccion
		NF_ScrollBg(0, 1, 0, 256);
		MENU_SELECT = 0;
		MENU_SPEED = 4;
		MENU_LAST = 2;
		MENU_MOVE = false;

		// Muestralo
		FadeIn(64);

	}



	// Funcion NavySetup
	void NavySetup(u8 player) {

		// Crea els escenario
		CreateNavySetup();

		// Variables locales
		s8 grid_x = -1;	// nº de casilla X
		s8 grid_y = -1;	// nº de casilla Y
		s8 pos_x = -1;	// Referencia casilla X
		s8 pos_y = -1;	// Referencia casilla Y

		bool is_touch = false;		// Control del TOUCHPAD

		s8 my_boat = -1;			// Direccion a posicionar el barco
		s8 current_boat = -1;		// Barco actual
		bool all_placed = false;	// Todos colocados?

		bool loop = true;			// Control del bucle principal

		u8 n = 0;	// Uso comun


		// Posiciona la flota
		while (loop) {

			// Actualiza el teclado y TOUCHPAD
			ReadKeypad();
			ReadTouchpad();

			// Scroll de la pantalla inferior
			MoveScreen(1);

			// Menu de seleccion de Navio (Si no estas colocando ya el barco)
			OptionsMenu(!is_touch);

			// Consulta un barco disponible si no se esta moviendo el menu
			current_boat = -1;		// Ninguno seleccionado
			if (!MENU_MOVE) {
				for (n = 0; n < 6; n ++) {
					// Opcion 1: 4 Casillas
					if ((MENU_SELECT == 0) && (n == 0) && (!SHIP[PLAYER_1][n].in_position)) current_boat = n;
					// Opcion 2: 3 Casillas
					if ((MENU_SELECT == 1) && (n >= 1) && (n <= 2) && (!SHIP[PLAYER_1][n].in_position)) current_boat = n;
					// Opcion 3: 2 Casillas
					if ((MENU_SELECT == 2) && (n >= 3) && (!SHIP[PLAYER_1][n].in_position)) current_boat = n;
				}
			}

			// Registra si se realiza una nueva pulsacion sobre el touchpad (Si hay navios disponibles)
			if (!is_touch && TOUCHPAD.held && (current_boat != -1)) {
				is_touch = true;				// Flag de plsacion arriba
				pos_x = GetGridX();				// Guarda sobre que casilla has pulsado	
				pos_y = GetGridY();
				my_boat = -1;					// Flag de navio a -1
				GridTmp(PLAYER_1, true);		// Crea un backup temporal del estado actual
			}

			// Si el lapiz esta tocando la pantalla y se ha pulsado sobre una casilla...
			if (is_touch && TOUCHPAD.held && (pos_x != -1) && (pos_y != -1)) {
				// Lee la posicion del lapiz
				grid_x = GetGridX();
				grid_y = GetGridY();
				// Resetea el flag de navio
				my_boat = -1;
				// Si has movido el lapiz a la derecha...
				if ((grid_x > pos_x) && (grid_y == pos_y)) {
					my_boat = 1;
				}
				// Si has movido el lapiz a la izquierda...
				if ((grid_x < pos_x) && (grid_y == pos_y)) {
					my_boat = 2;
				}
				// Si has movido el lapiz arriba...
				if ((grid_x == pos_x) && (grid_y < pos_y)) {
					my_boat = 3;
				}
				// Si has movido el lapiz abajo...
				if ((grid_x == pos_x) && (grid_y > pos_y)) {
					my_boat = 4;
				}
			}

			// Haz una previsual del navio
			if (is_touch && TOUCHPAD.held) {
				// Si se ha realizado un movimiento de lapiz correcto
				if (my_boat != -1) {
					if (CheckBoatPlace(my_boat, pos_x, pos_y)) {
						PreviewMyBoat(my_boat, pos_x, pos_y, GREEN);
					} else {
						PreviewMyBoat(my_boat, pos_x, pos_y, RED);
					}
				}
				// Actualiza la parrilla en pantalla
				UpdateScreenGrid(0, 1, 2);
				// Restaura el GRID de la copia temporal
				GridTmp(PLAYER_1, false);
			}

			// Registra si se levanta el touchpad y estavas presionandolo
			if (is_touch && !TOUCHPAD.held) {
				is_touch = false;	// Flag abajo
				// Vamos a ver si el navio se puede colocar...
				if (
					(my_boat != -1)		// Si se ha realizado un movimiento de lapiz correcto
					&&
					(CheckBoatPlace(my_boat, pos_x, pos_y))	// Y hay espacio en la parrilla
					) {
						// Coloca el navio
						PlaceMyBoat(current_boat, my_boat, pos_x, pos_y);
						// Actualiza la parrilla en pantalla
						UpdateScreenGrid(0, 1, 2);
						// Actualiza el array de Cheksum
						GridBackup(PLAYER_1);
				} else {
					// Si no has podido colocar el navio, restaura la parrilla
					GridTmp(PLAYER_1, false);
					// Actualiza la parrilla en pantalla
					UpdateScreenGrid(0, 1, 2);
				}
			}


			// Actualizacion de los marcadores
			all_placed = true;	// Temporalmente, pon el flag de "todos colocados" arriba
			for (n = 0; n < 6; n ++) {
				if (SHIP[PLAYER_1][n].in_position) {
					NF_SpriteFrame(0, n, 1);
				} else {
					all_placed = false;	// Aun no se han colocado todos, flag abajo.
					NF_SpriteFrame(0, n, 0);
				}
			}

			// Actualiza el Array del OAM
			NF_SpriteOamSet(0);

			// Espera al sincronismo vertical
			swiWaitForVBlank();

			// Actualiza la memoria del OAM
			oamUpdate(&oamMain);

			// Actualiza los Scrolls
			NF_ScrollBg(1, 1, SCREEN_BG_X[1], SCREEN_BG_Y[1]);
			NF_ScrollBg(1, 2, SCREEN_BG_X[1], SCREEN_BG_Y[1]);

			// Al finalizar el ciclo, verifica si debes de salir
			if (all_placed) {
				if (ExitNavySetup()) loop = false;
			}

		}

		// Si sales de la funcion, FadeOut
		FadeOut(64);

	}



	// Funcion CheckBoatPlace();
	bool CheckBoatPlace(s8 boat, s8 pos_x, s8 pos_y) {

		// Variables locales
		u8 n = 0;
		s8 start = 0;
		s8 end = 0;
		u8 size = 0;
		bool vertical = false;
		bool can_place = true;

		// Obten el tamaño del barco a posicionar
		size = 4 - MENU_SELECT;

		// Obten el inicio y el final segun la orientacion del barco
		switch (boat) {
			case 1:		// A la derecha
				start = pos_x;
				end = (pos_x + (size - 1));
				break;
			case 2:		// A la izquierda
				start = (pos_x - (size - 1));
				end = pos_x;
				break;
			case 3:		// Arriba
				start = (pos_y - (size - 1));
				end = pos_y;
				vertical = true;
				break;
			case 4:		// Abajo
				start = pos_y;
				end = (pos_y + (size - 1));
				vertical = true;
				break;
			default:	// Todo lo demas es invalido
				can_place = false;
				break;
		}

		// Verifica si se puede posicionar el barco en la parrilla
		if ((start >= 0) && (end <= 7)) {
			for (n = start; n <= end; n ++) {
				if (vertical) {
					if (GRID[PLAYER_1][pos_x][n] != EMPTY) can_place = false;
				} else {
					if (GRID[PLAYER_1][n][pos_y] != EMPTY) can_place = false;
				}
			}
		} else {
			// Si esta fuera de rango, no se puede colocar
			can_place = false;
		}

		// Devuelve el resultado
		return can_place;

	}


	// Funcion PreviewMyBoat();
	void PreviewMyBoat(s8 boat, s8 pos_x, s8 pos_y, u8 color) {

		// Variables locales
		u8 n = 0;
		s8 start = 0;
		s8 end = 0;
		u8 size = 0;
		bool vertical = false;
		u8 x = 0;
		u8 y = 0;

		// Obten el tamaño del barco a posicionar
		size = 4 - MENU_SELECT;

		// Obten el inicio y el final segun la orientacion del barco
		switch (boat) {
			case 1:		// A la derecha
				start = pos_x;
				end = (pos_x + (size - 1));
				x = start;
				y = pos_y;
				break;
			case 2:		// A la izquierda
				start = (pos_x - (size - 1));
				end = pos_x;
				x = start;
				y = pos_y;
				break;
			case 3:		// Arriba
				start = (pos_y - (size - 1));
				end = pos_y;
				vertical = true;
				x = pos_x;
				y = start;
				break;
			case 4:		// Abajo
				start = pos_y;
				end = (pos_y + (size - 1));
				vertical = true;
				x = pos_x;
				y = start;
				break;
			default:
				break;
		}

		// Posiciona el barco en la parrilla
		if ((start >= 0) && (end <= 7)) {
			for (n = start; n <= end; n ++) {
				if (vertical) {
					GRID[PLAYER_1][pos_x][n] = color;
				} else {
					GRID[PLAYER_1][n][pos_y] = color;
				}
			}
		} else {
			if (start < 0) start = 0;
			if (end > 7) end = 7;
			for (n = start; n <= end; n ++) {
				if (vertical) {
					GRID[PLAYER_1][pos_x][n] = color;
				} else {
					GRID[PLAYER_1][n][pos_y] = color;
				}
			}
		}

	}



	// Funcion PlaceMyBoat();
	void PlaceMyBoat(u8 current_boat, s8 boat, s8 pos_x, s8 pos_y) {

		// Variables locales
		u8 n = 0;
		s8 start = 0;
		s8 end = 0;
		u8 size = 0;
		bool vertical = false;
		u8 x = 0;
		u8 y = 0;

		// Obten el tamaño del barco a posicionar
		size = 4 - MENU_SELECT;

		// Obten el inicio y el final segun la orientacion del barco
		switch (boat) {
			case 1:		// A la derecha
				start = pos_x;
				end = (pos_x + (size - 1));
				x = start;
				y = pos_y;
				break;
			case 2:		// A la izquierda
				start = (pos_x - (size - 1));
				end = pos_x;
				x = start;
				y = pos_y;
				break;
			case 3:		// Arriba
				start = (pos_y - (size - 1));
				end = pos_y;
				vertical = true;
				x = pos_x;
				y = start;
				break;
			case 4:		// Abajo
				start = pos_y;
				end = (pos_y + (size - 1));
				vertical = true;
				x = pos_x;
				y = start;
				break;
			default:
				break;
		}

		// Posiciona el barco en la parrilla
		if ((start >= 0) && (end <= 7)) {
			for (n = start; n <= end; n ++) {
				if (vertical) {
					GRID[PLAYER_1][pos_x][n] = (current_boat + 10);
				} else {
					GRID[PLAYER_1][n][pos_y] = (current_boat + 10);
				}
			}
		} else {
			if (start < 0) start = 0;
			if (end > 7) end = 7;
			for (n = start; n <= end; n ++) {
				if (vertical) {
					GRID[PLAYER_1][pos_x][n] = (current_boat + 10);
				} else {
					GRID[PLAYER_1][n][pos_y] = (current_boat + 10);
				}
			}
		}

		// Marca el navio como colocado
		SHIP[PLAYER_1][current_boat].in_position = true;
		// Guarda la coordenada X
		SHIP[PLAYER_1][current_boat].x = x;
		// Guarda la coordenada Y
		SHIP[PLAYER_1][current_boat].y = y;
		// Y si esta en posicion vertical
		SHIP[PLAYER_1][current_boat].vertical = vertical;

	}



	// Funcion ExitNavySetup();
	bool ExitNavySetup(void) {

		// Variables locales
		bool loop = true;
		bool option = true;
		bool is_touch = false;
		u8 n = 0;
		u8 x = 0;
		u8 y = 0;

		// Muestra los sprites
		for (n = 0; n < 4; n ++) {
			NF_ShowSprite(1, n, true);
		}
		NF_SpriteFrame(1, 2, 1);
		NF_SpriteFrame(1, 3, 0);

		// Actualiza el OAM de la pantalla inferior
		NF_SpriteOamSet(1);
		swiWaitForVBlank();
		oamUpdate(&oamSub);

		// Nucleo de la funcion
		while (loop) {

			// Actualiza el teclado y TOUCHPAD
			ReadKeypad();
			ReadTouchpad();

			// Si pulsas arriba o abajo
			if (KEYPRESS.up && !KEYPRESS.down) option = true;
			if (!KEYPRESS.up && KEYPRESS.down) option = false;

			// Si con el lapiz has tocado la pantalla...
			if (TOUCHPAD.held && !is_touch) {
				// Flag arriba
				is_touch = true;
				// Si se toca el boton "Done"
				if (
					((TOUCHPAD.pos_x >= 96) && (TOUCHPAD.pos_x <= 159))
					&&
					((TOUCHPAD.pos_y >= 95) && (TOUCHPAD.pos_y <= 111))
					) {
						option = true;
						loop = false;
				}
				// Si se toca el boton "Redo"
				if (
					((TOUCHPAD.pos_x >= 96) && (TOUCHPAD.pos_x <= 159))
					&&
					((TOUCHPAD.pos_y >= 127) && (TOUCHPAD.pos_y <= 143))
					) {
						option = false;
						loop = false;
				}
			}

			// Si se levanta el lapiz, flag abajo
			if (!TOUCHPAD.held) is_touch = false;

			// Actualiza el menu
			if (option) {	// Opcion "Done"
				NF_SpriteFrame(1, 2, 1);
				NF_SpriteFrame(1, 3, 0);
			} else {		// Opcion "Redo"
				NF_SpriteFrame(1, 2, 0);
				NF_SpriteFrame(1, 3, 1);
			}

			// Si se pulsa "A" sal
			if (KEYPRESS.a) loop = false;

			// Actualiza los OAM y espera al sincronismo
			NF_SpriteOamSet(1);
			swiWaitForVBlank();
			oamUpdate(&oamSub);

		}

		// Espera un poco (permite ver si se ha pulsado "Redo")
		WaitTime(5);

		// Ocualta los sprites
		for (n = 0; n < 4; n ++) {
			NF_ShowSprite(1, n, false);
		}

		// Actualiza el OAM de la pantalla inferior
		NF_SpriteOamSet(1);
		swiWaitForVBlank();
		oamUpdate(&oamSub);

		// Si tienes que rehacer, reinicialo todo
		if (!option) {
			// Inicializa las parrillas del jugador
			for (y = 0; y < 8; y ++) {
				for (x = 0; x < 8; x ++) {
					GRID[PLAYER_1][x][y] = EMPTY;
				}
			}
			// Actualiza la parrilla en pantalla
			UpdateScreenGrid(0, 1, 2);
			// Ahora actualiza los arrays de backup y temporal
			GridBackup(PLAYER_1);
			GridTmp(PLAYER_1, true);
			// Reinicia los valores de los navios
			for (n = 0; n < 6; n ++) {
				SHIP[PLAYER_1][n].in_position = false;
				SHIP[PLAYER_1][n].vertical = false;
				SHIP[PLAYER_1][n].x = -1;
				SHIP[PLAYER_1][n].y = -1;
			}
		}

		// Sal de la funcion y devuelve el resultado
		return option;

	}



	// Funcion ComputerSetup();
	void ComputerSetup(void) {

		// Variables locales
		u8 boat = 0;				// nº de navio a colocar
		u8 x = 0;					// Coordenada X
		u8 y = 0;					// Coordenada Y
		bool vertical = false;		// Orientacion
		u8 start = 0;				// Inicio
		u8 end = 0;					// Fin
		bool can_place = false;		// Puede colocarse el barco ?
		u8 n = 0;					// Uso general

		// Inicializa el Random (Usa el TIME para inicializarla)
		srand(time(NULL));

		// Coloca los barcos de la CPU en su parrilla
		while (boat < 6) {
			// Mientras no posiciones el navio, repite
			while (!SHIP[PLAYER_2][boat].in_position) {
				// Coordenadas
				x = (rand() % 8);
				y = (rand() % 8);
				// Orientacion
				if ((rand() % 100) > 50) {
					vertical = true;
				} else {
					vertical = false;
				}
				// Define el Inicio y el final
				if (vertical) {
					if ((y + SHIP[PLAYER_2][boat].size) > 7) {
						start = (y - (SHIP[PLAYER_2][boat].size - 1));
						end = y;
					} else {
						start = y;
						end = (y + (SHIP[PLAYER_2][boat].size - 1));
					}
					// Guarda la posicion del navio
					SHIP[PLAYER_2][boat].x = x;
					SHIP[PLAYER_2][boat].y = start;
				} else {
					if ((x + SHIP[PLAYER_2][boat].size) > 7) {
						start = (x - (SHIP[PLAYER_2][boat].size - 1));
						end = x;
					} else {
						start = x;
						end = (x + (SHIP[PLAYER_2][boat].size - 1));
					}
					// Guarda la posicion del navio
					SHIP[PLAYER_2][boat].x = start;
					SHIP[PLAYER_2][boat].y = y;
				}
				// Ahora verifica si hay espacio libre
				can_place = true;
				for (n = start; n <= end; n ++) {
					if (vertical) {
						if (GRID[PLAYER_2][x][n] != EMPTY) can_place = false;
					} else {
						if (GRID[PLAYER_2][n][y] != EMPTY) can_place = false;
					}
				}
				// Si es posible colocarlo, indicalo
				SHIP[PLAYER_2][boat].in_position = can_place;
				// Y si esta en vertical o horizontal
				SHIP[PLAYER_2][boat].vertical = vertical;
			}
			// Ahora indica en la parrilla donde estara el navio
			for (n = start; n <= end; n ++) {
				if (vertical) {
					GRID[PLAYER_2][x][n] = (boat + 10);
				} else {
					GRID[PLAYER_2][n][y] = (boat + 10);
				}
			}
			// Pasa al siguiente navio
			boat ++;
		}

	}



	// Funcion TwoPlayers();	// Debug
	void TwoPlayers(void) {

		// Borra todo el contenido
		ResetAll();

		// Pantalla en negro
		setBrightness(3, -16);
		swiWaitForVBlank();

		// Carga los fondos
		NF_LoadTiledBg("bg/twoplayers/cs_up", "up", 256, 256);	
		NF_LoadTiledBg("bg/twoplayers/cs_down", "down", 256, 256);

		// Y mandalos a la pantalla
		NF_CreateTiledBg(0, 3, "up");
		NF_CreateTiledBg(1, 3, "down");

		// Efecto Fade In
		FadeIn(64);

		// Espera a que se pulse la pantalla o "A"

		bool loop = true;

		while (loop) {

			ReadKeypad();
			ReadTouchpad();

			if (KEYHELD.a || TOUCHPAD.held) loop = false;

			swiWaitForVBlank();

		}

		// Efecto Fade out
		FadeOut(64);

	}



	// Funcion OptionsMenu();
	void OptionsMenu(bool can_change) {
		// Menu seleccion
		if (!MENU_MOVE && can_change) {
			// Si se pulsa L
			if (KEYPRESS.l && !KEYPRESS.r) {
				// Cambia e valor del menu
				MENU_SELECT --;
				if (MENU_SELECT < 0) {	// Si has llegado al tope, aborta
					MENU_SELECT = 0;
				} else {				// Si no, prepara el cambio de valor
					MENU_MOVE = true;
					MENU_DESTINATION = 256 - (MENU_SELECT << 5);
					MENU_ORIGIN = MENU_DESTINATION - 32;
					MENU_DIRECTION = 1;
				}
			}
			// Se se pulsa R
			if (KEYPRESS.r && !KEYPRESS.l) {
				// Cambia e valor del menu
				MENU_SELECT ++;
				if (MENU_SELECT > MENU_LAST) {	// Si has llegado al tope, aborta
					MENU_SELECT = MENU_LAST;
				} else {				// Si no, prepara el cambio de valor
					MENU_MOVE = true;
					MENU_DESTINATION = 256 - (MENU_SELECT << 5);
					MENU_ORIGIN = MENU_DESTINATION + 32;
					MENU_DIRECTION = -1;
				}
			}
		}

		// Animacion de la barra de menus
		if (MENU_MOVE) {
			// Si se ha pulsado L
			if (MENU_DIRECTION > 0) {
				MENU_ORIGIN += MENU_SPEED;
				// Si ya has acabado el movimiento
				if (MENU_ORIGIN >= MENU_DESTINATION) {
					MENU_ORIGIN = MENU_DESTINATION;
					MENU_MOVE = false;
				}
			} else {	// Si se ha pulsado R
				MENU_ORIGIN -= MENU_SPEED;
				// Si ya has acabado el movimiento
				if (MENU_ORIGIN <= MENU_DESTINATION) {
					MENU_ORIGIN = MENU_DESTINATION;
					MENU_MOVE = false;
				}
			}
			NF_ScrollBg(0, 1, 0, MENU_ORIGIN);
		}
	}





#ifdef __cplusplus
}
#endif
