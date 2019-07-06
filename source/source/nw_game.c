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
	#include "nw_game.h"
	#include "nw_ia.h"
	#include "nw_system.h"
	#include "nw_common.h"





	// Funcion CreateBattleField();
	void CreateBattleField(void) {

		// Borra todo el contenido
		ResetAll();

		// Variables locales
		u8 x = 0;	// Uso comun
		u8 y = 0;
		u8 n = 0;
		char name[32];

		u8 w = 0;		// Creacion de navios
		u8 gfx = 0;		
		u8 inc_x = 0;
		u8 inc_y = 0;
		u8 ply = 0;
		u8 grid_x = 0;
		u8 grid_y = 0;

		u8 tile_x = 0;	// Generacion del mar
		u8 tile_y = 0;
		u8 start_x = 0;
		u8 start_y = 0;
		u8 end_x = 0;
		u8 end_y = 0;
		u8 frame = 0;

		// Pantalla en negro
		setBrightness(3, -16);
		swiWaitForVBlank();

		// Inicializa las parrillas de backup de datos de ambos jugadores
		for (y = 0; y < 8; y ++) {
			for (x = 0; x < 8; x ++) {
				GRID[PLAYER_1][x][y] = 255;			// Posicion de los Navios
				GRID[PLAYER_2][x][y] = 255;
				BKGRID[PLAYER_1][x][y] = 255;		// Posicion de los Sprites
				BKGRID[PLAYER_2][x][y] = 255;
				TMPGRID[PLAYER_1][x][y] = 255;		// Casilla Cubierta o descubierta
				TMPGRID[PLAYER_2][x][y] = 255;
			}
		}

		// Carga los fondos
		NF_LoadTiledBg("bg/common/mainframe", "mainframe", 256, 256);	// Marco pantalla
		NF_LoadTiledBg("bg/common/grid", "grid", 512, 512);				// Efecto parrilla
		NF_LoadTilesForBg("bg/maingame/water", "water_up", 512, 512, 0, 175);		// Agua pantalla superior
		NF_LoadTilesForBg("bg/maingame/water", "water_down", 512, 512, 0, 175);		// Agua pantalla inferior
		NF_LoadTilesForBg("bg/maingame/mask", "mask_up", 512, 512, 0, 26);			// Mascara de tiles pantalla superior
		NF_LoadTilesForBg("bg/maingame/mask", "mask_down", 512, 512, 0, 18);		// Mascara de tiles pantalla inferior

		// Carga los Sprites
		for (x = 0; x < 18; x ++) {		// Gfx de los Navios
			sprintf(name, "sprite/maingame/ship%02d", x);
			NF_LoadSpriteGfx(name, (SHIP_GFX_RAM + x), 32, 32);
		}
		NF_LoadSpritePal("sprite/maingame/ships", SHIP_PAL_RAM);	// Paleta de los navios

		NF_LoadSpriteGfx("sprite/maingame/explosion", EXPLOSION_GFX_RAM, 32, 32);	// Gfx de la explosion
		NF_LoadSpritePal("sprite/maingame/explosion", EXPLOSION_PAL_RAM);	// Paleta de la explosion
		NF_LoadSpriteGfx("sprite/maingame/watershot", WATERSHOT_GFX_RAM, 32, 32);	// Gfx del disparo al agua
		NF_LoadSpritePal("sprite/maingame/watershot", WATERSHOT_PAL_RAM);	// Paleta del disparo al agua
		NF_LoadSpriteGfx("sprite/maingame/smoke", SMOKE_GFX_RAM, 32, 32);	// Gfx del humo
		NF_LoadSpritePal("sprite/maingame/smoke", SMOKE_PAL_RAM);	// Paleta del humo

		// Carga los efectos de sonido
		NF_LoadRawSound("sfx/explosion", 0, 11025, 0);	// Explosion
		NF_LoadRawSound("sfx/watershot", 1, 11025, 0);	// Watershot
		NF_LoadRawSound("sfx/dingding", 2, 11025, 0);	// Campana de final


		// Crea los fondos pantalla superior
		NF_CreateTiledBg(0, 0, "mainframe");
		NF_CreateTiledBg(0, 2, "mask_up");
		NF_CreateTiledBg(0, 3, "water_up");

		// Crea los fondos pantalla inferior
		NF_CreateTiledBg(1, 0, "mainframe");
		NF_CreateTiledBg(1, 1, "grid");
		NF_CreateTiledBg(1, 2, "mask_down");
		NF_CreateTiledBg(1, 3, "water_down");

		// Transfiere a la VRAM los graficos de los Navios, manteniendo en RAM la animacion
		for (x = 0; x < 18; x ++) {		// Gfx de los navios
			NF_VramSpriteGfx(0, (SHIP_GFX_RAM + x), (SHIP_GFX_VRAM + x), true);
			NF_VramSpriteGfx(1, (SHIP_GFX_RAM + x), (SHIP_GFX_VRAM + x), true);
		}
		// Ahora transfiere sus paletas
		NF_VramSpritePal(0, SHIP_PAL_RAM, SHIP_PAL_VRAM);	// Paleta de los navios
		NF_VramSpritePal(1, SHIP_PAL_RAM, SHIP_PAL_VRAM);

		// Transfiere a la VRAM los graficos de la explosion, manteniendo en RAM la animacion
		NF_VramSpriteGfx(0, EXPLOSION_GFX_RAM, EXPLOSION_GFX_VRAM, true);
		NF_VramSpriteGfx(1, EXPLOSION_GFX_RAM, EXPLOSION_GFX_VRAM, true);
		// Ahora transfiere sus paletas
		NF_VramSpritePal(0, EXPLOSION_PAL_RAM, EXPLOSION_PAL_VRAM);
		NF_VramSpritePal(1, EXPLOSION_PAL_RAM, EXPLOSION_PAL_VRAM);

		// Transfiere a la VRAM los graficos del disparo al agua, manteniendo en RAM la animacion
		NF_VramSpriteGfx(0, WATERSHOT_GFX_RAM, WATERSHOT_GFX_VRAM, true);
		NF_VramSpriteGfx(1, WATERSHOT_GFX_RAM, WATERSHOT_GFX_VRAM, true);
		// Ahora transfiere sus paletas
		NF_VramSpritePal(0, WATERSHOT_PAL_RAM, WATERSHOT_PAL_VRAM);
		NF_VramSpritePal(1, WATERSHOT_PAL_RAM, WATERSHOT_PAL_VRAM);

		// Transfiere a la VRAM los graficos del humo, manteniendo en RAM la animacion
		NF_VramSpriteGfx(0, SMOKE_GFX_RAM, SMOKE_GFX_VRAM, true);
		NF_VramSpriteGfx(1, SMOKE_GFX_RAM, SMOKE_GFX_VRAM, true);
		// Ahora transfiere sus paletas
		NF_VramSpritePal(0, SMOKE_PAL_RAM, SMOKE_PAL_VRAM);
		NF_VramSpritePal(1, SMOKE_PAL_RAM, SMOKE_PAL_VRAM);

		// General el mapa para la Pantalla superior, capa 2
		for (y = 0; y < 36; y ++) {
			for (x = 0; x < 36; x ++) {
				NF_SetTileOfMap(0, 2, x, y, NONE);
			}
		}
		NF_UpdateVramMap(0, 2);

		// General el mapa para la Pantalla inferior, capa 2
		for (y = 0; y < 36; y ++) {
			for (x = 0; x < 36; x ++) {
				NF_SetTileOfMap(1, 2, x, y, SOLID);
			}
		}
		NF_UpdateVramMap(1, 2);

		// Inicializa la animacion del agua
		srand(time(NULL));
		for (y = 0; y < 9; y ++) {
			for (x = 0; x < 9; x ++) {
				WATER_FRAME[0][x][y] = (rand() % (WATER_FRAMES + 1));
				WATER_FRAME[1][x][y] = (rand() % (WATER_FRAMES + 1));
			}
		}
		// Genera el mapa de el fondo marino en ambas pantalla
		for (y = 0; y < 9; y ++) {
			for (x = 0; x < 9; x ++) {
				// Calcula el area a rellenar
				start_x = (x << 2);
				end_x = start_x + 4;
				start_y = (y << 2);
				end_y = start_y +4;
				// Desplazamiento
				n = 0;
				// Rellena la cuadricula
				for (tile_y = start_y; tile_y < end_y; tile_y ++) {
					for (tile_x = start_x; tile_x < end_x; tile_x ++) {
						frame = WATER_FRAME[0][x][y];
						if (frame > 10) frame = 10;
						NF_SetTileOfMap(0, 3, tile_x, tile_y, ((frame << 4) + n));
						frame = WATER_FRAME[1][x][y];
						if (frame > 10) frame = 10;
						NF_SetTileOfMap(1, 3, tile_x, tile_y, ((frame << 4) + n));
						n ++;
					}
				}				
			}
		}
		// Actualiza los mapas en VRAM
		NF_UpdateVramMap(0, 3);
		NF_UpdateVramMap(1, 3);

		// Calcula la posicion de los navios
		for (ply = 0; ply < 2; ply ++) {
			for (n = 0; n < 16; n ++) {
				// Inicializa los contadores para cambiar de navio
				if (n == 0) {	// Navios de 4 cuadriculas
					w = 0;
					x = (SHIP[ply][w].x << 5) + GRID_LEFT;
					y = (SHIP[ply][w].y << 5) + GRID_TOP;
					if (SHIP[ply][w].vertical) {
						gfx = 5;
						inc_x = 0;
						inc_y = 32;
					} else {
						gfx = 14;
						inc_x = 32;
						inc_y = 0;
					}
				}
				if ((n == 4) || (n == 7)) {	// Navios de 3 cuadriculas
					w ++;
					x = (SHIP[ply][w].x << 5) + GRID_LEFT;
					y = (SHIP[ply][w].y << 5) + GRID_TOP;
					if (SHIP[ply][w].vertical) {
						gfx = 2;
						inc_x = 0;
						inc_y = 32;
					} else {
						gfx = 11;
						inc_x = 32;
						inc_y = 0;
					}
				}
				if ((n == 10) || (n == 12) || (n == 14)) {	// Navios de 2 cuadriculas
					w ++;
					x = (SHIP[ply][w].x << 5) + GRID_LEFT;
					y = (SHIP[ply][w].y << 5) + GRID_TOP;
					if (SHIP[ply][w].vertical) {
						gfx = 0;
						inc_x = 0;
						inc_y = 32;
					} else {
						gfx = 9;
						inc_x = 32;
						inc_y = 0;
					}
				}
				// Asigna los parametros a las variables del Sprite
				SHIP_SPRITE[ply][n].x = x;
				SHIP_SPRITE[ply][n].y = y;
				SHIP_SPRITE[ply][n].gfx = gfx;
				// Recalcula la casilla donde estara este sprite
				grid_x = ((x - GRID_LEFT) >> 5);
				grid_y = ((y - GRID_TOP) >> 5);
				// Almacena que navio ocupa esa casilla
				GRID[ply][grid_x][grid_y] = w;
				// Y ahora almacena tambien que Sprite ocupa esa casilla
				BKGRID[ply][grid_x][grid_y] = n;
				// Calcula los parametros para el siguiente sprite
				x += inc_x;
				y += inc_y;
				gfx ++;
			}
		}


		// Crea los Navios en sus posiciones
		for (n = 0; n < 16; n ++) {
			NF_CreateSprite(0, (SHIP_SPR + n), (SHIP_GFX_VRAM + SHIP_SPRITE[0][n].gfx), SHIP_PAL_VRAM, SHIP_SPRITE[0][n].x, SHIP_SPRITE[0][n].y);
			NF_SpriteLayer(0, (SHIP_SPR + n), 1);
			NF_CreateSprite(1, (SHIP_SPR + n), (SHIP_GFX_VRAM + SHIP_SPRITE[1][n].gfx), SHIP_PAL_VRAM, SHIP_SPRITE[1][n].x, SHIP_SPRITE[1][n].y);
			NF_SpriteLayer(1, (SHIP_SPR + n), 3);
		}
		// Crea los sprites de humo en sus posiciones y ocultala
		for (n = 0; n < 16; n ++) {
			NF_CreateSprite(0, (SMOKE_SPR + n), SMOKE_GFX_VRAM, SMOKE_PAL_VRAM, (SHIP_SPRITE[0][n].x + 8), (SHIP_SPRITE[0][n].y - 8));
			NF_SpriteLayer(0, (SMOKE_SPR + n), 1);
			NF_ShowSprite(0, (SMOKE_SPR + n), false);
			NF_CreateSprite(1, (SMOKE_SPR + n), SMOKE_GFX_VRAM, SMOKE_PAL_VRAM, (SHIP_SPRITE[1][n].x + 8), (SHIP_SPRITE[1][n].y - 8));
			NF_SpriteLayer(1, (SMOKE_SPR + n), 3);
			NF_ShowSprite(1, (SMOKE_SPR + n), false);
		}
		// Crea y oculta el sprite de la explosion
		NF_CreateSprite(0, EXPLOSION_SPR, EXPLOSION_GFX_VRAM, EXPLOSION_PAL_VRAM, -32, -32);
		NF_SpriteLayer(0, EXPLOSION_SPR, 1);
		NF_ShowSprite(0, EXPLOSION_SPR, false);
		NF_CreateSprite(1, EXPLOSION_SPR, EXPLOSION_GFX_VRAM, EXPLOSION_PAL_VRAM, -32, -32);
		NF_SpriteLayer(1, EXPLOSION_SPR, 3);
		NF_ShowSprite(1, EXPLOSION_SPR, false);
		// Crea y oculta el sprite del disparo al agua
		NF_CreateSprite(0, WATERSHOT_SPR, WATERSHOT_GFX_VRAM, WATERSHOT_PAL_VRAM, -32, -32);
		NF_SpriteLayer(0, WATERSHOT_SPR, 1);
		NF_ShowSprite(0, WATERSHOT_SPR, false);
		NF_CreateSprite(1, WATERSHOT_SPR, WATERSHOT_GFX_VRAM, WATERSHOT_PAL_VRAM, -32, -32);
		NF_SpriteLayer(1, WATERSHOT_SPR, 3);
		NF_ShowSprite(1, WATERSHOT_SPR, false);

		// Posiciona los navios en pantalla
		MoveShips();

		// Habilita el canal Alpha entre los fondos 2 y 3 (Mainscreen)
		REG_BLDCNT = BLEND_ALPHA | BLEND_SRC_BG2 | BLEND_DST_BG3;
		// Nivel de Alpha (Mainscreen)
		REG_BLDALPHA = 0x05 | (0x0F << 8);

		// Habilita el canal Alpha entre los fondos 1 y 2/3 (Subscreen)
		REG_BLDCNT_SUB = BLEND_ALPHA | BLEND_SRC_BG1 | (BLEND_DST_BG2 + BLEND_DST_BG3 + BLEND_DST_SPRITE);
		// Nivel de Alpha (Subscreen)
		REG_BLDALPHA_SUB = 0x05 | (0x0F << 8);

		// Reinicia las posiciones de todos los scrolls
		SCREEN_BG_X[0] = 0;
		SCREEN_BG_Y[0] = 0;
		SCREEN_BG_X[1] = 0;
		SCREEN_BG_Y[1] = 0;


		// Inicializa las variables de control de animacion
		WATER_SPEED[0] = WATER_DELAY;		// Animacion del Mar
		WATER_SPEED[1] = WATER_DELAY;
		AnimateWater(0, 3, WATER_DELAY);
		AnimateWater(1, 3, WATER_DELAY);

		SHIP_ANIM_FRAME = 0;				// Animacion de los navios
		SHIP_ANIM_DELAY = 0;
		SHIP_ANIM_NEXT = 1;
		AnimateShips();

		SMOKE_ANIM_FRAME = 0;		// Animacion del humo
		SMOKE_ANIM_DELAY = 0;


		// Inicializa los FLAGS necesarios
		SQUARE_DELETE = false;		// Borrado de casillas (pantalla inferior)
		MARK_FLAG = false;			// Marcado de casillas (pantalla superior)
		TOPBG_FLAG = false;			// Autoscroll de la pantalla superior

		// Inicialila las animaciones de explosiones
		EXPLOSION[0].flag = false;
		EXPLOSION[1].flag = false;
		FAILURE[0].flag = false;
		FAILURE[1].flag = false;

		// Inicializa la IA
		InitCpuIa();

		// Inicializa las unidades restantes para cada jugador
		UNITS_LEFT[PLAYER_1] = 16;
		UNITS_LEFT[PLAYER_2] = 16;

		// Total de casillas restantes
		GRID_REST[PLAYER_1] = 64;
		GRID_REST[PLAYER_2] = 64;

		// Actualiza los OAM's
		NF_SpriteOamSet(0);
		NF_SpriteOamSet(1);
		swiWaitForVBlank();
		oamUpdate(&oamMain);
		oamUpdate(&oamSub);

		// Espera al sincronismo
		swiWaitForVBlank();

		// Efecto Fade-In
		s16 brillo = (16 << 8);
		while (brillo > 0) {
			brillo -= 64;
			// Si ya has subido el brillo
			if (brillo <= 0) {
				brillo = 0;
			}
			// Animaciones
			AnimateWater(0, 3, WATER_DELAY);
			AnimateWater(1, 3, WATER_DELAY);
			AnimateShips();
			// Aplica el valor del brillo 
			setBrightness(3, (0 - (brillo >> 8)));
			// Actualiza los OAM's
			NF_SpriteOamSet(0);
			NF_SpriteOamSet(1);
			swiWaitForVBlank();
			oamUpdate(&oamMain);
			oamUpdate(&oamSub);
		}

	}



	// Funcion PlayBattle();
	void PlayBattle(void) {

		// Variables locales
		bool loop = true;
		u16 n = 0;
		u16 brillo = 0;
		bool out = false;

		// Crea el campo de batalla
		CreateBattleField();

		// Inicializa variables
		GAME_ABORTED = false;

		// Juego
		while (loop) {
			// Mueve el jugador nº1
			Player1Move();
			// Si no se ha abortado la partida
			if (!GAME_ABORTED) {
				// Mueve la CPU (Si le quedan unidades disponibles)
				if (UNITS_LEFT[PLAYER_2] > 0) CpuMove();
				// Verifica si hay fin de juego
				if ((UNITS_LEFT[PLAYER_1] == 0) || (UNITS_LEFT[PLAYER_2] == 0)) loop = false;
			}
			// Verifica si el juego se ha abortado
			if (GAME_ABORTED) loop = false;
		}

		// Reproduce el sonido de final de batalla
		NF_PlayRawSound(2, 127, 64, false, 0);

		// Espera 2 segundos despues del ultimo disparo y sal
		loop = true;
		while (loop) {
			// Contador de tiempo (disparo a los 2 segundo)
			n ++;
			if (n > 120) {
				n = 120;
				if (!out && loop) out = true;
			}
			// Anima el agua
			AnimateWater(0, 3, WATER_DELAY);
			AnimateWater(1, 3, WATER_DELAY);
			// Anima los navios
			AnimateShips();
			// Animacion del humo
			AnimateSmoke();
			// Actualiza el Array del OAM
			NF_SpriteOamSet(0);
			NF_SpriteOamSet(1);
			// Efecto "Fade Out" del menu
			if (out) {
				brillo += 64;
				// Si ya has subido el brillo
				if (brillo >= (16 << 8)) {
					brillo = (16 << 8);
					out = false;
					loop = false;
				}
				// Aplica el valor del brillo 
				setBrightness(3, (0 - (brillo >> 8)));
			}
			// Espera al sincronismo vertical
			swiWaitForVBlank();
			// Actualiza la memoria del OAM
			oamUpdate(&oamMain);
			oamUpdate(&oamSub);
		}

	}



	// Funcion Player1Move();
	void Player1Move(void) {

		// Variables locales
		s8 grid_x = -1;	// nº de casilla X
		s8 grid_y = -1;	// nº de casilla Y

		bool is_touch = false;		// Control del TOUCHPAD

		bool loop = true;

		bool sfx_explosion = true;	// Flags de efectos de sonido
		bool sfx_watershot = true;

		u8 n = 0;
		u8 z = 0;
		
		while (loop) {

			// Lee el teclado y touchpad
			ReadKeypad();
			ReadTouchpad();

			// Control scroll de fondos
			if (KEYHELD.r || KEYHELD.l) {
				MoveScreen(0);
			} else {
				MoveScreen(1);
			}

			// Si pulsas "Select", aborta la partida
			if (KEYPRESS.select) {
				GAME_ABORTED = true;
				loop = false;
			}

			// Registra si se realiza una nueva pulsacion sobre el touchpad
			if (!is_touch && TOUCHPAD.held && !SQUARE_DELETE && !EXPLOSION[1].flag && !FAILURE[1].flag) {

				is_touch = true;			// Flag de plsacion arriba
				grid_x = GetGridX();		// Guarda sobre que casilla has pulsado	
				grid_y = GetGridY();

				// Ahora verifica si tienes que borrar una casilla
				if ((grid_x != -1) && (grid_y != -1)) {		// Si has tocado una casilla valida
					// Y la casilla aun no ha sido destapada
					if (TMPGRID[PLAYER_2][grid_x][grid_y] == 255) {
						// Activa el borrado de cuadriculas
						SQUARE_DELETE = true;
						SQUARE_FRAME = 0;
						SQUARE_DELAY = 0;
						SQUARE_X = grid_x;
						SQUARE_Y = grid_y;
						// Marca la casilla como descubierta
						TMPGRID[PLAYER_2][grid_x][grid_y] = 0;
						// Si hay un navio debajo, explosion
						if (BKGRID[PLAYER_2][grid_x][grid_y] != 255) {
							EXPLOSION[1].flag = true;
							EXPLOSION[1].x = SHIP_SPRITE[PLAYER_2][BKGRID[PLAYER_2][grid_x][grid_y]].x;
							EXPLOSION[1].y = SHIP_SPRITE[PLAYER_2][BKGRID[PLAYER_2][grid_x][grid_y]].y;
							// Quitale energia al navio dañado
							z = GRID[PLAYER_2][grid_x][grid_y];
							SHIP[PLAYER_2][z].live --;
							// Quita una unidad al Jugador 2 / CPU
							UNITS_LEFT[PLAYER_2] --;
						} else {	// Si no hay navio, agua
							FAILURE[1].flag = true;
							FAILURE[1].x = (grid_x << 5) + GRID_LEFT;
							FAILURE[1].y = (grid_y << 5) + GRID_TOP;
						}
					}
				}

			}

			// Si levantas el lapiz, flag abajo
			if (is_touch && !TOUCHPAD.held && !SQUARE_DELETE && !EXPLOSION[1].flag && !FAILURE[1].flag) {
				is_touch = false;
				grid_x = -1;
				grid_y = -1;
			}

			// Animacion explosion y devuelve el flag al terminarla
			if (!SQUARE_DELETE && EXPLOSION[1].flag) loop = AnimateExplosion(1);

			// Sonido de la explosion
			if (sfx_explosion && !SQUARE_DELETE && EXPLOSION[1].flag) {
				NF_PlayRawSound(0, 127, 64, 0, 0);
				sfx_explosion = false;
			}

			// Animacion del disparo al agua y devuelve el flag al terminarla
			if (!SQUARE_DELETE && FAILURE[1].flag) loop = AnimateWatershot(1);

			// Sonido del disparo al agua
			if (sfx_watershot && !SQUARE_DELETE && FAILURE[1].flag) {
				NF_PlayRawSound(1, 127, 64, 0, 0);
				sfx_watershot = false;
			}

			// Animacion borrado cuadricula
			if (SQUARE_DELETE) DeleteSquare();

			// Si vas a salir del loop, muestra humo donde toca (si toca)
			if (!loop && (BKGRID[PLAYER_2][grid_x][grid_y] != 255)) NF_ShowSprite(1, (SMOKE_SPR + BKGRID[PLAYER_2][grid_x][grid_y]), true);

			// Anima el agua
			AnimateWater(0, 3, WATER_DELAY);
			AnimateWater(1, 3, WATER_DELAY);

			// Posiciona los navios en pantalla y animalos
			HideShips();
			MoveShips();
			AnimateShips();

			// Animacion del humo
			AnimateSmoke();

			// Actualiza el Array del OAM
			NF_SpriteOamSet(0);
			NF_SpriteOamSet(1);

			// Espera al sincronismo vertical
			swiWaitForVBlank();

			// Actualiza la memoria del OAM
			oamUpdate(&oamMain);
			oamUpdate(&oamSub);

			// Actualiza los Scrolls
			for (n = 0; n < 2; n ++) {
				NF_ScrollBg(n, 1, SCREEN_BG_X[n], SCREEN_BG_Y[n]);
				NF_ScrollBg(n, 2, SCREEN_BG_X[n], SCREEN_BG_Y[n]);
				NF_ScrollBg(n, 3, SCREEN_BG_X[n], SCREEN_BG_Y[n]);
			}

		}

		// Al salir del LOOP, resta una casilla
		GRID_REST[PLAYER_2] --;

	}



	// Funcion CpuMove();
	void CpuMove(void) {

		// Variables locales
		s8 grid_x = -1;	// nº de casilla X
		s8 grid_y = -1;	// nº de casilla Y

		bool loop = true;		// Flag del loop
		bool shot = true;		// Flag del disparo
		bool smoke = true;		// Flag de muestreo de humo

		bool sfx_explosion = true;	// Flags de efectos de sonido
		bool sfx_watershot = true;

		u8 index = 0;		// Indice seleccionado para el disparo

		u8 n = 0;
		u8 z = 0;
		
		while (loop) {

			// Lee el teclado y touchpad
			ReadKeypad();
			ReadTouchpad();

			// Mueve la pantalla inferior
			MoveScreen(1);

			// Si pulsas "Select", aborta la partida
			if (KEYPRESS.select) {
				GAME_ABORTED = true;
				loop = false;
			}

			// Registra si se tiene que realizar un nuevo disparo
			if (shot && !EXPLOSION[0].flag && !FAILURE[0].flag) {

				shot = false;					// Flag de disparo abajo

				// Obten una casilla libre mediante la IA
				index = CpuIa();

				// Calcula que casilla es
				grid_y = (IA_GRID[0][index] >> 3);				
				grid_x = (IA_GRID[0][index] - (grid_y << 3));
				// Ahora copia el Array de la IA a su backup
				for (n = 0; n < GRID_REST[PLAYER_1]; n ++) {
					IA_GRID[1][n] = IA_GRID[0][n];
				}
				// Hecho esto, restauralo, pero omitiendo el indice seleccionado, con esto dejamos solo en 
				// el array los indices no usados
				z = 0;
				for (n = 0; n < GRID_REST[PLAYER_1]; n ++) {
					// Si no es el indice seleccionado, restauralo
					if (n != index) {
						IA_GRID[0][z] = IA_GRID[1][n];
						z ++;
					}
				}
				// Marca la casilla como descubierta
				TMPGRID[PLAYER_1][grid_x][grid_y] = 0;
				// Si hay un navio debajo, explosion
				if (BKGRID[PLAYER_1][grid_x][grid_y] != 255) {
					EXPLOSION[0].flag = true;
					EXPLOSION[0].x = SHIP_SPRITE[PLAYER_1][BKGRID[PLAYER_1][grid_x][grid_y]].x;
					EXPLOSION[0].y = SHIP_SPRITE[PLAYER_1][BKGRID[PLAYER_1][grid_x][grid_y]].y;
					// Calcula si tiene que desplazarse el fondo
					TOPBG_X = ((EXPLOSION[0].x - 128) + 16);
					TOPBG_Y = ((EXPLOSION[0].y - 96) + 16);
					// Quitale una unidad al Jugador 1
					UNITS_LEFT[PLAYER_1]--;
					// Quitale energia al navio dañado
					z = GRID[PLAYER_1][grid_x][grid_y];
					SHIP[PLAYER_1][z].live --;
					// Es el primer disparo acertado?
					if (IA.hits == 0) {
						// Guarda las coordenadas del acierto
						IA.first_x = grid_x;
						IA.first_y = grid_y;
					}
					// Ahora, selecciona el siguiente paso de la IA
					IaNextAction(true);
				} else {	// Si no hay navio, agua
					FAILURE[0].flag = true;
					FAILURE[0].x = (grid_x << 5) + GRID_LEFT;
					FAILURE[0].y = (grid_y << 5) + GRID_TOP;
					// Calcula si tiene que desplazarse el fondo
					TOPBG_X = ((FAILURE[0].x - 128) + 16);
					TOPBG_Y = ((FAILURE[0].y - 96) + 16);
					// Ahora, selecciona el siguiente paso de la IA
					IaNextAction(false);
				}
				// Guarda que casilla tienes que marcar
				MARK_X = grid_x;
				MARK_Y = grid_y;
				// Calcula la posicion donde tiene que desplazarse el fondo
				if (TOPBG_X < MAP_LEFT) TOPBG_X = MAP_LEFT;
				if (TOPBG_X > MAP_RIGHT) TOPBG_X = MAP_RIGHT;
				if (TOPBG_Y < MAP_TOP) TOPBG_Y = MAP_TOP;
				if (TOPBG_Y > MAP_BOTTOM) TOPBG_Y = MAP_BOTTOM;
				// Y activa el flag
				TOPBG_FLAG = true;

			}

			// Si es necesario, mueve el fondo
			if (TOPBG_FLAG) TopAutoscroll();

			// Marca la casilla que se ha seleccionado
			if (MARK_FLAG) loop = MarkSquare();

			// Muestra el humo de la seccion del barco dañada (si procede) tras la explosion
			if (smoke && MARK_FLAG && (BKGRID[PLAYER_1][grid_x][grid_y] != 255)) {
				NF_ShowSprite(0, (SMOKE_SPR + BKGRID[PLAYER_1][grid_x][grid_y]), true);
				smoke = false;
			}

			// Animacion explosion y devuelve el flag al terminarla
			if (EXPLOSION[0].flag && !TOPBG_FLAG) MARK_FLAG = !AnimateExplosion(0);

			// Sonido de la explosion
			if (sfx_explosion && EXPLOSION[0].flag && !TOPBG_FLAG) {
				NF_PlayRawSound(0, 127, 64, 0, 0);
				sfx_explosion = false;
			}

			// Animacion del disparo al agua y devuelve el flag al terminarla
			if (FAILURE[0].flag && !TOPBG_FLAG) MARK_FLAG = !AnimateWatershot(0);

			// Sonido del disparo al agura
			if (sfx_watershot && FAILURE[0].flag && !TOPBG_FLAG) {
				NF_PlayRawSound(1, 127, 64, 0, 0);
				sfx_watershot = false;
			}

			// Anima el agua
			AnimateWater(0, 3, WATER_DELAY);
			AnimateWater(1, 3, WATER_DELAY);

			// Posiciona los navios en pantalla y animalos
			MoveShips();
			AnimateShips();

			// Animacion del humo
			AnimateSmoke();

			// Actualiza el Array del OAM
			NF_SpriteOamSet(0);
			NF_SpriteOamSet(1);

			// Espera al sincronismo vertical
			swiWaitForVBlank();

			// Actualiza la memoria del OAM
			oamUpdate(&oamMain);
			oamUpdate(&oamSub);

			// Actualiza los Scrolls
			for (n = 0; n < 2; n ++) {
				NF_ScrollBg(n, 1, SCREEN_BG_X[n], SCREEN_BG_Y[n]);
				NF_ScrollBg(n, 2, SCREEN_BG_X[n], SCREEN_BG_Y[n]);
				NF_ScrollBg(n, 3, SCREEN_BG_X[n], SCREEN_BG_Y[n]);
			}

		}

		// Al salir del LOOP, resta una casilla
		GRID_REST[PLAYER_1] --;

	}



	// Funcion MoveShips();
	void MoveShips(void) {

		// Variables locales
		s16 x = 0;
		s16 y = 0;
		u8 n = 0;
		u8 ply = 0;

		for (ply = 0; ply < 2; ply ++) {	// Jugadores
			for (n = 0; n < 16; n ++) {
				// Calcula la X en pantalla
				x = SHIP_SPRITE[ply][n].x - SCREEN_BG_X[ply];
				if (x < -32) x = -32;
				if (x > 256) x = 256;
				// Calcula la Y en pantalla
				y = SHIP_SPRITE[ply][n].y - SCREEN_BG_Y[ply];
				if (y < -32) y = -32;
				if (y > 192) y = 192;
				// Posiciona el Sprite
				NF_MoveSprite(ply, (SHIP_SPR + n), x, y);	// Sprite del navio
				NF_MoveSprite(ply, (SMOKE_SPR + n), (x + 8), (y - 8));	// Sprite del humo
			}
		}

	}



	// Funcion AnimateShips();
	void AnimateShips(void) {
		// Variables locales
		u8 n = 0;
		// Retardo en la animacion
		SHIP_ANIM_DELAY ++;
		// Si toca, anima
		if (SHIP_ANIM_DELAY > 15) {
			SHIP_ANIM_DELAY = 0;
			SHIP_ANIM_FRAME += SHIP_ANIM_NEXT;
			if (SHIP_ANIM_FRAME == 0 || SHIP_ANIM_FRAME == 2) SHIP_ANIM_NEXT *= -1;
			for (n = 0; n < 16; n ++) {
				NF_SpriteFrame(0, (SHIP_SPR + n), SHIP_ANIM_FRAME);
				NF_SpriteFrame(1, (SHIP_SPR + n), SHIP_ANIM_FRAME);
			}
		}
	}



	// Funcion DeleteSquare();
	void DeleteSquare(void) {
		SQUARE_DELAY ++;
		if (SQUARE_DELAY > SQUARE_ANIM_DELAY) {
			SQUARE_DELAY = 0;
			FillWithTiles(1, 2, ((SQUARE_X << 2) + 2), ((SQUARE_Y << 2) + 2), 4, 4, (SQUARE_FRAME + 10));
			NF_UpdateVramMap(1, 2);
			SQUARE_FRAME ++;
			if (SQUARE_FRAME > 9) SQUARE_DELETE = false;
		}
	}



	// Funcion MarkMap
	bool MarkSquare(void) {
		// Variables locales
		bool loop = true;
		// Aumenta el contador de retraso
		MARK_DELAY ++;
		if (MARK_DELAY > MARK_ANIM_DELAY) {
			MARK_DELAY = 0;
			// Rellena el cuadrado con el tile de la animacion
			FillWithTiles(0, 2, ((MARK_X << 2) + 2), ((MARK_Y << 2) + 2), 4, 4, (MARK_FRAME + 20));
			NF_UpdateVramMap(0, 2);
			// Siguiente frame
			MARK_FRAME ++;
			// Si se ha terminado la animacion...
			if (MARK_FRAME > 6) {
				MARK_FLAG = false;
				MARK_FRAME = 0;
				// Marca para salir
				loop = false;
			}
		}
		// Devuelve el resultado
		return loop;
	}



	// Funcion TopAutoscroll();
	void TopAutoscroll(void) {
		if (TOPBG_X > SCREEN_BG_X[0]) {
			if ((TOPBG_X - SCREEN_BG_X[0]) < TOPBG_SPEED) {
				SCREEN_BG_X[0] = TOPBG_X;
			} else {
				SCREEN_BG_X[0] += TOPBG_SPEED;
			}
		}
		if (TOPBG_X < SCREEN_BG_X[0]) {
			if ((SCREEN_BG_X[0] - TOPBG_X) < TOPBG_SPEED) {
				SCREEN_BG_X[0] = TOPBG_X;
			} else {
				SCREEN_BG_X[0] -= TOPBG_SPEED;
			}
		}
		if (TOPBG_Y > SCREEN_BG_Y[0]) {
			if ((TOPBG_Y - SCREEN_BG_Y[0]) < TOPBG_SPEED) {
				SCREEN_BG_Y[0] = TOPBG_Y;
			} else {
				SCREEN_BG_Y[0] += TOPBG_SPEED;
			}
		}
		if (TOPBG_Y < SCREEN_BG_Y[0]) {
			if ((SCREEN_BG_Y[0] - TOPBG_Y) < TOPBG_SPEED) {
				SCREEN_BG_Y[0] = TOPBG_Y;
			} else {
				SCREEN_BG_Y[0] -= TOPBG_SPEED;
			}
		}
		// Si se igualan las posiciones, flag de movimiento abajo
		if ((TOPBG_X == SCREEN_BG_X[0]) && (TOPBG_Y == SCREEN_BG_Y[0])) TOPBG_FLAG = false;
	}



	// Funcion AnimateExplosion();
	bool AnimateExplosion(u8 screen) {

		// Variables locales
		s16 x = 0;
		s16 y = 0;
		bool loop = true;

		// Calcula la X en pantalla
		x = EXPLOSION[screen].x - SCREEN_BG_X[screen];
		if (x < -32) x = -32;
		if (x > 256) x = 256;
		// Calcula la Y en pantalla
		y = EXPLOSION[screen].y - SCREEN_BG_Y[screen];
		if (y < -32) y = -32;
		if (y > 192) y = 192;
		// Posiciona el Sprite
		NF_MoveSprite(screen, EXPLOSION_SPR, x, y);

		EXPLOSION[screen].delay ++;
		if (EXPLOSION[screen].delay > EXPLOSION_DELAY) {
			// Resetea el contador
			EXPLOSION[screen].delay = 0;
			// Si es el primer frame, haz visible el sprite
			if (EXPLOSION[screen].frame == 0) NF_ShowSprite(screen, EXPLOSION_SPR, true);
			// Si esta en rango, animalo
			if (EXPLOSION[screen].frame <= 9) {
				NF_SpriteFrame(screen, EXPLOSION_SPR, EXPLOSION[screen].frame);
			}
			// Siguiente frame
			EXPLOSION[screen].frame ++;
			// Si ya los has pasado todos, oculta el sprite y resetea el contador
			if (EXPLOSION[screen].frame > 10) {
				EXPLOSION[screen].frame = 0;
				EXPLOSION[screen].flag = false;
				NF_ShowSprite(screen, EXPLOSION_SPR, false);
				// Informa que ha terminado la animacion
				loop = false;
			}
		}

		// Devuelve si la animacion aun esta activa
		return loop;

	}



	// Funcion AnimateWatershot();
	bool AnimateWatershot(u8 screen) {

		// Variables locales
		s16 x = 0;
		s16 y = 0;
		bool loop = true;

		// Calcula la X en pantalla
		x = FAILURE[screen].x - SCREEN_BG_X[screen];
		if (x < -32) x = -32;
		if (x > 256) x = 256;
		// Calcula la Y en pantalla
		y = FAILURE[screen].y - SCREEN_BG_Y[screen];
		if (y < -32) y = -32;
		if (y > 192) y = 192;
		// Posiciona el Sprite
		NF_MoveSprite(screen, WATERSHOT_SPR, x, y);

		FAILURE[screen].delay ++;
		if (FAILURE[screen].delay > WATERSHOT_DELAY) {
			// Resetea el contador
			FAILURE[screen].delay = 0;
			// Si es el primer frame, haz visible el sprite
			if (FAILURE[screen].frame == 0) NF_ShowSprite(screen, WATERSHOT_SPR, true);
			// Si esta en rango, animalo
			if (FAILURE[screen].frame <= 14) {
				NF_SpriteFrame(screen, WATERSHOT_SPR, FAILURE[screen].frame);
			}
			// Siguiente frame
			FAILURE[screen].frame ++;
			// Si ya los has pasado todos, oculta el sprite y resetea el contador
			if (FAILURE[screen].frame > 10) {
				FAILURE[screen].frame = 0;
				FAILURE[screen].flag = false;
				NF_ShowSprite(screen, WATERSHOT_SPR, false);
				// Informa que ha terminado la animacion
				loop = false;
			}
		}

		// Devuelve si la animacion aun esta activa
		return loop;

	}



	// Funcion HideShips();
	void HideShips(void) {

		// Variables locales
		u8 w = 0;
		u8 n = 0;

		// Bucle de todos los Sprites
		for (n = 0; n < 16; n ++) {

			// Si cambias de navio, indicalo
			if ((n == 4) || (n == 7)) w ++;
			if ((n == 10) || (n == 12) || (n == 14)) w ++;

			// Si el navio esta hundido, muestra el navio
			if (SHIP[PLAYER_2][w].live == 0) {

				NF_ShowSprite(1, (SHIP_SPR + n), true);

			} else {

				NF_ShowSprite(1, (SHIP_SPR + n), false);

			}

		}


	}



	// Funcionm AnimateSmoke();
	void AnimateSmoke(void) {

		// Contador de retardo en la animacion
		SMOKE_ANIM_DELAY ++;

		// Si tienes que cambiar de frame...
		if (SMOKE_ANIM_DELAY > SMOKE_DELAY) {

			// Resetea el contador
			SMOKE_ANIM_DELAY = 0;

			// Cambia el frame de animacion
			SMOKE_ANIM_FRAME ++;

			// Si has llegado al ultimo frame, vuelta a empezar
			if (SMOKE_ANIM_FRAME > 8) SMOKE_ANIM_FRAME = 0;

			// Y cambia el frame a todos los sprites asociados
			// Como todos los sprites comparten el Gfx y solo hace falta cambiarlo en uno de ellos
			NF_SpriteFrame(0, SMOKE_SPR, SMOKE_ANIM_FRAME);
			NF_SpriteFrame(1, SMOKE_SPR, SMOKE_ANIM_FRAME);

		}

	}





#ifdef __cplusplus
}
#endif
