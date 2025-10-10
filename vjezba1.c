#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAX_POINTS 50

typedef struct {      //Struktura student sa imenom, prezimenom i brojem bodova
	char name[50];
	char surname[50];
	double points;
} Student;

int countLines(){      //Funkcija za brojanje redova u datoteci
	FILE* f = fopen("studenti.txt", "r"); //Otvaranje datoteke za citanje
	if (!f) {
		printf("Greška: datoteka se ne može otvoriti!\n");
		return 0;

	}
	int count = 0; //Brojac linija
	char buffer[250]; //privremeni buffer za citanje linija
	while (fgets(buffer, sizeof(buffer), f) != NULL) {
		count++; // povecava brojac za svaki red
	}
	fclose(f);
	return count;

}

void readStudents(Student* students, int count) { // funkcija za ucitavanje studenata iz datoteke u strukturu

	FILE* f = fopen("studenti.txt", "r");
	if (!f) {
		printf("Greška: datoteka se ne može otvoriti\n");
		return;

	}
	for (int i = 0; i < count; i++) { // for petlja za citanje studenata iz datoteke
		fscanf(f, " %s %s %lf", students[i].name, students[i].surname, &students[i].points);

	}
	fclose(f); //zatvaranje datoteke
}

double getRelativePoints(double points) { //Funkcija za racunanje relativnih bodova

	double rel = (points / MAX_POINTS) * 100;
	return rel;
}


void printStudents(Student* students, int count) { // funkcija za ispisivanje studenata i bodova

	printf("Ime  Prezime  Bodovi   Relativni\n\n");

	for (int i = 0; i < count; i++) {
		double rel = getRelativePoints(students[i].points);

		printf("%s %s %lf %lf%% \n", students[i].name, students[i].surname, students[i].points, rel);

	}
}

int main() {

	int lineCount = countLines();

	if (lineCount == 0) {
		printf("Datoteka je prazna\n");
		return -1; //izlaz iz programa ako se datoteka ne moze otvoriti
	}

	Student* list = (Student*)malloc(lineCount * sizeof(Student)); //dinamicka alokacija memorije
	if (!list) {
		printf("Greška: ne mogu alocirati memoriju\n");
		return -1; //izlaz ako memorija nije uspjesno alocirana
	}

	readStudents(list, lineCount); //ucitavanje studenata u memoriju
	printStudents(list, lineCount); //ispis podataka

	free(list); //oslobaðanje zauzete memorije
	
	return 0;





}