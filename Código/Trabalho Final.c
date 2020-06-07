#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#define TAM 100

//Representacao do nodo da arvore
typedef struct Nodo
{
    int n;              // Numero de chaves no nodo
    int chaves[TAM];    // Vetor de chaves
    int filhos[TAM];    // Vetor de filhos
} no;

//Variaveis globais
no arvore; // Unica variavel da struct declarada,sendo global
int grauD;    // Unica variavel representado o grau Maximo da arvore
int elemento_splitado; // Variavel para armazenar o elemento splitado(split)
int filho; 
int ID_raiz; // Variavel para armazenar a raiz inicialmente, e se necessario trocada o valor
bool ok; // Variavel booleana para indicar que esta acontecendo o split
int leitura, escrita; // Variaveis usadas para indicar quantidade de leituras e escritas

// Encontra o maior do vetor(Auxiliar na Selection Sort Recursiva)
int Maior(int *vet, int tamanho)
{
    int maior= 0, aux =0, i = 0;

    for(i = 0 ; i<=tamanho ; i++)
    {
        if(i==0)
            maior = vet[i];

        if(vet[i] > maior)
        {
            maior = vet[i];
            aux = i;
        }
    }
    return aux;
}

//Executa a função de trocar elementos em um vetor
void troca(int *vet,int maior,int posicao_final)
{
    int aux = vet[maior];
    vet[maior] = vet[posicao_final];
    vet[posicao_final] = aux;
}

//Função para ordenar o vetor e modificar o vetor de chaves
void SelectionSortRecursivo(int *vet,int tamanho)
{
    if(tamanho==1)
    {
        return;
    }

    int maior = Maior(vet,tamanho-1);
    troca(vet,maior,tamanho-1);
    SelectionSortRecursivo(vet,tamanho-1);
}

//Abre o arquivo ID.dat e carrega o nodo na struct arvore
void le(int ID)
{
    int i;
    char ID_temp[10]; // String auxiliar no fopen

    sprintf(ID_temp, "%d.dat", ID); //Coloca na string ID o ID concatenado com o .dat

    FILE *nodo; // Ponteiro para o tipo arquivo

    nodo = fopen(ID_temp, "r"); // Modo leitura

    fscanf(nodo, "%d", &arvore.n); // Atribui na struct o numero de filhos

    for(i = 0; i<arvore.n; i++)
    {
        fscanf(nodo, "%d", &arvore.chaves[i]); // Lê os valores das chaves
    }

    for(i = 0; i <(arvore.n+1); i++)
    {
        fscanf(nodo, "%d", &arvore.filhos[i]);  // Lê os valores dos filhos
    }

    // Mostra na tela o que foi lido
    printf("Lido ("); 

    for(i = 0; i<arvore.n; i++)
    {
        if(i+1==arvore.n)
        {
            printf("%d", arvore.chaves[i]);
        }
        else
        {
            printf("%d ", arvore.chaves[i]);
        }
    }
    printf(")\n");

    leitura++; // A cada leitura, adiciona no contador

    fclose(nodo);
}

// Busca a chave x na arvore
void busca(int ID, int chave)
{
    int j;

    le(ID); //Abre o arquivo ID.dat e carrega o nodo na struct

    //Retorna se a chave procurada estiver no vetor de char chaves
    for(j = 0; j<arvore.n; j++)
    {
        if(arvore.chaves[j]==chave)
        {
            printf("%d encontrado.\n", chave);
            return;
        }
    }

    if(arvore.filhos[0]==-1) // Se o primeiro filho for -1, então é folha
    {
        printf("%d nao encontrado.\n",chave);
        return;
    }

    if(chave<arvore.chaves[0]) // Verifica a chave inferior
    {
        return busca(arvore.filhos[0], chave);
    }
    else if(chave>arvore.chaves[arvore.n-1]) // Verifica a chave superior
    {
        return busca(arvore.filhos[arvore.n], chave);
    }
    else // Senão, está entre o intervalo
    {
        for(j = 1; j<arvore.n; j++) // Loop para encontrar no determinado intervalo
        {
            if(chave<arvore.chaves[j] && chave > arvore.chaves[j-1])
            {
                return busca(arvore.filhos[j], chave);
            }
        }
    }
}

