#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TAMMAX 10// dps troca todos os "10" de tamanho p "TAMMAX"

char NAMES[TAMMAX][30];

float gas_price, kml; // kml = km/l

typedef struct{
int value,v;
int path[TAMMAX];
}na;

na M[TAMMAX][TAMMAX];

void dataChange (FILE *log){
  printf("---------------CHANGE DATA------------------\n");
  //in console
  printf("-------------------DATA--------------------\n");
  printf("Enter the gas price:\n");
  scanf("%f", &gas_price);
  printf("Enter the vehicle's efficiency (kilometers per liter):\n");
  scanf("%f", &kml);
  printf("-------------------DATA--------------------\n\n");
  //in log
  fprintf(log,"---------------CHANGE DATA------------------\n");
  fprintf(log,"-------------------DATA--------------------\n");
  fprintf(log,"Gas price:%.2f\n", gas_price);
  fprintf(log, "efficiency: %.2f km/l\n", kml);
  fprintf(log,"-------------------DATA--------------------\n\n");

}


//define a diagonal do meio como 0, pois de uma cidade para ela mesma a distancia é 0 e os espaços foram usados para calcular as distancias minimas entre cada cidade
void resetMiddle(int n){
  for(int i =0; i<n; i++){
    M[i][i].value = 0;
  }
}

// seta um vetor como -1 pra quando percorrer, saber que -1 é value inválido.
void setVector(int vet[], int tam){
  for(int i=0;i<tam;i++){
    vet[i] = -1;
  }
}

// define uma matriz como um numero muito alto pra fazer a comparacao de menor numero depois
void maxMatriz(){
  for(int i = 0;i<10;i++){
    for(int j = 0;j<10;j++){
      M[i][j].value = 100000000;
      setVector(M[i][j].path, 10);
      M[i][j].v = 0;
    }
  }
}

//grava os NAMES do arquivo em um vetor de strings e coloca os valores das arestas na matriz com os numeros altos a partir dos caracteres que aparecem no final de cada linha
int readGraph(){

 int i=0,l=0,n,m,v=0,num=0;
  char character ;
  FILE *arquivo = fopen("Entrada.txt", "r");

  if(arquivo == NULL){exit(0);}


  for(character = fgetc(arquivo); character != EOF;character = fgetc(arquivo)){

    if(character != '(' && character != '\n'){ // se for alguma coisa que não é parenteses, é definição de numeração da cidade

      // define o indice pra cada cidade 

      for(;character!=' '; character = fgetc(arquivo)){}

      character = fgetc(arquivo);

      for(;character != '\n'; l++){

        NAMES[num] [l] = character;
        character = fgetc(arquivo);


      }
      NAMES[num] [l] = '\0';
      num++;
      l= 0;
    }

    else if (character == '\n'){}

    else {  

      i = 0;
      l = 0;
      v = 0;

      character = fgetc(arquivo);
      i = character - '1';
      character = fgetc(arquivo);
      character = fgetc(arquivo);
      l = character - '1';
      character = fgetc(arquivo);
      for(character = fgetc(arquivo); character >='0' && character <='9'; character = fgetc(arquivo)){  

        v= (v*10) + (character - '0');

      } 
      M[i][l].value = v;
      M[l][i].value = v;
    }
  }
  fclose(arquivo);
  return num;
} 

//menu para seleção do que o programa deve fazer, ta dentro de um while no main entao sempre roda até o programa dar exit.
int menu (){

  int escolha;
  printf("_________________menu_____________________\n\n");
  printf("Escolha o que o programa deve fazer:\n1) Calcular menor trajeto entre duas cidades\n2) Trajetos que retornam para a cidade atual\n3) menor trajeto entre uma cidade e outra passando por uma terceira\n4)mudar preço do combustivel e consumo\n5) sair do programa\n");
  printf("__________________________________________\n");

  scanf("%d", &escolha);

  return escolha;
  }

