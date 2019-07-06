#ifdef __cplusplus
extern "C" {
#endif





	// Navy Wars DS - Funciones de Creditos
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
	#include "nw_credits.h"
	#include "nw_system.h"
	#include "nw_common.h"





	// Funcion Credits();
	void Credits(void) {

		// Borra todo el contenido
		ResetAll();


		// Variables locales

		bool loop = true;	// Control de bucle

		u8 n = 0;			// Uso general
		u8 x = 0;
		u8 y = 0;

		u16 delay = 0;		// Control de animacion
		u8 tile = 0;

		u8 thread = 0;		// Hilo a Ejecutar

		s16 brillo = (16 << 8);		// Nivel del brillo

		u8 credit = 0;		// Credito a mostrar

		s16 bgy = 0;		// Coordenada Y de las interferencias

		// Pantalla en negro
		setBrightness(3, -16);
		swiWaitForVBlank();

		// Carga los fondos a la RAM
		NF_LoadTiledBg("bg/common/interferencias", "interferencias", 256, 512);
		NF_LoadTiledBg("bg/credits/attributions", "attributions", 256, 256);
		NF_LoadTiledBg("bg/credits/m_keyframe", "m_keyframe", 256, 256);
		NF_LoadTiledBg("bg/credits/m_nightfox", "m_nightfox", 256, 256);
		NF_LoadTiledBg("bg/credits/m_tonimax", "m_tonimax", 256, 256);
		NF_LoadTiledBg("bg/credits/p_keyframe", "p_keyframe", 256, 256);
		NF_LoadTiledBg("bg/credits/p_nightfox", "p_nightfox", 256, 256);
		NF_LoadTiledBg("bg/credits/p_tonimax", "p_tonimax", 256, 256);
		NF_LoadTiledBg("bg/credits/thanks", "thanks", 256, 256);

		// Ahora carga los tilesets
		NF_LoadTilesForBg("bg/credits/venecia", "mask_up", 256, 256, 0, 20);
		NF_LoadTilesForBg("bg/credits/venecia", "mask_down", 256, 256, 0, 20);

		// Crea los fondos en pantalla para el primer credito
		NF_CreateTiledBg(0, 3, "p_nightfox");
		NF_CreateTiledBg(1, 3, "m_nightfox");
		NF_CreateTiledBg(0, 2, "mask_up");
		NF_CreateTiledBg(1, 2, "mask_down");
		NF_CreateTiledBg(0, 1, "interferencias");
		NF_CreateTiledBg(1, 1, "interferencias");

		// Crea los mapas del Layer 2 con tiles transparentes (nº10)
		for (n = 0; n < 2; n ++) {
			for (y = 0; y < 32; y ++) {
				for (x = 0; x < 32; x ++) {
					NF_SetTileOfMap(n, 2, x, y, 10);
				}
			}
			NF_UpdateVramMap(n, 2);
		}

		// Crea los mapas del Layer 2 con la mascara (nº0)
		for (n = 0; n < 2; n ++) {
			for (y = 2; y < 22; y ++) {
				for (x = 2; x < 30; x ++) {
					NF_SetTileOfMap(n, 2, x, y, 0);
				}
			}
			NF_UpdateVramMap(n, 2);
		}

		// Habilita el canal Alpha entre los fondos 1 y 2, 3
		REG_BLDCNT = BLEND_ALPHA | BLEND_SRC_BG1 | (BLEND_DST_BG2 + BLEND_DST_BG3);			// Alpha del fondos 1 sobre el 2 y 3 (Main Screen)
		REG_BLDCNT_SUB = BLEND_ALPHA | BLEND_SRC_BG1 | (BLEND_DST_BG2 + BLEND_DST_BG3);		// Alpha del fondos 1 sobre el 2 y 3 (Sub Screen)
		// Nivel de Alpha
		REG_BLDALPHA = 0x03 | (0x0F << 8);		// Nivel de alpha (3 sobre 15) en Main Screen
		REG_BLDALPHA_SUB = 0x03 | (0x0F << 8);	// Nivel de alpha (3 sobre 15) en Sub Screen


		// Bucle Principal
		while (loop) {

			// Lee el teclado y touchpad
			ReadKeypad();
			ReadTouchpad();

			// Si se pulsa A, sal
			if (KEYPRESS.a && (thread != 0) && (thread != 7)) thread = 7;

			// Si se toca la pantalla, sal
			if (TOUCHPAD.held && (thread != 0) && (thread != 7)) thread = 7;


			// Ejecucion de threads
			switch (thread) {

				case 0:		// Fade In inicial
					brillo -= 64;
					// Si ya has subido el brillo
					if (brillo <= 0) {
						brillo = 0;
						delay = 0;
						thread = 1;		// Salta al siguiente thread
					}
					// Aplica el valor del brillo 
					setBrightness(3, (0 - (brillo >> 8)));
					break;

				case 1:		// Espera 0,5 segundos
					delay ++;
					if (delay > 30) {
						delay = 0;
						thread = 2;		// Salta al siguiente thread
					}
					break;

				case 2:		// Efecto grafico, mostrar el fondo
					delay ++;
					if (delay > 3) {
						delay = 0;
						tile ++;
						for (n = 0; n < 2; n ++) {
							for (y = 2; y < 22; y ++) {
								for (x = 2; x < 30; x ++) {
									NF_SetTileOfMap(n, 2, x, y, tile);
								}
							}
							NF_UpdateVramMap(n, 2);
						}
						if (tile == 10) {
							delay = 0;
							thread = 3;		// Salta al siguiente thread
						}
					}
					break;

				case 3:		// Espera 3 segundos
					delay ++;
					if (delay > 180) {
						delay = 0;
						thread = 4;		// Salta al siguiente thread
					}
					break;

				case 4:		// Efecto grafico, ocultar el fondo
					delay ++;
					if (delay > 3) {
						delay = 0;
						tile ++;
						for (n = 0; n < 2; n ++) {
							for (y = 2; y < 22; y ++) {
								for (x = 2; x < 30; x ++) {
									NF_SetTileOfMap(n, 2, x, y, tile);
								}
							}
							NF_UpdateVramMap(n, 2);
						}
						if (tile == 20) {
							tile = 0;
							delay = 0;
							thread = 5;		// Salta al siguiente thread
						}
					}
					break;

				case 5:		// Espera 0,5 segundo y decide que hacer
					delay ++;
					if (delay > 30) {
						delay = 0;
						// Siguiente pantalla de creditos
						credit ++;
						// Si aun no has mostrado todos los creditos
						if (credit < 4) {
							thread = 6;		// Salta al thread de carga de fondos
						} else {
							thread = 7;		// Si era el ultimo, sal
						}
					}
					break;

				case 6:		// Pon en pantalla el siguiente credito
					if (credit == 1) {
						NF_CreateTiledBg(0, 3, "p_tonimax");
						NF_CreateTiledBg(1, 3, "m_tonimax");
					}
					if (credit == 2) {
						NF_CreateTiledBg(0, 3, "p_keyframe");
						NF_CreateTiledBg(1, 3, "m_keyframe");
					}
					if (credit == 3) {
						NF_CreateTiledBg(0, 3, "attributions");
						NF_CreateTiledBg(1, 3, "thanks");
					}
					// Despues de cargarlo vuelve a l thread de mostrar fondos
					thread = 2;
					break;

				case 7:		// Fade Out y sal de la rutina
					brillo += 64;
					// Si ya has subido el brillo
					if (brillo >= (16 << 8)) {
						brillo = (16 << 8);
						loop = false;
					}
					// Aplica el valor del brillo 
					setBrightness(3, (0 - (brillo >> 8)));
					break;

			}

			// Calcula la posicion de las interferencias
			bgy += 64;
			if (bgy > (64 << 8)) {
				bgy = 0;
			}

			// Espera al sincronismo vertical
			swiWaitForVBlank();

			// Mueve las interferencias
			NF_ScrollBg(0, 1, 0, (bgy >> 8));
			NF_ScrollBg(1, 1, 0, (bgy >> 8));

		}

	}




#ifdef __cplusplus
}
#endif