// Escreve em disco o nodo presente na struct, no arquivo ID.dat
void escreve(int ID)
{
    int i;

    char ID_temp[10];

    sprintf(ID_temp, "%d.dat", ID);

    FILE *nodo;

    nodo = fopen(ID_temp, "w");

    // Verifica Split e atribui o determinado ID ao filho caso esteja acontecendo split (OK=1)
    if(ok)
    {
        filho = ID;
    }

    fprintf(nodo, "%d\n", arvore.n); // Escrita de dados no arquivo

    if(arvore.n==0) // Se o n(numero de chaves) for 0, devemos inicializar o arquivo com filho = -1
    {
        fprintf(nodo, "-1 ");
    }
    else
    {
        for(i=0; i<arvore.n; i++) // Percorrendo o numero de chaves do nodo e escrevendo em disco
        {
            fprintf(nodo, "%d ", arvore.chaves[i]);
        }
        fprintf(nodo,"\n");

        for(i=0; i<(arvore.n+1); i++) // Percorrendo o numero de filhos (chaves+1)
        {
            fprintf(nodo, "%d ", arvore.filhos[i]);
        }
    }

    // Mostra na tela o que foi lido
    printf("Escrito ("); 

    for(i = 0; i<arvore.n; i++)
    {
        // Verificando se a forma de imprimir é com espaço ou sem
        if(i+1==arvore.n)
        {
            printf("%d", arvore.chaves[i]);
        }
        else
        {
            printf("%d ", arvore.chaves[i]);
        }
    }
    printf(")\n");

    // Verifica se a arvore não tem 0 chaves (caso inicial)
    if(arvore.n!=0)
    {
        escrita++;
    }

    fclose(nodo);
}

// Função que retorna numero aleatório para o ID do novo nodo criado
int geraIdAleatorio()
{
    double d;   
    d = (double) rand () / ((double) RAND_MAX + 1);
    int k = d * (9999 + 1);

    // Variavel para ter um numero aleatorio entre 0 e 9999
    int aleatorio = k ;

    char ID_aleatorio[10]; // String para utilização no fopen

    sprintf(ID_aleatorio, "%d.dat", aleatorio); //Coloca na string ID o numero aleatorio concatenado com o .txt
    
    //Verifica se já tem o ID, se não cria um novo
    while(!(fopen(ID_aleatorio,"r")==NULL))
    {
        double d;
        d = (double) rand () / ((double) RAND_MAX + 1);
        int k = d * (9999 + 1);
    }
    return aleatorio;
}

