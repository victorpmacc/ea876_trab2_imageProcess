#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>

#include "imageprocessing.h"
#define N 5
#define QTTEST 6

imagem img;

/* Cada thread lidará com uma das 3 matrizes de cores da imagem, similar ao multiprocess: por não operarem sob mesmo argumento, não há necessidade de criação de trava com mutex*/
void* funcao_thread(void *arg){
  float* matrizCOR = malloc(sizeof(float) * img.height * img.width);
  memcpy(matrizCOR, arg, (sizeof(float) * img.height * img.width)); //matrizCOR contem a matriz de determinada cor (entre R G e B) da imagem, passada por arg
  float COR = 0, npixel = 0;
  
  for (int i=0; i<(img.width); i++){
    for (int j=0; j<(img.height); j++){
      for (int x = j-N; x < j+N; x++){ // percorre vizinhança na altura
        if (x >= 0 && x < img.height){ // condições para cálculo a respeito da altura
          for (int y = i-N; y < i+N; y++){ // percorre vizinhança na largura
            if (y >= 0 && y < img.width){ // condições para cálculo a respeito da largura
              COR = COR + matrizCOR[x * img.width + y];
              npixel++;
            }
          }
        }
      }
      ((float*)arg)[j * img.width + i] = COR / npixel; // pixel com efeito do blur é passado diretamente à sua posição no arg (img.COR)
      COR = 0;
      npixel =0; 
    }
  }
  return NULL;
}


int main() {
  img = abrir_imagem("data/cachorro.jpg");
  
  pthread_t pr, pg, pb;
  
  struct timeval rt0, rt1, drt;
  int t = 0;
  float vetor_tempos[QTTEST];
 
  printf("Execuções do arquivo (de 100):\n");
  while (t < QTTEST){ // O código será executado 100 vezes e os tempos referentes a cada execução salvos em vetor_tempos
    printf("%d\n",t+1);
    float tempo = 0;
    gettimeofday(&rt0, NULL); // inicia contagem do tempo de execução
  
    void* argR = malloc(sizeof(float *));
    void* argG = malloc(sizeof(float *));
    void* argB = malloc(sizeof(float *));
  
    argR = img.r; // os argumentos passados são proprimanete as matrizes de definição das cores da imagem 
    argG = img.g;
    argB = img.b;
  
  
    pthread_create(&(pr), NULL, funcao_thread, argR); // iniciadas threads
    pthread_create(&(pg), NULL, funcao_thread, argG);
    pthread_create(&(pb), NULL, funcao_thread, argB);
  
    pthread_join(pr, NULL); // retornadas
    pthread_join(pg, NULL);
    pthread_join(pb, NULL);
    
    gettimeofday(&rt1, NULL);
    timersub(&rt1, &rt0, &drt);
    tempo = (double)drt.tv_sec + ((double)drt.tv_usec/1000000); // criamos um float segundos.microssegundos
  
    printf("Tempo de execução %d: %f\n", t+1, tempo);
  
    vetor_tempos[t] = tempo;
    t++;
  
  }
  
  /*jogar os valores do vetor para um arquivo de leitura*/
  FILE *fp = fopen("doc/tempos_multithread.txt", "wb");
  for(int w = 0; w < QTTEST-1; w++)
    fprintf(fp, "%f,", vetor_tempos[w]);
  fprintf(fp, "%f", vetor_tempos[QTTEST-1]);
  fclose(fp);
  
  salvar_imagem("out/cachorro-out-multithread.jpg", &img);
  liberar_imagem(&img);
  return 0;
}
