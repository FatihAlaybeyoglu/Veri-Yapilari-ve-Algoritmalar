#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
/*
@brief bu fonksiyon dosyadaki verileri okumayý saðlamaktadýr.
@param fileName dosyanýn adýný belirtir.
@param elementCount key ve lock dizilerinin uzunluðudur. elementCount,keyValues ve lockValues'in dýþarýdan parametre olarak alýnma sebebi call by reference'dir.
@param keyValues key dizisi
@param lockValues lock dizisi
@return
*/
void readFile(char *fileName, int *elementCount, int ***keyValues, int ***lockValues) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Dosya açýlamadý!\n");
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
        (*keyValues)[0][i] = i + 1;//kaçýncý anahtar olduðunu göstermektedir.
        fscanf(file, "%d", &(*keyValues)[1][i]); //anahtarlarýn deðerleri
    }

    *lockValues = (int **)malloc(2 * sizeof(int *));//memory allocation
    (*lockValues)[0] = (int *)malloc(*elementCount * sizeof(int)); 
    (*lockValues)[1] = (int *)malloc(*elementCount * sizeof(int));

    for (i=0;i<*elementCount;i++) {
        (*lockValues)[0][i] = i + 1;//kaçýncý kilit olduðunu göstermektedir. 
        fscanf(file, "%d", &(*lockValues)[1][i]);//kilitlerin deðerleri 
    }

    fclose(file);
}
/*
@brief bu fonksiyon values dizisinin içindeki 2 elemanýn yer deðiþtirmesini saðlamaktadýr.
@param firstElement yer deðiþtirelecek elemanlardan ilki
@param secondElement yer deðiþtirilecek elemanlardan ikincisi
@param values elemanlarýn yer deðiþtireleceði dizi
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
@brief bu fonksiyon 2 diziyi senkronize bir þekilde sýralayabilmek için pivotValue'nin lockValues'deki indexini bulmaktadýr.
@param lockValues pivotValue'nin indexinin arandýðý dizi
@param l aranacak yerin alt sýnýrý
@param r aranacak yerin üst sýnýrý
@param pivotValue indexi aranan deðer
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
@brief bu fonksiyon quick sort iþlemi için pivot belirlemeyi saðlamaktadýr.pivottan büyük elemanlarý saða küçük elemanlarý sola toplamayý saðlamaktadýr.
@param values bu iþlemlerin yapýlacaðý dizi
@param l values dizisinin iþlem yapýlacak en küçük parametresi
@param r values dizisinin iþlem yapýlacak en büyük parametresi
@return rightPointer belirlenen pivotun indexini döndürmektedir.
*/
int partition(int randomIndex, int **keyValues, int **lockValues,  int l, int r) {
    //pivot baþa alýnmaktadýr.
    swapElements(l,randomIndex,keyValues);
    int pivotValue = keyValues[1][l];//pivot olarak belirlenen deðer
    //buradaki iþlem þu þekilde iþlemektedir. 2 tane pointerimiz vardýr. Bunlardan ilki en soldan diðeri de en saðdan baþlamaktadýr.
    //Soldaki pointer pivottan büyük bir elemanla karþýlaþýnca saðdaki pointer azalmaya baþlamaktadýr.Saðdaki pointer da pivottan küçük bir elemanla karþýlaþýnca sol pointer ve sað pointerin tuttuðu deðerler yer deðiþtirmektedir.
    //En sonda 2 pointer ayný yeri gösterince bu yerle de pivot yer deðiþtirmektedir.Bu sayede pivottan küçük elemanlar solda pivottan büyük elemanlar da saðda toplanmaktadýr.
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
    int pivotLockIndex = findPivotIndex(lockValues,l,r,pivotValue);//pivot value'nin lockValues dizisindeki indexi bulunmaktadýr.
    swapElements(l,pivotLockIndex,lockValues);//pivotu baþa alma iþlemi
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
@brief bu fonksiyon verilen diziyi quick sort algoritmasý ile sýralar.
@param lockValues keyValues dizisiyle senkronize bir biçimde sýralanacak olan dizi
@param keyValues sýralanacak olan dizi
@param l dizinin iþlem yapýlacak en küçük indeksi
@param r dizinin iþlem yapýlacak en büyük indeksi
@return
*/
void quickSort(int **lockValues, int **keyValues, int l, int r) {
    // Eðer sol indeks saðdan küçükse iþlem yapýlmaktadýr, aksi halde durmaktadýr.
    if (l < r) {
    	// seçme iþlemini gerçekleþtirilmektedir.
    	int randomIndex = l + rand() % (r - l + 1);
        // Diziyi bölen pivot'un indeksini belirlemektedir.
        int pivotIndex = partition(randomIndex, keyValues, lockValues, l, r);
        // Pivotun sol tarafýný sýralamak için recursive çaðrý yapmaktadýr.
        if (pivotIndex > l) quickSort(lockValues, keyValues, l, pivotIndex - 1);
        
        // Pivotun sað tarafýný sýralamak için recursive çaðrý yapmaktadýr.
        if (pivotIndex < r) quickSort(lockValues, keyValues, pivotIndex + 1, r);
    }
}
int main() {
    char fileName[100];
    printf("Enter Filename: ");
    scanf("%s", fileName);//Dosya adý alýnmýþtýr.
    int elementCount,i;
    int **keyValues, **lockValues;
    //Dosya okunmuþtur.
    readFile(fileName, &elementCount, &keyValues, &lockValues);
    //anahtarlar ve kilitler sýralanmýþtýr. matrisin 0. satýrýnda ilk sýralarý tutulduðu için ilk sýralar kaybolmamýþtýr.
    if(elementCount < 1)
    {
    	printf("No elements in this file");
	}
	else{
    quickSort(lockValues ,keyValues, 0, elementCount - 1);
    //burada da dizilerin son sýralanmýþ hali yazdýrýlmýþtýr.
    for (i=0;i<elementCount;i++){
    	printf("%d. key : %d | %d. lock : %d\n",keyValues[0][i],keyValues[1][i],lockValues[0][i],lockValues[1][i]);
	}
    //dinamik olarak tahsis edilmiþ alaný serbest býrakýlmýþtýr.
    free(keyValues[0]);
    free(keyValues[1]);
    free(keyValues);
    free(lockValues[0]);
    free(lockValues[1]);
    free(lockValues);}
    return 0;
}
