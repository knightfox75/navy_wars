#ifdef __cplusplus
extern "C" {
#endif





	// Navy Wars DS - Funciones de Game Over
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
	#include "nw_gameover.h"
	#include "nw_system.h"
	#include "nw_common.h"





	// Funcion GameOver();
	void GameOver(void) {

		// Borra todo el contenido
		ResetAll();

		// Variables locales
		bool loop = true;
		u8 n = 0;
		u8 ships = 0;

		u8 x = 0;					// Variables del control de efectos
		u8 y = 0;
		u8 tile[32][24];
		s8 speed[32][24];

		s16 brillo = (16 << 8);		// Nivel del brillo
		bool in = true;				// Control Fade In
		bool out = false;			// Control Fade Out

		// Pantalla en negro
		setBrightness(3, -16);
		swiWaitForVBlank();

		// Inicializa las variables
		srand(time(NULL));
		for (y = 0; y < 24; y ++) {
			for (x = 0; x < 32; x ++) {
				tile[x][y] = (rand() % 20);
				n = (rand() % 100);
				if (n > 50) {
					speed[x][y] = 1;
				} else {
					speed[x][y] = -1;
				}
			}
		}

		// Carga los fondos
		NF_LoadTiledBg("bg/gameover/scr_up", "scr_up", 256, 256);
		NF_LoadTiledBg("bg/gameover/scr_down", "scr_down", 256, 256);
		NF_LoadTiledBg("bg/gameover/bottom", "bottom", 256, 256);
		NF_LoadTiledBg("bg/gameover/won", "won", 256, 256);
		NF_LoadTiledBg("bg/gameover/lose", "lose", 256, 256);

		// Carga el tileset
		NF_LoadTilesForBg("bg/gameover/colors", "colors", 256, 256, 0, 19);

		// Carga los Sprites
		NF_LoadSpriteGfx("sprite/gameover/numbers", 0, 32, 32);
		NF_LoadSpritePal("sprite/gameover/numbers", 0);

		// Transfiere los Sprites a la VRAM
		NF_VramSpriteGfx(0, 0, 0, true);
		NF_VramSpritePal(0, 0, 0);

		// Pon los fondos en la pantalla
		NF_CreateTiledBg(0, 3, "colors");
		NF_CreateTiledBg(0, 2, "scr_up");
		NF_CreateTiledBg(1, 3, "colors");
		NF_CreateTiledBg(1, 2, "scr_down");
		NF_CreateTiledBg(1, 1, "bottom");

		// Pon el sprite del numero en pantalla
		NF_CreateSprite(0, 0, 0, 0, 40, 121);
		NF_SpriteLayer(0, 0, 1);

		// Muestra el texto segun gane el Jugador 1 o el 2
		if ((UNITS_LEFT[PLAYER_1] > 0) && !GAME_ABORTED) {
			// Jugador 1 Gana
			for (n = 0; n < 6; n ++) {
				if (SHIP[PLAYER_1][n].live > 0) ships ++;
			}
			// Carga su fondo
			NF_CreateTiledBg(0, 1, "won");
		} else {
			// Jugador 1 Pierde
			for (n = 0; n < 6; n ++) {
				if (SHIP[PLAYER_2][n].live > 0) ships ++;
			}
			// Carga su fondo
			NF_CreateTiledBg(0, 1, "lose");
		}

		// Selecciona el numero a mostrar
		NF_SpriteFrame(0, 0, ships);

		// Habilita el canal Alpha entre los fondos 2 y 3 (Mainscreen)
		REG_BLDCNT = BLEND_ALPHA | BLEND_SRC_BG2 | BLEND_DST_BG3;
		// Nivel de Alpha (Mainscreen)
		REG_BLDALPHA = 0x0F | (0x03 << 8);

		// Habilita el canal Alpha entre los fondos 2 y 3 (Subscreen)
		REG_BLDCNT_SUB = BLEND_ALPHA | BLEND_SRC_BG2 | BLEND_DST_BG3;
		// Nivel de Alpha (Subscreen)
		REG_BLDALPHA_SUB = 0x0F | (0x03 << 8);

		// Prepara el Efecto
		for (y = 0; y < 24; y ++) {
			for (x = 0; x < 32; x ++) {
				NF_SetTileOfMap(0, 3, x, y, tile[x][y]);
				NF_SetTileOfMap(1, 3, x, y, tile[x][y]);
			}
		}
		// Actualiza los mapas en VRAM
		NF_UpdateVramMap(0, 3);
		NF_UpdateVramMap(1, 3);

		// Actualiza los OAM's
		NF_SpriteOamSet(0);
		swiWaitForVBlank();
		oamUpdate(&oamMain);


		// Espera a que se pulse una tecla
		while (loop) {

			// Lee el teclado y touchpad
			ReadKeypad();
			ReadTouchpad();

			// Si se pulsa A, sal
			if (KEYPRESS.a && !in && !out) out = true;

			// Si se toca la pantalla, sal
			if (TOUCHPAD.held && !in && !out) out = true;

			// Efecto
			for (y = 0; y < 24; y ++) {
				for (x = 0; x < 32; x ++) {
					tile[x][y] += speed[x][y];
					if ((tile[x][y] == 0) || (tile[x][y] == 19)) speed[x][y] *= -1;
					NF_SetTileOfMap(0, 3, x, y, tile[x][y]);
					NF_SetTileOfMap(1, 3, x, y, tile[x][y]);
				}
			}
			// Actualiza los mapas en VRAM
			NF_UpdateVramMap(0, 3);
			NF_UpdateVramMap(1, 3);

			// Efecto "Fade In" del menu
			if (in) {
				brillo -= 64;
				// Si ya has subido el brillo
				if (brillo <= 0) {
					brillo = 0;
					in = false;
				}
				// Aplica el valor del brillo 
				setBrightness(3, (0 - (brillo >> 8)));
			}


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

		}

	}





#ifdef __cplusplus
}
#endif