//Verifica necessidade de fazer split
short verificaSplit()
{
    // chaves por nodo <= D-1
    if(arvore.n<=(grauD-1))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

//Split
void split(int ID)
{
    int numero_chave = arvore.n; // Armazena a quantidade inicial de chave presente
    int meio = (arvore.n)/2; // Posicao do elemento que está na metade do vetor

    //Altera o numero de chaves dependendo do meio(par ou impar)
    if(arvore.n%2==0)
    {
        arvore.n = meio-1;
        elemento_splitado=arvore.chaves[meio-1];
    }
    else
    {
        arvore.n=meio;
        elemento_splitado=arvore.chaves[meio];
    }

    /*Reutiliza o arquivo para criação de um nodo do split com a metade dos elementos*/
    escreve(ID);

    /*Cria um arquivo do split com a outra metade dos elementos*/
    int i, j;
    int aux;

    // Colocando o resto do vetor de chaves( meio para frente ) no inicio
    for(i = 0, j = meio; j<numero_chave; i++, j++)
    {
        if(numero_chave%2==0)
        {
            arvore.chaves[i] = arvore.chaves[j];
        }
        else
        {
            arvore.chaves[i] = arvore.chaves[j+1];
        }
    }

    // Colocando o resto do vetor de filhos( meio para frente ) no inicio
    for(i = 0; i <= meio; i++)
    {
        // Verificação se está sendo tratado o caso par ou impar
        if(numero_chave%2==0)
        {
            aux = arvore.filhos[i];
            arvore.filhos[i] = arvore.filhos[i+meio];
            arvore.filhos[i+meio] = aux;
        }
        else
        {
            aux = arvore.filhos[i];
            arvore.filhos[i] = arvore.filhos[i+meio+1];
            arvore.filhos[i+meio+1] = aux;
        }
    }

    // Atualizando o numero de chave
    if(numero_chave%2==0)
    {
        arvore.n = numero_chave - meio;
    }
    else
    {
        arvore.n=meio;
    }

    ok=1; // Indicação que está ocorrendo o split

    escreve(geraIdAleatorio()); // Escreve com o ID aleatório

    // Se ocorrer de chegarmos na raiz e for necessario split
    if(ID_raiz == ID)
    {
        arvore.n = 1;
        arvore.chaves[0] = elemento_splitado;
        arvore.filhos[0] = ID;
        arvore.filhos[1] = filho; // Utiliza a variavel global filho
        ID_raiz = geraIdAleatorio();
        escreve(ID_raiz);
        printf("-- Nova raiz com ID %d --\n",ID_raiz);
        ok=0;// Indica final do split
        return;
    }
}

// Insere a chave x na arvore
void insere(int ID, int chave)
{
    int i,j;

    le(ID); //Abre o arquivo ID.dat e carrega o nodo na struct

    if(arvore.filhos[0]==-1) // Se o primeiro filho for -1, então é folha
    {
        arvore.chaves[arvore.n] = chave; // Acrescenta a chave
        arvore.n++; //Insercao de +1 numero de chaves
        SelectionSortRecursivo(arvore.chaves,arvore.n);// Ordena o vetor de chaves
        arvore.filhos[arvore.n] = -1; // Acrescenta um -1 nos filhos

        // Verifica a ordem e a necessidade do split
        if(verificaSplit())
        {
            split(ID);
            return;
        }
        else
        {
            escreve(ID); //Atualiza no arquivo o novo elemento inserido
            return;
        }
    }

    if(chave<arvore.chaves[0]) // Verifica a chave inferior
    {
        insere(arvore.filhos[0], chave);
    }
    else if(chave>arvore.chaves[arvore.n-1]) // Verifica a chave superior
    {
        insere(arvore.filhos[arvore.n], chave);
    }
    else // Senão, está entre o intervalo
    {
        for(j = 1; j<arvore.n; j++) // Loop para encontrar onde será inserido
        {
            if(chave<arvore.chaves[j] && chave > arvore.chaves[j-1])
            {
                insere(arvore.filhos[j], chave);
            }
        }
    }

    // Na recursão//Verifica se está ocorrendo o split
    if(ok)
    {
        le(ID);

        arvore.chaves[arvore.n] = elemento_splitado; // Acrescenta a chave
        arvore.n++; //Insercao de +1 numero de chaves
        SelectionSortRecursivo(arvore.chaves,arvore.n);// Ordena o vetor de chaves
        
        //Dado a ocorrência do split,reorganiza os filhos
        for(i=0; i<arvore.n; i++)
        {
            if(arvore.chaves[i]==elemento_splitado)
            {
                for(j=arvore.n; j>(i+1) ; j-- )
                {
                    arvore.filhos[j]=arvore.filhos[j-1];
                }
                arvore.filhos[i+1]=filho;
            }
        }
    }

    //Realiza a operação split
    if(verificaSplit() && ok)
    {
        split(ID);

        // Na recursão, se for necessario a split na raiz
        if(ID_raiz == ID)
        {
            arvore.n = 1;
            arvore.chaves[0] = elemento_splitado;
            arvore.filhos[0] = ID;
            arvore.filhos[1] = filho;
            ID_raiz = geraIdAleatorio();
            escreve(ID_raiz);
            printf("-- Nova raiz com ID %d --\n",ID_raiz);
            return;
        }

    }
    else
    {
        if(ok)
        {
            escreve(ID);
            ok=0;
        }
        return;
    }
}

int main()
{
    int chave, ID;

    //Inicializando as variaveis com zero
    ok=0; 
    leitura = 0;
    escrita = 0;

    // Variaveis para manipulação de arquivos
    char ID_temp[10];
    char comando[10];
    comando[0] ='\0';

    arvore.n = 0; // Inicializando o numero de chaves no nodo com 0
    srand(time(NULL));// Comando com função de auxiliar na geração de numeros aleatórios

    printf("Digite Grau Maximo: ");
    scanf("%d", &grauD);
    printf("Digite ID raiz: ");
    scanf("%d", &ID);

    ID_raiz = ID; // Atribuição da raiz inicialmente colocada

    sprintf(ID_temp, "%d.dat", ID); // Atribui a string ID_temp o ID concatenado com .dat

    FILE *arquivo; // Ponteiro para o arquivo

    arquivo = fopen(ID_temp, "r");

    // Verifica se o ponteiro para o arquivo não existe ou se foi encontrada
    if(arquivo==NULL)
    {
        escreve(ID);
        printf("Criada raiz vazia em %s.\n\n", ID_temp);
    }
    else
    {
        printf("Raiz em %s encontrada.\n\n", ID_temp);       
        fclose(arquivo);
    } 

    // Opções de escolha para execução, até encontrar o "fim"
    while(strcmp(comando, "fim")!=0)
    {
        printf("> ");
        scanf(" %s", comando);

        if(strcmp(comando, "insere")==0)
        {
            scanf("%d", &chave);
            insere(ID, chave);
            printf("%d leitura(s), %d escrita(s)\n\n", leitura, escrita);
        }
        if(strcmp(comando,"busca")==0)
        {
            scanf("%d", &chave);
            busca(ID, chave);
            printf("%d leitura(s), %d escrita(s)\n\n", leitura, escrita);
        }

        ID = ID_raiz;
        // A cada processo os contadores de leitura e escrita são resetados 
        leitura = 0; 
        escrita = 0;
    }

    return 0;
}