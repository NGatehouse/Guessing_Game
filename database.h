#ifndef DATABASE_H
#define DATABASE_H
extern int emptyLines;
extern int leaves;
struct node{
   char * sentence;
   struct node * yes;/*left*/
   struct node * no;/*right*/
};

struct node* create_tree(FILE *databaseR);
struct node* createNode(char *str);
void writeToDatabase(struct node* root, FILE *databaseW);
void delete_tree(struct node* leaf);
void updateTree(char* animal, char* question, struct node* guess, int boolean);
#endif
