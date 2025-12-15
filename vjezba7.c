/*struct directory {
	char name[MAX];
	dirPosition next;
	dirPosition subDirPosition;
};

struct stack {
	stackPos next;
	dirPosition dirLevel;

};
1) parentDir = headStack->next->dirLevel;

childDir->next = parentDir->subDirPos;
parentDir->subDirPos = childDir;

2)
temp = headStack->next->dirLevel->subDirPosition;
if (temp->name == ime dir)
push(&headStack, temp);

temp = temp->next;

3)
pop(&headStack) :

	4)
	printDir(dirPosition current)
	dirPosition currentSubDir = NULL;
if (current->SubDirPos != NULL) {
	currentSubDir = current->subDirPos;
	while (current SubDir != NULL) {
		print currentSubDir->next ime
			print Dir(currentSubDir)
			currentSubDir=currentSubDir->next
	}
}
}*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 50

typedef struct directory* dirPosition;
 typedef struct directory {
    char name[MAX];
    dirPosition next;
    dirPosition subDirPosition;
} Directory;


typedef struct stack* stackPos;
 typedef struct stack {
    dirPosition dirLevel;
    stackPos next;
} Stack;


 int push(stackPos head, dirPosition dir);
 int pop(stackPos head);
 dirPosition top(stackPos head);
 int makeDir(stackPos head, char* name);
 int changeDir(stackPos head, char* name);
 int changeDirBack(stackPos head);
 int printDir(dirPosition current);
 int freeTree(dirPosition root);                
 int freeStack(stackPos head);



int main() {
    int choice;
    char name[MAX];

    dirPosition root = (dirPosition)malloc(sizeof(Directory));
    strcpy(root->name, "C:");
    root->next = NULL;
    root->subDirPosition = NULL;

    stackPos headStack = (stackPos)malloc(sizeof(Stack));
    headStack->dirLevel = NULL;
    headStack->next = NULL;

    push(headStack, root);

    do {
        printf("\n--- IZBORNIK ---\n");
        printf("1 - md (napravi direktorij)\n");
        printf("2 - cd dir (udji u direktorij)\n");
        printf("3 - cd.. (nazad)\n");
        printf("4 - dir (ispis direktorija)\n");
        printf("5 - izlaz\n");

        printf("Trenutni direktorij: %s\n", top(headStack)->name);
        printf("Odabir: ");
        scanf("%d", &choice);

        switch (choice) {

        case 1:
            printf("Ime direktorija: ");
            scanf("%s", name);
            makeDir(headStack, name);
            break;

        case 2:
            printf("Ime direktorija za ulazak: ");
            scanf("%s", name);
            changeDir(headStack, name);
            break;

        case 3:
            changeDirBack(headStack);
            break;

        case 4:
            printDir(top(headStack));
            break;

        case 5:
            printf("Izlaz iz programa\n");
            break;

        default:
            printf("Neispravna opcija!\n");
        }

    } while (choice != 5);

    freeStack(headStack);       
    freeTree(root);

    return 0;
}

int push(stackPos head, dirPosition dir) { //funkcija za guranje novog direktorija na stog
    stackPos newEl = (stackPos)malloc(sizeof(Stack));
    if (!newEl) return -1;

    newEl->dirLevel = dir;
    newEl->next = head->next;
    head->next = newEl;

    return 0;
}

int pop(stackPos head) { //funkcija za skidanje direktorija sa stoga
    if (head->next == NULL)
        return -1;

    stackPos temp = head->next;
    head->next = temp->next;
    free(temp);

    return 0;
}

dirPosition top(stackPos head) { //funkcija koja vraca direktorij na vrhu stoga
    if (head->next == NULL)
        return NULL;

        return head->next->dirLevel;
}



int makeDir(stackPos head, char* name) { //funkcija za stvaranje novog direktorija
    dirPosition parentDir = top(head); //trenutni direktorij

    dirPosition childDir = (dirPosition)malloc(sizeof(Directory));
    if (!childDir) {
        return -1;
    }

    strcpy(childDir->name, name);
    childDir->subDirPosition = NULL;

    childDir->next = parentDir->subDirPosition; //dodavanje direktorija na pocetak liste
    parentDir->subDirPosition = childDir;

    return 0;
}



int changeDir(stackPos head, char* name) { //funkcija za ulazak u poddirektorij
    dirPosition current = top(head);
    dirPosition temp = current->subDirPosition;

    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0) {
            push(head, temp);
            return 0;
        }
        temp = temp->next;
    }

    printf("Direktorij '%s' nije pronaden!\n", name);
    return -1;
}



int changeDirBack(stackPos head) { //funkcija za povratak u root direktorij
    if (head->next == NULL || head->next->next == NULL) {
        printf("Vec ste u korijenskom direktoriju!\n");
        return -1;
    }

    pop(head);
    return 0;
}



int printDir(dirPosition current) { //funkcija za ispis sadrzaja direktorija
    dirPosition temp = current->subDirPosition;

    if (temp == NULL) {
        printf("Direktorij je prazan.\n");
        return 0;
    }

    while (temp != NULL) {
        printf("DIR - \t%s\n", temp->name);
        temp = temp->next;
    }

    return 0;
}

int freeTree(dirPosition root) { //funkcija za oslobadjanje memorije cijelog stabla
    if (!root) return 0;               

    freeTree(root->subDirPosition);    
    freeTree(root->next);              

    free(root);                        
    return 0;
}

int freeStack(stackPos head) { //funkcija za oslobadjanje memorije stoga
    stackPos temp;
    while (head) {               
        temp = head->next;       
        free(head);              
        head = temp;            
    }
    return 0;
}