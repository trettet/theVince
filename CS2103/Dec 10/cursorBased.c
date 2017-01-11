#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <string.h>

#define SIZE 50
#define SENTINEL "-999"

typedef struct {
	char FN[24];
	char LN[15];
	char MI;
} nametype;

typedef struct {
	char ID[8];
	nametype name;
	int age;
	float height;
	char gender;
	char yearlvl;
	int next;
}heapNode;

typedef struct VHeap {
	heapNode *H;
	int avail;
}*VHeap;

typedef enum {TRUE, FALSE} boolean;

typedef int List;

void initList(List *);
void initVirtualHeap(VHeap *);
void populateList(VHeap VH, List *L);
void populateSorted(VHeap VH, List *L, List *S);
void insertFirst(VHeap VH, List *L, nametype name, int age, float height, char gender, char *ID, char year);
void insertSorted(VHeap VH, List *L, List *S);
boolean deleteElem(VHeap VH, List *L, char *LN);
int deleteAllOccur(VHeap VH, List *L, char *LN);
void displayList(VHeap VH, List, char *);
int myMalloc(VHeap);
boolean myFree(VHeap, int ndx);
char * getRandID();
int getRandYr();

void displayVHeap(VHeap VH){
	int i;
	
	for(i=0 ; i < SIZE; i++){
		printf("%s    %s    %d\n", VH->H[i].name.FN, VH->H[i].ID, VH->H[i].next);
	}
}

int main(void) {
	VHeap VH;
	List L, sorted;
	List deleted;
	
	char *delElem_01 = "Gaga";
	char *delElem_02 = "Abanid";
	char *delElem_03 = "Roberts";
	
	int delCount;
	char strDisp[100];
	// initialize lists & vheap
	initList(&L);
	initVirtualHeap(&VH);
	/*
	
	* 1. Populate the list normally
	
	
	* 2. Delete "Gaga" from the list
	
	* 3. Sort the list using the given sorted List var
	
	* 4. Delete all "Abanid" from the sorted list
	
	* 5. Release the memory */  /* consider it done */
	
	populateList(VH, &L);
	displayList(VH, L, "Populated List: ");
//	displayVHeap(VH);

	if (deleteElem(VH, &L, delElem_01) == TRUE) {
		printf("Deleted %s successfully!\n", delElem_01);
	} else {
		printf("%s not found!\n", delElem_01);
	}
	displayList(VH, L, "Deleted delElem_01: ");
//	displayVHeap(VH);

	initList(&sorted);
	populateSorted(VH,&L, &sorted);
	displayList(VH, sorted, "Sorted by ID: ");
	
	
	delCount = deleteAllOccur(VH, &sorted, delElem_02);
	sprintf(strDisp, "Deleted %d occurences of %s: ", delCount, delElem_02);
	displayList(VH, sorted, strDisp);
	
	displayVHeap(VH);
	
	return 0;
}

void initList(List *L)
{
	*L = -1;
}

int deleteAllOccur(VHeap VH, List *L, char *elem)
{
	List *trav, temp;
	int ret;
	
	for(trav = L, ret=0; *trav != -1;){
		if(strcmp(VH->H[*trav].name.LN, elem) == 0){
			temp = *trav;
			*trav = VH->H[temp].next;
			myFree(VH, temp);
			ret++;
		} else {
			trav = &VH->H[*trav].next;
		}
	}
	return ret;
}

boolean deleteElem(VHeap VH, List *L, char *elem)
{
	List *trav, temp;
	boolean disp;
	
	disp = FALSE;
	for(trav = L; *trav != -1 && strcmp(VH->H[*trav].name.LN, elem) != 0 ; trav = &VH->H[*trav].next){}
	
	if(*trav != -1){
		temp = *trav;
		*trav = VH->H[temp].next;
		disp = myFree(VH, temp);
	}
	return disp;
}

void initVirtualHeap(VHeap *VH)
{
	int i;
	
	*VH = (VHeap)malloc(sizeof(struct VHeap));
	
	if(*VH != NULL){
		(*VH)->H = (heapNode*)malloc(sizeof(heapNode) * SIZE);
		
		for(i=0; i < SIZE; i++){
		strcpy((*VH)->H[i].ID, SENTINEL);
		(*VH)->H[i].next = i-1;
		}
		
		(*VH)->avail = SIZE-1;
	}
}

int myMalloc(VHeap VH)
{
	int ret;
	
	if(VH->avail != -1){
		ret = VH->avail;
		VH->avail = VH->H[ret].next;
		VH->H[ret].next = -2;
	}else{
		ret = -1;
		puts("Virtual heap full!");
	}
	return ret;
}

boolean myFree(VHeap VH, int ndx)
{
	boolean ret;
	
	ret = FALSE;
	
	if(ndx >= 0 && ndx < SIZE){
		VH->H[ndx].next = VH->avail;
		VH->avail = ndx;
		ret = TRUE;
	} else {
		puts("Virtual memspace not found!");
	}
	return ret;
}

