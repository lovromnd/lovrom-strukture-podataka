#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 250

    

typedef struct city* CityPos;
typedef struct country* CountryPos;
typedef struct hashTable* HashPos;

typedef struct city {
    char name[MAX];
    int population;
    CityPos left;
    CityPos right;
} City;

typedef struct country {
    char name[MAX];
    CountryPos next;     // za vezanu listu u hash tablici
    CityPos root;        // stablo gradova
} Country;

typedef struct hashTable {
    int size;
    CountryPos* HashList;   // polje pokazivaèa na liste država
} HashTable;




HashPos initHash(int size);
int calcKey(char* name, int size);

CountryPos createCountry(char* name);
CityPos createCity(char* name, int pop);

int insertCountryHash(HashPos h, CountryPos newEl);
CityPos insertCityTree(CityPos root, CityPos newEl);

int readCountries(HashPos h, char* filename);
int readCities(char* filename, CountryPos c);

CountryPos findCountry(HashPos h, char* name);

int printHash(HashPos h);
int printCityTree(CityPos root);

int searchCityTree(CityPos root, int minPop);

int freeCityTree(CityPos root);
int freeHash(HashPos h);




int main()
{
    HashPos h = initHash(11);

    readCountries(h, "drzave.txt");

    printf("\n ISPIS HASH TABLICE\n");
    printHash(h);

    char name[MAX];
    int minPop;

    printf("\n\nUnesi ime drzave: ");
    scanf("%s", name);

    CountryPos c = findCountry(h, name);

    if (!c)
        printf("Drzava ne postoji!\n");
    else
    {
        printf("Unesi minimalan broj stanovnika: ");
        scanf("%d", &minPop);

        printf("\nGradovi s vise od %d stanovnika:\n", minPop);
        searchCityTree(c->root, minPop);
    }

    freeHash(h);
    return 0;
}




HashPos initHash(int size)
{
    HashPos h = (HashPos)malloc(sizeof(HashTable));
    if (!h)
    return NULL;

    h->size = size;
    h->HashList = (CountryPos*)malloc(sizeof(CountryPos) * size);

    for (int i = 0; i < size; i++)
        h->HashList[i] = NULL;

    return h;
}

int calcKey(char* name, int size)
{
    int sum = 0;

    for (int i = 0; i < 5 && name[i] != '\0'; i++)
        sum += name[i];

    return sum % size;
}




CountryPos createCountry(char* name)
{
    CountryPos p = (CountryPos)malloc(sizeof(Country));
    if (!p) 
     return NULL;
    
    strcpy(p->name, name);
    p->next = NULL;
    p->root = NULL;

    return p;
}

int insertCountryHash(HashPos h, CountryPos newEl)
{
    int key = calcKey(newEl->name, h->size); //izracun indeksa

    CountryPos curr = h->HashList[key]; //pocetak liste
    CountryPos prev = NULL;

    while (curr && strcmp(curr->name, newEl->name) < 0)
    {
        prev = curr;
        curr = curr->next;
    }

    if (!prev) //ako ide na pocetak liste
    {
        newEl->next = h->HashList[key];
        h->HashList[key] = newEl;
    }
    else
    {
        newEl->next = curr;
        prev->next = newEl;
    }

    return 0;
}




CityPos createCity(char* name, int pop)
{
    CityPos p = (CityPos)malloc(sizeof(City));
    if (!p)
    return NULL;

    strcpy(p->name, name);
    p->population = pop;
    p->left = p->right = NULL;

    return p;
}

CityPos insertCityTree(CityPos root, CityPos newEl)
{
    if (!root)
     return newEl;

    if (newEl->population < root->population)
        root->left = insertCityTree(root->left, newEl);

    else if (newEl->population > root->population)
        root->right = insertCityTree(root->right, newEl);

    else if (strcmp(newEl->name, root->name) < 0)
        root->left = insertCityTree(root->left, newEl);

    else
        root->right = insertCityTree(root->right, newEl);

    return root;
}




int readCountries(HashPos h, char* filename)
{
    FILE* f = fopen(filename, "r");
    if (!f)
    return -1;

    char country[MAX], file[MAX];

    while (fscanf(f, "%s %s", country, file) == 2)
    {
        CountryPos newC = createCountry(country);
        readCities(file, newC);
        insertCountryHash(h, newC);
    }

    fclose(f);
    return 0;
}

int readCities(char* filename, CountryPos c)
{
    FILE* f = fopen(filename, "r");
    if (!f)
    return -1;

    char name[MAX];
    int pop;

    while (fscanf(f, "%s %d", name, &pop) == 2)
    {
        CityPos newCity = createCity(name, pop);
        c->root = insertCityTree(c->root, newCity);
    }

    fclose(f);
    return 0;
}




CountryPos findCountry(HashPos h, char* name)
{
    int key = calcKey(name, h->size);

    CountryPos temp = h->HashList[key];

    while (temp)
    {
        if (strcmp(temp->name, name) == 0)
            return temp;
        temp = temp->next;
    }

    return NULL;
}

int searchCityTree(CityPos root, int minPop)
{
    if (!root)
     return 0;

    searchCityTree(root->left, minPop);

    if (root->population > minPop)
        printf("%s (%d)\n", root->name, root->population);

    searchCityTree(root->right, minPop);

    return 0;
}




int printHash(HashPos h)
{
    for (int i = 0; i < h->size; i++)
    {
        printf("\n[%d]: ", i);

        CountryPos temp = h->HashList[i];
        while (temp)
        {
            printf("%s -> ", temp->name);
            temp = temp->next;
        }
        printf("NULL");
    }
    printf("\n");
    return 0;
}

int printCityTree(CityPos root)
{
    if (!root) return 0;

    printCityTree(root->left);
    printf("   %s (%d)\n", root->name, root->population);
    printCityTree(root->right);

    return 0;
}




int freeCityTree(CityPos root)
{
    if (!root) return 0;

    freeCityTree(root->left);
    freeCityTree(root->right);
    free(root);

    return 0;
}

int freeHash(HashPos h)
{
    for (int i = 0; i < h->size; i++)
    {
        CountryPos temp = h->HashList[i];
        while (temp)
        {
            CountryPos del = temp;
            temp = temp->next;

            freeCityTree(del->root);
            free(del);
        }
    }

    free(h->HashList);
    free(h);
    return 0;
}

