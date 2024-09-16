#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
struct Node{
    char * name;
    int isFile;
    struct Node * sibling;
    struct Node * child;
};

//isfile when 0 means it is a directory

struct Node * createNode(char * name1, int isfile){
    struct Node * newNode = (struct Node * )malloc(sizeof(struct Node));
    newNode->sibling = NULL;
    newNode->child = NULL;
    newNode->name = (char * )malloc(100*sizeof(char));
    strcpy(newNode->name,name1);
    newNode->isFile = isfile;
    return newNode;
}

void insertNode(struct Node * parent, char * name1, int isfile){
    struct Node * newNode = createNode(name1,isfile);
    if (parent->child == NULL){
        parent->child = newNode;
    }
    else{
        struct Node * temp = parent->child;
        while(temp->sibling!=NULL){
            temp = temp->sibling;
        }
        temp->sibling = newNode;
        
    }
}

struct Node * searchDirectory(struct Node * current, char * name1){
    if (current == NULL){
        return NULL;
    }
    if (!current->isFile && strcmp(current->name,name1) == 0){
        return current;
    }
    struct Node * found = searchDirectory(current->sibling,name1);

    if (found != NULL){
        return found;
    }
    return searchDirectory(current->child,name1);
}

void printFileSystem(struct Node* current, int level) {
    if (current == NULL) {
        return;
    }
    for (int i = 0; i < level; i++) {
        printf("    ");  // Indentation based on depth
    }
    printf("%s%s\n", current->name, current->isFile ? " (File)" : " (Directory)");
    printFileSystem(current->child, level + 1);   // Print child nodes recursively
    printFileSystem(current->sibling, level);     // Print sibling nodes
}

int main(){

    struct Node * root = createNode("root",0);

    char command[15];
    char name[100];
    char parentname[100];
    int isfile1;

    while(1){
        printf("Enter the operation : \"add\" , \"print\" , \"exit\"\t:" );
        scanf("%s",command);
        for(int i = 0; command[i]; i++){
            command[i] = tolower(command[i]);
        }
        if (strcmp(command,"add") == 0){
            printf("Parent Name : ");
            scanf("%s",parentname);
            printf("Name of File : ");
            scanf("%s",name);
            printf("Directory?? (0 for yes, 1 for no) : ");
            scanf("%d",&isfile1);
            struct Node * temp = searchDirectory(root,parentname);
            if (temp == NULL){
                printf("Directory Not Found\n");
            }
            else if (temp->isFile){
                printf("Directory Not Found\n");
            }
            else{
                insertNode(temp,name,isfile1);
            }
        }
        else if (strcmp(command,"print") == 0){
            printf("/n/n/n");
            printFileSystem(root,0);
            printf("/n/n/n");
        }
        else{
            printf("Exiting\n");
            break;
        }
    }
    printf(" ");
    return 0;
}