#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

typedef struct Node {
    int          vertex;
    struct Node* next;
} Node;

/**
 * @brief Yeni bir komþu düðüm oluþturur.
 * @param v Eklenecek düðümün vertex numarasý.
 * @return Oluþturulan Node iþaretçisi.
 */
Node* createNode(int v) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) { perror("malloc node"); exit(EXIT_FAILURE); }
    newNode->vertex = v;
    newNode->next   = NULL;
    return newNode;
}

/**
 * @brief Komþuluk listesine yeni bir kenar ekler.
 * @param adjList Grafýn adjacency list dizisi.
 * @param src     Kaynak vertex numarasý.
 * @param dest    Hedef vertex numarasý.
 */
void addEdge(Node* adjList[], int src, int dest) {
    Node* newNode = createNode(dest);
    newNode->next = adjList[src];
    adjList[src]  = newNode;
}

 /**
 * @brief  Girilen dosyayý tarayarak en yüksek vertex numarasýný tespit eder.
 * @param  filename Okunacak graf dosyasýnýn yolu.
 * @return Grafýn vertex sayýsý (0-tabanlý en büyük numara + 1).
 */
int determineVertexCount(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) { perror("Dosya açýlamadý"); exit(EXIT_FAILURE); }

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
 * @brief Belirtilen dosyadan grafý okur ve vertex sayýsýný döner.
 *        Eðer dosya boþsa hata mesajý basýp programý sonlandýrýr.
 * @param filename Okunacak dosyanýn yolu.
 * @param adjList  Okunan kenarlarýn ekleneceði adjacency list.
 * @return Grafýn vertex sayýsý.
 */
int readGraphFromFile(const char* filename, Node* adjList[]) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Dosya açýlamadý");
        exit(EXIT_FAILURE);
    }

    // Boþ dosya kontrolü:
    fseek(file, 0, SEEK_END);
    if (ftell(file) == 0) {
        fprintf(stderr, "Hata: Dosya boþ\n");
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
 * @brief Grafýn adjacency listesini ekrana basar.
 * @param adjList     Grafýn adjacency list dizisi.
 * @param vertexCount Grafýn vertex sayýsý.
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
 * @brief Bir düðümün ödül düðümüne komþu olup olmadýðýný kontrol eder.
 * @param adjList Grafýn adjacency list dizisi.
 * @param x       Kontrol edilecek kaynak vertex.
 * @param prize   Ödül vertex numarasý.
 * @return Komþuysa 1, deðilse 0.
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
 * @brief isPossible dizisinde yalnýzca bir tane 0 kalmýþsa o indexi,
 *        birden fazla veya hiç 0 yoksa -1 döner.
 * @param isPossible  Olasýlýk durumu dizisi.
 * @param vertexCount Dizinin uzunluðu (vertex sayýsý).
 * @return Tek bir 0 kaldýysa o index, aksi halde -1.
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
 * @brief Ödül düðümünü bulmak için grafý sorgular (heuristik tarzý).
 * @param adjList     Grafýn adjacency list dizisi.
 * @param vertexCount Grafýn vertex sayýsý.
 * @param prize       Ödül vertex numarasý.
 * @return Bulunan ödül düðümünün indexi, bulunamadýysa -1.
 */
int findPrizeNode(Node* adjList[], int vertexCount, int prize) {
    int* degree     = malloc(vertexCount * sizeof(int));
    int* nodeOrder  = malloc(vertexCount * sizeof(int));
    int* isPossible = calloc(vertexCount, sizeof(int));
    int result = -1, i, idx;

    /* --- degree[] ve nodeOrder[] baþlatýlmasý --- */
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

    /* 1) Degree’ya göre büyükten küçüðe insertion-sort */
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

    /* 2) Sorgulama döngüsü */
    {
        int stop = 0, x, komsu, finishIdx;
        idx = 0;
        Node* p;

        while (idx < vertexCount && !stop) {
            x = nodeOrder[idx++];
            komsu = isNeighbor(adjList, x, prize);
            printf("%d düðümü ödüle komþu mu? --> %s\n",
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
    printf("\n>> Ödül düðüm bunlardan biri olabilir: %d\n", i);
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

    printf("Ödül düðümünü gir: ");
    scanf("%d", &prize);

    findPrizeNode(adjList, vertexCount, prize);
    return 0;
}

