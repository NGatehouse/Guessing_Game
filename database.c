#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "readLine.h"
#include "database.h"
#include "guess.h"

#define YES 1
#define NO 0

/* globals for corrupt file checking*/
int emptyLines = 0;
int leaves = 0;

/* traverses the tree and free's all the nodes
 * starting at the leaf and working it's way up
 *
 * leaf -- the current leaf up for freeing
 * */

void delete_tree(struct node* leaf){
   if(leaf != NULL){
      delete_tree(leaf->yes);
      delete_tree(leaf->no);
      free(leaf->sentence);
      free(leaf);
   }
}

/* creates a new node and returns it's pointer
 *
 * str -- the string to be held by the node
 * */

struct node* createNode(char *str){
   struct node* newNode = (struct node*) malloc(sizeof(struct node));
   newNode->sentence=str;
   newNode->yes = NULL;
   newNode->no = NULL;

   return newNode;
}

/* used when the program is terminated
 * prints the tree in preorder traversal to qa.db
 *
 * root -- the actual root of the tree at this time
 * databaseW -- qa.db opened to 'write'
 * */

void writeToDatabase(struct node* root, FILE *databaseW){
   if( root!=NULL){
      fputs(root->sentence,databaseW);
      fputs("\n",databaseW);
      writeToDatabase(root->yes,databaseW);
      writeToDatabase(root->no,databaseW);
   }else{
      fputs("\n",databaseW);
   }
     
}

/* initializes the tree at the start of the program
 * also aids in corrupt file checking by mutating the globals
 *
 * databaseR -- FILE* to be read from (qa.db)
 */

struct node* create_tree(FILE *databaseR){
   char *input;
   struct node* root;
   if((input = readLine(databaseR)) == NULL){
      free(input);
      return NULL;
   }else if(*input == '\0'){
      free(input);
      emptyLines = emptyLines + 1;
      return NULL;
   }
   root = createNode(input);
   root->yes = create_tree(databaseR);
   root->no = create_tree(databaseR);
   if((root->yes == NULL) && (root->no == NULL)){
      leaves = leaves + 1;
   } 
   return root;
}

/* Replace guess (an animal) with the question given
 * but store the animal in a temp value.
 * Depending on the answer given by the user put the 
 * animals in their correct orientation
 *
 * animal -- new value given
 * question -- question corresponding to guess and animal
 * guess -- what the program thought it was
 * boolean -- determines if it was a yes or no
 * */

void updateTree(char* animal, char* question, struct node* guess, int boolean){
   char * tempGuess = guess->sentence;
   guess->sentence = question;
   if(boolean == NO){
      guess->no = createNode(animal);
      guess->yes = createNode(tempGuess);
   }else{
      guess->yes = createNode(animal);
      guess->no = createNode(tempGuess);
   }
}
