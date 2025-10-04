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
 * @brief Yeni bir çalýþan düðümü oluþturur ve adýný kopyalar.
 * @param name Oluþturulacak düðümün adý.
 * @param age Oluþturulacak düðümün yaþý.
 * @param salary Oluþturulacak düðümün maaþý.
 * @return Ayrýlmýþ ve baþlatýlmýþ TreeNode iþaretçisi.
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
 * @brief Ýsimine göre aðaçta düðüm arar.
 * @param root Aramanýn baþladýðý kök düðümü.
 * @param name Aranan düðüm adý.
 * @return Bulursa ilgili TreeNode iþaretçisi, bulunamazsa NULL.
 */
TreeNode* findByName(TreeNode *root, const char *name) {
    if (!root) return NULL;
    if (strcmp(root->name, name) == 0) return root;

    TreeNode *found = findByName(root->underLevel, name);
    if (found) return found;
    return findByName(root->sameLevel, name);
}

/**
 * @brief Belirtilen patronun altýna yeni bir çalýþan ekler.
 * @param root Aðaç kök düðümü.
 * @param workerName Eklenen çalýþanýn adý.
 * @param age Eklenen çalýþanýn yaþý.
 * @param salary Eklenen çalýþanýn maaþý.
 * @param bossName Çalýþanýn ekleneceði patronun adý.
 */
void push(TreeNode *root,
          const char *workerName,
          int age,
          float salary,
          const char *bossName)
{
    TreeNode *boss = findByName(root, bossName);
    if (!boss) {
        printf("Hata: Patron '%s' bulunamadi. '%s' atlanýyor.\n",bossName, workerName);
        return;
    }
    TreeNode *worker = createNode(workerName, age, salary);
    worker->sameLevel  = boss->underLevel;
    boss->underLevel   = worker;
}

/**
 * @brief Aðaçtaki tüm düðümleri ve isimleri serbest býrakýr.
 * @param root Serbest býrakýlacak aðacýn kök düðümü.
 */
void freeTree(TreeNode* root) {
    if (!root) return;
    freeTree(root->underLevel);
    freeTree(root->sameLevel);
    free(root->name);
    free(root);
}

/**
 * @brief Alt aðaçta yaþ toplamý ve kiþi sayýsýný biriktirir.
 * @param root Ýþlem yapýlacak kök düðümü.
 * @param sum Yaþ toplamýný tutan deðiþken referansý.
 * @param count Kiþi sayýsýný tutan deðiþken referansý.
 */
void accumulateAge(TreeNode* root, int *sum, int *count) {
    if (!root) return;
    *sum   += root->age;
    (*count)++;
    accumulateAge(root->underLevel, sum, count);
    accumulateAge(root->sameLevel,  sum, count);
}

/**
 * @brief Tüm çalýþanlarýn yaþ ortalamasýný hesaplar.
 * @param root Yaþ ortalamasý hesaplanacak aðacýn kök düðümü.
 * @return Hesaplanan yaþ ortalamasý (0 dönerse çalýþan yok).
 */
float ageAverage(TreeNode* root) {
    int sum = 0, count = 0;
    accumulateAge(root, &sum, &count);
    if (count == 0) return 0.0f;
    return (float)sum / count;
}

/**
 * @brief Alt aðaçta maaþ toplamýný biriktirir.
 * @param root Ýþlem yapýlacak kök düðümü.
 * @param sum Maaþ toplamýný tutan deðiþken referansý.
 */
void accumulateSalary(TreeNode* root, float *sum) {
    if (!root) return;
    *sum += root->salary;
    accumulateSalary(root->underLevel, sum);
    accumulateSalary(root->sameLevel,  sum);
}

/**
 * @brief Tüm çalýþanlarýn maaþ giderini hesaplar.
 * @param root Maaþ gideri hesaplanacak aðacýn kök düðümü.
 * @return Hesaplanan toplam maaþ gideri.
 */
float salaryExpense(TreeNode* root) {
    float sum = 0.0f;
    accumulateSalary(root, &sum);
    return sum;
}

/**
 * @brief Aðacýn katman sayýsýný (derinliðini) hesaplar.
 * @param root Derinliði hesaplanacak aðacýn kök düðümü.
 * @return Aðacýn seviye sayýsý.
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
 * @brief Her seviyedeki çalýþan sayýsýný sayar.
 * @param root Çalýþan sayýsý sayýlacak aðacýn kök düðümü.
 * @param level Mevcut seviye (0’dan baþlar).
 * @param employeeForLevels Her seviye için sayaç dizisi.
 */
void howManyEmployeeAtEachLevel(TreeNode* root, int level, int** employeeForLevels){
	if (!root) return;
	(*employeeForLevels)[level]++;
	howManyEmployeeAtEachLevel(root->underLevel, level + 1, employeeForLevels);
	howManyEmployeeAtEachLevel(root->sameLevel, level, employeeForLevels);
}

/**
 * @brief Belirtilen seviyede en fazla alt çalýþana sahip patronu bulur ve çýktý verir.
 * @param root Aðaç kök düðümü.
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
 * @brief Dosyadan personel verilerini okuyarak aðaç yapýsýný oluþturur.
 * @param fileName Okunacak dosya adý.
 * @param root Oluþan aðacýn kök düðümüne iþaretçi referansý.
 */
void readFile(const char *fileName, TreeNode **root) {
    FILE *fp = fopen(fileName, "r");
    if (!fp) { perror("Dosya açýlamadý"); exit(EXIT_FAILURE); }

    int n;
    if (fscanf(fp, "%d", &n) != 1) {
        fprintf(stderr, "Ýlk satýr sayý deðil!\n");
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
            fprintf(stderr, "Satýr %d format hatasý, atlanýyor\n", i+2);
        }

        if (strcmp(boss, "NULL") == 0) {
            if (*root == NULL) {
                *root = createNode(name, age, sal);
            } else {
                fprintf(stderr,
                        "Uyarý: Ýkinci kök tanýmý atlandý: %s\n",
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
 * @brief Programýn giriþ noktasýdýr; dosyayý okur ve analiz sonuçlarýný basar.
 * @return Çýkýþ durumu (0 baþarýlý).
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

