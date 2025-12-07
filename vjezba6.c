#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LEN 200
#define LINE_LEN 250

typedef struct article* ArticlePosition;
typedef struct article {
    char name[NAME_LEN];
    int quantity;
    double price;
    ArticlePosition next;
} Article;

typedef struct bill* BillPosition;
typedef struct bill {
    char date[11];
    ArticlePosition articleHead;
    BillPosition next;
} Bill;

/* Prototipovi */
BillPosition CreateBill(const char* date);
ArticlePosition CreateArticle(const char* name, int quantity, double price);
int InsertBillSorted(BillPosition head, BillPosition newBill);
int InsertArticleSorted(ArticlePosition head, ArticlePosition newArticle);
int LoadBills(BillPosition billHead, const char* fileName);
int PrintBills(BillPosition billHead);
int QueryArticle(BillPosition billHead);
int FreeAll(BillPosition billHead);

int main()
{
    Bill billHead;
    billHead.next = NULL;
    billHead.articleHead = NULL;

    if (LoadBills(&billHead, "racuni.txt") != 0) { //ucitavanje racuna
        printf("Greska pri ucitavanju racuna.\n");
        return 1;
    }

    printf("Ucitani racuni:\n"); //ispis racuna
    PrintBills(&billHead);

    QueryArticle(&billHead); //upit korisinika

    FreeAll(&billHead);

    return 0;
}

BillPosition CreateBill(const char* date) //funkcija z kreiranje novog racuna s artiklima
{
    
    BillPosition newBill = (BillPosition)malloc(sizeof(Bill));
    if (!newBill) {
        printf("Greska pri alokaciji racuna.\n");
        return NULL;
    }

    strncpy(newBill->date, date, 10);
    newBill->date[10] = '\0';

   ArticlePosition artHead = (ArticlePosition)malloc(sizeof(Article)); //novi head za artikle unutar racuna
    if (!artHead) {
        printf("Greska pri alokaciji!\n");
        free(newBill);
        return NULL;
    }

    artHead->next = NULL;

    newBill->articleHead = artHead;
    newBill->next = NULL;

    return newBill;
}

ArticlePosition CreateArticle(const char* name, int quantity, double price) //funkcija za kreiranje novog artikla
{

   ArticlePosition newArt = (ArticlePosition)malloc(sizeof(Article));
    if (!newArt) {
        printf("Greska pri alokaciji!\n");
        return NULL;
    }

    strncpy(newArt->name, name, NAME_LEN - 1);
    newArt->name[NAME_LEN - 1] = '\0';
    newArt->quantity = quantity;
    newArt->price = price;
    newArt->next = NULL;

    return newArt;
}

int InsertBillSorted(BillPosition head, BillPosition newBill) //funkcija za sortirano umetanje racuna po datumu
{
    BillPosition prev = head;
    BillPosition curr = head->next;

    while (curr != NULL && strcmp(curr->date, newBill->date) < 0) {
        prev = curr;
        curr = curr->next;
    }

    newBill->next = curr;
    prev->next = newBill;

    return 0;
}

int InsertArticleSorted(ArticlePosition head, ArticlePosition newArticle) //funkcija za sortirano umetanje artikla po nazivu
{
    ArticlePosition prev = head;
    ArticlePosition curr = head->next;

    while (curr != NULL && strcmp(curr->name, newArticle->name) < 0) {
        prev = curr;
        curr = curr->next;
    }

    newArticle->next = curr;
    prev->next = newArticle;

    return 0;
}

