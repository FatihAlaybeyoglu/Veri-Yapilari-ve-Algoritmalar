#include<stdio.h>
int main(){
	int row,col;
	printf("Satýr sayýsýný giriniz: ");
	scanf("%d",&row);
	printf("Sütun sayýsýný giriniz: ");
	scanf("%d",&col);
	int**matrix = (int**)malloc(row*sizeof(int*));
	int i,j;
	for(i=0;i<row;i++){
		matrix[i] = (int*)malloc(col*sizeof(int));
	}
	for(i=0;i<row;i++){
		for(j=0;j<col;j++){
			printf("Lütfen elemaný giriniz: ");
			scanf("%d",&matrix[i][j]);
		}
	}
	int*elemanSayisi = (int*)malloc(row*sizeof(int));
	int toplamElemanSayisi;
	for(i=0;i<row;i++){
		int k = 0;
		int satirdakiElemanSayisi = 0;
		while(k<col && matrix[i][k] != 0){
			satirdakiElemanSayisi++;
			k++;
		}
		elemanSayisi[i] = satirdakiElemanSayisi;
		printf("%d\n",satirdakiElemanSayisi);
	}
	toplamElemanSayisi = 0;
	for(i=0;i<row;i++){
		toplamElemanSayisi+= elemanSayisi[i];
	}
	printf("%d\n",toplamElemanSayisi);
	int elemanlar[100];
	int elemanIndex = 0;
	for(i=0;i<row;i++){
		for(j=0;j<elemanSayisi[i];j++){
			elemanlar[elemanIndex] = matrix[i][j];
			elemanIndex++;
		}
	}
	printf("Satýrlardaki eleman sayýsý: ");
	for(i=0;i<row;i++){
		printf("%d ",elemanSayisi[i]);
	}
	printf("\n");
	printf("Elemanlar");
	for(i=0;i<toplamElemanSayisi;i++){
		printf("%d ",elemanlar[i]);
	}
	for(i=0;i<row;i++){
		free(matrix[i]);
	}
	free(matrix);
	free(elemanSayisi);
}
