#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
/*
@brief bu fonksiyon dosyadaki verileri okumay� sa�lamaktad�r.
@param fileName dosyan�n ad�n� belirtir.
@param elementCount key ve lock dizilerinin uzunlu�udur. elementCount,keyValues ve lockValues'in d��ar�dan parametre olarak al�nma sebebi call by reference'dir.
@param keyValues key dizisi
@param lockValues lock dizisi
@return
*/
void readFile(char *fileName, int *elementCount, int ***keyValues, int ***lockValues) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Dosya a��lamad�!\n");
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
        (*keyValues)[0][i] = i + 1;//ka��nc� anahtar oldu�unu g�stermektedir.
        fscanf(file, "%d", &(*keyValues)[1][i]); //anahtarlar�n de�erleri
    }

    *lockValues = (int **)malloc(2 * sizeof(int *));//memory allocation
    (*lockValues)[0] = (int *)malloc(*elementCount * sizeof(int)); 
    (*lockValues)[1] = (int *)malloc(*elementCount * sizeof(int));

    for (i=0;i<*elementCount;i++) {
        (*lockValues)[0][i] = i + 1;//ka��nc� kilit oldu�unu g�stermektedir. 
        fscanf(file, "%d", &(*lockValues)[1][i]);//kilitlerin de�erleri 
    }

    fclose(file);
}
/*
@brief bu fonksiyon values dizisinin i�indeki 2 eleman�n yer de�i�tirmesini sa�lamaktad�r.
@param firstElement yer de�i�tirelecek elemanlardan ilki
@param secondElement yer de�i�tirilecek elemanlardan ikincisi
@param values elemanlar�n yer de�i�tirelece�i dizi
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
@brief bu fonksiyon 2 diziyi senkronize bir �ekilde s�ralayabilmek i�in pivotValue'nin lockValues'deki indexini bulmaktad�r.
@param lockValues pivotValue'nin indexinin arand��� dizi
@param l aranacak yerin alt s�n�r�
@param r aranacak yerin �st s�n�r�
@param pivotValue indexi aranan de�er
@return i pivotValue'nin lockValues'deki indexini d�nd�rmektedir
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
@brief bu fonksiyon quick sort i�lemi i�in pivot belirlemeyi sa�lamaktad�r.pivottan b�y�k elemanlar� sa�a k���k elemanlar� sola toplamay� sa�lamaktad�r.
@param values bu i�lemlerin yap�laca�� dizi
@param l values dizisinin i�lem yap�lacak en k���k parametresi
@param r values dizisinin i�lem yap�lacak en b�y�k parametresi
@return rightPointer belirlenen pivotun indexini d�nd�rmektedir.
*/
int partition(int randomIndex, int **keyValues, int **lockValues,  int l, int r) {
    //pivot ba�a al�nmaktad�r.
    swapElements(l,randomIndex,keyValues);
    int pivotValue = keyValues[1][l];//pivot olarak belirlenen de�er
    //buradaki i�lem �u �ekilde i�lemektedir. 2 tane pointerimiz vard�r. Bunlardan ilki en soldan di�eri de en sa�dan ba�lamaktad�r.
    //Soldaki pointer pivottan b�y�k bir elemanla kar��la��nca sa�daki pointer azalmaya ba�lamaktad�r.Sa�daki pointer da pivottan k���k bir elemanla kar��la��nca sol pointer ve sa� pointerin tuttu�u de�erler yer de�i�tirmektedir.
    //En sonda 2 pointer ayn� yeri g�sterince bu yerle de pivot yer de�i�tirmektedir.Bu sayede pivottan k���k elemanlar solda pivottan b�y�k elemanlar da sa�da toplanmaktad�r.
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
    int pivotLockIndex = findPivotIndex(lockValues,l,r,pivotValue);//pivot value'nin lockValues dizisindeki indexi bulunmaktad�r.
    swapElements(l,pivotLockIndex,lockValues);//pivotu ba�a alma i�lemi
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
@brief bu fonksiyon verilen diziyi quick sort algoritmas� ile s�ralar.
@param lockValues keyValues dizisiyle senkronize bir bi�imde s�ralanacak olan dizi
@param keyValues s�ralanacak olan dizi
@param l dizinin i�lem yap�lacak en k���k indeksi
@param r dizinin i�lem yap�lacak en b�y�k indeksi
@return
*/
void quickSort(int **lockValues, int **keyValues, int l, int r) {
    // E�er sol indeks sa�dan k���kse i�lem yap�lmaktad�r, aksi halde durmaktad�r.
    if (l < r) {
    	// se�me i�lemini ger�ekle�tirilmektedir.
    	int randomIndex = l + rand() % (r - l + 1);
        // Diziyi b�len pivot'un indeksini belirlemektedir.
        int pivotIndex = partition(randomIndex, keyValues, lockValues, l, r);
        // Pivotun sol taraf�n� s�ralamak i�in recursive �a�r� yapmaktad�r.
        if (pivotIndex > l) quickSort(lockValues, keyValues, l, pivotIndex - 1);
        
        // Pivotun sa� taraf�n� s�ralamak i�in recursive �a�r� yapmaktad�r.
        if (pivotIndex < r) quickSort(lockValues, keyValues, pivotIndex + 1, r);
    }
}
int main() {
    char fileName[100];
    printf("Enter Filename: ");
    scanf("%s", fileName);//Dosya ad� al�nm��t�r.
    int elementCount,i;
    int **keyValues, **lockValues;
    //Dosya okunmu�tur.
    readFile(fileName, &elementCount, &keyValues, &lockValues);
    //anahtarlar ve kilitler s�ralanm��t�r. matrisin 0. sat�r�nda ilk s�ralar� tutuldu�u i�in ilk s�ralar kaybolmam��t�r.
    if(elementCount < 1)
    {
    	printf("No elements in this file");
	}
	else{
    quickSort(lockValues ,keyValues, 0, elementCount - 1);
    //burada da dizilerin son s�ralanm�� hali yazd�r�lm��t�r.
    for (i=0;i<elementCount;i++){
    	printf("%d. key : %d | %d. lock : %d\n",keyValues[0][i],keyValues[1][i],lockValues[0][i],lockValues[1][i]);
	}
    //dinamik olarak tahsis edilmi� alan� serbest b�rak�lm��t�r.
    free(keyValues[0]);
    free(keyValues[1]);
    free(keyValues);
    free(lockValues[0]);
    free(lockValues[1]);
    free(lockValues);}
    return 0;
}
