#include "sd.h"
extern UART_HandleTypeDef huart2;
extern FATFS fs;  // file system
extern FIL fil; // File
extern FILINFO fno;
extern FRESULT fr;  // result
extern UINT br, bw;  // File read/write count


fileState verifyAccessRegister(char *name){
	/*Esta funcion es un wrapper nada mas*/
	/**** chequea si existe o no el archivo ****/
	fileState State;
	char *buf = pvPortMalloc(100*sizeof(char));
	Mount_SD("/");
	fr = f_stat (name, &fno);					//Compruebo si existe el archivo
	switch(fr){
	case FR_OK:
		State = FILE_EXISTS;
		sprintf(buf, "*%s* existe en SD. OK.\n", name);
		Send_Uart(buf);
		break;
	case FR_NO_FILE:
	case FR_NO_PATH:
		State = FILE_DOESNT_EXIST;
		sprintf(buf, "*%s* no existe en SD!!!\nCreating new access register\n", name);
		Send_Uart(buf);
		Create_File(name);
		break;
	default:
		sprintf(buf, "Ocurrio un error desconocido!!!");
		Send_Uart(buf);
	}
	Unmount_SD("/");
	vPortFree(buf);
	return State;
}

fileState verifyDatabase(char *name){
	fileState State;
	char *buf = pvPortMalloc(100*sizeof(char));
	Mount_SD("/");
	fr = f_stat (name, &fno);					//Compruebo si existe el archivo
	switch(fr){
	case FR_OK:
		State = FILE_EXISTS;
		sprintf(buf, "*%s* existe en SD. OK.\n", name);
		Send_Uart(buf);
		break;
	case FR_NO_FILE:
	case FR_NO_PATH:
		State = FILE_DOESNT_EXIST;
		sprintf(buf, "*%s* no existe en SD!!! Cargar base de datos y reiniciar\n", name);
		Send_Uart(buf);
		break;
	default:
		sprintf(buf, "Ocurrio un error desconocido!!!");
		Send_Uart(buf);
	}
	Unmount_SD("/");
	vPortFree(buf);
	if(State == FILE_DOESNT_EXIST){
		while(1);
	}
	return State;
}




