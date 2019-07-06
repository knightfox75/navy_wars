#ifndef __NW_IA_H__
#define __NW_IA_H__

#ifdef __cplusplus
extern "C" {
#endif




	// Navy Wars DS - Include funciones de la IA
	// Requiere DevkitARM R26
	// Requiere NightFox's Lib
	// Requiere EFS Lib
	// Codigo por NightFox
	// http://blogs.gamefilia.com/knightfox
	// Inicio 22 de Junio del 2009





	// Includes propietarios NDS
	#include <nds.h>




	// Funcion CpuIa();
	extern u8 CpuIa(void);
	// Ejecuta la IA de la CPU para decidir donde disparar



	// Funcion InitCpuIA();
	void InitCpuIa(void);
	// Inicializa la IA de la CPU



	// Funcion IaCheckArround
	void IaCheckArround(s8 x, s8 y);
	// Obtiene el estado de las casillas alrededor de las coordenadas



	// Funcion IaNextAction();
	void IaNextAction(bool hit);
	// Despues ejecutar el turno, decide que hacer en el siguiente



	// Funciona IaScanForShips();
	bool IaScanForShips(void);
	// Busca los posibles navios descubiertos y no hundidos



	// Funcion IaDecideNextMove();
	void IaDecideNextMove(void);
	// En caso de impacto, decide el siguiente movimiento





#ifdef __cplusplus
}
#endif


#endif