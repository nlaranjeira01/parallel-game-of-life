#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <omp.h>

struct stLattice{
    unsigned char *buff0;
    unsigned char *buff1;
    unsigned char *buffSum;
    int width;
    int height;
    int steps;
    int threads;
};
typedef struct stLattice tpLattice;

void InitRandness(tpLattice *mLattice, float p);
void GameOfLife(tpLattice *mLattice);
void print2File(tpLattice *mLattice);
int main(int ac, char**av)
{
    tpLattice mLattice;
    //Inicializa variável
    mLattice.width  = atoi(av[1]);
    mLattice.height = atoi(av[2]);
    mLattice.steps  = atoi(av[3]);

    int flagSave = atoi(av[4]);
    float prob   = atof(av[5]);

    mLattice.threads = -1;
    if (ac > 6)
      mLattice.threads = atoi(av[6]);

    if (mLattice.threads == -1)
      mLattice.threads = omp_get_num_procs();

    fprintf(stdout, "\nGame of life");
    fprintf(stdout, "\nDominio(%d, %d, %d) Prob. %5.3f Threads %d\n",   mLattice.width,   mLattice.height, mLattice.steps, prob, mLattice.threads);

    fflush(stdout);

    mLattice.buff0 = (unsigned char*) malloc (mLattice.width *   mLattice.height *  sizeof(unsigned char));
    mLattice.buff1 = (unsigned char*) malloc (mLattice.width *   mLattice.height *  sizeof(unsigned char));
    InitRandness(&mLattice, prob);

    for (int t = 0; t < mLattice.steps; t++)
    {
      GameOfLife(&mLattice);
      unsigned char *swap = mLattice.buff0;
      mLattice.buff0 = mLattice.buff1;
      mLattice.buff1 = swap;
    }

    if (flagSave == 1)
      print2File(&mLattice);

    free(mLattice.buff0);
    free(mLattice.buff1);
    return EXIT_SUCCESS;
}

/*
 * Função utilizada para iniciar a matriz. Não mudar o valor constante do seed do rand
 */
void InitRandness(tpLattice *mLattice, float p){
  memset(mLattice->buff0, 0x00,  mLattice->width *   mLattice->height *  sizeof(unsigned char));
  memset(mLattice->buff1, 0x00,  mLattice->width *   mLattice->height *  sizeof(unsigned char));
  srand (42);
  for (int j = 1; j < mLattice->height - 1; j++){
      for (int i = 1; i < mLattice->width - 1; i++){
          int k = j * mLattice->width  +  i;
          float r = (rand() / (float)RAND_MAX);
          if (r <= p)
            mLattice->buff0[k] = 1;

      }//end-  for (int i = 0; i < mLattice->width; i++){
  }//end-for (int j = 0; j < mLattice->height; j++){
}//end-void InitRandness(tpLattice *mLattice, float p){

/*
 * Função que resolve o GOL. Adota-se os chamados pontos fantasmas como condição de contorno, ou seja,
 * os elementos da borda não são alterados em nenhum dos dois buffers.
 * Adota-se zero para esses pontos como valor padrão
 */
void GameOfLife(tpLattice *mLattice){
    int nw = -1, n = -1, ne = -1, w = -1, e = -1, sw = -1, s = -1, se = -1, c = -1, sum;
    int left, right, up, down, center;
    /*
        nw | n | ne
       ----|---|----
        w  | c |  e
       ----|---|----
        sw | s | se
    */
    unsigned char* buff0 = mLattice->buff0;
    unsigned char* buff1 = mLattice->buff1;

    int lowerLimit = mLattice->height - 1;
    int width = mLattice->width;
    int rightLimit = mLattice->width - 1;

    #pragma omp parallel for num_threads(mLattice->threads) private(up, down, center, left, right, nw, n, ne, w, c, sw, s, se, sum)
    for (int j = 1; j < lowerLimit; j++){
        up = (j - 1) * width;
        down = (j + 1) * width;
        center = j * width;

        for (int i = 1; i < rightLimit; i++){
          left = i-1;
          right = i+1;

          nw = buff0[up +  left];
           n = buff0[up +  i];
          ne = buff0[up +  right];
          w  = buff0[center +  left];
          c  = buff0[center  +  i];
          e  = buff0[center  +  right];
          sw = buff0[down +  left];
          s  = buff0[down +  i];
          se = buff0[down +  right];

          sum = nw + n + ne + w + e + sw + s + se;

          buff1[center + i] = c == 1 && sum == 2 || sum == 3;
        }
    }
}

/*
 * Função para imprimir para arquivo. Formato do arquivo .txt
 */
void print2File(tpLattice *mLattice)
{
  fprintf(stdout, "Save to file: game_of_life_parallel.txt");
  FILE *ptr = fopen("game_of_life_parallel.txt", "w+");
  assert(ptr  != NULL);

  for (int j = 1; j < mLattice->height - 1; j++){
      for (int i = 1; i < mLattice->width - 1; i++){
          int k = j * mLattice->width  +  i;
          if (mLattice->buff0[k] == 1)
            fputc('#', ptr);
          else
           fputc(' ', ptr);
      }//end-  for (int i = 0; i < mLattice->width; i++){
      fputc('\n', ptr);
  }//end-for (int j = 0; j < mLattice->height; j++){

  fclose(ptr);
  fprintf(stdout, "\t[OK]\n");

}
