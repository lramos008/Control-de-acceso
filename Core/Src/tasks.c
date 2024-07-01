#include "main.h"
#include "cmsis_os.h"
#include "fatfs.h"
#include "tasks.h"

FSM StateMachine;
extern QueueHandle_t uiQueue;

void VoiceProcessing(void *pvParameters){

}

void KeypadScanning (void *pvParameters){
	eventoSecuencia eventoEnviado;
	uint8_t keyPressed;
	FSM *FSM1 = &StateMachine;
	FSM1->CurrentState = ESPERA_DIGITO_1;
	/*Inicio el funcionamiento con la pantalla inicial*/
	eventoEnviado = PANTALLA_INGRESE_CLAVE;
	xQueueSend(uiQueue, (void * ) &eventoEnviado, portMAX_DELAY);
	while(1){
		switch(FSM1->CurrentState){
		case ESPERA_DIGITO_1:
			/*Parseo la primer tecla*/
			keyPressed = GetKey();
			if(keyPressed != 0){
            	if((keyPressed != BORRAR_DIGITO) && (keyPressed != ENTER)){
					FSM1->CurrentIndex = 0;												//Reinicializo indice del buffer
					FSM1->Buffer[FSM1->CurrentIndex] = keyPressed;						//Guardo el primer digito
					FSM1->CurrentIndex++;
					FSM1->StartTime = xTaskGetTickCount();								//Guardo tiempo de inicio para el timeout
					FSM1->CurrentState = ESPERA_DIGITO_2;								//Config estado para parsear la sig tecla
					eventoEnviado = PANTALLA_AÑADIR_ENTRADA;							//Defino indicaciones para el display
            	}
			}
			else{
				eventoEnviado = PANTALLA_INGRESE_CLAVE;
			}
			break;
		case ESPERA_DIGITO_2:
			eventoEnviado = HandleInput(FSM1);
			break;
		case ESPERA_DIGITO_3:
			eventoEnviado = HandleInput(FSM1);
			break;
		case ESPERA_DIGITO_4:
			eventoEnviado = HandleInput(FSM1);
			break;
		case ESPERA_DIGITO_5:
			eventoEnviado = HandleInput(FSM1);
			break;
		case ESPERA_DIGITO_6:
			eventoEnviado = HandleInput(FSM1);
			break;
		case ESPERA_ENTER:
			if((xTaskGetTickCount() - FSM1->StartTime) < pdMS_TO_TICKS(KEYPAD_TIMEOUT)){
				keyPressed = GetKey();
				if(keyPressed != 0){
					if(keyPressed == BORRAR_DIGITO){
						FSM1->CurrentIndex--;
						FSM1->Buffer[FSM1->CurrentIndex] = 0;					//El digito obtenido en el estado anterior lo borro
						FSM1->StartTime = xTaskGetTickCount();					//Tomo nueva referencia para calcular timeout
						FSM1->CurrentState--;									//Vuelvo al estado anterior
						eventoEnviado = PANTALLA_BORRAR_ENTRADA;
					}
					else{
						if(keyPressed == ENTER){
							eventoEnviado = PANTALLA_ACCESO_CONCEDIDO;
							ClearBuffer(FSM1->Buffer, 6);
							FSM1->CurrentState = ESPERA_DIGITO_1;
						}
					}
				}
				else{
					eventoEnviado = PANTALLA_IDLE;								//No actualizar la pantalla
				}
			}
			else{
				FSM1->CurrentState = TIMEOUT;
				eventoEnviado = PANTALLA_TIMEOUT;

			}
			break;
		case TIMEOUT:
			ClearBuffer(FSM1->Buffer, 6);								//Limpio buffer y vuelvo a estado inicial
			FSM1->CurrentState = ESPERA_DIGITO_1;
			eventoEnviado = PANTALLA_IDLE;
			break;
		case INCOMPLETO:
			ClearBuffer(FSM1->Buffer, 6);
			FSM1->CurrentState = ESPERA_DIGITO_1;
			eventoEnviado = PANTALLA_IDLE;
			break;
		default:
			ClearBuffer(FSM1->Buffer, 6);
			FSM1->CurrentState = ESPERA_DIGITO_1;
			eventoEnviado = PANTALLA_IDLE;
			break;
		}
		/*Si el evento es PANTALLA_IDLE no se envian indicaciones al display*/
		/*En ese caso se deja la pantalla previa*/
		if(eventoEnviado != PANTALLA_IDLE) xQueueSend(uiQueue, (void * ) &eventoEnviado, portMAX_DELAY);
	}
}

void ScreenManager  (void *pvParameters){
	eventoSecuencia eventoRecibido = PANTALLA_INGRESE_CLAVE;
	uint8_t x = 10;														//x e y definen coordenadas en pantalla
	uint8_t y = 26;
	displayInit();
	while(1){
		xQueueReceive(uiQueue, &eventoRecibido, portMAX_DELAY);			//La tarea espera indicaciones para cambiar la pantalla
		switch(eventoRecibido){
		case PANTALLA_INGRESE_CLAVE:
			x = 10;
			displayInsertKeyMsg();
			break;
		case PANTALLA_AÑADIR_ENTRADA:
			ssd1306_SetCursor(x, y);
			ssd1306_WriteString("*", Font_16x26, White);				//Añade *'s para representar ingreso de digitos
			ssd1306_UpdateScreen();
			x += 16;													//Avanzo una posición en pantalla acorde al font seleccionado
			break;
		case PANTALLA_BORRAR_ENTRADA:
			x -= 16;													//Retrocedo una posicion en pantalla acorde al font seleccionado
			ssd1306_SetCursor(x, y);
			ssd1306_WriteString(" ", Font_16x26, White);				//En caso de borrar relleno con espacio en blanco
			ssd1306_UpdateScreen();
			break;
		case PANTALLA_TIMEOUT:
			displayTimeoutMsg();
			HAL_Delay(3000);											//Timer bloqueante para evitar el ingreso de otra secuencia
			break;														//mientras se muestra el mensaje.
		case PANTALLA_SECUENCIA_INCOMPLETA:
			displayIncompleteMsg();
			HAL_Delay(3000);
			break;
		case PANTALLA_ACCESO_CONCEDIDO:
			displayAccessState();
			HAL_Delay(3000);
			break;
		case PANTALLA_ACCESO_DENEGADO:
			break;
		case PANTALLA_IDLE:
			/*No hacer nada*/
			break;
		default:
			break;
		}
	}
}
void LockControl(void *pvParameters){

}

void sdHandler(void *pvParameters){
	/*Compruebo si existe el archivo de ingreso de personas*/
	Mount_SD("/");
	if(fileIsOnSD("registro_personas.csv" == FR_NO_FILE)){
		Create_File("registro_personas.csv");
	}
}
void IdleTask(void *pvParameters){
	while(1){

	}
}
