#ifdef __cplusplus
extern "C" {
#endif





	// Navy Wars DS - Funciones de WI-FI
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
	#include <nf_wifi.h>

	// Includes del juego
	#include "nw_wifi.h"
	#include "nw_system.h"
	#include "nw_common.h"





	// Funcion WifiLobby();
	void WifiLobby(void) {
		
		// Crea el GUI del WIFI
		CreateWifiLayout();

		// Debes conectarte el punto de acceso por defecto?
		if (WifiAskConnect()) {
			
			// Si se conecta al AP con exito
			if (WifiConnectAp()) {

				// Segun la opcion seleccionada
				switch (WifiSelectMode()) {

					case 0:		// Servidor
						break;

					case 1:		// Cliente
						break;

					case 2:		// Cancelar
						break;

				}

			}

			// Cierra la conexion
			NF_WiFiDisconnectAp();

		}

		FadeOut(64);

	}



	// Funcion CreateWifiLayout();
	void CreateWifiLayout(void) {

		// Borra todo el contenido
		ResetAll();

		// Pantalla en negro
		setBrightness(3, -16);
		swiWaitForVBlank();

		// Carga archivos de fondos
		NF_LoadTiledBg("bg/wifi/wifi_up", "wifi_up", 256, 256);
		NF_LoadTiledBg("bg/wifi/wifi_down", "wifi_down", 256, 256);
		NF_LoadTiledBg("bg/wifi/marker", "marker", 512, 512);

		// Carga el archivo de fuentes
		NF_LoadTextFont("fnt/navyfont", "navy_up", 256, 256, 0);
		NF_LoadTextFont("fnt/navyfont", "navy_down", 256, 256, 0);

		// Carga el efecto de sonido
		NF_LoadRawSound("sfx/click", 0, 11025, 0);

		// Crea los fondos en pantalla
		NF_CreateTiledBg(0, 3, "wifi_up");
		NF_CreateTiledBg(1, 3, "wifi_down");
		NF_CreateTiledBg(1, 2, "marker");

		// Crea las capas de texto
		NF_CreateTextLayer(0, 1, 0, "navy_up");
		NF_CreateTextLayer(1, 1, 0, "navy_down");

		// Habilita el canal Alpha entre los fondos 2 y 3 (Subscreen)
		REG_BLDCNT_SUB = BLEND_ALPHA | BLEND_SRC_BG2 | BLEND_DST_BG3;
		// Nivel de Alpha (Subscreen)
		REG_BLDALPHA_SUB = 0x03 | (0x0F << 8);

		// Muestralo todo
		FadeIn(64);

	}



	// Funcion WifiAskConnect();
	bool WifiAskConnect(void) {

		// Variables
		bool loop = true;		// Loop general
		s16 mark_x = 256;		// Coordenadas del selector
		s16 mark_y = 213;
		s16 n = 0;				// Uso general
		s8 option = 0;			// Opcion seleccionada
		bool move = false;		// Flag de movimiento del marcador

		// Mensage "Sistema de batalla listo"
		CreateTextBox(1, 1, 1, 1, 30, 22, "Starting WIFI system.||Connect to default AP?|||Yes||No");
		while (loop) {
			if (WifiConsoleText(1) == 0) loop = false;
			swiWaitForVBlank();
		}

		// Cartelito de seleccion In
		for (n = 0; n < 20; n ++ ) {
			mark_x -= 2;
			NF_ScrollBg(1, 2, mark_x, mark_y);
			swiWaitForVBlank();
		}

		// Menu SI/NO
		loop = true;
		while (loop) {

			// Lee el teclado
			ReadKeypad();
			ReadTouchpad();

			// Su se pulsa arriba y esta seleccionada la opcion "No"...
			if (KEYPRESS.up && (option == 1) && !move) {
				move = true;
				option = 0;
				n = 0;
			}

			// Su se pulsa abajo y esta seleccionada la opcion "Si"...
			if (KEYPRESS.down && (option == 0) && !move) {
				move = true;
				option = 1;
				n = 0;
			}

			// Si pulsas A, acepta la seleccion
			if (KEYPRESS.a && !move) loop = false;

			// Movimiento del marcador
			if (move) {
				if (option == 0) {
					mark_y += 2;
				} else {
					mark_y -= 2;
				}
				NF_ScrollBg(1, 2, mark_x, mark_y);
				n ++;
				if (n > 7) move = false;
			}

			// Espera al sincronismo
			swiWaitForVBlank();

		}

		// Cartelito de seleccion Out
		for (n = 0; n < 20; n ++ ) {
			mark_x += 2;
			NF_ScrollBg(1, 2, mark_x, mark_y);
			swiWaitForVBlank();
		}

		// Borra la consola
		WifiConsoleClear(1, 1);

		// Devuelve la opcion seleccionada
		if (option == 0) {
			return true;
		} else {
			return false;
		}
		
	}



	// Funcion WifiConnectAp();
	bool WifiConnectAp(void) {

		// Variables locales
		bool loop = true;
		char text[32];
		u8 n = 0;

		// Mensage "Conectando al AP"
		CreateTextBox(0, 1, 1, 1, 30, 1, "Connecting to default AP...");
		while (loop) {
			if (WifiConsoleText(0) == 0) loop = false;
			swiWaitForVBlank();
		}

		// Intenta conectarte al punto de acceso por defecto
		if (NF_WiFiConnectDefaultAp()) {

			// Devuelve los parametros de la conexion TCP/IP con el AP
			for (n = 0; n < 7; n ++ ) {
				// Selecciona el mensage
				switch (n) {
					case 0:
						sprintf(text, "Connection established.");
						break;
					case 1:
						sprintf(text, " ");
						break;
					case 2:
						sprintf(text, " Ip      :  %s", inet_ntoa(NF_IP)); 
						break;
					case 3:
						sprintf(text, " Mask    :  %s", inet_ntoa(NF_MASK)); 
						break;
					case 4:
						sprintf(text, " Gateway :  %s", inet_ntoa(NF_GATEWAY)); 
						break;
					case 5:
						sprintf(text, " Dns1    :  %s", inet_ntoa(NF_DNS1)); 
						break;
					case 6:
						sprintf(text, " Dns2    :  %s", inet_ntoa(NF_DNS2)); 
						break;
				}
				// Escribe el mensage
				loop = true;
				CreateTextBox(0, 1, 1, (n + 4), 30, (n + 4), text);
				while (loop) {
					if (WifiConsoleText(0) == 0) loop = false;
					swiWaitForVBlank();
				}
			}

			// Devuelve el estado de conectado
			return true;
			
		} else {

			// No se ha podido conectar
			loop = true;
			CreateTextBox(0, 1, 1, 4, 30, 22, "Connection failed.|||Press A or touchscreen|to back to main menu.");
			while (loop) {
				if (WifiConsoleText(0) == 0) loop = false;
				swiWaitForVBlank();
			}

			// Cierra la conexion
			NF_WiFiDisconnectAp();

			// Espera a que se pulse A o la pantalla
			loop = true;
			while (loop) {
				ReadKeypad();
				ReadTouchpad();
				if (KEYPRESS.a || TOUCHPAD.held) loop = false;
				swiWaitForVBlank();
			}

			// Devuelve conexion fallida
			return false;

		}

	}



	// Funcion WifiSelectMode();
	u8 WifiSelectMode(void) {

		// Variables
		bool loop = true;		// Loop general
		s16 mark_x = 256;		// Coordenadas del selector
		s16 mark_y = 229;
		s16 n = 0;					// Uso general
		s8 option = 0;				// Opcion seleccionada
		bool move = false;			// Flag de movimiento del marcador
		bool direction = false;		// Flag de direccion

		// Mensage "Sistema de batalla listo"
		CreateTextBox(1, 1, 1, 1, 30, 22, "What you want to do?|||Host game||Join game||Cancel");
		while (loop) {
			if (WifiConsoleText(1) == 0) loop = false;
			swiWaitForVBlank();
		}

		// Cartelito de seleccion In
		for (n = 0; n < 44; n ++ ) {
			mark_x -= 2;
			NF_ScrollBg(1, 2, mark_x, mark_y);
			swiWaitForVBlank();
		}

		// Menu SI/NO
		loop = true;
		while (loop) {

			// Lee el teclado
			ReadKeypad();
			ReadTouchpad();

			// Su se pulsa arriba...
			if (KEYPRESS.up && (option > 0) && !move) {
				move = true;
				option --;
				direction = true;
				n = 0;
			}

			// Su se pulsa abajo...
			if (KEYPRESS.down && (option < 2) && !move) {
				move = true;
				option ++;
				direction = false;
				n = 0;
			}

			// Si pulsas A, acepta la seleccion
			if (KEYPRESS.a && !move) loop = false;

			// Movimiento del marcador
			if (move) {
				if (direction) {
					mark_y += 2;
				} else {
					mark_y -= 2;
				}
				NF_ScrollBg(1, 2, mark_x, mark_y);
				n ++;
				if (n > 7) move = false;
			}

			// Espera al sincronismo
			swiWaitForVBlank();

		}

		// Cartelito de seleccion Out
		for (n = 0; n < 44; n ++ ) {
			mark_x += 2;
			NF_ScrollBg(1, 2, mark_x, mark_y);
			swiWaitForVBlank();
		}

		// Borra la consola
		WifiConsoleClear(1, 1);

		// Devuelve la opcion seleccionada
		return option;
		
	}



	// Funcion WifiConsoleText();
	u8 WifiConsoleText(u8 screen) {

		// Variable de control de balance
		u8 pan = 0;

		// Escribe el texto y guarda el resultado
		u8 status = WriteTextBox(screen, 3);

		// Si se ha escrito un caracter
		if (status > 0) {
			// Si era una letra (no un espacio en blanco)
			if (status == 2) {
				pan = ((TEXTBOX[1].pos_x * 127) / 31);
				NF_PlayRawSound(0, 127, pan, false, 0);
			}
			NF_UpdateTextLayers();
		}

		// Devuelve el estado
		return status;

	}



	// Funcion WifiConsoleClear();
	void WifiConsoleClear(u8 screen, u8 layer) {

		u8 x = 0;
		u8 y = 0;

		for (y = 0; y < 24; y ++) {
			for (x = 0; x < 32; x ++) {
				NF_WriteText(screen, layer, x, y, " ");
			}
			NF_UpdateTextLayers();
			swiWaitForVBlank();
		}

	}



	// Funcion WifiWaitForClient
	u8 WifiWaitForClient(void) {
		return 0;
	}





#ifdef __cplusplus
}
#endif
