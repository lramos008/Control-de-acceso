#include "sd.h"

void checkUserOnSD(char *userKey){
	Mount_SD("/");
	if(fileIsOnSD("registro_personas.csv") == FR_NO_FILE){
		Create_File("registro_personas.csv");
	}

}
