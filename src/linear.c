#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

#include "imageprocessing.h"


#define N 5
#define QTTEST 6

int main() {
  imagem img, imgprocessada;
  
  img = abrir_imagem("data/cachorro.jpg");
  imgprocessada = abrir_imagem("data/cachorro.jpg");
  
  // Registro das somas dos valores dos pixels e a quantidade, para cálculo da média
  float R = 0, G = 0, B = 0, npixel = 0;
  
  struct timeval rt0, rt1, drt;
  float tempo;
  int t = 0;
  float vetor_tempos[QTTEST];
  
  printf("Execuções do arquivo (de 100):\n");
  while (t < QTTEST){ // O código será executado 100 vezes e os tempos referentes a cada execução salvos em vetor_tempos
  printf("%d\n",t+1);
  float tempo = 0;
  gettimeofday(&rt0, NULL); // inicia contagem do tempo de execução
  
  /* Blur Red */
  for (int i=0; i<(img.width); i++){
    for (int j=0; j<(img.height); j++){
      for (int x = j-N; x < j+N; x++){ // percorre vizinhança na altura
        if (x >= 0 && x < img.height){ // condições para cálculo a respeito da altura
          for (int y = i-N; y < i+N; y++){ // percorre vizinhança na largura
            if (y >= 0 && y < img.width){ // condições para cálculo a respeito da largura
              R = R + img.r[x * img.width + y];
              npixel++;
            }
          }
        }
      }
      imgprocessada.r[j * img.width + i] = R / npixel;
      R = 0;
      npixel =0; 
    }
  }
  
  /* Blur Green */
  for (int i=0; i<(img.width); i++){
    for (int j=0; j<(img.height); j++){
      for (int x = j-N; x < j+N; x++){ // percorre vizinhança na altura
        if (x >= 0 && x < img.height){ // condições para cálculo a respeito da altura
          for (int y = i-N; y < i+N; y++){ // percorre vizinhança na largura
            if (y >= 0 && y < img.width){ // condições para cálculo a respeito da largura
              G = G + img.g[x * img.width + y];
              npixel++;
            }
          }
        }
      }
      imgprocessada.g[j * img.width + i] = G / npixel;
      G = 0;
      npixel =0; 
    }
  }
  
  /* Blur Blue */
  for (int i=0; i<(img.width); i++){
    for (int j=0; j<(img.height); j++){
      for (int x = j-N; x < j+N; x++){ // percorre vizinhança na altura
        if (x >= 0 && x < img.height){ // condições para cálculo a respeito da altura
          for (int y = i-N; y < i+N; y++){ // percorre vizinhança na largura
            if (y >= 0 && y < img.width){ // condições para cálculo a respeito da largura
              B = B + img.b[x * img.width + y];
              npixel++;
            }
          }
        }
      }
      imgprocessada.b[j * img.width + i] = B / npixel;
      B = 0;
      npixel =0; 
    }
  }
  
  gettimeofday(&rt1, NULL);
  timersub(&rt1, &rt0, &drt);
  tempo = (double)drt.tv_sec + ((double)drt.tv_usec/1000000); // criamos um float segundos.microssegundos
  vetor_tempos[t] = tempo;
  printf("Tempo de execução %d: %f\n", t+1, tempo);
  t++;
}

  /*jogar os valores do vetor para um arquivo de leitura*/
  FILE *fp = fopen("doc/tempos_linear.txt", "wb");
  for(int w = 0; w < QTTEST-1; w++)
    fprintf(fp, "%f,", vetor_tempos[w]);
  fprintf(fp, "%f", vetor_tempos[QTTEST-1]);
  fclose(fp);
  
  /*salvar a imagem trabalhada*/
  salvar_imagem("out/cachorro-out_linear.jpg", &imgprocessada);
  liberar_imagem(&imgprocessada); 
  return 0;
}