int LoadBills(BillPosition billHead, const char* fileName) //funkcija za ucitavanje svih racuna
{
    FILE* f = fopen(fileName, "r");
    if (!f) {
        printf("Ne mogu otvoriti %s!\n", fileName);
        return -1;
    }

    char billFileName[NAME_LEN];
    char line[LINE_LEN];

    while (fgets(billFileName, NAME_LEN, f)) { //citanje imena racuna
        
        int len = strlen(billFileName);
        if (len > 0 && billFileName[len - 1] == '\n')
            billFileName[len - 1] = '\0';

        if (billFileName[0] == '\0') continue; // 

        FILE* fbill = fopen(billFileName, "r");
        if (!fbill) {
            printf("Ne mogu otvoriti racun %s\n", billFileName);
            continue;
        }

        
        if (!fgets(line, LINE_LEN, fbill)) { 
          fclose(fbill);
          continue; 
        }

        len = strlen(line);
        if (len > 0 && line[len - 1] == '\n')
         line[len - 1] = '\0';

        BillPosition newBill = CreateBill(line);
        if (!newBill) { 
            fclose(fbill);
            continue; 
        }

        //citanje artikala
        while (fgets(line, LINE_LEN, fbill)) {
            char name[NAME_LEN];
            int quantity;
            double price;

            if (sscanf(line, " %s %d %lf", name, &quantity, &price) == 3) {
                ArticlePosition newArt = CreateArticle(name, quantity, price);

                if (newArt) 
                    InsertArticleSorted(newBill->articleHead, newArt);
            }
        }

        fclose(fbill);
        InsertBillSorted(billHead, newBill);
    }

    fclose(f);
    return 0;
}



int PrintBills(BillPosition billHead) //funkcija za ispis racuna
{
    BillPosition currBill = billHead->next;

    if (!currBill) {
        printf("Nema ucitanih racuna.\n");
        return 0;
    }

    while (currBill != NULL) {
        printf("\nRacun - datum: %s\n", currBill->date);
        printf("\n");

        ArticlePosition currArt = currBill->articleHead->next;
        if (!currArt)
            printf(" (Nema artikala)\n");

        while (currArt != NULL) {
            printf(" %s | kolicina: %d | cijena: %f\n",
                currArt->name, currArt->quantity, currArt->price);
            currArt = currArt->next;
        }

        currBill = currBill->next;
    }

    printf("\n");
    return 0;
}

int QueryArticle(BillPosition billHead) //funkcija za upit korisnika
{
    char Name[NAME_LEN];
    char startDate[11];
    char endDate[11];

    double totalMoney = 0.0;
    int totalQuantity = 0;

    printf("Unesi naziv artikla: ");
    scanf(" %s", Name);

    printf("Unesi pocetni datum (YYYY-MM-DD): ");
    scanf(" %s", startDate);

    printf("Unesi zavrsni datum (YYYY-MM-DD): ");
    scanf(" %s", endDate);

    BillPosition currBill = billHead->next;

    while (currBill != NULL) {

        if (strcmp(currBill->date, startDate) >= 0 && strcmp(currBill->date, endDate) <= 0) { //provjera je li datum racuna u trazenom intervalu

            ArticlePosition currArt = currBill->articleHead->next;

            while (currArt != NULL) {
                if (strcmp(currArt->name, Name) == 0) {
                    totalQuantity += currArt->quantity;
                    totalMoney += currArt->quantity * currArt->price;
                }
                currArt = currArt->next;
            }
        }
        currBill = currBill->next;
    }

    printf("\nRezultat za artikl %s od %s do %s:\n", Name, startDate, endDate);
    printf("Ukupna kolicina: %d\n", totalQuantity);
    printf("Ukupno potroseno: %f\n", totalMoney);

    return 0;
}

int FreeAll(BillPosition billHead) //funkcija za oslobadjanje memorije
{
    BillPosition currBill = billHead->next;

    while (currBill != NULL) {
        BillPosition temp = currBill;
        currBill = currBill->next;

        ArticlePosition currArt = temp->articleHead->next;
        while (currArt != NULL) {
            ArticlePosition temp2 = currArt;
            currArt = currArt->next;
            free(temp2);
        }

        free(temp->articleHead);
        free(temp);
    }

    billHead->next = NULL;
    return 0;
}
