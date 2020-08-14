#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h> /* define pid_t*/
#include <unistd.h> /* fork() */
#include <sys/wait.h> /* define waitpid*/
#include <string.h> /* memcpy */
#include <sys/time.h>

#include "imageprocessing.h"
#define N 5
#define QTTEST 6

int main() {
  int protection = PROT_READ | PROT_WRITE;
  int visibility = MAP_SHARED | MAP_ANON;
  
  /* A lógica seguirá execução de 3 processos simultâneos, um para cada matriz de cores (R, G, e B)*/
  imagem img, imgprocessada;
  img = abrir_imagem("../data/cachorro.jpg");
  imgprocessada = abrir_imagem("../data/cachorro.jpg");
  
  float* matrizR = (float*) mmap(NULL, sizeof(float) * img.height * img.width, protection, visibility, 0, 0);
  float* matrizG = (float*) mmap(NULL, sizeof(float) * img.height * img.width, protection, visibility, 0, 0);
  float* matrizB = (float*) mmap(NULL, sizeof(float) * img.height * img.width, protection, visibility, 0, 0);
  
  float R = 0, G = 0, B = 0, npixel = 0;
  
  struct timeval rt0, rt1, drt;
  int t = 0;
  float vetor_tempos[QTTEST];
 
  printf("Execuções do arquivo (de 100):\n");
  
  while (t < QTTEST){ // O código será executado 100 vezes e os tempos referentes a cada execução salvos em vetor_tempos
  printf("%d\n",t+1);
  float tempo = 0;
  gettimeofday(&rt0, NULL); // inicia contagem do tempo de execução
  
  /* Após chamada do fork de cada processo, a lógica é a mesma da execução única, a par de cada cor ser tratada de forma individual por um processo*/
  
  /* Matriz Red*/
  pid_t pr, pg, pb;
  pr = fork();
  if (pr == 0){
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
        matrizR[j * img.width + i] = R / npixel;
        R = 0;
        npixel =0; 
      }
    }
    exit(0);
  }

  /* Matriz Green*/
  pg = fork();
  if (pg == 0){
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
        matrizG[j * img.width + i] = G / npixel;
        G = 0;
        npixel =0; 
      }
    }
    exit(0);
  }
  
  /* Matriz Blue*/
  pb = fork();
  if (pb == 0){
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
        matrizB[j * img.width + i] = B / npixel;
        B = 0;
        npixel =0; 
      }
    }
    exit(0);
  }
  
  // Aguarda processos mudarem de estado
  waitpid(pr, NULL, 0);
  waitpid(pg, NULL, 0);
  waitpid(pb, NULL, 0);
  
  // Copia valores dos pixels com blur nas matrizes para a memória da imagem processada
  memcpy(imgprocessada.r, matrizR, sizeof(float) * img.height * img.width);
  memcpy(imgprocessada.g, matrizG, sizeof(float) * img.height * img.width);
  memcpy(imgprocessada.b, matrizB, sizeof(float) * img.height * img.width);
  
  gettimeofday(&rt1, NULL);
  timersub(&rt1, &rt0, &drt);
  tempo = (double)drt.tv_sec + ((double)drt.tv_usec/1000000); // criamos um float segundos.microssegundos
  
  printf("Tempo de execução %d: %f\n", t+1, tempo);
  
  vetor_tempos[t] = tempo;
  t++;
  }
  

  /*jogar os valores do vetor para um arquivo de leitura*/
  FILE *fp = fopen("../doc/tempos_multiprocess.txt", "wb");
  for(int w = 0; w < QTTEST-1; w++)
    fprintf(fp, "%f,", vetor_tempos[w]);
  fprintf(fp, "%f", vetor_tempos[QTTEST-1]);
  fclose(fp);
  
  salvar_imagem("../out/cachorro-out-multiprocess.jpg", &imgprocessada);
  liberar_imagem(&imgprocessada);
  return 0;
}
