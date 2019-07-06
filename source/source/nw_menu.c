#ifdef __cplusplus
extern "C" {
#endif





	// Navy Wars DS - Funciones del Menu
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

	// // Includes del juego
	#include "nw_menu.h"
	#include "nw_system.h"





	// Funcion MainMenu();
	u8 MainMenu(void) {

		// Resetealo todo antes que nada
		ResetAll();

		// Variables
		u8 n = 0;

		bool loop = true;			// Control del bucle

		s16 brillo = (16 << 8);		// Nivel del brillo
		bool in = true;				// Control Fade In
		bool out = false;			// Control Fade Out

		s16 bgy = 0;				// Coordenada Y de las interferencias

		s16 boton_x[2];				// Coordenadas de los botones
		s16 boton_y[2];
		boton_x[0] = 64;
		boton_y[0] = 50;
		boton_x[1] = 64;
		boton_y[1] = 100;

		s8 option = 1;				// Opcion seleccionada en el menu

		bool touched = false;		// Control del TOUCHPAD

		// Pantalla en negro
		setBrightness(3, -16);
		swiWaitForVBlank();

		// Carga los fondos a la RAM
		NF_LoadTiledBg("bg/menu/title_up", "title_up", 256, 256);
		NF_LoadTiledBg("bg/menu/title_down", "title_down", 256, 256);
		NF_LoadTiledBg("bg/common/interferencias", "interferencias", 256, 512);

		// Transfiere los fondos a la VRAM
		NF_CreateTiledBg(0, 3, "title_up");
		NF_CreateTiledBg(1, 3, "title_down");
		NF_CreateTiledBg(0, 2, "interferencias");
		NF_CreateTiledBg(1, 2, "interferencias");

		// Carga los botones del menu a la RAM
		NF_LoadSpriteGfx("sprite/menu/playersel", 0, 64, 32);	// Grafico
		NF_LoadSpritePal("sprite/menu/menu", 0);	// Paleta
		NF_LoadSpriteGfx("sprite/menu/crew", 1, 64, 32);	// Grafico
		NF_LoadSpritePal("sprite/menu/crew", 1);	// Paleta


		// Transfiere los datos de los botones a la VRAM
		NF_VramSpriteGfx(1, 0, 0, false);	// Gfx de los botones a la pantalla 1
		NF_VramSpritePal(1, 0, 0);			// Pal de los botones a la pantalla 1
		NF_VramSpriteGfx(1, 1, 1, true);	// Gfx de el "Crew"
		NF_VramSpritePal(1, 1, 1);			// Pal de el "Crew"

		// Crea boton 1 jugador (Id 0 y 1)
		NF_CreateSprite(1, 0, 0, 0, boton_x[0], boton_y[0]);
		NF_CreateSprite(1, 1, 0, 0, (boton_x[0] + 64), boton_y[0]);
		NF_SpriteLayer(1, 0, 3);
		NF_SpriteLayer(1, 1, 3);
		NF_SpriteFrame(1, 0, 0);
		NF_SpriteFrame(1, 1, 1);
		// Crea boton 2 jugadores (Id 2 y 3)
		NF_CreateSprite(1, 2, 0, 0, boton_x[1], boton_y[1]);
		NF_CreateSprite(1, 3, 0, 0, (boton_x[1] + 64), boton_y[1]);
		NF_SpriteLayer(1, 2, 3);
		NF_SpriteLayer(1, 3, 3);
		NF_SpriteFrame(1, 2, 4);
		NF_SpriteFrame(1, 3, 5);
		// Crea boton Crew (Id 4)
		NF_CreateSprite(1, 4, 1, 1, 183, 151);
		NF_SpriteLayer(1, 4, 3);
		NF_SpriteFrame(1, 4, 0);

		// Habilita el canal Alpha entre los fondos 2 y 3
		REG_BLDCNT = BLEND_ALPHA | BLEND_SRC_BG2 | BLEND_DST_BG3;			// Alpha del fondos 2 sobre el 3 (Main Screen)
		REG_BLDCNT_SUB = BLEND_ALPHA | BLEND_SRC_BG2 | (BLEND_DST_BG3 + BLEND_DST_SPRITE);		// Alpha del fondos 2 sobre el 3 y sprites (Sub Screen)
		// Nivel de Alpha
		REG_BLDALPHA = 0x05 | (0x0F << 8);		// Nivel de alpha (5 sobre 15) en Main Screen
		REG_BLDALPHA_SUB = 0x05 | (0x0F << 8);	// Nivel de alpha (5 sobre 15) en Sub Screen
		

		// Bucle principal
		while (loop) {

			// Si no estas en fade...
			if (!in && !out) {

				// Cambia la opcion si se pulsa Arriba / Abajo
				ReadKeypad();
				if (KEYPRESS.down) {
					option ++;
					if (option > 3) option = 3;
				}

				if (KEYPRESS.up) {
					option --;
					if (option < 1) option = 1;
				}

				// Acepta la seleccion del menu si se pulsa "A"
				if (KEYPRESS.a) {
					out = true;
				}

				// Cambia la opcion con la pantalla tactil
				ReadTouchpad();
				if (TOUCHPAD.held) {
					// Verifica si tocas el Boton de 1 o 2 jugadores
					for (n = 0; n < 2; n ++) {
						if (
							(TOUCHPAD.pos_x > boton_x[n])
							&&
							(TOUCHPAD.pos_x < (boton_x[n] + 128))
							&&
							(TOUCHPAD.pos_y > boton_y[n])
							&&
							(TOUCHPAD.pos_y < (boton_y[n] + 32))
							&&
							(!touched)
							) {
								option = (n + 1);
								out = true;
						}
					}
					// Verifica si se pulsa el boton de "Crew"
					if (
						(TOUCHPAD.pos_x > 183)
						&&
						(TOUCHPAD.pos_x < 247)
						&&
						(TOUCHPAD.pos_y > 151)
						&&
						(TOUCHPAD.pos_y < 183)
						&&
						(!touched)
						) {
							option = 3;
							out = true;
					}
					touched = true;
				} else {
					touched = false;
				}

			}

			// Todos los botones apagados
			NF_SpriteFrame(1, 0, 0);	// 1 Jugador
			NF_SpriteFrame(1, 1, 1);
			NF_SpriteFrame(1, 2, 4);	// 2 Jugadores
			NF_SpriteFrame(1, 3, 5);
			NF_SpriteFrame(1, 4, 0);	// Crew

			// Cambia el estado de los botones segun el boton de accion
			switch (option) {
				case 1:		// 1 Jugador
					NF_SpriteFrame(1, 0, 2);
					NF_SpriteFrame(1, 1, 3);
					break;
				case 2:		// 2 Jugadores
					NF_SpriteFrame(1, 2, 6);
					NF_SpriteFrame(1, 3, 7);
					break;
				case 3:		// Crew
					NF_SpriteFrame(1, 4, 1);
					break;
			}

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

			// Calcula la posicion de las interferencias
			bgy += 64;
			if (bgy > (64 << 8)) {
				bgy = 0;
			}

			// Actualiza el array de OAM
			NF_SpriteOamSet(1);

			// Espera al sincronismo vertical
			swiWaitForVBlank();

			// Actualiza el OAM
			oamUpdate(&oamSub);

			// Mueve las interferencias
			NF_ScrollBg(0, 2, 0, (bgy >> 8));
			NF_ScrollBg(1, 2, 0, (bgy >> 8));

		}


		return option;

	}





#ifdef __cplusplus
}
#endif
