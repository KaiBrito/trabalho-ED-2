#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct synonyms {
    char synonymsWord[48];
    struct synonyms *nextSynonyms; 
}synonyms;

typedef struct dictionaries {
    char keyWord[48];
    int height;
    struct dictionaries *left;
    struct dictionaries *rigth;
    synonyms *nextSynonyms; 
}dictionaries;

                    /*PROTÓTIPOS DOS MÉTODOS*/
dictionaries *createNode(char *str);
int bigger(int subLeft, int subRigth);
int heightNode(dictionaries *node);
int balancingFactor(dictionaries *node);
                    /*MÉTODOS DE ROTAÇÕES E BALANCEAMENTO*/
dictionaries *leftRotation(dictionaries *node);
dictionaries *rigthRotation(dictionaries *node);
dictionaries *leftDoubleRotation(dictionaries *node);
dictionaries *rigthDoubleRotation(dictionaries *node);
dictionaries *balance(dictionaries *node);
                    /*MÉTODOS DE INSERÇÃO*/
dictionaries *insertDictionaries(dictionaries *root, char *str);
dictionaries *insertTree(dictionaries *root, char *str1, char *str2);
void insertSynonyms(dictionaries *root, char *str1, char *str2);
                    /*MÉTODOS DE BUSCA*/
synonyms *searchSynonyms(synonyms *pointerSynonyms, char *str);
dictionaries *searchDictionaries(dictionaries *root, char *str);
void searchWord(dictionaries *node, char *str);
void listWords(dictionaries *root, char start, char end);
void listSynonyms(synonyms *node);
                    /*MÉTODOS DE REMOÇÃO*/
dictionaries *removeNode(dictionaries *root, char *str);
dictionaries *removeWord(dictionaries *node, char *str);
dictionaries *removeWords(dictionaries *node, char *str1, char *str2);
                    /*MÉTODOS DE PERSISTÊNCIA*/
dictionaries *loadFiles(dictionaries *root);
void saveFiles(dictionaries *root);
void saveWords(dictionaries *root, FILE *file);


int main() {
    dictionaries *root = NULL;
    char option[103], command[7], str1[48], str2[48];

    root = loadFiles(root);

    do
    {
        //printf("=> ");
        scanf(" %[^\n]", option);
        sscanf(option, "%s %s %s", command, str1, str2);

        if (strcmp(command, "insere") == 0) {
            root = insertTree(root, str1, str2);
            root = insertTree(root, str2, str1);
        }
        if (strcmp(command, "remove") == 0) {
            if (strlen(str2) > 1) {
                root = removeWords(root, str1, str2);
                root = removeWords(root, str2, str1);
            } else {
                root = removeWord(root, str1);
            }        
        }
        if (strcmp(command, "busca") == 0) 
            searchWord(root, str1);
        
        if (strcmp(command, "lista") == 0) 
            listWords(root, str1[0], str2[0]);

        // Limpa a variável str2
        strcpy(str2, "");
        
    } while (strcmp(command, "fim") != 0);

    saveFiles(root);

    return 0;
}

// Método cria um nó e retorna ele mesmo 
dictionaries *createNode(char *str){
    dictionaries *node = (dictionaries *) malloc(sizeof(dictionaries));

    if (node != NULL){
        strcpy(node->keyWord, str);
        node->height = 1;
        node->left = NULL;
        node->rigth = NULL;
        node->nextSynonyms = NULL;
    } else 
        printf("Opps, falha na alocacao de memoria!\n");

    return node;  
}

// Método para descobrir qual a maior sub-arvore
int bigger(int subLeft, int subRigth){
    if (subLeft > subRigth)
        return subLeft;
    else 
        return subRigth;
}

// Método para descobrir a altura de um nó
int heightNode(dictionaries *node){
    if (node == NULL)
        return -1;
    else 
        return node->height;
    
}

// Calcula o fb de um nó
int balancingFactor(dictionaries *node){
    if (node != NULL)
        return heightNode(node->left) - heightNode(node->rigth);
    else 
        return 0;
}


// Método para fazer uma rotação à esquerda
dictionaries *leftRotation(dictionaries *node){
    dictionaries *v, *u;

    // v = filho direito do node e u = filho de esquerdo de v
    v = node->rigth;
    u = v->left;

    // v se torna o pai do node e o node recebe o filho esquerdo de v como sendo seu filho direito
    v->left = node;
    node->rigth = u;

    // Recalcula a altura de ambos 
    node->height = bigger(heightNode(node->left), heightNode(node->rigth)) + 1;
    v->height = bigger(heightNode(v->left), heightNode(v->rigth)) + 1;

    return v;
}

// Método para fazer uma rotação à direita
dictionaries *rigthRotation(dictionaries *node){
    dictionaries *v, *u;

    // v = filho esquerdo do node e u = filho direito de v
    v = node->left;
    u = v->rigth;

    // v se torna o pai do node e o node recebe o filho direito de v como sendo seu filho esquerdo
    v->rigth = node;
    node->left = u;

    // Recalcula a altura de ambos 
    node->height = bigger(heightNode(node->left), heightNode(node->rigth)) + 1;
    v->height = bigger(heightNode(v->left), heightNode(v->rigth)) + 1;

    return v;
}

