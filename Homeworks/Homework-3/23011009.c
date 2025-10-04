#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct TreeNode {
    char             *name;
    int               age;
    float             salary;
    struct TreeNode  *sameLevel;
    struct TreeNode  *underLevel;
} TreeNode;

/**
 * @brief Yeni bir �al��an d���m� olu�turur ve ad�n� kopyalar.
 * @param name Olu�turulacak d���m�n ad�.
 * @param age Olu�turulacak d���m�n ya��.
 * @param salary Olu�turulacak d���m�n maa��.
 * @return Ayr�lm�� ve ba�lat�lm�� TreeNode i�aret�isi.
 */
TreeNode* createNode(const char *name, int age, float salary) {
    TreeNode *node = malloc(sizeof(TreeNode));
    if (!node) { perror("malloc node"); exit(EXIT_FAILURE); }

    node->name = malloc(strlen(name) + 1);
    if (!node->name) { perror("malloc name"); exit(EXIT_FAILURE); }
    strcpy(node->name, name);

    node->age        = age;
    node->salary     = salary;
    node->sameLevel  = NULL;
    node->underLevel = NULL;
    return node;
}

/**
 * @brief �simine g�re a�a�ta d���m arar.
 * @param root Araman�n ba�lad��� k�k d���m�.
 * @param name Aranan d���m ad�.
 * @return Bulursa ilgili TreeNode i�aret�isi, bulunamazsa NULL.
 */
TreeNode* findByName(TreeNode *root, const char *name) {
    if (!root) return NULL;
    if (strcmp(root->name, name) == 0) return root;

    TreeNode *found = findByName(root->underLevel, name);
    if (found) return found;
    return findByName(root->sameLevel, name);
}

/**
 * @brief Belirtilen patronun alt�na yeni bir �al��an ekler.
 * @param root A�a� k�k d���m�.
 * @param workerName Eklenen �al��an�n ad�.
 * @param age Eklenen �al��an�n ya��.
 * @param salary Eklenen �al��an�n maa��.
 * @param bossName �al��an�n eklenece�i patronun ad�.
 */
void push(TreeNode *root,
          const char *workerName,
          int age,
          float salary,
          const char *bossName)
{
    TreeNode *boss = findByName(root, bossName);
    if (!boss) {
        printf("Hata: Patron '%s' bulunamadi. '%s' atlan�yor.\n",bossName, workerName);
        return;
    }
    TreeNode *worker = createNode(workerName, age, salary);
    worker->sameLevel  = boss->underLevel;
    boss->underLevel   = worker;
}

/**
 * @brief A�a�taki t�m d���mleri ve isimleri serbest b�rak�r.
 * @param root Serbest b�rak�lacak a�ac�n k�k d���m�.
 */
void freeTree(TreeNode* root) {
    if (!root) return;
    freeTree(root->underLevel);
    freeTree(root->sameLevel);
    free(root->name);
    free(root);
}

/**
 * @brief Alt a�a�ta ya� toplam� ve ki�i say�s�n� biriktirir.
 * @param root ��lem yap�lacak k�k d���m�.
 * @param sum Ya� toplam�n� tutan de�i�ken referans�.
 * @param count Ki�i say�s�n� tutan de�i�ken referans�.
 */
void accumulateAge(TreeNode* root, int *sum, int *count) {
    if (!root) return;
    *sum   += root->age;
    (*count)++;
    accumulateAge(root->underLevel, sum, count);
    accumulateAge(root->sameLevel,  sum, count);
}

/**
 * @brief T�m �al��anlar�n ya� ortalamas�n� hesaplar.
 * @param root Ya� ortalamas� hesaplanacak a�ac�n k�k d���m�.
 * @return Hesaplanan ya� ortalamas� (0 d�nerse �al��an yok).
 */
float ageAverage(TreeNode* root) {
    int sum = 0, count = 0;
    accumulateAge(root, &sum, &count);
    if (count == 0) return 0.0f;
    return (float)sum / count;
}

/**
 * @brief Alt a�a�ta maa� toplam�n� biriktirir.
 * @param root ��lem yap�lacak k�k d���m�.
 * @param sum Maa� toplam�n� tutan de�i�ken referans�.
 */
void accumulateSalary(TreeNode* root, float *sum) {
    if (!root) return;
    *sum += root->salary;
    accumulateSalary(root->underLevel, sum);
    accumulateSalary(root->sameLevel,  sum);
}

/**
 * @brief T�m �al��anlar�n maa� giderini hesaplar.
 * @param root Maa� gideri hesaplanacak a�ac�n k�k d���m�.
 * @return Hesaplanan toplam maa� gideri.
 */
float salaryExpense(TreeNode* root) {
    float sum = 0.0f;
    accumulateSalary(root, &sum);
    return sum;
}

/**
 * @brief A�ac�n katman say�s�n� (derinli�ini) hesaplar.
 * @param root Derinli�i hesaplanacak a�ac�n k�k d���m�.
 * @return A�ac�n seviye say�s�.
 */
