#include "classe.h"
#include <string.h>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define G_LEDS         3
#define BUTTONS        6


using namespace std;


 
int main(){
	const uint32_t trash = 0, ledg1 = 3, ledg2 = 12, ledg3 = 48, ledg4 = 192;
  	uint32_t bt = 0;

  	printf("Abrindo FPGA...\n");
  	int dev = open("/dev/de2i150_altera", O_RDWR);
	if(dev == -1){
		cout << "bugou";
		return -1;
	}
  	printf("Limpando memória da placa...\n");
  	write(dev, &trash, G_LEDS);
  	

   
      while(1){
      	//read(dev, &bt, BUTTONS);
		  if(read(dev, &bt, BUTTONS)){
		  	cout << bt << endl;
			  switch(bt){
				 case 14:
				    //oldId = id;
				    //opa = false;
				    //id = mylines[id].escolhas[0];
				    //write(dev, &ledg1, G_LEDS);
				    break;
				 case 13:
				    //oldId = id;
				    //opa = false;
				   //id = mylines[id].escolhas[1];
				    //write(dev, &ledg2, G_LEDS);
				    break;
				 case 11:
				    //oldId = id;
				    //opa = false;
				    //id = mylines[id].escolhas[2];
				    //write(dev, &ledg3, G_LEDS);
				    break;
				 case 7: //repetir sua escolha
				    //id = id;
				    //opa = false;
				    //al_rewind_audio_stream(mylines[id].som);
				    break;
				 default:
				 	break;
				 	//opa = false;
				    //id = -1;
			  }
		 }
		
   }

	close(dev);
    return 0;
}
