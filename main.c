#include "ehashing.h"

void read_input(FILE * f){
  char buffer[50];
  while(fgets(buffer, 50, f) != 0){
    int chave = atoi(buffer);

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
  op_add(d, 12);
  op_add(d, 5);

  print_diretorio(d);

  op_add(d, 15);
  op_add(d, 13);
  op_add(d, 14);
  op_add(d, 7);

  print_diretorio(d);

  system("pause");
  return 0;
}
