#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct _poly* Position;
typedef struct _poly {
	int coeff;
	int pow;
	Position next;

} poly;

Position CreateElement(int coeff, int pow);
int InsertAfter(Position temp, Position newEL);
int DeleteAfter(Position temp);
int InsertSorted(Position head, Position newEl);
int ReadFile(Position p1, Position p2, char* filename);
int PrintPoly(Position p, char* name);
int AddPoly(Position p1, Position p2, Position result);
int MultiplyPoly(Position p1, Position p2, Position result);
int FreePoly(Position head);


int main() {

	poly firstPoly = { 0, 0, NULL };
	poly secondPoly = { 0, 0, NULL };
	poly sum = { 0, 0, NULL };
	poly product = { 0, 0, NULL };


	ReadFile(&firstPoly, &secondPoly, "polinomi.txt"); // ucitavanje polinoma

	PrintPoly(&firstPoly, "Prvi polinom"); //ispis
	PrintPoly(&secondPoly, "Drugi polinom");

	AddPoly(&firstPoly, &secondPoly, &sum); 
	MultiplyPoly(&firstPoly, &secondPoly, &product);
                                                      //racunanje i ispis zbroja i umnoska
	PrintPoly(&sum, "Zbroj polinoma");
	PrintPoly(&product, "Umnozak polinoma");

	FreePoly(&firstPoly);
	FreePoly(&secondPoly);
	FreePoly(&sum);
	FreePoly(&product);


	return 0;
}

Position CreateElement(int coeff, int pow) { //funkcija za stvaranje novog elementa
	Position newEl = (Position)malloc(sizeof(poly)); //alociranje memorije
	if (!newEl) {
		printf("Greska pri alokaciji memorije!\n");
		return NULL;
	}
	newEl->coeff = coeff; //postavljanje koeficijenta i eksponenta
	newEl->pow = pow;
	newEl->next = NULL;

	return newEl;

}

int InsertAfter(Position temp, Position newEl) { //funkcija za umetanje novog elementa odmah nakon zadanog

	if (!temp || !newEl) {
		return -1;
	}

	newEl->next = temp->next; //novi pokazauje na element nakon temp
	temp->next = newEl; // temp pokazuje na novi

	return 0;
}
int DeleteAfter(Position temp) { //funkcija za brisanje elementa odmah nakon zadanog
	if (!temp || !temp->next) {
		return -1;
	}
	Position p = temp->next; //pokazivac na element koji se brise
	temp->next = p->next; //preskakanje tog elementa
	free(p); // oslobadjanje memorije
	return 0;
}

int InsertSorted(Position head, Position newEl) { //funkcija za umetanje elementa u listu sortirano po eksponentu od najveceg do najmanjeg

	Position temp = head; //pomocni pokazivac temp za prolazenje kroz listu

	while (temp->next != NULL && temp->next->pow > newEl->pow) //prolazi se dok se ne dodje do kraja liste ili dok je trenutni element veci od novog
		temp = temp->next;


	if (temp->next != NULL && temp->next->pow == newEl->pow) { //ako postoji element s istim eksponentom, koeficijenti se zbroje
		temp->next->coeff += newEl->coeff;
		if (temp->next->coeff == 0) //ako koeficijent u tom slucaju postane 0, element se brise
			DeleteAfter(temp);
		free(newEl); //novi vise nije potreban
	}
	else {
		InsertAfter(temp, newEl); // inace se novi element umetne na pravo mjesto
	}
	return 0;
}