// Método para fazer uma rotação dupla à esquerda
dictionaries *leftDoubleRotation(dictionaries *node){
    node->rigth = rigthRotation(node->rigth);
    return leftRotation(node);
}

// Método para fazer uma rotação dupla à direita
dictionaries *rigthDoubleRotation(dictionaries *node){
    node->left = leftRotation(node->left);
    return rigthRotation(node);
}

// Método para fazer o balanceamento de um nó desregulado 
dictionaries *balance(dictionaries *node){
    int fb = balancingFactor(node);

    // Rotação à esquerda
    if (fb < -1 && balancingFactor(node->rigth) <= 0)
        node = leftRotation(node);
    // Rotação à direita
    else if (fb > 1 && balancingFactor(node->left) >= 0)
        node = rigthRotation(node);
    // Rotação dupla à direita
    else if (fb > 1 && balancingFactor(node->left) < 0)
        node = rigthDoubleRotation(node);
    // Rotação dupla à esquerda
    else if (fb < -1 && balancingFactor(node->rigth) > 0)
        node = leftDoubleRotation(node);

    return node;
}


// Método para inserir elementos na AVL
dictionaries *insertDictionaries(dictionaries *root, char *str){
    // Inserção na árvore
    if (root == NULL)
        root = createNode(str);
    else {
        if (strcmp(root->keyWord, str) == 1)
            root->left = insertDictionaries(root->left, str);  
        else
            root->rigth = insertDictionaries(root->rigth, str);
    }

    // Recalcula a altura de todos os nós entre a raiz e o novo nó
    root->height = bigger(heightNode(root->left), heightNode(root->rigth)) + 1;

    // Verifica se há algum nó para balancear na árvore
    root = balance(root);

    return root;
}

// Método para verificar se a palavra já existe na estrutura ou não, na hora de adicionar um sinônimo ou ela mesma na estrutura também
dictionaries *insertTree(dictionaries *root, char *str1, char *str2) {
    dictionaries *verify;

    // Verifica se a palavra-chave já existe na árvore
    verify = searchDictionaries(root, str1);

    if (verify == NULL)
        root = insertDictionaries(root, str1);

    insertSynonyms(root, str1, str2);

    return root; 
}

// Método para adicionar sinônimos a um nó
void insertSynonyms(dictionaries *root, char *str1, char *str2) {
    synonyms *current, *new, *back, *verify;
    dictionaries *pointerDicti;

    new = (synonyms *) malloc(sizeof(synonyms));
    if (new == NULL) {
		fprintf(stderr, "Opps, falha na alocação de memoria!\n");
		return;
	}

    pointerDicti = searchDictionaries(root, str1);

    // Verifica se a palavra já existe na lista de sinonimos
    verify = searchSynonyms(pointerDicti->nextSynonyms, str2);

    if (verify == NULL) {
        current = pointerDicti->nextSynonyms;
        back = NULL;

        while (current != NULL && strcmp(current->synonymsWord, str2) < 0) {
            back = current;
            current = current->nextSynonyms; 
        }

        if (back == NULL) {
            pointerDicti->nextSynonyms = new;
        } else {
            back->nextSynonyms = new; 
        }

        // Dados do nó
        strcpy(new->synonymsWord, str2);
        new->nextSynonyms = current;
    }
}


// Método para fazer a busca de sinônimos
synonyms *searchSynonyms(synonyms *pointerSynonyms, char *str) {
    synonyms *current = pointerSynonyms;

    while (current != NULL && strcmp(current->synonymsWord, str) != 0)
        current = current->nextSynonyms;  

    if (current == NULL)
        return NULL;
    else 
        return current; 
}

// Método para procurar por um elemento informado pelo o usúario
dictionaries *searchDictionaries(dictionaries *root, char *str){
    if (root == NULL)
        return NULL;
    else if (strcmp(root->keyWord, str) == 0)
        return root;
    else if (strcmp(root->keyWord, str) == 1)
        searchDictionaries(root->left, str);
    else 
        searchDictionaries(root->rigth, str);  
}

// Método para buscar por uma palavra e imprimir na tela todos os seus sinônimos
void searchWord(dictionaries *node, char *str){
    dictionaries *verify;
    synonyms *word;

    verify = searchDictionaries(node, str);

    if (verify != NULL) {
        word = verify->nextSynonyms;

        while (word != NULL) {
            printf("%s\n", word->synonymsWord);
            word = word->nextSynonyms;
        }      
    }
    else   
        printf("hein?\n"); 
}

// Método para listar as palavras em um intervalo determinado pelo usuário, por ordem simétrica para ficar em ordem alfabética
void listWords(dictionaries *root, char start, char end){
    if (root != NULL){
        listWords(root->left, start, end);

        if (start <= root->keyWord[0] && root->keyWord[0] <= end) {
            printf("%s: ", root->keyWord);
            listSynonyms(root->nextSynonyms);
            printf("\n");
        }

        listWords(root->rigth, start, end);
    } 
} 

