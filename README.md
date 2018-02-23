# Extensible-Hasing
Este trabalho foi desenvolvido para a disciplina de Organização e Recuperação de Dados (Ciência da Computação - UEM) em Agosto/2017 por:
- Rafael Rodrigues dos Santos (ra94075@uem.br)
- Ricardo Henrique Brunetto (ra94182@uem.br)

## Funcionalidade
Dado um arquivo de entrada `chaves.txt` contendo chaves numéricas, o programa solicita o tamanho que cada *bucket* deve ter e aplica um Hash Extensível nas chaves.
Por fim, uma listagem completa do diretório é impressa no arquivo `log.txt`.

## Especificações Tecnológicas
Todo o programa foi escrito em C. O `makefile` local pode ser perfeitamente utilizado, caso haja um `make` instalado no Windows, visto que não há dependências externas que requeiram complexidade. Caso contrário, deve-se seguir as instruções no relatório.

## Implementação
O relatório com detalhes de implementação pode ser encontrado [aqui](Documentação/Relatório.pdf).

## Lista de Afazeres
- [x] Definição das estruturas
- [x] Implementações
  - [x] make_address
  - [x] op_find
  - [x] op_add
  - [x] bk_add_key
  - [x] bk_split
  - [x] dir_double
  - [x] file_new_range
  - [x] dir_ins_bucket
- [x] Redistribuição das chaves
  - *(Apenas após a implementação desta pode-se garantir o bom funcionamento do programa)*
- [x] Função de leitura do arquivo
- [x] Interface da main

## Licença
Este projeto segue a licença [Creative Commons Attribution-ShareAlike (BY-SA)](https://creativecommons.org/licenses/by-sa/4.0/), que está detalhada no arquivo [`LICENSE.md`](LICENSE.md).
<p align="center">
  <img src="https://licensebuttons.net/l/by-sa/3.0/88x31.png">
</p>
