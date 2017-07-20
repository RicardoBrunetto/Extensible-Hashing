#include "ehashing.h"

void read_input(FILE * f){
  char buffer[50];
  while(fgets(buffer, 50, f) != 0){
    int chave = atoi(buffer);
    printf("\n%d", chave);
  }
}

int main(int argc, char ** argv){

  Diretorio *d;
  int k;

  /*O tamanho do bucket pode ser passado como parâmetro ou não*/
  if(argc == 2)
    k = atoi(argv[1]);
  else{
    printf("\nInforme o tamanho do Bucket: ");
    scanf("%d", &k);
  }

  /*Ocorre a inicialização do diretório com o tamanho do bucket especificado*/
  d = inicializar_diretorio(k);

  FILE *f = fopen("chaves.txt", "r");


  char buffer[50];
  while(fgets(buffer, 50, f) != 0){
    int chave = atoi(buffer);
    printf("\nInserindo: %d", chave);
    op_add(d, chave);
  }

  // op_add(d, 135220);
  // op_add(d, 213705);
  // op_add(d, 113602);
  // op_add(d, 153603);
  // op_add(d, 113603);
  // op_add(d, 223462);
  // op_add(d, 123316);
  // op_add(d, 184590);
  // op_add(d, 147242);
  // op_add(d, 106345);

  // op_add(d, 12);
  // op_add(d, 5);

  print_diretorio(d);

  // op_add(d, 15);
  // op_add(d, 13);
  // op_add(d, 14);
  // op_add(d, 7);
  //
  // print_diretorio(d);

  system("pause");
  return 0;
}
