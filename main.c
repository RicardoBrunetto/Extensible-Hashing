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

  print_diretorio(d);

  printf("\n\nArquivo log.txt contem o diretorio impresso.\n\n");
  system("pause");
  system("notepad log.txt");
  return 0;
}
