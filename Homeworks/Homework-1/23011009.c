#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
/*
@brief bu fonksiyon dosyadaki verileri okumayı sağlamaktadır.
@param fileName dosyanın adını belirtir.
@param elementCount key ve lock dizilerinin uzunluğudur. elementCount,keyValues ve lockValues'in dışarıdan parametre olarak alınma sebebi call by reference'dir.
@param keyValues key dizisi
@param lockValues lock dizisi
@return
*/
void readFile(char *fileName, int *elementCount, int ***keyValues, int ***lockValues) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Dosya açılamadı!\n");
        return;
    }
    int i;
    fscanf(file, "%d", elementCount);
    if(*elementCount < 1){
    	return;
	}
    *keyValues = (int **)malloc(2 * sizeof(int *));//memory allocation
    (*keyValues)[0] = (int *)malloc(*elementCount * sizeof(int)); 
    (*keyValues)[1] = (int *)malloc(*elementCount * sizeof(int)); 

    for (i=0;i<*elementCount;i++) {
        (*keyValues)[0][i] = i + 1;//kaçıncı anahtar olduğunu göstermektedir.
        fscanf(file, "%d", &(*keyValues)[1][i]); //anahtarların değerleri
    }

    *lockValues = (int **)malloc(2 * sizeof(int *));//memory allocation
    (*lockValues)[0] = (int *)malloc(*elementCount * sizeof(int)); 
    (*lockValues)[1] = (int *)malloc(*elementCount * sizeof(int));

    for (i=0;i<*elementCount;i++) {
        (*lockValues)[0][i] = i + 1;//kaçıncı kilit olduğunu göstermektedir. 
        fscanf(file, "%d", &(*lockValues)[1][i]);//kilitlerin değerleri 
    }

    fclose(file);
}
/*
@brief bu fonksiyon values dizisinin içindeki 2 elemanın yer değiştirmesini sağlamaktadır.
@param firstElement yer değiştirelecek elemanlardan ilki
@param secondElement yer değiştirilecek elemanlardan ikincisi
@param values elemanların yer değiştireleceği dizi
@return
*/
void swapElements(int firstElement, int secondElement, int **values) {
    int temp;
    temp = values[0][firstElement];
    values[0][firstElement] = values[0][secondElement];
    values[0][secondElement] = temp;
    temp = values[1][firstElement];
    values[1][firstElement] = values[1][secondElement];
    values[1][secondElement] = temp;
}
/*
@brief bu fonksiyon 2 diziyi senkronize bir şekilde sıralayabilmek için pivotValue'nin lockValues'deki indexini bulmaktadır.
@param lockValues pivotValue'nin indexinin arandığı dizi
@param l aranacak yerin alt sınırı
@param r aranacak yerin üst sınırı
@param pivotValue indexi aranan değer
@return i pivotValue'nin lockValues'deki indexini döndürmektedir
*/
int findPivotIndex(int **lockValues, int l, int r, int pivotValue){
	int i;
	for(i=l;i<=r;i++){
		if(lockValues[1][i] == pivotValue){
			return i;
		}
	}
}

