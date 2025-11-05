#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 50

typedef struct postfix* Position; //definicija strukture
typedef struct postfix {
    double number; //broj koji se pohranjuje na stog
    Position next;
} postfix;

int Push(Position head, double num); //deklaracije funkcija
double Pop(Position head);

int main() {
    Position head = (Position)malloc(sizeof(postfix)); //alokacija memorije za head stoga
    if (!head) {
        printf("Greska pri alokaciji memorije!\n");
        return -1;
    }
    head->next = NULL;

    FILE* f = fopen("postfix.txt", "r"); //otvaranje datoteke
    if (!f) {
        printf("Greska pri otvaranju datoteke!\n");
        return -1;
    }

    char token[N]; //buffer za svaku procitani broj ili operator
    double x;

    while (fscanf(f, " %s", token) == 1) {
        
        if (sscanf(token, "%lf", &x) == 1) { //ako je token broj, stavlja ga se na vrh stoga
            Push(head, x);
        }
        else {                   //za slucaj da je token operator
            char op = token[0];
            double b = Pop(head); //skidanje 2 broja sa stoga
            double a = Pop(head);
            double result = 0;

            switch (op) {  //racunanje rezultata ovisno o operatoru
            case '+': result = a + b; 
            break;
            case '-': result = a - b; 
            break;
            case '*': result = a * b; 
            break;
            case '/': result = a / b; 
            break;

            default:
                printf("Nepoznat operator: %c\n", op);
                fclose(f);
                return -1;
            }

            Push(head, result); //rezultat se stavlja na vrh stoga
        }
    }
    double finalResult = Pop(head); //broj koji ostane na stogu je konacni rezultat
    printf("Rezultat: %lf\n", finalResult);

    fclose(f);
    free(head);
    return 0;
}

int Push(Position head, double num) { //funkcija za guranje novog broja na vrh stoga
   
    Position newEl = (Position)malloc(sizeof(postfix)); //alociranje novog cvora
    if (!newEl) {
        printf("Greska pri alokaciji memorije!\n");
        return -1;
    }

    newEl->number = num; //broj se upisuje u novi cvor
    newEl->next = NULL;

    Position q = head;

    newEl->next = q->next;
    q->next = newEl;
    return 0;
}

double Pop(Position head) { //funkcija za skidanje broja s vrha stoga
    if (head->next == NULL) {
        printf("Stog je prazan!\n");
        return 0;
    }
    double num;

    Position temp = head->next; //privremeni pokazivac na prvi element
     num = temp->number; //spremanje broja iz tog elementa

    head->next = temp->next; //preskakanje prvog elementa
    free(temp);
    temp = NULL;

    return num; //vraca se broj koji je bio na vrhu stoga
}