//função que encontra o menor path entre dois vertices comparando distancias de todas as possibilidades
void min(int n){
  int j, k, i;
  int aux=1;

  for (k = 0; k < n; k++){
  for (i = 0; i < n; i++){
  for (j = 0; j < n; j++){
    if(M[i][j].value>(M[i][k].value+M[k][j].value)){
      if((M[i][k].v == 2) && (M[k][j].v == 2)){
        //setando caminhos com uma parada
        M[i][j].path[M[i][j].v] = i;
        M[i][j].v++;
        M[i][j].path[M[i][j].v] = k;
        M[i][j].v++;
        M[i][j].path[M[i][j].v] = j;
        M[i][j].v++;
        M[i][j].value = M[i][k].value+M[k][j].value;
        //setando caminhos com uma parada
      }
      else if((M[i][k].v == 3) && (M[k][j].v == 2)){

        for(aux=0;aux<M[i][k].v;aux++){


          M[i][j].path[M[i][j].v] = M[i][k].path[aux];
          M[i][j].v++;

        }
        M[i][j].path[M[i][j].v] = j;
        M[i][j].v++;
        M[i][j].value = (M[i][k].value) + (M[k][j].value);
      }
      else if((M[i][k].v == 2) && (M[k][j].v == 3)){

        M[i][j].value = (M[i][k].value) + (M[k][j].value);
        M[i][j].path[M[i][j].v] = i;
        M[i][j].v++;
          for(aux=0;aux<M[k][j].v;aux++){

          M[i][j].path[M[i][j].v] = M[k][j].path[aux];
          M[i][j].v++; 
        }
        } 
      }
    }
  }
  }
}

// seta vetor de path entre cidades diretas
void defineCaminhos1(int n){


  for(int i = 0; i<n; i++){
    for(int j = 0; j<n; j++){

      if(M[i][j].value == 100000000  || M[i][j].value == 0){
        M[i][j].v=0;
      }
      else{
        M[i][j].v=0;
        M[i][j].path[M[i][j].v] = i;
        M[i][j].v++;
        M[i][j].path[M[i][j].v] = j;
        M[i][j].v++;
      }
    }
  }
} 

