#include <stdio.h>
#include <string.h>
#include "vector.h"

vector minHeap;
vector minNodes;
vector results;

int GlobalEncodedSum = 0;
int GlobalSesgo;
int GlobalFlag = 0;
int GlobalCopyR;

typedef struct treeNode{
	char letter;
	int value;
	char code[20];
	vector * sons;
	int isChecked;
} Node;

int CompareLetters(const void * elementA, const void * elementB){
	Node * nodeA = (Node *)elementA;
	Node * nodeB = (Node *)elementB;
	return (nodeA->letter - nodeB->letter);
}

char * printCodes(Node * temp, int * arr, int size){
	int i;
	char * buffer = malloc(sizeof(char) * size);
	for (i = 0; i < size; i++){
		buffer[i] = arr[i] + '0';
	}
	buffer[size] = '\0';
	return buffer;
}

void fillMinHeap(int * arr, int size){
	int i;
	for (i = 0; i < size; i++){
		Node * temp = malloc(sizeof(Node));
		temp->letter = (char)(i + 65);
		temp->value = arr[i];
		strcpy(temp->code, "");
		temp->sons = malloc(sizeof(vector *));
		VectorNew(temp->sons, sizeof(Node), NULL, 2);
		temp->isChecked = 0;

		VectorAppend(&minHeap, temp);
	}
}

void extractMinHeap(int numElements){
	int i, k, minValue = 1000, minPos = -1;
	int flag = 0;

	int sizeMinNodes = VectorLength(&minNodes);
	for (i = 0; i < sizeMinNodes; i++){
		VectorDelete(&minNodes, 0);
	}

	//In case of slant
	if (GlobalFlag){
		numElements = GlobalSesgo;
		GlobalFlag = 0;
		flag = 1;
	}
	else{
		numElements = GlobalCopyR;
	}

	for (k = 0; k < numElements && VectorLength(&minHeap) > 0; k++){
		Node * temp;
		for (i = 0; i < VectorLength(&minHeap); i++){
			temp = (Node *)VectorNth(&minHeap, i);
			if (temp->value < minValue && temp->isChecked == 0){
				minValue = temp->value;
				minPos = i;
				temp->isChecked = 1;
			}
		}

		//In case of slant, reuse isChecked
		if (!flag){
			Node * changeCheck = VectorNth(&minHeap, minPos);
			changeCheck->isChecked = 0;
		}
		
		VectorAppend(&minNodes, VectorNth(&minHeap, minPos));
		VectorDelete(&minHeap, minPos);
		minValue = 1000;
		minPos = -1;

		for (i = 0; i < VectorLength(&minHeap); i++){
			Node * temp2 = (Node *)VectorNth(&minHeap, i);
			temp2->isChecked = 0;
		}

	}
}

void treeTraversal(Node * root, int * arr, int * codes, int top){
	int i;
	for (i = 0; i < VectorLength(root->sons); i++){
		Node * temp = (Node *)VectorNth(root->sons, i);
		codes[top] = i;
		if (VectorLength(temp->sons) == 0){
			if (temp->isChecked == 1){
				codes[top] = GlobalCopyR - GlobalSesgo + i;
			}
			strcpy(temp->code, printCodes(temp, codes, top + 1));
			GlobalEncodedSum += strlen(temp->code) * arr[temp->letter - 65];
			VectorAppend(&results, temp);
		}
		treeTraversal(VectorNth(root->sons, i), arr, codes, top+1);
	}
}


int main(){
	int r = -1, n, i, setCounter = 0, frequencySum = 0;

	while (r != 0){
		VectorNew(&minHeap, sizeof(Node), NULL, 2);
		VectorNew(&minNodes, sizeof(Node), NULL, 2);
		VectorNew(&results, sizeof(Node), NULL, 2);

		frequencySum = 0;
		GlobalEncodedSum = 0;
		scanf("%d", &r);

		if (r == 0){
			break;
		}

		scanf("%d", &n);

		//In case of slant
		GlobalSesgo = r;
		GlobalCopyR = r;
		double k = (double)(n - r) / (double)(r - 1);
		if (k != (int)k){
			GlobalSesgo = n - (int)(k + 1)*(r - 1);
			GlobalFlag = 1;
		}

		int * arr = malloc(sizeof(int) * n);
		for (i = 0; i<n; i++){
			scanf("%d", &arr[i]);
			frequencySum += arr[i];
		}

		fillMinHeap(arr, n);

		Node * nodeSum;
		while (1){
			extractMinHeap(r);

			//Create sums node
			nodeSum = (Node *)malloc(sizeof(Node));
			nodeSum->letter = ' ';
			strcpy(nodeSum->code, "");
			nodeSum->sons = malloc(sizeof(vector *));
			VectorNew(nodeSum->sons, sizeof(Node), NULL, 2);
			nodeSum->isChecked = 0;

			int sum = 0;
			for (i = 0; i < VectorLength(&minNodes); i++){
				Node * temp = (Node *)VectorNth(&minNodes, i);
				sum += temp->value;
				VectorAppend(nodeSum->sons, temp);
			}

			nodeSum->value = sum;

			if (VectorLength(&minHeap) == 0){
				break;
			}

			VectorAppend(&minHeap, nodeSum);
		}

		//Calculate codes
		int * codes = malloc(sizeof(int) * 27);
		for (i = 0; i < 10; i++){
			codes[i] = -1;
		}
		int top = 0;
		treeTraversal(nodeSum, arr, codes, top);

		//Sort and show results
		VectorSort(&results, CompareLetters);
		setCounter++;
		printf("Set %d; average length %.2lf\n", setCounter, GlobalEncodedSum/(double)frequencySum);
		for (i = 0; i < VectorLength(&results); i++){
			Node * temp = (Node *)VectorNth(&results, i);
			printf("\t%c: %s\n", temp->letter, temp->code);
		}
		
		//Dispose min heap, min nodes and results
		VectorDispose(&minHeap);
		VectorDispose(&minNodes);
		VectorDispose(&results);
	}

	return 0;
}