int howManyLevels(TreeNode* root){
	if(!root) return 0;
	int maxChildLevels = 0;
	TreeNode* c;
	for (c = root->underLevel; c; c = c->sameLevel) {
        int childLevels = howManyLevels(c);
        if (childLevels > maxChildLevels)
            maxChildLevels = childLevels;
    }
    return maxChildLevels+1;
}

/**
 * @brief Her seviyedeki �al��an say�s�n� sayar.
 * @param root �al��an say�s� say�lacak a�ac�n k�k d���m�.
 * @param level Mevcut seviye (0�dan ba�lar).
 * @param employeeForLevels Her seviye i�in saya� dizisi.
 */
void howManyEmployeeAtEachLevel(TreeNode* root, int level, int** employeeForLevels){
	if (!root) return;
	(*employeeForLevels)[level]++;
	howManyEmployeeAtEachLevel(root->underLevel, level + 1, employeeForLevels);
	howManyEmployeeAtEachLevel(root->sameLevel, level, employeeForLevels);
}

/**
 * @brief Belirtilen seviyede en fazla alt �al��ana sahip patronu bulur ve ��kt� verir.
 * @param root A�a� k�k d���m�.
 */
TreeNode* maxEmployeeForLevel(TreeNode* root,int* level,int *max){
	int maxLevel = howManyLevels(root);
	TreeNode* maxEmployee;
	int *employeeForLevels = (int*)calloc(maxLevel+1,sizeof(int));
	int currLevel;
	for(currLevel = 0; currLevel < *level; currLevel++){
		root = root->underLevel;
	}
	while(root != NULL){
		howManyEmployeeAtEachLevel(root->underLevel,*level,&employeeForLevels);
		int count = 0,i;
		for(i = *level;i < maxLevel;i++){
			count += employeeForLevels[i];
		}
		if(count > *max) {
			*max = count;
			maxEmployee = root;
		}
		for(i = *level;i < maxLevel;i++){
			employeeForLevels[i] = 0;
		}
		root = root->sameLevel;
	}
	return maxEmployee;
	
}

/**
 * @brief Dosyadan personel verilerini okuyarak a�a� yap�s�n� olu�turur.
 * @param fileName Okunacak dosya ad�.
 * @param root Olu�an a�ac�n k�k d���m�ne i�aret�i referans�.
 */
void readFile(const char *fileName, TreeNode **root) {
    FILE *fp = fopen(fileName, "r");
    if (!fp) { perror("Dosya a��lamad�"); exit(EXIT_FAILURE); }

    int n;
    if (fscanf(fp, "%d", &n) != 1) {
        fprintf(stderr, "�lk sat�r say� de�il!\n");
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    *root = NULL;
    int i;
    for (i = 0; i < n; i++) {
        char name[64], boss[64];
        int age;
        float sal;
        if (fscanf(fp, "%s %d %f %s",
                   name, &age, &sal, boss) != 4)
        {
            fprintf(stderr, "Sat�r %d format hatas�, atlan�yor\n", i+2);
        }

        if (strcmp(boss, "NULL") == 0) {
            if (*root == NULL) {
                *root = createNode(name, age, sal);
            } else {
                fprintf(stderr,
                        "Uyar�: �kinci k�k tan�m� atland�: %s\n",
                        name);
            }
        } else {
            if (*root == NULL) {
                fprintf(stderr,
                        "Hata: Root yok, '%s' eklenemedi (boss: %s)\n",
                        name, boss);
            } else {
                push(*root, name, age, sal, boss);
            }
        }
    }

    fclose(fp);
}

/**
 * @brief Program�n giri� noktas�d�r; dosyay� okur ve analiz sonu�lar�n� basar.
 * @return ��k�� durumu (0 ba�ar�l�).
 */
int main(){
	TreeNode* root;
	readFile("personel3.txt",&root);
	int maxLevel = howManyLevels(root);
	printf("Ilgili sirketin personel agaci %d seviyeden olusmaktadir\n",maxLevel);
	int *employeeForLevels = (int*)calloc(maxLevel,sizeof(int));
	howManyEmployeeAtEachLevel(root,0,&employeeForLevels);
	int i;
	for(i = 0; i < maxLevel; i++){
		printf("-Seviye %d: %d\n",i + 1, employeeForLevels[i]);
	}
	int level;
	printf("Alt calisan sayisi en fazla olan kisiyi ogrenmek icin bir seviye giriniz: ");
	scanf("%d",&level);
	level--;
	int max = 0; 
	TreeNode* maxEmployee = maxEmployeeForLevel(root,&level,&max);
	level++;
	printf("-%d.seviyede en fazla calisana sahip olan kisi %d ile %s dir\n",level,max,maxEmployee->name);
	float average = ageAverage(root);
	printf("-Tum calisanlarin yas ortalamasi: %.2f\n",average);
	float sum = salaryExpense(root);
	printf("-Sirketin odedigi aylik personel maas gideri: %.2f\n",sum);
	freeTree(root);
	free(employeeForLevels);
	return 0;
}

