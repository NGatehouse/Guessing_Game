#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "readLine.h"
#include "database.h"
#include "guess.h"

#define YES 1
#define NO 0
#define Y 89
#define y 121
#define QUESTION 12
#define QMARK 13
#define GETANIMAL 14
#define GUESS 15
#define EMPTY 16
#define ANSWER 17

/* attempts to read from database
 * creates a database if the file doesn't exist
 * otherwise checks for a corrupted or empty file
 * if the tree is larger than one leaf, begin asking questions
 * otherwise make an immediate guess
 * */

int main(){
   FILE * dataBase;
   if( (dataBase = fopen("qa.db","r")) == NULL){
      printf("qa.db: No such file or directory\n");
      printf("Unable to read database qa.db. Starting fresh.\n\n");		
      createDatabase();	
   }else{
      struct node* root = create_tree(dataBase);
      fclose(dataBase);

      if(root==NULL){
         printf("\n");
         createDatabase();
      }else if(emptyLines % 2 != 0){
         fprintf(stderr,"corrupted database -- prompt with missing response.\n");
         exit(2);
      }else if((2*leaves) != emptyLines){
         fprintf(stderr,"corrupted database -- prompt with missing response.\n");
         exit(3);
      }

      if((root->yes == NULL) && (root->no == NULL)){     
         makeGuess(root,root);
      }else{
         askQuestion(root,root);
      }      
   }
   return 0;
}

/*creates database from scratch*/

void createDatabase(){
   FILE * dataBase;
   char * reply;
   struct node *root;
   if((dataBase = fopen("qa.db","w")) == NULL){
      fprintf(stderr,"unable to write to file qa.db\n");
      exit(-1);
   }else{
      printf("What is it (with article)? ");
      reply = checkResponse(NULL,EMPTY);

      root = createNode(reply);
      writeToDatabase(root,dataBase);
      delete_tree(root);
      fclose(dataBase);
      exit(0);
      
   }
}

/* opens qa.db to write and calls writeToDatabase
 * rewrites the database every time
 *
 * root -- the actual root of the tree
 * */

void callWrite(struct node* root){
   FILE * dataBase;
   if((dataBase = fopen("qa.db","w")) == NULL){
      fprintf(stderr,"unable to write to file qa.db\n");
      exit(-2);
   }else{
      writeToDatabase(root,dataBase);
      fclose(dataBase);
   }

}

/* ask questions from the database to the user
 * keeps track of the root for terminating the tree
 *
 * root -- the actual root of the tree
 * question -- the current question we have traversed to
 * */

void askQuestion(struct node* root, struct node* question){
   char * yesOrNo;  
   if((question->yes ==NULL) && (question->no == NULL)){
      makeGuess(root,question);
   }
   printf("%s ",question->sentence);
   yesOrNo = checkResponse(question->sentence,QUESTION);
   if((yesOrNo[0] == y) || (yesOrNo[0] == Y)){
      free(yesOrNo);
      if((question->yes ==NULL) && (question->no == NULL)){
         makeGuess(root,question);
      }else{
         askQuestion(root,question->yes);
      }
   }else{
      free(yesOrNo);
      if( (question->yes == NULL) && (question->no == NULL) ){
         makeGuess(root,question);
      }else{
         askQuestion(root,question->no);
      }
   }   
}

/* checks for empty responses from the user
 *
 * question -- if a certain string needs to be reasked
 * boolean -- determines where in the program the user is responding to
 * */

char * checkResponse(char * question, int boolean){
   char * response;
   if((response = readLine(stdin)) == NULL){
      fprintf(stderr,"error reading input from stdin\n");
   }else if(*response == '\0'){
      printf("An empty response is not acceptable. Please answer again.\n");
      if(boolean == GUESS){   
         printf("Is it %s? ",question);
         return checkResponse(question,GUESS);
      }else if(boolean == QMARK){
         printf("? ");
         return checkResponse(NULL,QMARK);
      }else if((boolean == GETANIMAL) || (boolean == EMPTY) ){
         printf("What is it (with article)? ");
         return checkResponse(NULL,GETANIMAL);
      }else if(boolean == ANSWER){
         printf("\nWhat is the answer to the question for %s? ",question);
         return checkResponse(question,ANSWER);
      }else if(boolean == QUESTION){ 
         printf("%s ",question);
         return checkResponse(question,QUESTION);
      }
   }
   return response; 
}

/* now at the leaf of the tree and going to make a guess
 * win: pats itself on the back for guessing right, exits
 * loss: knows that failure is an opportunity for growth
 * asks what the actual guess is and learns from it's mistake
 * 
 * root -- the actual root of the tree
 * guess -- the leaf we have arrived at
 * */

void makeGuess(struct node* root, struct node* guess){
   char * yesOrNo;
   char * newQuestion;
   char * newAnimal;
   char * oldAnimal = guess->sentence;
   printf("Is it %s? ",oldAnimal);
   yesOrNo = checkResponse(oldAnimal,GUESS);
   if((yesOrNo[0] == y) || (yesOrNo[0] == Y)){
      free(yesOrNo);
      printf("\nMy, am I clever. :)\nThanks for playing.\n\n");
      callWrite(root);
      delete_tree(root);
      exit(0);
   }else{
      free(yesOrNo);
      printf("\nHow disappointing.\n");
      printf("What is it (with article)? ");
      newAnimal = checkResponse(NULL,GETANIMAL);
     
      printf("What is a yes/no question that will distinguish %s from %s?\n? ",newAnimal,oldAnimal);
      newQuestion = checkResponse(NULL,QMARK);
      printf("\nWhat is the answer to the question for %s? ",newAnimal);
      yesOrNo = checkResponse(newAnimal,ANSWER);
      if((yesOrNo[0] == y) ||( yesOrNo[0] == Y)){
         updateTree(newAnimal,newQuestion,guess,YES);
      }else{
         updateTree(newAnimal,newQuestion,guess,NO);
      }
      free(yesOrNo);
      printf("\nI'll get it next time, I'm sure.\nThanks for playing.\n\n");
      callWrite(root);
      delete_tree(root);
      exit(0);        
   } 
}
