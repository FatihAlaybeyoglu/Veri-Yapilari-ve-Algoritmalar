#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

typedef struct Node {
    int          vertex;
    struct Node* next;
} Node;

/**
 * @brief Yeni bir kom�u d���m olu�turur.
 * @param v Eklenecek d���m�n vertex numaras�.
 * @return Olu�turulan Node i�aret�isi.
 */
Node* createNode(int v) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) { perror("malloc node"); exit(EXIT_FAILURE); }
    newNode->vertex = v;
    newNode->next   = NULL;
    return newNode;
}

/**
 * @brief Kom�uluk listesine yeni bir kenar ekler.
 * @param adjList Graf�n adjacency list dizisi.
 * @param src     Kaynak vertex numaras�.
 * @param dest    Hedef vertex numaras�.
 */
void addEdge(Node* adjList[], int src, int dest) {
    Node* newNode = createNode(dest);
    newNode->next = adjList[src];
    adjList[src]  = newNode;
}

 /**
 * @brief  Girilen dosyay� tarayarak en y�ksek vertex numaras�n� tespit eder.
 * @param  filename Okunacak graf dosyas�n�n yolu.
 * @return Graf�n vertex say�s� (0-tabanl� en b�y�k numara + 1).
 */
int determineVertexCount(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) { perror("Dosya a��lamad�"); exit(EXIT_FAILURE); }

    int maxV = -1;
    char line[256];
    while (fgets(line, sizeof(line), f)) {
        char *tok = strtok(line, " \n");
        while (tok) {
            int v = atoi(tok);
            if (v > maxV) maxV = v;
            tok = strtok(NULL, " \n");
        }
    }
    fclose(f);
    return maxV + 1;  
}
/**
 * @brief Belirtilen dosyadan graf� okur ve vertex say�s�n� d�ner.
 *        E�er dosya bo�sa hata mesaj� bas�p program� sonland�r�r.
 * @param filename Okunacak dosyan�n yolu.
 * @param adjList  Okunan kenarlar�n eklenece�i adjacency list.
 * @return Graf�n vertex say�s�.
 */
