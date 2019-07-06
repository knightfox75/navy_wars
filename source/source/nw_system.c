#ifdef __cplusplus
extern "C" {
#endif





	// Navy Wars DS - Funciones de sistema
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
	#include "nw_system.h"





	// Array para almacenar las teclas "HELD"
	keypad_info KEYHELD;

	// Array para almacenar las teclas "DOWN"
	keypad_info KEYPRESS;

	// Array para almacenar las teclas "UP"
	keypad_info KEYUP;

	// Array para almacenar la info de la pantalla tactil
	touchscreen_info TOUCHPAD;





	// Funcion BootGame();
	void BootGame(void) {

		// Pantalla de espera inicializando la EFS
		NF_Set2D(0, 0);
		NF_Set2D(1, 0);	
		consoleDemoInit();
		iprintf("\n NitroFS init. Please wait.\n\n");
		iprintf(" Iniciando NitroFS,\n por favor, espere.\n");
		swiWaitForVBlank();

		// Define el ROOT e inicializa el sistema de archivos
		NF_SetRootFolder("NITROFS");	// Define la carpeta ROOT para usar NitroFS

		// Inicializa el motor 2D
		NF_Set2D(0, 0);				// Modo 2D_0 en ambas pantallas
		NF_Set2D(1, 0);	

		// Inicializa el engine basico de Sonido (Libnds)
		soundEnable();

		// Inicializa los fondos tileados
		NF_InitTiledBgBuffers();	// Inicializa los buffers para almacenar fondos
		NF_InitTiledBgSys(0);		// Inicializa los fondos Tileados para la pantalla superior
		NF_InitTiledBgSys(1);		// Iniciliaza los fondos Tileados para la pantalla inferior

		// Inicializa los Sprites
		NF_InitSpriteBuffers();		// Inicializa los buffers para almacenar sprites y paletas
		NF_InitSpriteSys(0);		// Inicializa los sprites para la pantalla superior
		NF_InitSpriteSys(1);		// Inicializa los sprites para la pantalla inferior

		// Inicializa el engine de texto
		NF_InitTextSys(0);
		NF_InitTextSys(1);

		// Inicializa las buffers de audio
		NF_InitRawSoundBuffers();

	}



	// Funcion SplashScreens();
	void SplashScreens(void) {

		// Resetealo todo antes que nada
		ResetAll();

		u8 thread = 0;			// Proceso
		bool next = false;		// Flag del siguiente proceso
		bool loop = true;		// Control del LOOP


		while (loop) {

			// Nucleo de la funcion
			switch (thread) {

				case 0:	// Pantalla en negro
					setBrightness(3, -16);
					next = true;
					break;

				case 1:	// Espera 30 frames
					WaitTime(30);
					next = true;
					break;

				case 2:	// Carga las pantallas SceneBeta
					NF_LoadTiledBg("bg/splash/scenebeta_up", "scenebeta_up", 256, 256);
					NF_LoadTiledBg("bg/splash/scenebeta_down", "scenebeta_down", 256, 256);
					NF_CreateTiledBg(0, 0, "scenebeta_up");
					NF_CreateTiledBg(1, 0, "scenebeta_down");
					// Siguiente thread
					next = true;
					break;

				case 3:	// Fade In
					FadeIn(64);
					next = true;
					break;

				case 4: // Espera 90 frames
					WaitTime(90);
					next = true;
					break;

				case 5: // Fade Out
					FadeOut(64);
					next = true;
					break;

				case 6:	// Carga las pantallas devkitPro y NF Lib y muestralas
					// Borra las pantallas SceneBeta
					NF_DeleteTiledBg(0, 0);
					NF_DeleteTiledBg(1, 0);
					NF_UnloadTiledBg("scenebeta_up");
					NF_UnloadTiledBg("scenebeta_down");
					// Carga las pantallas devkitPro y NF Lib y muestralas
					NF_LoadTiledBg("bg/splash/devkitpro", "devkitpro", 256, 256);
					NF_LoadTiledBg("bg/splash/nfl", "nfl", 256, 256);
					NF_CreateTiledBg(0, 0, "devkitpro");
					NF_CreateTiledBg(1, 0, "nfl");
					next = true;
					break;

				case 7:	// Fade In
					FadeIn(64);
					next = true;
					break;

				case 8: // Espera 90 frames
					WaitTime(90);
					next = true;
					break;

				case 9:	// Fade Out
					FadeOut(64);
					next = true;
					break;

				case 10:	// Carga las pantallas NightFox & co. y muestralas
					// Borra las pantallas devkitPro y NF Lib
					NF_DeleteTiledBg(0, 0);
					NF_DeleteTiledBg(1, 0);
					NF_UnloadTiledBg("devkitpro");
					NF_UnloadTiledBg("nfl");
					// Carga las pantallas NightFox & co. y muestralas
					NF_LoadTiledBg("bg/splash/nfandco_up", "nfandco_up", 256, 256);
					NF_LoadTiledBg("bg/splash/nfandco_down", "nfandco_down", 256, 256);
					NF_CreateTiledBg(0, 0, "nfandco_up");
					NF_CreateTiledBg(1, 0, "nfandco_down");
					next = true;
					break;

				case 11:	// Fade In
					FadeIn(64);
					next = true;
					break;

				case 12:	// Espera 90 frames
					WaitTime(90);
					next = true;
					break;

				case 13:	// Fade Out
					FadeOut(64);
					next = true;
					break;

				case 14:	// // Borra las pantallas NightFox & co. y termina
					NF_DeleteTiledBg(0, 0);
					NF_DeleteTiledBg(1, 0);
					NF_UnloadTiledBg("nfandco_up");
					NF_UnloadTiledBg("nfandco_down");
					loop = false;
					break;

			}


			// Si debes saltar al siguiente thread, hazlo
			if (next) {
				thread ++;
				next = false;
			}

			swiWaitForVBlank();		// Espera al sincronismo vertical

		}


	}



	// Funcion ReadKeypad();
	void ReadKeypad(void) {

		// Variables
		u16 keys = 0;

		// Lee el teclado via Libnds
		scanKeys();

		// Teclas "HELD"
		keys = keysHeld();
		if (keys & KEY_UP) {KEYHELD.up = true;} else {KEYHELD.up = false;}
		if (keys & KEY_DOWN) {KEYHELD.down = true;} else {KEYHELD.down = false;}
		if (keys & KEY_LEFT) {KEYHELD.left = true;} else {KEYHELD.left = false;}
		if (keys & KEY_RIGHT) {KEYHELD.right = true;} else {KEYHELD.right = false;}
		if (keys & KEY_A) {KEYHELD.a = true;} else {KEYHELD.a = false;}
		if (keys & KEY_B) {KEYHELD.b = true;} else {KEYHELD.b = false;}
		if (keys & KEY_X) {KEYHELD.x = true;} else {KEYHELD.x = false;}
		if (keys & KEY_Y) {KEYHELD.y = true;} else {KEYHELD.y = false;}
		if (keys & KEY_L) {KEYHELD.l = true;} else {KEYHELD.l = false;}
		if (keys & KEY_R) {KEYHELD.r = true;} else {KEYHELD.r = false;}
		if (keys & KEY_START) {KEYHELD.start = true;} else {KEYHELD.start = false;}
		if (keys & KEY_SELECT) {KEYHELD.select = true;} else {KEYHELD.select = false;}

		// Teclas "PRESS"
		keys = keysDown();
		if (keys & KEY_UP) {KEYPRESS.up = true;} else {KEYPRESS.up = false;}
		if (keys & KEY_DOWN) {KEYPRESS.down = true;} else {KEYPRESS.down = false;}
		if (keys & KEY_LEFT) {KEYPRESS.left = true;} else {KEYPRESS.left = false;}
		if (keys & KEY_RIGHT) {KEYPRESS.right = true;} else {KEYPRESS.right = false;}
		if (keys & KEY_A) {KEYPRESS.a = true;} else {KEYPRESS.a = false;}
		if (keys & KEY_B) {KEYPRESS.b = true;} else {KEYPRESS.b = false;}
		if (keys & KEY_X) {KEYPRESS.x = true;} else {KEYPRESS.x = false;}
		if (keys & KEY_Y) {KEYPRESS.y = true;} else {KEYPRESS.y = false;}
		if (keys & KEY_L) {KEYPRESS.l = true;} else {KEYPRESS.l = false;}
		if (keys & KEY_R) {KEYPRESS.r = true;} else {KEYPRESS.r = false;}
		if (keys & KEY_START) {KEYPRESS.start = true;} else {KEYPRESS.start = false;}
		if (keys & KEY_SELECT) {KEYPRESS.select = true;} else {KEYPRESS.select = false;}

		// Teclas "UP"
		keys = keysUp();
		if (keys & KEY_UP) {KEYUP.up = true;} else {KEYUP.up = false;}
		if (keys & KEY_DOWN) {KEYUP.down = true;} else {KEYUP.down = false;}
		if (keys & KEY_LEFT) {KEYUP.left = true;} else {KEYUP.left = false;}
		if (keys & KEY_RIGHT) {KEYUP.right = true;} else {KEYUP.right = false;}
		if (keys & KEY_A) {KEYUP.a = true;} else {KEYUP.a = false;}
		if (keys & KEY_B) {KEYUP.b = true;} else {KEYUP.b = false;}
		if (keys & KEY_X) {KEYUP.x = true;} else {KEYUP.x = false;}
		if (keys & KEY_Y) {KEYUP.y = true;} else {KEYUP.y = false;}
		if (keys & KEY_L) {KEYUP.l = true;} else {KEYUP.l = false;}
		if (keys & KEY_R) {KEYUP.r = true;} else {KEYUP.r = false;}
		if (keys & KEY_START) {KEYUP.start = true;} else {KEYUP.start = false;}
		if (keys & KEY_SELECT) {KEYUP.select = true;} else {KEYUP.select = false;}

	}



	// Funcion ReadTouchpad();
	void ReadTouchpad(void) {

		// Variables
		u16 keys = 0;
		touchPosition touchscreen;

		// Lee el teclado via Libnds
		scanKeys();

		// Verifica el estado del touchscreen
		keys = keysHeld();
		if (keys & KEY_TOUCH) {TOUCHPAD.held = true;} else {TOUCHPAD.held = false;}

		// Lee el TOUCHPAD via Libnds
		touchRead(&touchscreen); 

		// Guarda el estado del touchscreen
		TOUCHPAD.pos_x = touchscreen.px;
		TOUCHPAD.pos_y = touchscreen.py;
		TOUCHPAD.raw_x = touchscreen.rawx;
		TOUCHPAD.raw_y = touchscreen.rawy;
		TOUCHPAD.press_a = touchscreen.z1;
		TOUCHPAD.press_b = touchscreen.z2;

	}



	// Funcion FadeIn();
	void FadeIn(u16 speed) {
		// Brillo Inicial
		s16 brillo = (16 << 8);
		// Mientras el brillo no llegue a 0
		while (brillo > 0) {
			brillo -= speed;
			// Si ya has subido el brillo
			if (brillo <= 0) {
				brillo = 0;
			}
			// Aplica el valor del brillo 
			setBrightness(3, (0 - (brillo >> 8)));
			swiWaitForVBlank();
		}
	}



	// Funcion FadeOut();
	void FadeOut(u16 speed) {
		// Brillo Inicial
		s16 brillo = 0;
		// Mientras el brillo no llegue a 0
		while (brillo < (16 << 8)) {
			brillo += speed;
			// Si ya has subido el brillo
			if (brillo >= (16 << 8)) {
				brillo = (16 << 8);
			}
			// Aplica el valor del brillo 
			setBrightness(3, (0 - (brillo >> 8)));
			swiWaitForVBlank();
		}
	}



	// Funcion WaitTime();
	void WaitTime(u16 wait) {
		s16 timer = 0;
		while (timer < wait) {
			timer ++;
			if (timer >= wait) {
				timer = wait;
			}
			swiWaitForVBlank();
		}
	}



	// Funcion ResetBuffers();
	void ResetAll(void) {

		// Variables locales
		u8 screen = 0;
		u8 layer = 0;

		// Borra los buffers de Fondos
		NF_ResetTiledBgBuffers();

		// Borra los buffers de Sprites
		NF_ResetSpriteBuffers();

		// Reinicializa los fondos
		NF_InitTiledBgSys(0);
		NF_InitTiledBgSys(1);

		// Reinicializa los Sprites
		NF_InitSpriteSys(0);
		NF_InitSpriteSys(1);

		// Reinicializa las capas de texto
		NF_InitTextSys(0);
		NF_InitTextSys(1);

		// Elimina los posibles efectos de Alpha Blending
		REG_BLDCNT = BLEND_NONE;
		REG_BLDCNT_SUB = BLEND_NONE;
		REG_BLDALPHA = 0x0F | (0x0F << 8);
		REG_BLDALPHA_SUB = 0x0F | (0x0F << 8);

		// Resetea la posicion de todas las capas de fondo
		for (screen = 0; screen < 2; screen ++) {
			for (layer = 0; layer < 4; layer ++) {
				NF_ScrollBg(screen, layer, 0, 0);
			}
		}

		// Reinicializa las buffers de audio
		NF_ResetRawSoundBuffers();

	}





#ifdef __cplusplus
}
#endif
