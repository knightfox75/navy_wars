#ifndef __NW_SETUP_H__
#define __NW_SETUP_H__

#ifdef __cplusplus
extern "C" {
#endif




	// Navy Wars DS - Include funciones Navy Setup
	// Requiere DevkitARM R26
	// Requiere NightFox's Lib
	// Requiere EFS Lib
	// Codigo por NightFox
	// http://blogs.gamefilia.com/knightfox
	// Inicio 22 de Junio del 2009





	// Includes propietarios NDS
	#include <nds.h>





	// Funcion NavySetup
	void NavySetup(u8 player);
	// Posiciona tu flota en el mapa



	// Funcion CreateNavySetup();
	void CreateNavySetup(void);
	// Crea la pantalla para posicionar tu flota



	// Funcion CheckBoatPlace();
	extern bool CheckBoatPlace(s8 boat, s8 pos_x, s8 pos_y);
	// Verifica si se puede posicionar el barco en la parrilla



	// Funcion PreviewMyBoat();
	void PreviewMyBoat(s8 boat, s8 pos_x, s8 pos_y, u8 color);
	// Haz una preview de donde se colocara y si se puede colocar el navio



	// Funcion PlaceMyBoat();
	void PlaceMyBoat(u8 current_boat, s8 boat, s8 pos_x, s8 pos_y);
	// Posiciona en la parrilla si es posible el navio del jugador



	// ExitNavySetup();
	extern bool ExitNavySetup(void);
	// Pregunta una vez se han colocado los navios si todo es correcto



	// Funcion ComputerSetup();
	void ComputerSetup(void);
	// Coloca los navios controlador por la CPU



	// Funcion TwoPlayers();	// Debug
	void TwoPlayers(void);
	// Pantalla de proximamente (Debug)



	// Funcion OptionsMenu();
	void OptionsMenu(bool can_change);
	// Funcion generica de selector (Menu de opciones)





#ifdef __cplusplus
}
#endif


#endif