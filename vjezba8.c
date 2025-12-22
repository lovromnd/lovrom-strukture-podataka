#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct tree* treePos;
typedef struct tree {
    int value;
    treePos left;
    treePos right;
} Tree;

treePos Insert(treePos root, int value);
treePos Delete(treePos root, int value);
treePos Find(treePos root, int value);
treePos FindMin(treePos root);

int Inorder(treePos root);
int Preorder(treePos root);
int Postorder(treePos root);
int Height(treePos root);
int PrintLevel(treePos root, int level);
int LevelOrder(treePos root);
int FreeTree(treePos root);

int main()
{
    treePos root = NULL;
    int choice, value;

    do {
        printf("\n--- BINARNO STABLO ---\n");
        printf("1. Unos elementa\n");
        printf("2. Inorder ispis\n");
        printf("3. Preorder ispis\n");
        printf("4. Postorder ispis\n");
        printf("5. Level order ispis\n");
        printf("6. Pronadji element\n");
        printf("7. Obrisi element\n");
        printf("0. Izlaz\n");
        printf("Izbor: ");
        scanf("%d", &choice);

        switch (choice) {

        case 1:
            printf("Unesi vrijednost: ");
            scanf("%d", &value);
            root = Insert(root, value);
            break;

        case 2:
            Inorder(root);
            break;

        case 3:
            Preorder(root);
            break;

        case 4:
            Postorder(root);
            break;

        case 5:
            LevelOrder(root);
            break;

        case 6:
            printf("Unesi vrijednost: ");
            scanf("%d", &value);
            if (Find(root, value))
                printf("Element postoji\n");
            else
                printf("Element ne postoji\n");
            break;

        case 7:
            printf("Unesi vrijednost: ");
            scanf("%d", &value);
            root = Delete(root, value);
            break;

        case 0:
            FreeTree(root);
            break;

        default:
            printf("Pogresan izbor!\n");
        }

    } while (choice != 0);

    return 0;
}

treePos Insert(treePos root, int value) //za umetanje novog elementa u stablo
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


treePos Find(treePos root, int value) //za pronalazak elementa u stablu
{
    if (root == NULL)
        return NULL;

    if (value < root->value)
        return Find(root->left, value); //ako je trazena vrijednost manja od vrijednosti u cvoru pretrazuje se lijevo podstablo
    else if (value > root->value)
        return Find(root->right, value); //ako je veca pretrazuje se desno
    else
        return root; //ako je jednaka element je pronadjen
}


treePos FindMin(treePos root)
{
    if (root == NULL)
        return NULL;

    while (root->left != NULL)
        root = root->left;

    return root;
}


treePos Delete(treePos root, int value)
{

    if (root == NULL)
        return NULL;


    if (value < root->value)
        root->left = Delete(root->left, value);


    else if (value > root->value)
        root->right = Delete(root->right, value);


    else { //ako je vrijednost jednaka pronadjen je cvor
        if (root->left != NULL && root->right != NULL) { //ako cvor ima i lijevo i desno dijete
            treePos temp = FindMin(root->right); //u desnom podstablu trazi se najmanji element
            root->value = temp->value; //njegova vrijednost se kopira u trenutni cvor
            root->right = Delete(root->right, temp->value); //brise se cvor cija je vrijednost preuzeta
        }
        else { //ako ima jedno ili nijedno dijete
            treePos temp = root;

            if (root->left == NULL) //ako nema lijevo veze se desno
                root = root->right;
            else
                root = root->left; //ako nema desno veze se lijevo

            free(temp);
        }
    }
    return root; //vraca se novi root stabla
}


int Inorder(treePos root)
{
    if (root != NULL) {
        Inorder(root->left);
        printf("%d ", root->value);
        Inorder(root->right);
    }
    return 0;
}

int Preorder(treePos root)
{
    if (root != NULL) {
        printf("%d ", root->value);
        Preorder(root->left);
        Preorder(root->right);
    }
    return 0;
}

int Postorder(treePos root)
{
    if (root != NULL) {
        Postorder(root->left);
        Postorder(root->right);
        printf("%d ", root->value);
    }
    return 0;
}


int Height(treePos root)
{
    if (root == NULL)
        return 0;

    if (Height(root->left) > Height(root->right)) ///ako je lijevo podstablo vece od desnog
        return Height(root->left) + 1; //ukupna visina je visina lijevog + trenutni cvor
    else
        return Height(root->right) + 1;
}

int PrintLevel(treePos root, int level)
{
    if (root == NULL)
        return 0;

    if (level == 1)
        printf("%d ", root->value);
    else {
        PrintLevel(root->left, level - 1);
        PrintLevel(root->right, level - 1);
    }

    return 0;
}


int LevelOrder(treePos root)
{
    int i;
    int h = Height(root);

    for (i = 1; i <= h; i++)
        PrintLevel(root, i);

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
