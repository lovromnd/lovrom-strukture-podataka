#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 50

typedef struct person Position; // definicija strukture
struct person {
	char name[MAX];
	char surname[MAX];
	int birthYear;
	Position* next; // pokazivac na sljedeci element u listi


};

int addAtBeginning(Position* head);
int printList(Position* head);
int addAtEnd(Position* head);
Position* findBySurname(Position* head, const char* surname); // deklaracije funkcija
int deleteAnElement(Position* head, const char* surname);
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

	printf("Dodaj osobu na kraj: \n");
	addAtEnd(head);

	printf("Ispis liste: \n");
	printList(head);

	char sr[MAX];
	printf("Unesi prezime osobe koju zelis pronaci: ");
	scanf("%s", sr);

	Position* found = findBySurname(head, sr); // trazenje osobe
	if (found) {
		printf("Pronadjena osoba: %s %s %d\n", found->name, found->surname, found->birthYear);
	}
	else
		printf("Osoba nije pronadjena\n");

	printf("Unesi prezime osobe koju zelis izbrisati: \n");
	scanf("%s", sr);
	deleteAnElement(head, sr);

	printf("Popis nakon brisanja: \n");
	printList(head);

	freeList(head); //oslobadjanje memorije
	return 0;



}

int addAtBeginning(Position* head) {  //funkcija za dodavanje osobe na pocetak liste
	Position* newPerson = (Position*)malloc(sizeof(Position)); // alociranje memorije za novi cvor
	if (!newPerson) {
		printf("Greška pri alokaciji memorije!\n ");
		return 1;
	}
	char nm[MAX], sr[MAX];
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
int addAtEnd(Position* head) { // funkcija za dodoavnje osobe na kraj liste
	Position* newPerson = (Position*)malloc(sizeof(Position));
	if (!newPerson) {
		printf("Greška pri alokaciji memorije!\n ");
		return 1;
	}
	char nm[MAX], sr[MAX];
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

Position* findBySurname(Position* head, const char* surname) { // funkcija za pronalazak osobe

	Position* q;
	for (q = head->next; q != NULL; q = q->next) { // prolazak kroz sve elemente liste
		if (strcmp(q->surname, surname) == 0)
			return q; // vracanje pokazivaca na osobu cije se prezime poklapa
	}
	return NULL;
}

int deleteAnElement(Position* head, const char* surname) { // funkcija za brisanje osobe iz liste

	if (head == NULL) {
		printf("Greska: head je NULL\n");
		return 1;
	}
	Position* q = head;

	while (q->next != NULL && strcmp(q->next->surname, surname) != 0) {
		q = q->next;
	}
	if (q->next == NULL) {
		printf("Osoba s prezimenom %s nije pronadjena\n", surname);
		return 1;
	}
	Position* temp = q->next; // privremeni pokazivac na cvor koji se brise
	q->next = temp->next; // prespajanje pokazivaca

	printf("Obrisana osoba: %s %s %d\n", temp->name, temp->surname, temp->birthYear);

	free(temp);

}
int freeList(Position* head) {
	Position* p;

	for (p = head; p != NULL; ) {

		Position* temp = p;
		p = p->next;
		free(temp);
	}
	printf("\nSva memorija je oslobodjena\n");
	return 0;

}