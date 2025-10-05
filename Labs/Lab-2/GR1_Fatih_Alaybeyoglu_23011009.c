#include<stdio.h>
#include<stdlib.h>
typedef struct Node {
    int data;
    struct Node* next;
} Node;
Node* createNode(int data) {
	Node* p = (Node*)malloc(sizeof(Node));
	if(p == NULL){
		perror("Null error");
		exit(0);
	}
	p->data = data;
}
void freeList(Node* head) {
    Node* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}
void push(Node** headRef, int data) {
    Node* newNode = createNode(data);
    newNode->next = *headRef;
    *headRef = newNode;
}

int subtractLinkedLists(Node* l1, Node* l2) {
	int basamakKatsayi = 1;
	int sum = 0;
	int borrow = 0;
	while(l1 != NULL && l2 != NULL){
		if(borrow == 1){
			l1->data = l1->data - 1;
			borrow = 0;
		}			
		if(l1->data < l2->data){
			borrow = 1;
			l1->data = l1->data + 10;
		}
		sum = sum + basamakKatsayi*(l1->data - l2->data);
		basamakKatsayi *= 10;
		l1 = l1->next;
		l2 = l2->next;
	}
	if(borrow == 1){
		sum -= basamakKatsayi;
	}
	return sum;
}

int main() {
    int N = 5;
    
    //digits1 > digits2
    int digits1[] = {9,0,1,7,8};
    int digits2[] = {6,2,1,5,6};
    int i;
    
    Node* number1 = NULL;
    Node* number2 = NULL;
    
    for (i = 0; i < N; i++) {
        push(&number1, digits1[i]);
        push(&number2, digits2[i]);
    }
    
    int fark = subtractLinkedLists(number1, number2);
    printf("Sonuc: %d\n", fark);
    freeList(number1);
    freeList(number2);

    return 0;
}
