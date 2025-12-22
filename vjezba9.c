#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct tree* treePos;
typedef struct tree {
	int value;
	treePos right;
	treePos left;

} Tree;

treePos Insert(treePos root, int value);
treePos CreateTree(treePos root, int n);
int Replace(treePos root);
int InorderFile(treePos root, FILE* f);
int FreeTree(treePos root);



int main() 
  {
    treePos root = NULL;
    int n = 10;

        
    srand(time(NULL));

        
    root = CreateTree(root, n);

        
    FILE* f = fopen("stablo.txt", "w");
    if (f == NULL) {
    printf("Greska pri otvaranju datoteke!\n");
    return 1;
        }

    fprintf(f, "Inorder prije replace:\n");
    InorderFile(root, f);

        
    Replace(root);

    fprintf(f, "\nInorder nakon replace:\n");
    InorderFile(root, f);

        
    fclose(f);

        
    FreeTree(root);

    return 0;

   }

    
treePos Insert(treePos root, int value) //funkcija za umetanje elemenata
    {

    if (root == NULL) {
        root = (treePos)malloc(sizeof(Tree)); //zauzimanje memorije za novi cvor
        root->value = value;
        root->left = NULL;
        root->right = NULL;
        return root;
    }


    if (value < root->value)
        root->left = Insert(root->left, value);


    else if (value > root->value)
        root->right = Insert(root->right, value);

    return root;
    }

   
treePos CreateTree(treePos root, int n) //funkcija za kreiranje stabla
    {
      int i, value;

      for (i = 0; i < n; i++) {
          value = rand() % 81 + 10;   
          root = Insert(root, value);
      }

      return root;
    }

    
int Replace(treePos root) //funkcija za zamjenu vrijednosti
    {
      if (root == NULL)
          return 0;

        int prevValue = root->value; //pamcenje stare vrijenosti

        root->value = Replace(root->left) + Replace(root->right);

        return root->value + prevValue; //ukupna suma podstabla
    }

    
int InorderFile(treePos root, FILE * f) //inorder ispis
    {
      if (root != NULL) {
          InorderFile(root->left, f);
          fprintf(f, "%d ", root->value);
          InorderFile(root->right, f);
      }
        return 0;
    }

    
int FreeTree(treePos root)
    {
      if (root != NULL) {
          FreeTree(root->left);
          FreeTree(root->right);
          free(root);
      }
        return 0;
    }



