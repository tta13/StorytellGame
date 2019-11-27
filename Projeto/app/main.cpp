#define ALLEGRO_NO_MAGIC_MAIN
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
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

#define N_AUDIOS 20
#define wave ".wav"

using namespace std;

ALLEGRO_AUDIO_STREAM *musica = NULL; 
 
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
  	
   	char a[] = "a.wav", b[] = "b.wav";
  	bool audios_Inicializados[20]={false};
   	int minhaMatriz[20][3] = {1,2,3,
                             2,4,99,
                             5,6,99,
                             1,2,7,
                             8,9,10,
                             11,12,99,
                             5,11,12,
                             13,14,99,
                             0,-1,-1,
                             10,15,16,
                             17,18,99,
                             12,16,99,
                             18,19,99,
                             12,15,16,
                             0,-1,-1,
                             0,-1,-1,
                             0,-1,-1,
                             0,-1,-1,
                             0,-1,-1,
                             0,-1,-1}; 
   int id = 0, oldId = 0;
   int minhaEscolha = -1;
   vector<linhas> mylines;
   for(int i = 0; i < N_AUDIOS; i++){
      mylines.push_back(linhas(i, minhaMatriz[i], ""));
   }
   cout << "Inicializei o vector de linhas" << endl;
   for(int i=0; i<N_AUDIOS; i++){
      char aux[10] = " ";
      int tales = i%10;
      if(i>9){
         aux[0]='1';
         aux[1]= tales+ '0';
      }
      else{
         aux[0] = i + '0';
      }
      strcat(aux, wave);
      cout << aux << endl;
      mylines[i].inicializar(aux);
   }
   while(id != -1){
      sleep(2);
      cout << "ID igual a " << id << endl;
      //poe audio no buffer lido pelo drive de som
      if(audios_Inicializados[id]){
         al_rewind_audio_stream(mylines[id].som);
      }
      audios_Inicializados[id]=true;
      al_attach_audio_stream_to_mixer(mylines[id].som, al_get_default_mixer());
      //seta playmode (uma vez)
      al_set_audio_stream_playmode(mylines[id].som, ALLEGRO_PLAYMODE_ONCE);
      //toca audio
      al_set_audio_stream_playing(mylines[id].som, true);
      //espera audio acabar
      while(al_get_audio_stream_playing(mylines[id].som)){}
      cout << "Sai do loop com sucesso" << endl;
      al_detach_audio_stream(mylines[id].som);
      //esperar leitura do driver de botao aqui
      //cin >> minhaEscolha;
      bool opa = true;
      while(opa){
      	//read(dev, &bt, BUTTONS);
		  if(read(dev, &bt, BUTTONS)){
		  	cout << bt << endl;
		  	 //cout << "opa";
			  switch(bt){
				 case 7:
				    //oldId = id;
				    opa = false;
				    id = mylines[id].escolhas[0];
				    write(dev, &ledg3, G_LEDS);
				    break;
				 case 11:
				    //oldId = id;
				    opa = false;
				    id = mylines[id].escolhas[1];
				    write(dev, &ledg2, G_LEDS);
				    break;
				 case 13:
				    //oldId = id;
				    opa = false;
				    id = mylines[id].escolhas[2];
				    write(dev, &ledg1, G_LEDS);
				    break;
				 case 14: //repetir sua escolha
				    id = id;
				    opa = false;
				    //al_rewind_audio_stream(mylines[id].som);
				    break;
				 
				 default:
				 	break;
			  }
		 }
		
	}
      
   }
    //al_destroy_sample(som_azul);
    //al_destroy_sample(som_verm);
   for(int i=0; i<N_AUDIOS; i++){
      al_destroy_audio_stream(mylines[i].som);
   }
	close(dev);
    return 0;
}