int readGraphFromFile(const char* filename, Node* adjList[]) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Dosya a��lamad�");
        exit(EXIT_FAILURE);
    }

    // Bo� dosya kontrol�:
    fseek(file, 0, SEEK_END);
    if (ftell(file) == 0) {
        fprintf(stderr, "Hata: Dosya bo�\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    rewind(file);

    char line[256];
    int vertexCount = 0;
    while (fgets(line, sizeof(line), file)) {
        char* token = strtok(line, " \n");
        if (!token) continue;

        int src = atoi(token);
        if (src + 1 > vertexCount) vertexCount = src + 1;

        while ((token = strtok(NULL, " \n")) != NULL) {
            int dest = atoi(token);
            addEdge(adjList, src, dest);
        }
    }

    fclose(file);
    return vertexCount;
}

/**
 * @brief Graf�n adjacency listesini ekrana basar.
 * @param adjList     Graf�n adjacency list dizisi.
 * @param vertexCount Graf�n vertex say�s�.
 */
void printGraph(Node* adjList[], int vertexCount) {
    int i;
    for (i = 0; i < vertexCount; i++) {
        printf("%d: ", i);
        Node* temp = adjList[i];
        while (temp) {
            printf("%d ", temp->vertex);
            temp = temp->next;
        }
        printf("\n");
    }
}

/**
 * @brief Bir d���m�n �d�l d���m�ne kom�u olup olmad���n� kontrol eder.
 * @param adjList Graf�n adjacency list dizisi.
 * @param x       Kontrol edilecek kaynak vertex.
 * @param prize   �d�l vertex numaras�.
 * @return Kom�uysa 1, de�ilse 0.
 */
int isNeighbor(Node* adjList[], int x, int prize) {
    Node* temp = adjList[x];
    while (temp) {
        if (temp->vertex == prize) return 1;
        temp = temp->next;
    }
    return 0;
}

/**
 * @brief isPossible dizisinde yaln�zca bir tane 0 kalm��sa o indexi,
 *        birden fazla veya hi� 0 yoksa -1 d�ner.
 * @param isPossible  Olas�l�k durumu dizisi.
 * @param vertexCount Dizinin uzunlu�u (vertex say�s�).
 * @return Tek bir 0 kald�ysa o index, aksi halde -1.
 */
int isFinished(int* isPossible, int vertexCount) {
    int i, idx = -1, count = 0;
    for (i = 0; i < vertexCount; i++) {
        if (isPossible[i] == 0) {
            count++;
            idx = i;
        }
        if (count > 1) return -1;
    }
    return idx;
}
/**
 * @brief �d�l d���m�n� bulmak i�in graf� sorgular (heuristik tarz�).
 * @param adjList     Graf�n adjacency list dizisi.
 * @param vertexCount Graf�n vertex say�s�.
 * @param prize       �d�l vertex numaras�.
 * @return Bulunan �d�l d���m�n�n indexi, bulunamad�ysa -1.
 */
int findPrizeNode(Node* adjList[], int vertexCount, int prize) {
    int* degree     = malloc(vertexCount * sizeof(int));
    int* nodeOrder  = malloc(vertexCount * sizeof(int));
    int* isPossible = calloc(vertexCount, sizeof(int));
    int result = -1, i, idx;

    /* --- degree[] ve nodeOrder[] ba�lat�lmas� --- */
    {
        int i, d;
        for (i = 0; i < vertexCount; i++) {
            d = 0;
            Node* p = adjList[i];
            while (p) { d++; p = p->next; }
            degree[i]    = d;
            nodeOrder[i] = i;
        }
    }

    /* 1) Degree�ya g�re b�y�kten k����e insertion-sort */
    {
        int i, j, key, keyDeg;
        for (i = 1; i < vertexCount; i++) {
            key    = nodeOrder[i];
            keyDeg = degree[key];
            j      = i - 1;
            while (j >= 0 && degree[nodeOrder[j]] < keyDeg) {
                nodeOrder[j + 1] = nodeOrder[j];
                j--;
            }
            nodeOrder[j + 1] = key;
        }
    }

    /* 2) Sorgulama d�ng�s� */
    {
        int stop = 0, x, komsu, finishIdx;
        idx = 0;
        Node* p;

        while (idx < vertexCount && !stop) {
            x = nodeOrder[idx++];
            komsu = isNeighbor(adjList, x, prize);
            printf("%d d���m� �d�le kom�u mu? --> %s\n",
                   x, komsu ? "EVET" : "HAYIR");

            if (komsu) {
                for (p = adjList[x]; p; p = p->next)
                    isPossible[p->vertex]++;
                for (i = 0; i < vertexCount; i++)
                    isPossible[i]--;
            } else {
                for (p = adjList[x]; p; p = p->next) {
                    if (isPossible[p->vertex] == 0)
                        isPossible[p->vertex] = -1;
                }
            }
			for(i = 0; i < vertexCount; i++){
			printf("%d ",isPossible[i]);
			}
			printf("\n");
            finishIdx = isFinished(isPossible, vertexCount);
            if (finishIdx > -1) {
                result = finishIdx;
                stop   = 1;
            }
        }
    }
    printf("\n>> %d soruda bulundu!\n",idx);
	for(i = 0; i < vertexCount; i++){
		if(isPossible[i] == 0)
    printf("\n>> �d�l d���m bunlardan biri olabilir: %d\n", i);
	}
    free(degree);
    free(nodeOrder);
    free(isPossible);
    return result;
}

int main() {
    setlocale(LC_ALL, "Turkish");

    int prize;
	int vertexCount = determineVertexCount("G1_KList.txt");
	
    Node **adjList  = calloc(vertexCount, sizeof(Node*));
    if (!adjList) { perror("calloc"); exit(EXIT_FAILURE); }
    
    vertexCount = readGraphFromFile("G1_KList.txt", adjList);
    printGraph(adjList, vertexCount);

    printf("�d�l d���m�n� gir: ");
    scanf("%d", &prize);

    findPrizeNode(adjList, vertexCount, prize);
    return 0;
}

