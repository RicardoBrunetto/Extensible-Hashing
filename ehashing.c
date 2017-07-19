#include "ehashing.h"

/*Função que cria um bucket novo de acordo com a especificação do diretório*/
Bucket * novo_bucket_vazio(Diretorio d){
  /*Aloca o vetor de chaves do bucket*/
  TipoChave * chaves = (TipoChave *)malloc(sizeof(TipoChave) * d.Bucket_Size);
  /*Cria o bucket e preenche seus atributos padrão*/
  Bucket * bucket = (Bucket *)malloc(sizeof(Bucket));
  bucket->chaves = chaves;
  bucket->profundidade = 0;
  bucket->id = 0;
  bucket->count = 0;
}

/*Procedimento que inicaliza um diretório*/
Diretorio * inicializar_diretorio(int bucket_size){
  /*Aloca o espaço em memória para o diretório e preenche seus atributos padrão*/
  Diretorio * dir = (Diretorio *)malloc(sizeof(Diretorio));
  dir->Bucket_Size = bucket_size;
  dir->Profundidade_Global = 0;
  /*Aloca memória para a primeira célula do diretório*/
  Celula * celula = (Celula *)malloc(sizeof(Bucket));
  Bucket * bucket = novo_bucket_vazio(*dir);  /*Cria um novo bucket*/
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
  Celula * novas_celulas = (Celula *)malloc(sizeof(Celula) * (2*size));
  for(i=0; i<size; i++){
      novas_celulas[2*i].bucket_ref = d->celulas[i].bucket_ref;
      novas_celulas[2*i + 1].bucket_ref = d->celulas[i].bucket_ref;
  }
  free(d->celulas);
  d->celulas = novas_celulas;
  d->Profundidade_Global = d->Profundidade_Global + 1;
}

void dir_ins_bucket(Diretorio * d, Bucket * bucket, int start, int end){
  int i;
  for(i=start; i<=end; i++){
    d->celulas[i].bucket_ref = bucket;
  }
}

void find_new_range(Diretorio *d, Bucket * old_bucket, int * new_start, int * new_end){
  int mask = 1, i;
  int shared_address = make_address(old_bucket->chaves[0], old_bucket->profundidade);
  int new_shared = shared_address << 1;
  new_shared = new_shared | mask;
  int bits_to_fill = d->Profundidade_Global - (old_bucket->profundidade + 1);
  new_start = new_shared;
  new_end = new_shared;
  for(i=1; i<bits_to_fill; i++){
    new_start = *new_start << 1;
    new_end = *new_end << 1;
    new_end = *new_end | mask;
  }
}

int bk_split(Diretorio *d, Bucket * bucket){
  if(d->Profundidade_Global == bucket->profundidade) dir_double(d);
  Bucket * novo_bucket = novo_bucket_vazio(*d);
  int * new_start, * new_end;
  find_new_range(d, bucket, new_start, new_end);
  dir_ins_bucket(d, novo_bucket, new_start, new_end);
  bucket->profundidade = bucket->profundidade + 1;
  novo_bucket->profundidade = bucket->profundidade;
  novo_bucket->id = bucket->id + 1;
  //TODO: Redistribuir as chaves entre os buckets
}

int bk_add_key(Diretorio * d, Bucket * bucket, TipoChave chave){
  if(bucket->count < d->Bucket_Size){
    bucket->chaves[bucket->count] = chave;
    bucket->count = bucket->count+1;
  }else{
    bk_split(d, bucket);
    op_add(d, chave);
  }
}

int op_find(Diretorio *d, TipoChave chave, Bucket **found_Bucket){
  int endereco = make_address(chave, d->Profundidade_Global), i;
  *found_Bucket = d->celulas[endereco].bucket_ref;
  for(i=0; i<d->Bucket_Size; i++)
    if((*found_Bucket)->chaves[i] == chave) return SUCESSO;
  return FALHA;
}

int op_add(Diretorio *d, TipoChave chave){
  Bucket *found_Bucket;
  if(op_find(d, chave, &found_Bucket)) return FALHA;
  bk_add_key(d, found_Bucket, chave);
  return SUCESSO;
}









/*Procedimento que mostra as informações de um bucket*/
void print_bucket(Bucket *b, Diretorio *d){
  printf("\nBucket #%d\n", b->id);
  printf("\tQuantidade de chaves: %d\n", b->count);
  printf("\tProfundidade: %d\n", b->profundidade);
  int i;
  for(i=0; i<b->count; i++)
    printf("\t\t->Chave %d: %d\n", i, b->chaves[i]);
}

/*Procedimento que mostra as informações de um diretório*/
void print_diretorio(Diretorio *d){
  printf("\nInformacoes do diretorio\n\t->Profundidade Global: %d\n\t->Tamanho do Bucket: %d\n", d->Profundidade_Global, d->Bucket_Size);
  int i;
  int size = pow(2, d->Profundidade_Global);

  /*Printando as células*/
  for(i=0; i<size; i++){
    printf("\nCelula [%2d] -> Bucket #%d\n", i, d->celulas[i].bucket_ref->id);
  }

  /*Armazena o id do último bucket printado - para não repetir os prints*/
  int id_antecessor = -1;
  for(i=0; i<size; i++){
    int id_atual = d->celulas[i].bucket_ref->id; /*Variável auxiliar*/
    if(id_atual != id_antecessor){
      print_bucket(d->celulas[i].bucket_ref, d);
      id_antecessor = id_atual;
    }
  }
}
