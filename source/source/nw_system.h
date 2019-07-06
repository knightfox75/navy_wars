#ifndef __NW_SYSTEM_H__
#define __NW_SYSTEM_H__

#ifdef __cplusplus
extern "C" {
#endif




	// Navy Wars DS - Include funciones de sistema
	// Requiere DevkitARM R26
	// Requiere NightFox's Lib
	// Requiere EFS Lib
	// Codigo por NightFox
	// http://blogs.gamefilia.com/knightfox
	// Inicio 22 de Junio del 2009





	// Includes propietarios NDS
	#include <nds.h>





	// Define la estructura de lectura del KeyPad
	typedef struct {
		bool up;
		bool down;
		bool left;
		bool right;
		bool a;
		bool b;
		bool x;
		bool y;
		bool l;
		bool r;
		bool start;
		bool select;
	} keypad_info;

	// Array para almacenar las teclas "HELD"
	extern keypad_info KEYHELD;

	// Array para almacenar las teclas "DOWN"
	extern keypad_info KEYPRESS;

	// Array para almacenar las teclas "UP"
	extern keypad_info KEYUP;



	// Define la estructura de lectura del Touchscreen
	typedef struct {
		u16 pos_x;
		u16 pos_y;
		u16 raw_x;
		u16 raw_y;
		u16 press_a;
		u16 press_b;
		bool held;
	} touchscreen_info;

	// Array para almacenar la info de la pantalla tactil
	extern touchscreen_info TOUCHPAD;
	




	// Funcion BootGame();
	void BootGame(void);
	// Inicializa el engine del juego



	// Funcion SplashScreens();
	void SplashScreens(void);
	// Muestra las pantallas de presentacion



	// Funcion ReadKeypad();
	void ReadKeypad(void);
	// Lee el keypad y actualiza el array de datos del mismo



	// Funcion ReadTouchpad();
	void ReadTouchpad(void);
	// Lee el TOUCHPAD y actualiza el array de datos del mismo



	// Funcion FadeIn();
	void FadeIn(u16 speed);
	// Efecto FadeIn en ambas pantallas
	// Toma el control del programa hasta finalizar la ejecuccion



	// Funcion FadeOut();
	void FadeOut(u16 speed);
	// Efecto FadeOut en ambas pantallas
	// Toma el control del programa hasta finalizar la ejecuccion



	// Funcion WaitTime();
	void WaitTime(u16 wait);
	// Espera los frames indicados
	// Toma el control del programa hasta finalizar la ejecuccion



	// Funcion ResetAll();
	void ResetAll(void);
	// Vacia todos los buffers





#ifdef __cplusplus
}
#endif


#endif