// Método recebe o início de uma lista e imprime ela por completo
void listSynonyms(synonyms *node){
    if (node != NULL){
        while (node != NULL){
            printf("%s, ", node->synonymsWord);
            node = node->nextSynonyms;
        }        
    }    
}


// Método para remover um elemento da árvore
dictionaries *removeNode(dictionaries *root, char *str){
    dictionaries *aux;

    if (root == NULL)
        return NULL;
    else 
    {
        if (strcmp(root->keyWord, str) == 0){
            // remove o nó folha
            if (root->left == NULL && root->rigth == NULL)
            {
                free(root);
                return NULL;
            } else {
                // Remove o nó que só possui um filho 
                if (root->left == NULL || root->rigth == NULL)
                {
                    if (root->left != NULL)
                        aux = root->left;
                    else 
                        aux = root->rigth;

                    free(root);
                    return aux;
                } else { 
                    // Remove o nó que possuí 2 filhos
                    aux = root->left;

                    while (aux->rigth != NULL)
                        aux = aux->rigth;

                    strcpy(root->keyWord, aux->keyWord);
                    root->nextSynonyms = aux->nextSynonyms;
                    strcpy(aux->keyWord, str);

                    root->left = removeNode(root->left, str);
                    return root;
                }   
            }
        } else {
            if (strcmp(root->keyWord, str) == 1)
                root->left = removeNode(root->left, str);
            else 
                root->rigth = removeNode(root->rigth, str);
        }

        // Recalcula a altura de todos os nós entre a raiz e o novo nó
        root->height = bigger(heightNode(root->left), heightNode(root->rigth)) + 1;
        
        // Verifica se há algum nó para balancear na árvore
        root = balance(root);

        return root;
    }  
}

// Método para fazer a remoção de uma palavra na estrutura e todos seus sinônimos
dictionaries *removeWord(dictionaries *node, char *str){
    dictionaries *pointerNode;
    synonyms *currentS, *backS;
    
    pointerNode = searchDictionaries(node, str);

    if (pointerNode != NULL) {
        currentS = pointerNode->nextSynonyms;

        // Libera todos os sinônimos da palavra chave
        while (currentS != NULL) {
            backS = currentS;
            currentS = currentS->nextSynonyms;
            free(backS);
        }

        // Remove o nó da estrutura
        node = removeNode(node, str);
    }

    return node;
}

// Método para fazer a remoção de uma palavra e um sinônimo 
dictionaries *removeWords(dictionaries *node, char *str1, char *str2){
    dictionaries *pointerNode;
    synonyms *currentS, *backS;

    pointerNode = searchDictionaries(node, str1);

    if (pointerNode != NULL) {
        currentS = pointerNode->nextSynonyms;
        backS = NULL;

        while (currentS != NULL && strcmp(currentS->synonymsWord, str2) != 0) {
            backS = currentS;
            currentS = currentS->nextSynonyms;
        }

        // Excluí o sinonimo
        if (backS == NULL) {
            pointerNode->nextSynonyms = currentS->nextSynonyms;
        } else {
            backS->nextSynonyms = currentS->nextSynonyms;
        }

        free(currentS);

        // Verifica se a lista ficou vazia e remove ele da estrutura caso tenha ficado vazio
        if (pointerNode->nextSynonyms == NULL)
            node = removeNode(node, str1);
    }

    return node;
}


// Método para carregar os dados anteriores
dictionaries *loadFiles(dictionaries *root){
    FILE *loadWords;
    char str1[48], str2[48];

    loadWords = fopen("dicionario.txt", "a+");

    if (loadWords == NULL) {
		printf("Infelizmente seu arquivo dicionario.txt não foi aberto!\n");
		return NULL;
	}

    fscanf(loadWords, "%s %s", str1, str2);

    while (!feof(loadWords)) {
        root = insertTree(root, str1, str2);
        fscanf(loadWords, "%s %s", str1, str2);
    }  

    fclose(loadWords);

    return root;
}

// Método para salvar os dados ao fim do programa
void saveFiles(dictionaries *root){
    FILE *saveWord;
    int i;

    saveWord = fopen("dicionario.txt", "w+");

    if (saveWords == NULL) {
		printf("Infelizmente seu arquivo dicionario.txt não foi aberto!\n");
		return;
	}

    saveWords(root, saveWord);

    fclose(saveWord);
}

// Método para fazer a leitura dos dados em um arquivo
void saveWords(dictionaries *root, FILE *file){
    synonyms *node;

    if (root != NULL){
        saveWords(root->left, file);

        node = root->nextSynonyms;
        while (node != NULL){
            fprintf(file, "%s %s\n", root->keyWord, node->synonymsWord);
            node = node->nextSynonyms;
        }
        
        saveWords(root->rigth, file);        
    } 
}