void insertFirst(VHeap VH, List *L, nametype name, int age, float height, char gender, char *ID, char year)
{
	List temp;
	
	temp = myMalloc(VH);
	
	if(temp != -1){
		VH->H[temp].name = name;
		VH->H[temp].age = age;
		VH->H[temp].height = height;
		VH->H[temp].gender = gender;
		strcpy(VH->H[temp].ID, ID);
		VH->H[temp].yearlvl = year;
		VH->H[temp].next = *L;
		*L = temp;
	}
}

void populateList(VHeap VH, List *L) {
	int ARR_SIZE = 14;
	
	nametype names[] = { {"Marvin", "Lim", 'S'}, {"Ash", "Ketchum", 0}, {"Terrence John", "Talara", 'C'}, {"John Paul", "Jayme", 'R'}, {"Teodora", "Punzalan", 'J'},
						{"Jericho Andre", "Mesina", 'E'}, {"Lady", "Gaga", 0}, {"Vera Marie", "Gascon", 'C'}, {"Sherra Monique", "Abanid", 'V'}, {"Clifford Jay", "Iting", 'I'},
						{"Chenny Marie", "Abanid", 'V'}, {"Jack", "dela Cruz", 'D'}, {"Ian", "Murdock", 'T'}, {"Joshua", "Isanan", 'I'} };
	
	int age[] = {23,13,17,18,39,18,27,19,19,19,18,19,45,19};
	float height[] = {23.3,13.3,17.32,18.32,39.43,18.0,27.4,19.42,19.32,19.32,18.23,19.32,45.323,19.32};
	char gender[] = {'M','M','M','M','F','M','F','F','F','M','F','M','M','M'};
	
	/* POPULATE THE HEAP WITH THE DATA ABOVE */
	
	int i;
	
	for(i=0; i<ARR_SIZE; i++) {
		insertFirst(VH, L, names[i], age[i], height[i], gender[i], getRandID(), getRandYr());
	}
}

void populateSorted(VHeap VH, List *L, List *S) {
	int ARR_SIZE = 14;
	
	nametype names[] = {{"Marvin", "Lim", 'S'}, {"Ash", "Ketchum", 0}, {"Terrence John", "Talara", 'C'}, {"John Paul", "Jayme", 'R'}, {"Teodora", "Punzalan", 'J'},
						{"Jericho Andre", "Mesina", 'E'}, {"Lady", "Gaga", 0}, {"Vera Marie", "Gascon", 'C'}, {"Sherra Monique", "Abanid", 'V'}, {"Clifford Jay", "Iting", 'I'},
						{"Chenny Marie", "Abanid", 'V'}, {"Jack", "dela Cruz", 'D'}, {"Ian", "Murdock", 'T'}, {"Joshua", "Isanan", 'I'} };
	
	int age[] = {23,13,17,18,39,18,27,19,19,19,18,19,45,19};
	float height[] = {23.3,13.3,17.32,18.32,39.43,18.0,27.4,19.42,19.32,19.32,18.23,19.32,45.323,19.32};
	char gender[] = {'M','M','M','M','F','M','F','F','F','M','F','M','M','M'};
	
	/* POPULATE THE HEAP WITH THE DATA ABOVE IN A SORTED FASHION */
	
//	List *trav, *front, temp;
//	heapNode A;
//	
//	front = L;
//	
//	for(trav = L; *trav != -1;){
//		front = &VH->H[*front].next;
//		
//		if(*front != -1){
//			if(strcmp(VH->H[*trav].ID, VH->H[*front].ID) < 0){
//				*S = *trav;
//				
//				
//			}else{
//				trav = &VH->H[*trav].next;
//			}
//		}
//	}
	List trav, *_trav, temp;
	for(trav=*L; trav!=-1; trav = VH->H[trav].next) {
		for(_trav=S; *_trav != -1 && strcmp(VH->H[*_trav].ID, VH->H[trav].ID) < 0; _trav=&VH->H[*_trav].next) {}
		
		temp = myMalloc(VH);
		if(temp != -1) {
			VH->H[temp].next = *_trav;
			
			VH->H[temp].name = VH->H[trav].name;
			VH->H[temp].age = VH->H[trav].age;
			VH->H[temp].height = VH->H[trav].height;
			VH->H[temp].gender = VH->H[trav].gender;
			strcpy(VH->H[temp].ID, VH->H[trav].ID);
			VH->H[temp].yearlvl = VH->H[trav].yearlvl;
			
			*_trav = temp;
		}
	}	
}

void displayList(VHeap VH, List L, char *str) {
	printf("\n%s:\n", str);
	printf("%-4s %-50s %-5s %-8s %-6s %-4s\n", "ID", "Name", "Age", "Height", "Gender", "Year Lvl");
	while(L!=-1) {
		printf("%-4s %-18s %c. %-28s %-5d %-10.2f %-7c %d   \n", VH->H[L].ID, VH->H[L].name.FN, VH->H[L].name.MI, VH->H[L].name.LN, VH->H[L].age, VH->H[L].height, VH->H[L].gender, VH->H[L].yearlvl);
		L = VH->H[L].next;
	}
	
	printf("\nPress any key to continue . . .");
	getchar();
}

int getRandYr() {
	return rand() % 4 + 1;
}

char * getRandID() {
	char *str = (char *)malloc(sizeof(char) * 8);
	
	sprintf(str, "%d", rand() % 3423 + 5020);
	
	return str;
}