// questão b - tem q fazer ainda
void retorno(int n, int cidin, FILE *log){


  int dist;

  if (cidin==0){
    dist = M[cidin][n-1].value + M[n-1][n-2].value + M[n-2][n-3].value + M[n-3][n-4].value + M[n-4][n-5].value + M[n-5][cidin].value;

    printf ("\nUm dos caminhos que passa por todas as cidades saindo de %d é de %dkm\n", cidin, dist);
    printf("O path que deve ser feito é: ");
    fprintf(log, "O path que deve ser feito é: "); 
    for(int aux=0; aux < M[cidin][n-1].v; aux ++){
      printf("%d ", (M[cidin][n-1].path[aux])+1);
      fprintf(log, "%d ", (M[cidin][n-1].path[aux])+1); // printando no log  
    }
    for(int aux=1; aux < M[n-1][n-2].v; aux ++){
      printf("%d ", (M[n-1][n-2].path[aux])+1);
      fprintf(log, "%d ", (M[n-1][n-2].path[aux])+1); // printando no log   
    }
    for(int aux=1; aux < M[n-2][n-3].v; aux ++){
      printf("%d ", (M[n-2][n-3].path[aux])+1);
    }
    for(int aux=1; aux < M[n-3][n-4].v; aux ++){
      printf("%d ", (M[n-3][n-4].path[aux])+1);
      fprintf(log, "%d ", (M[n-3][n-4].path[aux])+1); // printando no log 
    }
    for(int aux=1; aux < M[n-4][n-5].v; aux ++){
      printf("%d ", (M[n-4][n-5].path[aux])+1);
      fprintf(log, "%d ", (M[n-4][n-5].path[aux])+1); // printando no log 
    }
      for(int aux=1; aux < M[n-5][cidin].v; aux ++){
      printf("%d ", (M[n-5][cidin].path[aux])+1);
      fprintf(log, "%d ", (M[n-5][cidin].path[aux])+1); // printando no log 
    }
    printf("\n");
    fprintf(log, "\n");
  }
  else if (cidin==1){
   dist = M[cidin][n-1].value + M[n-1][n-2].value + M[n-2][n-3].value + M[n-3][n-4].value + M[n-4][n-5].value + M[n-5][cidin].value;

    printf ("\nUm dos caminhos que passa por todas as cidades saindo de %d é de %dkm\n", cidin, dist);
    printf("O path que deve ser feito é: ");
    fprintf(log, "O path que deve ser feito é: "); // printando no log
    for(int aux=0; aux < M[cidin][n-1].v; aux ++){
      printf("%d ", (M[cidin][n-1].path[aux])+1);
      fprintf(log, "%d ", (M[cidin][n-1].path[aux])+1); // printando no log  
    }
    for(int aux=1; aux < M[n-1][n-2].v; aux ++){
      printf("%d ", (M[n-1][n-2].path[aux])+1);
      fprintf(log, "%d ", (M[n-1][n-2].path[aux])+1); // printando no log   
    }
    for(int aux=1; aux < M[n-2][n-3].v; aux ++){
      printf("%d ", (M[n-2][n-3].path[aux])+1);
    }
    for(int aux=1; aux < M[n-3][n-4].v; aux ++){
      printf("%d ", (M[n-3][n-4].path[aux])+1);
      fprintf(log, "%d ", (M[n-3][n-4].path[aux])+1); // printando no log 
    }
    for(int aux=1; aux < M[n-4][n-5].v; aux ++){
      printf("%d ", (M[n-4][n-5].path[aux])+1);
      fprintf(log, "%d ", (M[n-4][n-5].path[aux])+1); // printando no log 
    }
      for(int aux=1; aux < M[n-5][cidin].v; aux ++){
      printf("%d ", (M[n-5][cidin].path[aux])+1);
      fprintf(log, "%d ", (M[n-5][cidin].path[aux])+1); // printando no log 
    }
    printf("\n");
    fprintf(log, "\n");
  }
  else if (cidin==2){
    dist = M[cidin][n-1].value + M[n-1][n-2].value + M[n-2][n-3].value + M[n-3][n-4].value + M[n-4][n-5].value + M[n-5][cidin].value;

    printf ("\nUm dos caminhos que passa por todas as cidades saindo de %d é de %dkm\n", cidin, dist);
    printf("O path que deve ser feito é: ");
    fprintf(log, "O path que deve ser feito é: "); // printando no log
    for(int aux=0; aux < M[cidin][n-1].v; aux ++){
      printf("%d ", (M[cidin][n-1].path[aux])+1);
      fprintf(log, "%d ", (M[cidin][n-1].path[aux])+1); // printando no log  
    }
    for(int aux=1; aux < M[n-1][n-2].v; aux ++){
      printf("%d ", (M[n-1][n-2].path[aux])+1);
      fprintf(log, "%d ", (M[n-1][n-2].path[aux])+1); // printando no log   
    }
    for(int aux=1; aux < M[n-2][n-3].v; aux ++){
      printf("%d ", (M[n-2][n-3].path[aux])+1);
    }
    for(int aux=1; aux < M[n-3][n-4].v; aux ++){
      printf("%d ", (M[n-3][n-4].path[aux])+1);
      fprintf(log, "%d ", (M[n-3][n-4].path[aux])+1); // printando no log 
    }
    for(int aux=1; aux < M[n-4][n-5].v; aux ++){
      printf("%d ", (M[n-4][n-5].path[aux])+1);
      fprintf(log, "%d ", (M[n-4][n-5].path[aux])+1); // printando no log 
    }
      for(int aux=1; aux < M[n-5][cidin].v; aux ++){
      printf("%d ", (M[n-5][cidin].path[aux])+1);
      fprintf(log, "%d ", (M[n-5][cidin].path[aux])+1); // printando no log 
    }
    printf("\n");
    fprintf(log, "\n");
  }
  else if (cidin==3){
    dist = M[cidin][n-1].value + M[n-1][n-2].value + M[n-2][n-3].value + M[n-3][n-4].value + M[n-4][n-5].value + M[n-5][cidin].value;

    printf ("\nUm dos caminhos que passa por todas as cidades saindo de %d é de %dkm\n", cidin, dist);
    printf("O path que deve ser feito é: ");
    fprintf(log, "O path que deve ser feito é: "); // printando no log
    for(int aux=0; aux < M[cidin][n-1].v; aux ++){
      printf("%d ", (M[cidin][n-1].path[aux])+1);
      fprintf(log, "%d ", (M[cidin][n-1].path[aux])+1); // printando no log  
    }
    for(int aux=1; aux < M[n-1][n-2].v; aux ++){
      printf("%d ", (M[n-1][n-2].path[aux])+1);
      fprintf(log, "%d ", (M[n-1][n-2].path[aux])+1); // printando no log   
    }
    for(int aux=1; aux < M[n-2][n-3].v; aux ++){
      printf("%d ", (M[n-2][n-3].path[aux])+1);
    }
    for(int aux=1; aux < M[n-3][n-4].v; aux ++){
      printf("%d ", (M[n-3][n-4].path[aux])+1);
      fprintf(log, "%d ", (M[n-3][n-4].path[aux])+1); // printando no log 
    }
    for(int aux=1; aux < M[n-4][n-5].v; aux ++){
      printf("%d ", (M[n-4][n-5].path[aux])+1);
      fprintf(log, "%d ", (M[n-4][n-5].path[aux])+1); // printando no log 
    }
      for(int aux=1; aux < M[n-5][cidin].v; aux ++){
      printf("%d ", (M[n-5][cidin].path[aux])+1);
      fprintf(log, "%d ", (M[n-5][cidin].path[aux])+1); // printando no log 
    }
    printf("\n");
    fprintf(log, "\n");
  }
  else if (cidin==4){
    dist = M[cidin][n-1].value + M[n-1][n-2].value + M[n-2][n-3].value + M[n-3][n-4].value + M[n-4][n-5].value + M[n-5][cidin].value;

    printf ("\nUm dos caminhos que passa por todas as cidades saindo de %d é de %dkm\n", cidin, dist);
    printf("O path que deve ser feito é: ");
    fprintf(log, "O path que deve ser feito é: "); // printando no log
    for(int aux=0; aux < M[cidin][n-1].v; aux ++){
      printf("%d ", (M[cidin][n-1].path[aux])+1);
      fprintf(log, "%d ", (M[cidin][n-1].path[aux])+1); // printando no log  
    }
    for(int aux=1; aux < M[n-1][n-2].v; aux ++){
      printf("%d ", (M[n-1][n-2].path[aux])+1);
      fprintf(log, "%d ", (M[n-1][n-2].path[aux])+1); // printando no log   
    }
    for(int aux=1; aux < M[n-2][n-3].v; aux ++){
      printf("%d ", (M[n-2][n-3].path[aux])+1);
    }
    for(int aux=1; aux < M[n-3][n-4].v; aux ++){
      printf("%d ", (M[n-3][n-4].path[aux])+1);
      fprintf(log, "%d ", (M[n-3][n-4].path[aux])+1); // printando no log 
    }
    for(int aux=1; aux < M[n-4][n-5].v; aux ++){
      printf("%d ", (M[n-4][n-5].path[aux])+1);
      fprintf(log, "%d ", (M[n-4][n-5].path[aux])+1); // printando no log 
    }
      for(int aux=1; aux < M[n-5][cidin].v; aux ++){
      printf("%d ", (M[n-5][cidin].path[aux])+1);
      fprintf(log, "%d ", (M[n-5][cidin].path[aux])+1); // printando no log 
    }
    printf("\n");
    fprintf(log, "\n");
  }

            printf("\n a distancia do trajeto eh = %d km",dist);
          fprintf(log,"\n a distancia do trajeto eh = %d km",dist);

          printf("\n o custo do trajeto eh = %.2f reais\n",((dist/kml)*gas_price));
          fprintf(log,"\n o custo do trajeto eh = %.2f reais",(dist/kml)*gas_price);

}