/*
@brief bu fonksiyon quick sort işlemi için pivot belirlemeyi sağlamaktadır.pivottan büyük elemanları sağa küçük elemanları sola toplamayı sağlamaktadır.
@param values bu işlemlerin yapılacağı dizi
@param l values dizisinin işlem yapılacak en küçük parametresi
@param r values dizisinin işlem yapılacak en büyük parametresi
@return rightPointer belirlenen pivotun indexini döndürmektedir.
*/
int partition(int randomIndex, int **keyValues, int **lockValues,  int l, int r) {
    //pivot başa alınmaktadır.
    swapElements(l,randomIndex,keyValues);
    int pivotValue = keyValues[1][l];//pivot olarak belirlenen değer
    //buradaki işlem şu şekilde işlemektedir. 2 tane pointerimiz vardır. Bunlardan ilki en soldan diğeri de en sağdan başlamaktadır.
    //Soldaki pointer pivottan büyük bir elemanla karşılaşınca sağdaki pointer azalmaya başlamaktadır.Sağdaki pointer da pivottan küçük bir elemanla karşılaşınca sol pointer ve sağ pointerin tuttuğu değerler yer değiştirmektedir.
    //En sonda 2 pointer aynı yeri gösterince bu yerle de pivot yer değiştirmektedir.Bu sayede pivottan küçük elemanlar solda pivottan büyük elemanlar da sağda toplanmaktadır.
	int leftPointer = l + 1;
    int rightPointer = r;
    while (leftPointer <= rightPointer) {
        while (leftPointer <= r && keyValues[1][leftPointer] < pivotValue)
            leftPointer++;
        while (rightPointer > l && keyValues[1][rightPointer] > pivotValue)
            rightPointer--;

        if (leftPointer < rightPointer) {
            swapElements(leftPointer, rightPointer, keyValues);
        }
    }
    swapElements(l, rightPointer, keyValues);
    int pivotLockIndex = findPivotIndex(lockValues,l,r,pivotValue);//pivot value'nin lockValues dizisindeki indexi bulunmaktadır.
    swapElements(l,pivotLockIndex,lockValues);//pivotu başa alma işlemi
    leftPointer = l + 1;
    rightPointer = r;
    while (leftPointer <= rightPointer) {
        while (leftPointer <= r && lockValues[1][leftPointer] < pivotValue)
            leftPointer++;
        while (rightPointer > l && lockValues[1][rightPointer] > pivotValue)
            rightPointer--;

        if (leftPointer < rightPointer) {
            swapElements(leftPointer, rightPointer, lockValues);
        }
    }
    swapElements(l, rightPointer, lockValues);
    return rightPointer;
}
/*
@brief bu fonksiyon verilen diziyi quick sort algoritması ile sıralar.
@param lockValues keyValues dizisiyle senkronize bir biçimde sıralanacak olan dizi
@param keyValues sıralanacak olan dizi
@param l dizinin işlem yapılacak en küçük indeksi
@param r dizinin işlem yapılacak en büyük indeksi
@return
*/
void quickSort(int **lockValues, int **keyValues, int l, int r) {
    // Eğer sol indeks sağdan küçükse işlem yapılmaktadır, aksi halde durmaktadır.
    if (l < r) {
    	// seçme işlemini gerçekleştirilmektedir.
    	int randomIndex = l + rand() % (r - l + 1);
        // Diziyi bölen pivot'un indeksini belirlemektedir.
        int pivotIndex = partition(randomIndex, keyValues, lockValues, l, r);
        // Pivotun sol tarafını sıralamak için recursive çağrı yapmaktadır.
        if (pivotIndex > l) quickSort(lockValues, keyValues, l, pivotIndex - 1);
        
        // Pivotun sağ tarafını sıralamak için recursive çağrı yapmaktadır.
        if (pivotIndex < r) quickSort(lockValues, keyValues, pivotIndex + 1, r);
    }
}
int main() {
    char fileName[100];
    printf("Enter Filename: ");
    scanf("%s", fileName);//Dosya adı alınmıştır.
    int elementCount,i;
    int **keyValues, **lockValues;
    //Dosya okunmuştur.
    readFile(fileName, &elementCount, &keyValues, &lockValues);
    //anahtarlar ve kilitler sıralanmıştır. matrisin 0. satırında ilk sıraları tutulduğu için ilk sıralar kaybolmamıştır.
    if(elementCount < 1)
    {
    	printf("No elements in this file");
	}
	else{
    quickSort(lockValues ,keyValues, 0, elementCount - 1);
    //burada da dizilerin son sıralanmış hali yazdırılmıştır.
    for (i=0;i<elementCount;i++){
    	printf("%d. key : %d | %d. lock : %d\n",keyValues[0][i],keyValues[1][i],lockValues[0][i],lockValues[1][i]);
	}
    //dinamik olarak tahsis edilmiş alanı serbest bırakılmıştır.
    free(keyValues[0]);
    free(keyValues[1]);
    free(keyValues);
    free(lockValues[0]);
    free(lockValues[1]);
    free(lockValues);}
    return 0;
}
