/*
Напишите программу, которая возвращает сумму всех элементов в дереве.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node
{
    int key;
    struct node *left, *right, *root;
} node;


node *getNode(int value, node *root);                           
void pushNode(int value, node **tree);                        
void printTreeNode(node *tree, const char *dir, int level);      
int sumNode(node *tree);                                        


int main(int argc, char const *argv[])
{
    int n[] = {1, 15, 8, 4, 13, 9, 16, 12, 11};
    node *tree = NULL;
    for (int i = 0; i < 9; i++)
    {
        pushNode(n[i], &tree);
    }
    printTreeNode(tree, "Root", 0);
    printf("%d ", sumNode(tree));  
    return 0;
}

node *getNode(int value, node *root)
{
    node *tmp = (node *)malloc(sizeof(node));
    tmp->key = value;
    tmp->left = tmp->right = NULL;
    tmp->root = root;
    return tmp;
}

void pushNode(int value, node **tree)
{
    node *tmp = NULL;
    node *insert = NULL;
    if (*tree == NULL)
    {
        *tree = getNode(value, NULL);
        return;
    }
    tmp = *tree;
    while (tmp)
    {
        if (value > tmp->key)
        {
            if (tmp->right)
            {
                tmp = tmp->right;
                continue;
            }
            else
            {
                tmp->right = getNode(value, tmp);
                return;
            }
        }
        else if (value < tmp->key)
        {
            if (tmp->left)
            {
                tmp = tmp->left;
                continue;
            }
            else
            {
                tmp->left = getNode(value, tmp);
                return;
            }
        }
    }
}

void printTreeNode(node *tree, const char *dir, int level)
{
    if (tree) {
        printf("lvl %d %s = %d\n", level, dir, tree->key);
        printTreeNode(tree->left, "left", level+1);
        printTreeNode(tree->right, "right", level+1);
    }
}

int sumNode(node *tree)
{
    if (tree == NULL)
    {
        return 0;
    }
    return tree->key + sumNode(tree->left) + sumNode(tree->right);
}