//soma a distancia entre a cidade de inicio, cidade intermediaria e cidade final e junta os caminhos
int terceiracid(int a, int b, int c, FILE *log){
  int vetor[20];
  int i,iv=0,value=0;
  setVector(vetor, 20);

  for(i=0;(M[a][b].path[i]) != -1;i++){
    vetor[iv] = M[a][b].path[i];
    iv++;
  }
  for(i=1;(M[b][c].path[i]) != -1;i++){
    vetor[iv] = M[b][c].path[i];
    iv++;
  }
  for(i=0; vetor[i] != -1; i++){
    if (i==0){ 
      printf("[%d]%s",(vetor[i])+1, NAMES[vetor[i]]);
       fprintf(log, "[%d]%s",(vetor[i])+1, NAMES[vetor[i]]);
      }else{
    printf("--> [%d]%s",(vetor[i])+1, NAMES[vetor[i]]);
      fprintf(log, "--> [%d]%s",(vetor[i])+1, NAMES[vetor[i]]);
      }
  }

  value = (M[a][b].value)+(M[b][c].value);
  printf("\na distancia do menor path eh = %d Km\n",value);
  fprintf(log, "\na distancia do menor path eh = %d Km\n",value);

   printf("\n o custo do trajeto eh = %.2f reais\n", value/kml);
   fprintf(log,"\n o custo do trajeto eh = %.2f reais\n", (value/kml)*gas_price);

  return value;
}

