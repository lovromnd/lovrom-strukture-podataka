#define _CRT_SECURE_NO_WARNINGS    
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 250                 

typedef struct city* CityPos;     
typedef struct country* CountryPos; 


typedef struct city {
    char name[MAX];               
    int population;               
    CityPos left;                 
    CityPos right;                
} City;


typedef struct country {
    char name[MAX];              
    CountryPos next;              
    CityPos root;                 
} Country;


CountryPos createCountry(char* name);       
CityPos createCity(char* name, int pop);    

CountryPos insertCountrySorted(CountryPos head, CountryPos newEl); //umetanje države u listu
CityPos insertCityTree(CityPos root, CityPos newCity);             //umetanje grada u stablo

int readCountries(char* filename, CountryPos head); //èitanje datoteke država
int readCities(char* filename, CountryPos c);       //èitanje datoteke gradova

int printCountries(CountryPos head); //ispis država i stabla gradova
int printCityTree(CityPos root);     //inorder ispis stabla gradova

CountryPos findCountry(CountryPos head, char* name); //traženje države po imenu
int searchCityTree(CityPos root, int minPop);        

int freeCityTree(CityPos root);    
int freeCountries(CountryPos head); 


int main()
{
    CountryPos head = NULL;                                  

    head = (CountryPos)malloc(sizeof(Country));              
    if (!head) 
    return -1;

    strcpy(head->name, "");                                  
    head->next = NULL;                                       
    head->root = NULL;                                       

    readCountries("drzave.txt", head);                       

    printf("\n ISPIS DRZAVA I GRADOVA \n");
    printCountries(head->next);           

    char name[MAX];                       
    int minPop;                           

    printf("\nUnesi ime drzave: ");      
    scanf(" %s", name);
    printf("Unesi minimalan broj stanovnika: "); 
    scanf("%d", &minPop);

    CountryPos c = findCountry(head->next, name); 
    if (!c) 
    printf("Drzava ne postoji!\n");      
    else
    {
        printf("\nGradovi s vise od %d stanovnika:\n", minPop); 
        searchCityTree(c->root, minPop);    
    }

    freeCountries(head->next);              
    return 0;
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

CityPos createCity(char* name, int pop)
{
    CityPos p = (CityPos)malloc(sizeof(City)); 
    if (!p) 
    return NULL;                       

    strcpy(p->name, name);  
    p->population = pop;    
    p->left = NULL;         
    p->right = NULL;        

    return p;              
}

CountryPos insertCountrySorted(CountryPos head, CountryPos newEl)
{
    if (head == NULL || strcmp(newEl->name, head->name) < 0) { //ako prazno ili manje od head
        newEl->next = head;      
        return newEl;            
    }

    CountryPos temp = head;      

    while (temp->next != NULL && strcmp(newEl->name, temp->next->name) > 0)
        temp = temp->next;       //trazenje mjesta za umetanje

    newEl->next = temp->next;    //umetanje novog èvora
    temp->next = newEl;

    return head;                 
}

CityPos insertCityTree(CityPos root, CityPos newCity)
{
    if (!root) 
    return newCity;           //ako prazno stablo, novi grad postaje root

    if (newCity->population < root->population)       // manje stanovnika ide lijevo
        root->left = insertCityTree(root->left, newCity);
    else if (newCity->population > root->population)  // više stanovnika ide desno
        root->right = insertCityTree(root->right, newCity);
    else if (strcmp(newCity->name, root->name) < 0)  
        root->left = insertCityTree(root->left, newCity);
    else
        root->right = insertCityTree(root->right, newCity); 

    return root;                        
}

int readCountries(char* filename, CountryPos head)
{
    FILE* f = fopen(filename, "r");      
    if (!f)
    { 
     printf("Greska u otvaranju!\n");
     return -1;
    }

    char country[MAX], file[MAX];         
    while (fscanf(f, " %s %s", country, file) == 2)
    {
        CountryPos newCountry = createCountry(country); //kreiranje države
        readCities(file, newCountry);                  //uèitavanje gradova
        head->next = insertCountrySorted(head->next, newCountry); //umetanje u listu
    }

    fclose(f);                            
    return 0;
}

int readCities(char* filename, CountryPos c)
{
    FILE* f = fopen(filename, "r");       
    if (!f) {
        printf("Greska otvaranja!\n");
        return -1; 
    }

    char line[MAX];                        // linija za èitanje
    while (fgets(line, MAX, f))
    {
        line[strcspn(line, "\n")] = 0;    
        char* cityName = strtok(line, ","); 
        char* popStr = strtok(NULL, ",");   
        if (cityName && popStr) {
            int pop = atoi(popStr);         //string u int
            CityPos newCity = createCity(cityName, pop); 
            c->root = insertCityTree(c->root, newCity); 
        }
    }

    fclose(f);
    return 0;
}

int printCityTree(CityPos root)
{
    if (!root) 
    return 0;         
    
    printCityTree(root->left);           
    printf("   %s (%d)\n", root->name, root->population); 
    printCityTree(root->right);          
    return 0;
}

int printCountries(CountryPos head)
{
    while (head) {                        
        printf("\nDrzava: %s\n", head->name); 
        printCityTree(head->root);        
        head = head->next;                
    }
    return 0;
}

CountryPos findCountry(CountryPos head, char* name)
{
    while (head) {                        
        if (strcmp(head->name, name) == 0)
            return head;                  
        head = head->next;                 
    }
    return NULL;                           
}

int searchCityTree(CityPos root, int minPop)
{
    if (!root)
    return 0;                   
    if (root->population > minPop)
        printf("%s (%d)\n", root->name, root->population); 
    searchCityTree(root->left, minPop);   
    searchCityTree(root->right, minPop);  
    return 0;
}

int freeCityTree(CityPos root)
{
    if (!root)
    return 0;                  
    freeCityTree(root->left);              
    freeCityTree(root->right);           
    free(root);                            
    return 0;
}

int freeCountries(CountryPos head)
{
    CountryPos temp;
    while (head) {
        temp = head;                       
        freeCityTree(head->root);          
        head = head->next;                 
        free(temp);                        
    }
    return 0;
}


