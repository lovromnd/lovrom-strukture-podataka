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
    CityPos next;                 
} City;


typedef struct country {
    char name[MAX];               
    CountryPos left;              
    CountryPos right;            
    CityPos head;                 
} Country;


CountryPos createCountry(char* name);       
CityPos createCity(char* name, int pop);    
CountryPos insertCountryTree(CountryPos root, CountryPos newEl); 
CityPos insertCityList(CityPos head, CityPos newEl);             

CountryPos readCountries(char* filename, CountryPos root); 
int readCities(char* filename, CityPos head);       

int printCountryTree(CountryPos root); 
int printCityList(CityPos head);       

CountryPos findCountry(CountryPos root, char* name); 
int searchCityList(CityPos head, int minPop);        

int freeCityList(CityPos head);       
int freeCountryTree(CountryPos root); 


int main()
{
    CountryPos head = NULL;

    head = (CountryPos)malloc(sizeof(Country));
    if (!head)
        return -1;

    
    head->left = NULL;
    head->right = NULL;                         

    head->right = readCountries("drzave.txt", head->right);    

    printf("\nISPIS DRZAVA I GRADOVA\n");
    printCountryTree(head->right);                 

    char name[MAX];                         
    int minPop;                             

    printf("\nUnesi ime drzave: ");        
    scanf(" %s", name);
    printf("Unesi minimalan broj stanovnika: "); 
    scanf("%d", &minPop);

    CountryPos c = findCountry(head->right, name); 
    if (!c)
        printf("Drzava ne postoji!\n"); 
    else
    {
        printf("\nGradovi s vise od %d stanovnika:\n", minPop);
        searchCityList(c->head, minPop);    
    }

    freeCountryTree(head->right);                  
    free(head);
    return 0;
}


CountryPos createCountry(char* name)
{
    CountryPos p = (CountryPos)malloc(sizeof(Country)); 
    if (!p) 
    return NULL;                                

    strcpy(p->name, name);       
    p->left = p->right = NULL;   

    p->head = (CityPos)malloc(sizeof(City)); 
    p->head->next = NULL;                    

    return p;                                
}


CityPos createCity(char* name, int pop)
{
    CityPos p = (CityPos)malloc(sizeof(City)); 
    if (!p) 
    return NULL;

    strcpy(p->name, name);      
    p->population = pop;        
    p->next = NULL;             

    return p;                   
}


CountryPos insertCountryTree(CountryPos root, CountryPos newEl)
{
    if (!root) 
    return newEl;                  

    if (strcmp(newEl->name, root->name) < 0) 
        root->left = insertCountryTree(root->left, newEl);
    else                                   
        root->right = insertCountryTree(root->right, newEl);

    return root;                             
}


CityPos insertCityList(CityPos head, CityPos newEl)
{
    CityPos temp = head;                     

    while (temp->next)
    {
        if (newEl->population < temp->next->population)
            break;                            
        else if (newEl->population == temp->next->population && strcmp(newEl->name, temp->next->name) < 0)
            break;                            //isti pop, sortiranje po imenu
        temp = temp->next;                    
    }

    newEl->next = temp->next;                 //umetanje
    temp->next = newEl;

    return head;
}


CountryPos readCountries(char* filename, CountryPos root)
{
    FILE* f = fopen(filename, "r");           
    if (!f) {
        printf("Ne mogu otvoriti %s\n", filename); 
        return root;
    }

    char country[MAX], file[MAX];             
    while (fscanf(f, " %s %s", country, file) == 2)
    {
        CountryPos newCountry = createCountry(country); 
        readCities(file, newCountry->head);             
        root = insertCountryTree(root, newCountry);   
    }

    fclose(f);                                  
    return root;
}


int readCities(char* filename, CityPos head)
{
    FILE* f = fopen(filename, "r");           
    if (!f) {
        printf("Ne mogu otvoriti %s\n", filename);
        return -1; 
    }

    char line[MAX];                           
    while (fgets(line, MAX, f))
    {
        line[strcspn(line, "\n")] = 0;        
        char* cityName = strtok(line, ",");   
        char* popStr = strtok(NULL, ",");     
        if (cityName && popStr)
        {
            int pop = atoi(popStr);           
            CityPos newCity = createCity(cityName, pop); 
            insertCityList(head, newCity);    
        }
    }

    fclose(f);
    return 0;
}


int printCountryTree(CountryPos root)
{
    if (!root) 
    return 0;                        
    printCountryTree(root->left);               
    printf("\nDrzava: %s\n", root->name);      
    printCityList(root->head);                  
    printCountryTree(root->right);              
    return 0;
}


int printCityList(CityPos head)
{
    CityPos temp = head->next;                 
    while (temp)
    {
        printf("   %s (%d)\n", temp->name, temp->population);
        temp = temp->next;
    }
    return 0;
}


CountryPos findCountry(CountryPos root, char* name)
{
    if (!root)
    return NULL;                      
    if (strcmp(name, root->name) == 0) 
    return root;  
    else if (strcmp(name, root->name) < 0) 
    return findCountry(root->left, name); 
    else
    return findCountry(root->right, name);
}


int searchCityList(CityPos head, int minPop)
{
    CityPos temp = head->next;                  
    while (temp)
    {
        if (temp->population > minPop)          
            printf("%s (%d)\n", temp->name, temp->population);
        temp = temp->next;
    }
    return 0;
}


int freeCityList(CityPos head)
{
    CityPos temp;
    while (head)
    {
        temp = head;                            
        head = head->next;                      
        free(temp);                             
    }
    return 0;
}


int freeCountryTree(CountryPos root)
{
    if (!root)
    return 0;                         
    freeCountryTree(root->left);                 
    freeCountryTree(root->right);                
    freeCityList(root->head);                    
    free(root);                                  
    return 0;
}
