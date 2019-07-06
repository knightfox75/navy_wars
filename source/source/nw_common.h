#ifndef __NW_COMMON_H__
#define __NW_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif




	// Navy Wars DS - Include funciones comunes
	// Requiere DevkitARM R26
	// Requiere NightFox's Lib
	// Requiere EFS Lib
	// Codigo por NightFox
	// http://blogs.gamefilia.com/knightfox
	// Inicio 22 de Junio del 2009





	// Includes propietarios NDS
	#include <nds.h>





	// Define los tiles
	#define NONE 0
	#define SOLID 1
	#define BLACK 2
	#define BLUE 3
	#define RED 4
	#define YELLOW 5
	#define GREEN 6
	#define WHITE 7

	// Define los valores de la parrilla
	#define EMPTY 255

	// Define los limites de la parrilla
	#define GRID_TOP 16
	#define GRID_BOTTOM 271
	#define GRID_LEFT 16
	#define GRID_RIGHT 271

	// Define los limites del mapa
	#define MAP_TOP 0
	#define MAP_BOTTOM 95
	#define MAP_LEFT 0
	#define MAP_RIGHT 31

	// Define jugadores
	#define PLAYER_1 0
	#define PLAYER_2 1

	// Define direcciones
	#define MOVE_UP 1
	#define MOVE_DOWN 2
	#define MOVE_LEFT 3
	#define MOVE_RIGHT 4

	// Define Threads de la IA
	#define IA_DO_RANDOM 0
	#define IA_DO_TRY_NEXT 1
	#define IA_DO_CONTINUE 2

	// Define la animacion del agua
	#define WATER_FRAMES 50
	#define WATER_DELAY 10

	// Define datos para el borrado animado de casillas
	#define SQUARE_ANIM_DELAY 1
	#define MARK_ANIM_DELAY 4

	// Define la velocidad del Autoscroll
	#define TOPBG_SPEED 2

	// Define datos de los navios
	#define SHIP_GFX_RAM 0		// Slot en RAM para el GFX				(0 - 17)
	#define SHIP_GFX_VRAM 0		// Slot en VRAM para el GFX				(0 - 17)
	#define SHIP_PAL_RAM 0		// Slot en RAM para la paleta			(0)
	#define SHIP_PAL_VRAM 0		// Slot en VRAM para la paleta			(0)
	#define SHIP_SPR 50			// Id del primer Sprite de la serie		(50 - 65)

	// Define datos del humo
	#define SMOKE_GFX_RAM 18	// Slot en RAM para el GFX				(18)
	#define SMOKE_GFX_VRAM 18	// Slot en VRAM para el GFX				(18)
	#define SMOKE_PAL_RAM 1		// Slot en RAM para la paleta			(1)
	#define SMOKE_PAL_VRAM 1	// Slot en VRAM para la paleta			(1)
	#define SMOKE_SPR 30		// Id del primer Sprite de la serie		(30 - 45)
	#define SMOKE_DELAY 10		// Restardo entre frames de la animacion

	// Define datos de la explosion
	#define EXPLOSION_GFX_RAM 19	// Slot en RAM para el GFX				(19)
	#define EXPLOSION_GFX_VRAM 19	// Slot en VRAM para el GFX				(19)
	#define EXPLOSION_PAL_RAM 2		// Slot en RAM para la paleta			(2)
	#define EXPLOSION_PAL_VRAM 2	// Slot en VRAM para la paleta			(2)
	#define EXPLOSION_SPR 21		// Id del primer Sprite de la serie		(25)
	#define EXPLOSION_DELAY 4		// Restardo entre frames de la animacion

	// Define datos del disparo al agua
	#define WATERSHOT_GFX_RAM 20	// Slot en RAM para el GFX				(20)
	#define WATERSHOT_GFX_VRAM 20	// Slot en VRAM para el GFX				(20)
	#define WATERSHOT_PAL_RAM 3		// Slot en RAM para la paleta			(3)
	#define WATERSHOT_PAL_VRAM 3	// Slot en VRAM para la paleta			(3)
	#define WATERSHOT_SPR 70		// Id del primer Sprite de la serie		(70)
	#define WATERSHOT_DELAY 6		// Restardo entre frames de la animacion





	// Variables Globales del juego //

	// Control del fondo, pantalla inferior
	extern s16 SCREEN_BG_X[2];		// Coordenada X
	extern s16 SCREEN_BG_Y[2];		// Coordenada Y

	// Control de la animacion del agua
	extern u8 WATER_SPEED[2];	// [pantalla]
	extern u8 WATER_FRAME[2][9][9];

	// Control de posiciones, pantalla inferior
	extern u8 GRID[2][8][8];		// [player][x][y] - Real
	extern u8 BKGRID[2][8][8];		// [player][x][y] - Checksum (Para actualizar VRAM)
	extern u8 TMPGRID [2][8][8];	// [player][x][y] - Backup (Para deshacer cambios)

	// Barra de seleccion de los menus
	extern s8 MENU_SELECT;
	extern s16 MENU_ORIGIN;
	extern s16 MENU_DESTINATION;
	extern u16 MENU_SPEED;
	extern u8 MENU_LAST;
	extern s8 MENU_DIRECTION;
	extern bool MENU_MOVE;

	// Control de navios
	typedef struct {
		u8 size;
		u8 live;
		bool in_position;
		bool vertical;
		s8 x;
		s8 y;
	} ship_info;
	extern ship_info SHIP[2][6];

	typedef struct {
		s16 x;		// Coordenada X
		s16 y;		// Coordenada Y
		u8 gfx;		// Gfx que usaras
	} shipspr_info;
	extern shipspr_info SHIP_SPRITE[2][16];		// [Player][Sprite]

	// Animacion de los navios (comun a todos y en ambas pantallas
	extern u8 SHIP_ANIM_FRAME;		// Frame de la animacion
	extern u8 SHIP_ANIM_DELAY;		// Retardo de la animacion
	extern s8 SHIP_ANIM_NEXT;		// Direccion de la animacion

	// Animacion del Humo
	extern u8 SMOKE_ANIM_FRAME;		// Frame de la animacion
	extern u8 SMOKE_ANIM_DELAY;		// Retardo en cambiar de frame

	// Animacion del borrado de las cuadriculas
	extern bool SQUARE_DELETE;		// Flag de animacion
	extern u8 SQUARE_FRAME;			// Frame de la animacion
	extern u8 SQUARE_DELAY;			// Retardo en la animacion
	extern u8 SQUARE_X;				// Coordenada X
	extern u8 SQUARE_Y;				// Coordenada Y

	// Animacion del marcado de las casillas CPU/Player 2
	extern bool MARK_FLAG;			// Flag de animacion
	extern u8 MARK_FRAME;			// Frame de la animacion
	extern u8 MARK_DELAY;			// Retardo en la animacion
	extern u8 MARK_X;				// Coordenada X
	extern u8 MARK_Y;				// Coordenada Y

	// Movimiento Autoscroll de la CPU en la pantalla superior
	extern bool TOPBG_FLAG;			// Flag de movimiento
	extern s16 TOPBG_X;				// Coordenada X de destino
	extern s16 TOPBG_Y;				// Coordenada Y de destino

	// Animacion de las explosiones
	typedef struct {
		bool flag;
		u8 frame;
		u8 delay;
		u8 x;
		u8 y;
	} explosion_info;
	extern explosion_info EXPLOSION[2];		// [Screen]
	extern explosion_info FAILURE[2];		// [Screen]

	// Datos de la IA
	extern u8 IA_GRID[2][64];	// [real/respaldo] [casilla]
	extern u8 GRID_REST[2];		// Cuantas casillas quedan por destapar ? [Player]
	typedef struct {
		bool flag;		// IA activa
		bool up;		// Flag de estado, moverse arriba
		bool down;		// Flag de estado, moverse abajo
		bool left;		// Flag de estado, moverse izquierda
		bool right;		// Flag de estado, moverse derecha
		u8 thread;		// Thread a ejecutar de la IA
		u8 hits;		// Numero de disparos acertados
		u8 first_x;		// Coordenada X del primer impacto
		u8 first_y;		// Coordenada Y del primer impacto
		u8 last_x;		// Coordenada X del ultimo movimiento acertado
		u8 last_y;		// Coordenada Y del ultimo movimiento acertado
		u8 next_move;	// Siguiente movimiento
	} ia_info;
	extern ia_info IA;	// Parametros de la IA

	// Control de daños, unidades restantes
	extern u8 UNITS_LEFT[2];	// Unidades restantes [player]

	// Escape - Abortar partida
	extern bool GAME_ABORTED;


	// Caja de texto, variables
	typedef struct {
		char data[768];		// Datos
		u16 start_x;		// Inicio de la caja
		u16 start_y;
		u16 end_x;			// Fin de la caja
		u16 end_y;
		u16 pos_x;			// Posicion actual 
		u16 pos_y;
		u8 layer;			// Capa donde esta el texto
		u16 delay;			// Contador de retardo
		u16 size;			// Tamaño del texto
		u16 character;		// nº de caracter a escribir
	} textbox_info;
	extern textbox_info TEXTBOX[2];		// Caja de texto [pantalla]

	
	// Funcion MoveScreen();
	void MoveScreen(u8 screen);
	// Calcula con el Keypad la posicion de la pantalla seleccionada



	// Funcion FillWithTiles();
	void FillWithTiles(u8 screen, u8 layer, u16 tile_x, u16 tile_y, u16 size_x, u16 size_y, u16 tile);
	// Rellena una seccion especificada de un mapa con los tiles seleccionados



	// Funcion GetGridSquareX();
	extern s8 GetGridX(void);
	// Devuelve el nº de casilla (X) o -1 si no se ha tocado ninguna



	// Funcion GetGridSquareY();
	extern s8 GetGridY(void);
	// Devuelve el nº de casilla (Y) o -1 si no se ha tocado ninguna



	// Funcion AnimateWater();
	void AnimateWater(u8 screen, u8 layer, u8 delay);
	// Anima el agua



	// Funcion UpdateScreenGrid();
	void UpdateScreenGrid(u8 player, u8 screen, u8 layer);
	// Actualiza la parrilla de juego



	// Funcion GridBackup();
	void GridBackup(u8 player);
	// Copia el GRID al Backup de cheksum



	// Funcion GridTmp();
	void GridTmp(u8 player, bool tmp);
	// Copia el GRID al temporal o restauralo
	// TRUE -> GRID a TMP, FALSE -> TMP a GRID



	// Funcion CreateTextBox();
	void CreateTextBox(u8 screen, u8 layer, u16 start_x, u16 start_y, u16 end_x, u16 end_y, const char* text);
	// Crea una caja de texto



	// Funcion WriteTextBox();
	extern u8 WriteTextBox(u8 screen, u8 delay);
	// Actualiza la caja de texto





#ifdef __cplusplus
}
#endif


#endif