int ReadFile(Position p1, Position p2, char* filename) { //funkcija za citanje polinoma iz datoteke
	FILE* f = fopen("polinomi.txt", "r");
	if (!f) {
		printf("Greška pri otvaranju datoteke!\n");
		return -1;
	}

	int coeff, pow; //privremene varijable za citanje koeficijente i eksponenta

	//citanje prvog reda
	while (fscanf(f, "%d %d", &coeff, &pow) == 2) { //èitaju se parovi brojeva
		InsertSorted(p1, CreateElement(coeff, pow)); //dodaju se u prvi polinom
		if (fgetc(f) == '\n') break; //ako se dodje do kraja reda, èitanje prestaje
	}

	
	while (fscanf(f, "%d %d", &coeff, &pow) == 2) { //èitanje drugog reda
		InsertSorted(p2, CreateElement(coeff, pow));
		if (fgetc(f) == '\n') break;
	}

	fclose(f);
	return 0;
}



int PrintPoly(Position p, char* name) { //funkcija za ispis polinoma

	printf("%s ", name); //ispis imena polinoma
	Position temp = p->next; //pokazivaè temp na prvi èlan

	if (!temp) { //ako nema èlanova ispisuje se 0

		printf("0\n");
		return 0;
	}

	while (temp) { // prolazi se kroz listu
		if (temp != p->next) { //ako nije prvi èlan
			if (temp->coeff > 0) //ako je poziitivan ispred se ispisuje +
				printf(" + ");
			else printf(" - "); // ako je negativan
		}
		else {
			if (temp->coeff < 0) //ako je prvi èlan negativan ispisuje se -
				printf(" - ");
		}

		int coeff = temp->coeff;
		if (coeff < 0) //ako je koeficijent negativan ispisujemo mu apsolutnu vrijednost
			coeff = -coeff;

		//ispis jednog èlana ovisno o eksponentu
		if (temp->pow == 0) // ako je x^0 ispisati samo koeficijent
			printf("%d", coeff);
		else if (temp->pow == 1) // ako je x^1 ispisati koeficijent i x
			printf("%dx", coeff);
		else //inaèe koeficijent, x i eksponent
			printf("%dx^%d", coeff, temp->pow);

		temp = temp->next;

	}

	printf("\n");
	return 0;
}

int AddPoly(Position p1, Position p2, Position result) { //funkcija za zbrajanje polinoma

	Position t1 = p1->next; //prvi èlan prvog polinoma
	Position t2 = p2->next; // drugi èlan drugog polinoma

	while (t1 && t2) {
		if (t1->pow > t2->pow) { //ako prvi ima veæi eksponent
			InsertSorted(result, CreateElement(t1->coeff, t1->pow)); //kopiramo ga
			t1 = t1->next;
		}
		else if (t1->pow < t2->pow) { // ako drugi ima veæi eksponent
			InsertSorted(result, CreateElement(t2->coeff, t2->pow)); // kopiramo njega
			t2 = t2->next;
		}
		else { //ako su eksponenti isti
			int sum = t1->coeff + t2->coeff; //koeficijenti se zbrajaju
			if (sum != 0)
				InsertSorted(result, CreateElement(sum, t1->pow)); // zbroj se dodaje u rezultat
			t1 = t1->next;
			t2 = t2->next;
		}

	}
	while (t1) { //ako je ostalo još èlanova u prvom polinomu
		InsertSorted(result, CreateElement(t1->coeff, t1->pow));
		t1 = t1->next;
	}
	while (t2) { //ako je ostalo još u drugom
		InsertSorted(result, CreateElement(t2->coeff, t2->pow));
		t2 = t2->next;
	}
	return 0;
}
int MultiplyPoly(Position p1, Position p2, Position result) { //funkcija za množenje polinoma

	for (Position i = p1->next; i != NULL; i = i->next) //prolazi se kroz svaki èlan prvog i drugog
		for (Position j = p2->next; j != NULL; j = j->next)

			InsertSorted(result, CreateElement(i->coeff * j->coeff, i->pow + j->pow)); // èlanovi se množe

	return 0; 
}


int FreePoly(Position head) { //funkcija za oslobaðanje memorije svih èlanova polinoma
	
	while (head->next) {
		Position temp = head->next;
		head->next = temp->next;
		free(temp);
	}

	return 0;
}