//função principal que aplica as outras funcoes criadas, juntando os valores e imprimindo os resultados 
int main(void) {


  FILE* log = fopen("Log.txt", "a");
  if(log == NULL){exit(0);}



  printf("-------------------DATA--------------------\n");
  printf("Por favor, insira o preço da gasolina:\n");
  scanf("%f", &gas_price);
  printf("Por favor, insira quantos Km o caminhão faz por litro:\n");
  scanf("%f", &kml);
  printf("-------------------DATA--------------------\n\n");

  fprintf(log,"-------------------DATA--------------------\n");
  fprintf(log,"preço da gasolina:%.2f\n", gas_price);
  fprintf(log, "consumo: %.2f km/l\n", kml);
  fprintf(log,"-------------------DATA--------------------\n\n");

  maxMatriz();
  int n = readGraph();
  resetMiddle(n);
  defineCaminhos1(n);
  min(n);

  printf("\n");

  fprintf(log, "\n----------inicio do programa----------\n\n");

  int acao=-1;

    while(1){
    acao = menu();
    fprintf(log, "opcao selecionada: %d\n\n", acao);
    fprintf(log, "------comeco-da-acao------\n\n");
    printf("\n");

    if (acao==1){

      int cid1, cid2;
      printf("Escolha a cidade inicial:\n");
      scanf("%d", &cid1);
      cid1--;
      if(cid1<0 || cid1>=n){printf("cidade invalida\n");
        fprintf(log, "Cidade invalida\n");
        }
      else {
        printf("Escolha a cidade final:\n");
        scanf("%d", &cid2);
        cid2--;
        if(cid2<0 || cid2>=n){printf("cidade invalida\n");
           fprintf(log, "Cidade invalida\n");
          }
        else {

          fprintf(log, "cidade 1: %d\ncidade 2: %d\nmenor path: \n", (cid1+1),(cid2+1));

          for(int aux=0;aux<M[cid1][cid2].v;aux++){
            if (aux==0){
              printf("[%d]%s ",(M[cid1][cid2].path[aux])+1,NAMES[M[cid1][cid2].path[aux]]);
              fprintf(log,"[%d]%s ",(M[cid1][cid2].path[aux])+1,NAMES[M[cid1][cid2].path[aux]]);
            }else{
            printf("--> [%d]%s ",(M[cid1][cid2].path[aux])+1,NAMES[M[cid1][cid2].path[aux]]);
            fprintf(log,"--> [%d]%s ",(M[cid1][cid2].path[aux])+1,NAMES[M[cid1][cid2].path[aux]]);
              }
            }
          printf("\n a distancia do trajeto eh = %d km",M[cid1][cid2].value);
          fprintf(log,"\n a distancia do trajeto eh = %d km",M[cid1][cid2].value);

          printf("\n o custo do trajeto eh = %.2f reais",((M[cid1][cid2].value)/kml)*gas_price);
          fprintf(log,"\n o custo do trajeto eh = %.2f reais",((M[cid1][cid2].value)/kml)*gas_price);
        }
        printf("\n");
      }
    }

    else if(acao==2){
      int cidin=-1;
      printf("Digite a cidade inicial :\n");
      scanf("%d", &cidin);
      cidin--;
      if(cidin>=0 && cidin<n)
      retorno(n,cidin,log);
    }

    else if(acao==3){
      int comec,passag,fina;
      printf("Escolha a cidade inicial:\n");
      scanf("%d", &comec);
      comec--;

      if(comec<0 || comec>=n){printf("Cidade invalida\n");
         fprintf(log, "Cidade invalida\n");
        }
      else{
        printf("Escolha a cidade de passagem:\n");
        scanf("%d", &passag);
        passag--;
        if(passag<0 || passag>=n){printf("Cidade invalida\n");
           fprintf(log, "Cidade invalida\n");
          }
        else{
          printf("Escolha a cidade final:\n");
          scanf("%d", &fina);
          fina--;
          if(fina<0 || fina>=n){printf("Cidade invalida\n");
             fprintf(log, "Cidade invalida\n");
            }
          else{
            terceiracid(comec,passag,fina,log);
          }
        }
      }
    }
    else if(acao==4){
      dataChange(log);
    }
    else if(acao==5){
      fprintf(log, "------Program is endned------\n\n");
      return 1;
    }
    else {
      printf("\nInvalid choice\n");
    }
    fprintf(log, "\n\n------Action is endned------\n\n");
  }

}