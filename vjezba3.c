

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define N 50

typedef struct person Position;
struct person {
	char name[N];
	char surname[N];
	int birthYear;
	Position* next;

};

int addAtBeginning(Position* head);
int printList(Position* head);
int addAtEnd(Position* head);


int addAfter(Position* head, const char* surname);
int addBefore(Position* head, const char* surname);
int sortList(Position* head);
int writeToFile(Position* head, const char* filename);
int readFromFile(Position* head, const char* filename);
int freeList(Position* head);

int main() {

	Position* head = (Position*)malloc(sizeof(Position)); // alociranje memorije za head
	if (!head) {
		printf("Greska pri alokaciji memorije!\n");
		return 1;
	}
	head->next = NULL;

	printf("Dodaj osobu na pocetak liste: \n");
	addAtBeginning(head);
	addAtBeginning(head);

	printf("\nDodaj osobu na kraj: \n");
	addAtEnd(head);

	printf("Ispis liste: \n");
	printList(head);

	printf("\nDodaj osobu nakon prezimena Mandic:\n");
	addAfter(head, "Mandic");
	


	printf("\nDodaj osobu ispred prezimena Mandic:\n");
    addBefore(head, "Mandic");
	printList(head);

	sortList(head);
	printf("\nLista nakon sortiranja po prezimenima:\n");
	printList(head);


	writeToFile(head, "list.txt");

	printf("\nCitanje iz datoteke:\n");
	Position* newHead = (Position*)malloc(sizeof(Position));
	newHead->next = NULL;
	readFromFile(newHead, "list.txt");
	printList(newHead);


	freeList(head);
	freeList(newHead);


	return 0;
}

int addAtBeginning(Position* head) {  //funkcija za dodavanje osobe na pocetak liste
	Position* newPerson = (Position*)malloc(sizeof(Position)); // alociranje memorije za novi cvor
	if (!newPerson) {
		printf("Greška pri alokaciji memorije!\n ");
		return 1;
	}
	char nm[N], sr[N];
	int yr;

	printf("Unesi ime: ");
	scanf("%s", nm);
	printf("Unesi prezime: ");
	scanf("%s", sr);
	printf("Unesi godinu rodjenja: ");
	scanf("%d", &yr);

	strcpy(newPerson->name, nm); // kopiranje imena i prezimena u strukturu
	strcpy(newPerson->surname, sr);
	newPerson->birthYear = yr; //postavljanje godine

	newPerson->next = head->next; // novi cvor pokazuje na prvi element
	head->next = newPerson;


	return 0;

}

int printList(Position* head) { // funkcija za ispis cijele liste
	Position* q;
	if (head->next == NULL) {
		printf("Lista je prazna\n");
		return 1;

	}
	printf("\n Popis osoba: \n");
	for (q = head->next; q != NULL; q = q->next) {
		printf("%s %s %d\n", q->name, q->surname, q->birthYear);
	}
	return 0;


}
int addAtEnd(Position* head) { // funkcija za dodavanje osobe na kraj liste
	Position* newPerson = (Position*)malloc(sizeof(Position));
	if (!newPerson) {
		printf("Greška pri alokaciji memorije!\n ");
		return 1;
	}
	char nm[N], sr[N];
	int yr;

	printf("Unesi ime: ");
	scanf("%s", nm);
	printf("Unesi prezime: ");
	scanf("%s", sr);
	printf("Unesi godinu rodjenja: ");
	scanf("%d", &yr);


	strcpy(newPerson->name, nm);
	strcpy(newPerson->surname, sr);
	newPerson->birthYear = yr;
	newPerson->next = NULL; // novi cvor je zadnji u listi

	Position* q;
	for (q = head; q->next != NULL; q = q->next); // for petlja ide do zadnjeg elementa u listi

	q->next = newPerson; // povezuje novi element na kraj liste

	return 0;

}

