#ifndef __NW_WIFI_H__
#define __NW_WIFI_H__

#ifdef __cplusplus
extern "C" {
#endif




	// Navy Wars DS - Include funciones de WI-FI
	// Requiere DevkitARM R26
	// Requiere NightFox's Lib
	// Requiere EFS Lib
	// Codigo por NightFox
	// http://blogs.gamefilia.com/knightfox
	// Inicio 22 de Junio del 2009





	// Includes propietarios NDS
	#include <nds.h>





	// Funcion WifiLobby();
	void WifiLobby(void);
	// Crea el entorno de conexion wifi



	// Funcion CreateWifiLayout();
	void CreateWifiLayout(void);
	// Carga los archivos y crea el entorno grafico del menu de WIFI



	// Funcion WifiAskConnect();
	extern bool WifiAskConnect(void);
	// Deseas conectarte al punto de acceso por defecto?



	// Funcion WifiConnectAp();
	extern bool WifiConnectAp(void);
	// Establece la conexion con el AP por defecto.



	// Funcion WifiSelectMode();
	u8 WifiSelectMode(void);
	// Pregunta si sera Cliente / Servidor o cancela


	// Funcion WifiConsoleText();
	extern u8 WifiConsoleText(u8 screen);
	// Escribe en la consola de texto (Efecto de consola antigua)



	// Funcion WifiConsoleClear();
	void WifiConsoleClear(u8 screen, u8 layer);
	// Borra la pantalla especificada



	// Funcion WifiWaitForClient
	u8 WifiWaitForClient(void);
	// Espera la conexion de un cliente remoto





#ifdef __cplusplus
}
#endif


#endif
