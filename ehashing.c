#include "ehashing.h"
#include <math.h>

/*Função que cria um bucket novo de acordo com a especificação do diretório*/
Bucket * novo_bucket_vazio(Diretorio * d, int temp){
  /*Aloca o vetor de chaves do bucket*/
  TipoChave * chaves = (TipoChave *)malloc(sizeof(TipoChave) * (d->Bucket_Size));
  /*Cria o bucket e preenche seus atributos padrão*/
  Bucket * bucket = (Bucket *)malloc(sizeof(Bucket));
  bucket->chaves = chaves;
  bucket->profundidade = 0;
  if(!temp){
    bucket->id = d->bucket_id;
    d->bucket_id = d->bucket_id + 1;
  }else{
    bucket->id = FLAG_TEMP;
  }
  bucket->count = 0;
  return bucket;
}

/*Procedimento que inicaliza um diretório*/
Diretorio * inicializar_diretorio(int bucket_size){
  /*Aloca o espaço em memória para o diretório e preenche seus atributos padrão*/
  Diretorio * dir = (Diretorio *)malloc(sizeof(Diretorio));
  dir->Bucket_Size = bucket_size;
  dir->Profundidade_Global = 0;
  dir->bucket_id = 0;
  /*Aloca memória para a primeira célula do diretório*/
  Celula * celula = (Celula *)malloc(sizeof(Bucket));
  Bucket * bucket = novo_bucket_vazio(dir, 0);  /*Cria um novo bucket*/
  celula->bucket_ref = bucket;  /*Insere o bucket na célula*/
  dir->celulas = celula; /*Inicializa as células do diretório com uma apenas*/
  return dir;
}

/*Função de hash - identidade*/
int hash(TipoChave chave){ return chave; }

/*Função make_address, que converte a chave em um endereço do diretório*/
int make_address(TipoChave chave, int profundidade){
  int retval = 0, mask = 1, i, lowbit;
  int hashval = hash(chave);
  for(i=0; i<profundidade; i++){
    retval = retval << 1;
    lowbit = hashval & mask;
    retval = retval | lowbit;
    hashval = hashval >> 1;
  }
  return retval;
}

void dir_double(Diretorio * d){
  int size = pow(2, d->Profundidade_Global), i;
  Celula * novas_celulas = (Celula *) malloc(sizeof(Celula) * (2*size));
  for(i=0; i<size; i++){
    novas_celulas[2*i].bucket_ref = d->celulas[i].bucket_ref;
    novas_celulas[2*i + 1].bucket_ref = d->celulas[i].bucket_ref;
  }
  free(d->celulas);
  d->celulas = novas_celulas;
  d->Profundidade_Global = d->Profundidade_Global + 1;
}

void dir_ins_bucket(Diretorio *d, Bucket * novo, int start, int end){
  int i;
  for(i=start; i<=end; i++){
    d->celulas[i].bucket_ref = novo;
  }
}

void find_new_range(Diretorio * d, Bucket * old, int * new_start, int * new_end){
  int shared_address = make_address(old->chaves[0], old->profundidade);
  int mask = 1, new_shared, bits_to_fill, j;
  new_shared = shared_address << 1;
  new_shared = new_shared | mask;
  bits_to_fill = d->Profundidade_Global - (old->profundidade + 1);
  *new_start = new_shared; *new_end = new_shared;
  for(j=0; j<bits_to_fill; j++){
    *new_start = (*new_start) << 1;
    *new_end = (*new_end) << 1;
    *new_end = (*new_end) | mask;
  }
}

void bk_split(Diretorio *d, Bucket * original){
  if(original->profundidade == d->Profundidade_Global) dir_double(d);
  Bucket * novo_bucket = novo_bucket_vazio(d, 0);
  int new_start, new_end;
  find_new_range(d, original, &new_start, &new_end);
  dir_ins_bucket(d, novo_bucket, new_start, new_end);
  original->profundidade = original->profundidade + 1;
  novo_bucket->profundidade = original->profundidade;

  Bucket * temp = novo_bucket_vazio(d, 1);
  int i;
  for(i=0; i<(d->Bucket_Size); i++){
    temp->chaves[i] = original->chaves[i];
  }

  original->count = 0;

  for(i=0; i<(d->Bucket_Size); i++){
    int address = make_address(temp->chaves[i], d->Profundidade_Global);
    d->celulas[address].bucket_ref->chaves[d->celulas[address].bucket_ref->count] = temp->chaves[i];
    d->celulas[address].bucket_ref->count = d->celulas[address].bucket_ref->count + 1;
  }
  free(temp);
}

void bk_add_key(Diretorio *d, Bucket * b, TipoChave chave){
  /*Verifica se o bucket não está cheio*/
  if(b->count < d->Bucket_Size){
    b->chaves[b->count] = chave;
    b->count = b->count + 1;
  }else{
    bk_split(d, b);
    op_add(d, chave);
  }
}


int op_find(Diretorio * d, TipoChave chave, Bucket ** found_Bucket){
  /*Calcula o endereço a que a chave pertence*/
  int endereco = make_address(chave, d->Profundidade_Global);
  /*Obtém o bucket*/
  *found_Bucket = d->celulas[endereco].bucket_ref;
  int i;
  /*Procura nas chaves do bucket*/
  for(i=0; i<(*found_Bucket)->count; i++){
    if((*found_Bucket)->chaves[i] == chave) return SUCESSO;
  }
  return FALHA;
}

int op_add(Diretorio *d, TipoChave chave){
  Bucket * found_Bucket;
  /*Verifica se a chave já existe*/
  if(op_find(d, chave, &found_Bucket)) return FALHA;
  bk_add_key(d, found_Bucket, chave);
  print_diretorio(d);
  return SUCESSO;
}

/*Procedimento que mostra as informações de um bucket*/
void print_bucket(FILE *f, Bucket *b, Diretorio *d){
  fprintf(f, "\nBucket #%d\n", b->id);
  fprintf(f, "\tQuantidade de chaves: %d\n", b->count);
  fprintf(f, "\tProfundidade: %d\n", b->profundidade);
  int i;
  for(i=0; i<b->count; i++)
    fprintf(f, "\t\t->Chave %d: %d\n", i, b->chaves[i]);
}

/*Procedimento que mostra as informações de um diretório*/
void print_diretorio(Diretorio *d){
  FILE *f = fopen("log.txt", "w+");
  fprintf(f, "\nInformacoes do diretorio\n\t->Profundidade Global: %d\n\t->Tamanho do Bucket: %d\n\t->Quantidade de Buckets: %d\n", d->Profundidade_Global, d->Bucket_Size, d->bucket_id);
  int i;
  int size = pow(2, d->Profundidade_Global);

  /*Printando as células*/
  for(i=0; i<size; i++){
    fprintf(f, "\nCelula [%2d] -> Bucket #%d", i, d->celulas[i].bucket_ref->id);
  }
  fprintf(f, "\n");
  /*Armazena o id do último bucket printado - para não repetir os prints*/
  int id_antecessor = -1;
  for(i=0; i<size; i++){
    int id_atual = d->celulas[i].bucket_ref->id; /*Variável auxiliar*/
    if(id_atual != id_antecessor){
      print_bucket(f, d->celulas[i].bucket_ref, d);
      id_antecessor = id_atual;
    }
  }
  fclose(f);
}