int addAfter(Position* head, const char* surname) { //funkcija za dodavanje osobe nakon trazenog prezimena

	Position* q = head->next;


	while (q != NULL && strcmp(q->surname, surname) != 0) { //trazenje cvora sa zadanim prezimenom
		q = q->next;
	}
	if (q == NULL) {
		printf("Osoba nije pronadjena\n");
		return 1;
	}

	Position* newPerson = (Position*)malloc(sizeof(Position));
	if (!newPerson) {
		printf("Greška pri alokaciji memorije!\n ");
		return 1;
	}

	printf("Unesi ime: ");
	scanf("%s", newPerson->name);
	printf("Unesi prezime: ");
	scanf("%s", newPerson->surname);
	printf("Unesi godinu rodjenja: ");
	scanf("%d", &newPerson->birthYear);

	

	newPerson->next = q->next; //umetanje novog cvora nakon pronadjenog
	q->next = newPerson;

	return 0;
}


int addBefore(Position* head, const char* surname) { //funkcija za dodavanje osobe prije rrazenog prezimena 

	Position* q = head;

	while (q->next != NULL && strcmp(q->next->surname, surname) != 0) { 
		q = q->next;
	}
	if (q == NULL) {
		printf("Osoba nije pronadjena\n");
		return 1;
	}

	Position* newPerson = (Position*)malloc(sizeof(Position));
	if (!newPerson) {
		printf("Greška pri alokaciji memorije!\n ");
		return 1;
	}

	printf("Unesi ime: ");
	scanf("%s", newPerson->name);
	printf("Unesi prezime: ");
	scanf("%s", newPerson->surname);
	printf("Unesi godinu rodjenja: ");
	scanf("%d", &newPerson->birthYear);

	

	newPerson->next = q->next; //umetanje novog cvora ispred pronadjenog
	q->next = newPerson;

	return 0;
}


int sortList(Position* head) { //funkcija za sortiranje liste po prezimenima

	if (head->next == NULL || head->next->next == NULL) {
		printf("lista je prazna ili ima samo 1 element!\n");
		return 1;
	}


	//bubble sort
	Position* sorted = NULL;
	while (head->next != sorted) {
		Position* q = head;
		Position* p = q->next;

		while (p->next != sorted) {
			if (strcmp(p->surname, p->next->surname) > 0) {
				Position* temp = p->next;
				p->next = temp->next;
				temp->next = p;
				q->next = temp;
				q = temp;
			}
			else {
				q = p;
				p = p->next;
			}
		}
		sorted = p;
	}
	return 0;

}
int writeToFile(Position* head, const char* filename) { //funkcija za upisivanje liste u datoteku
	FILE* f = fopen("list.txt", "w");
	if (!f) {
		printf("Datoteka se ne moze otvoriti!\n");
		return 1;
	}

	Position* q = head->next;
	while (q!=NULL) {  //upis svakog clana liste u novi red
		fprintf(f, "%s %s %d\n", q->name, q->surname, q->birthYear);
		q = q->next;
	}

	fclose(f);

	return 0;
}

int readFromFile(Position* head, const char* filename) { //funkcija za citanje liste iz datoteke

	FILE* f = fopen("list.txt", "r");
	if (!f) {
		printf("Datoteka se ne moze otvoriti!\n");
		return 1;
	}

	char nm[N], sr[N];
	int yr;
		
		

		while (fscanf(f, "%s %s %d\n", nm, sr, &yr) == 3) { //citanje linija sve dok ne dodje do kraja
			Position* newPerson = (Position*)malloc(sizeof(Position));
			if (!newPerson) {
				printf("Greska pri alokaciji\n");
				fclose(f);
				return 1;
			}
			strcpy(newPerson->name, nm);
			strcpy(newPerson->surname, sr);
			newPerson->birthYear = yr;
			newPerson->next = NULL;

			Position* q = head;  //pronalazenje kraja liste i dodavanje nove osobe
			while (q->next != NULL) {
				q = q->next;
				
			}	
			q->next = newPerson;
	}
	fclose(f);

	return 0;
}

int freeList(Position* head) {
	

	while(head!=NULL){

		Position* temp = head; // pamti temp
		head = head->next; // ide na sljedeci
		free(temp); // oslobadja temp
	}
	return 0;

}

