#ifndef __NW_GAME_H__
#define __NW_GAME_H__

#ifdef __cplusplus
extern "C" {
#endif




	// Navy Wars DS - Include funciones Game
	// Requiere DevkitARM R26
	// Requiere NightFox's Lib
	// Requiere EFS Lib
	// Codigo por NightFox
	// http://blogs.gamefilia.com/knightfox
	// Inicio 22 de Junio del 2009





	// Includes propietarios NDS
	#include <nds.h>





	// Funcion CreateBattleField();
	void CreateBattleField(void);
	// Crea el escenario para el juego



	// Funcion PlayBattle();
	void PlayBattle(void);
	// Nucleo de la batalla



	// Funcion Player1Move();
	void Player1Move(void);
	// Realiza el movimiento para el jugador 1



	// Funcion CpuMove();
	void CpuMove(void);
	// Realiza el movimiento de la CPU



	// Funcion MoveShips();
	void MoveShips(void);
	// Calcula la posicion de los navios en pantalla


	// Funcion AnimateShips();
	void AnimateShips(void);
	// Animacion de los navios


	// Funcion DeleteSquare();
	void DeleteSquare(void);
	// Animacion para descubrir casillas



	// Funcion MarkMap
	bool MarkSquare(void);
	// Animacion para marcar las casillas de la CPU / Player 2



	// Funcion TopAutoscroll();
	void TopAutoscroll(void);
	// Autoscroll de la pantalla superior



	// Funcion AnimateExplosion();
	bool AnimateExplosion(u8 screen);
	// Animacion de las explosiones



	// Funcion AnimateWatershot();
	bool AnimateWatershot(u8 screen);
	// Animacion de los disparos al agua



	// Funcion HideShips();
	void HideShips(void);
	// Ocualta los navios enemigos si aun no estan hundidos



	// Funcionm AnimateSmoke();
	void AnimateSmoke(void);
	// Animacion del efecto de humo





#ifdef __cplusplus
}
#endif


#endif