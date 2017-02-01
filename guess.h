#ifndef GUESS_H
#define GUESS_H
void callWrite(struct node* root);
void makeGuess(struct node* root, struct node* guess);
void askQuestion(struct node* root, struct node* question);
char * checkResponse();
void createDatabase();
#endif
