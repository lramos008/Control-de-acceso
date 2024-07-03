#include "sd.h"
extern UART_HandleTypeDef huart2;
extern FATFS fs;  // file system
extern FIL fil; // File
extern FILINFO fno;
extern FRESULT fresult;  // result
extern UINT br, bw;  // File read/write count


fileState verifyAccessRegister(char *name){
	/*Esta funcion es un wrapper nada mas*/
	/**** chequea si existe o no el archivo ****/
	fileState State;
	char *buf = pvPortMalloc(100*sizeof(char));
	Mount_SD("/");
	fresult = f_stat (name, &fno);					//Compruebo si existe el archivo
	switch(fresult){
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
	fresult = f_stat (name, &fno);					//Compruebo si existe el archivo
	switch(fresult){
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


char *searchUserOnDatabase(char *userSequence, char *databaseName){
	/***Esta funcion busca en una base de datos .txt al usuario asociado a la secuencia ingresada.***/
	/***Si lo encuentra, devuelve puntero al usuario (str). Si no, devuelve USER_ERROR.***/
	/***Se devuelve FILE_ERROR si no se puede abrir el archivo correctamente.***/
	/*Reservo espacio en memoria*/
	char *buf = pvPortMalloc(100*sizeof(char));
	char *userName = pvPortMalloc(25 * sizeof(char));
	char *currentUserKey = pvPortMalloc(7 * sizeof(char));
	Mount_SD("/");
	fresult = f_open(&fil, databaseName, FA_READ);
	if (fresult != FR_OK){
		/*Esto se implementa con fines de debugging*/
		sprintf (buf, "Error al abrir archivo *%s*\n\n", databaseName);
		Send_Uart(buf);
		vPortFree(buf);
		return FILE_ERROR;
	}
	/*Comienza la busqueda del usuario*/
	while(f_gets(buf, 100, &fil)){										//Avanza linea a linea del archivo hasta el final
		userName = strtok(buf, " ");									//Usando este delimitador consigo primero el usuario
		currentUserKey = strtok(NULL, " ");								//Luego consigo la clave, que viene despues del espacio
		if(currentUserKey != NULL && strcmp(currentUserKey, userSequence) == 0){
			vPortFree(buf);
			vPortFree(currentUserKey);
			return userName;											/*Recordar liberar memoria de userName en la tarea*/
		}
	}
	/*Libero memoria y desmonto tarjeta SD*/
	vPortFree(buf);
	vPortFree(userName);
	vPortFree(currentUserKey);
	Unmount_SD("/");
	return USER_ERROR;	/*No existe el usuario*/
}



