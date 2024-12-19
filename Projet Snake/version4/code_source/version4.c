/*!
*
* @file version4.c
* @author Parveau le Saint Korentin
* @brief Programme de jeu Snake
* @version 4.0
* @date 2024-11-24
*
* Ce programme est une quatrième version d'un jeu snake dans le terminal Linux. Cette quatrième version consiste au
* déplacement du serpent dans un plateau définis avec des bordures, des pavés et une pomme placés aléatoirement sur le plateau. 
* Le programme démarre sur le serpent se dirigeant vers la droite.
* L'utilisateur peut ensuite le diriger et doit aller manger des pommes pour grandir, si il rentre en collision avec un élément d'un pavé ou de la bordure, 
* celà mettra fin au programme.
* Il peut aussi mettre fin au programme en appuyant sur la touche a
*
*/

/*Déclaration des bibliothèques*/
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>



/********************************************************
*        Déclaration des constantes du programme        *
*********************************************************/



/*****************************
* Constantes liés au serpent * 
******************************/

/*!
*
* @def START_SNAKE_LENGTH
* @brief Taille du serpent au début du jeu
*
*/
#define START_SNAKE_LENGTH 10

/*!
*
* @def MAX_SNAKE_LENGTH
* @brief Taille max du serpent qu'il peut atteindre
*
*/
#define MAX_SNAKE_LENGTH 20

/*!
*
* @def START_X_POSITION
* @brief Représente la coordonnée en X de départ où la tête du serpent s'affiche en début de programme
*
*/
#define START_X_POSITION 40

/*!
*
* @def START_Y_POSITION
* @brief Représente la coordonnée en Y de départ où la tête du serpent s'affiche en début de programme
*
*/
#define START_Y_POSITION 20

/*!
*
* @def SNAKE_HEAD
* @brief Le caractère utilisé pour représenter la tête du serpent
*
*/
#define SNAKE_HEAD 'O'

/*!
*
* @def SNAKE_BODY
* @brief Le caractère utilisé pour représenter le corps du serpent
*
*/
#define SNAKE_BODY 'X'


/******************************************
* Constantes liés à la map/plateau du jeu *
*******************************************/

/* Constantes du plateau */

/*!
*
* @def MAP_LIMIT_MIN
* @brief Représente la valeur de borne minimal pour l'affichage de la zone de jeu
*
*/
#define MAP_LIMIT_MIN 1

/*!
*
* @def MAP_LIMIT_X_MAX
* @brief Représente la valeur de borne maximal en X pour l'affichage de la zone de jeu
*
*/
#define MAP_LIMIT_X_MAX 81

/*!
*
* @def MAP_LIMIT_Y_MAX
* @brief Représente la valeur de borne maximal en Y pour l'affichage de la zone de jeu
*
*/
#define MAP_LIMIT_Y_MAX 41

/*!
*
* @def WALL_CHAR
* @brief Le caractère utilisé pour représenter les bords de la map/plateau et les pavés à l'intérieur
*
*/
#define WALL_CHAR '#'

/*!
*
* @def EMPTY_CHAR
* @brief Le caractère utilisé pour représenter une case vide dans la map/plateau
*
*/
#define EMPTY_CHAR ' '

/* Constantes des pavés */

/*!
*
* @def SPACE_BORDER
* @brief Représente l'espace au maximum qu'il doit y avoir entre la bordure du plateau et un pavé
*
*/
#define SPACE_BORDER 2

/*!
*
* @def NB_BLOCK
* @brief Représente le nombre de pavés présent sur le plateau du jeu
*
*/
#define NB_BLOCK 4

/*!
*
* @def BLOCK_SIZE
* @brief Représente la taille de chaque côté d'un pavé
*
*/
#define BLOCK_SIZE 5

/*!
*
* @def MIN_POS_BLOCK
* @brief Représente la valeur minimal autorisé pour les coordonnées des pavés dans le plateau
*
*/
#define MIN_POS_BLOCK 4

/*!
*
* @def MIN_POS_X_BLOCK_FOR_SNAKE
* @brief Représente la valeur minimal de la coordonnée X interdite en fonction de la coordonnée Y lors de la génération des pavés
*
* Permet d'éviter qu'un pavé n'apparaîsse sûr ou directement à la droite du serpent pouvant causer une gêne lors de l'exécution par l'utilisateur
* 
*/
#define MIN_POS_X_BLOCK_FOR_SNAKE 36

/*!
*
* @def MAX_POS_X_BLOCK_FOR_SNAKE
* @brief Représente la valeur maximal de la coordonnée X interdite en fonction de la coordonnée Y lors de la génération des pavés
*
* Permet d'éviter qu'un pavé n'apparaîsse sûr ou directement à la droite du serpent pouvant causer une gêne lors de l'exécution par l'utilisateur
* 
*/
#define MAX_POS_X_BLOCK_FOR_SNAKE 43

/*!
*
* @def MIN_POS_Y_BLOCK_FOR_SNAKE
* @brief Représente la valeur minimal de la coordonnée Y interdite en fonction de la coordonnée X lors de la génération des pavés
*
* Permet d'éviter qu'un pavé n'apparaîsse sûr ou directement à la droite du serpent pouvant causer une gêne lors de l'exécution par l'utilisateur
* 
*/
#define MIN_POS_Y_BLOCK_FOR_SNAKE 16

/*!
*
* @def MAX_POS_Y_BLOCK_FOR_SNAKE
* @brief Représente la valeur minimal de la coordonnée Y interdite en fonction de la coordonnée X lors de la génération des pavés
*
* Permet d'éviter qu'un pavé n'apparaîsse sûr ou directement à la droite du serpent pouvant causer une gêne lors de l'exécution par l'utilisateur
* 
*/
#define MAX_POS_Y_BLOCK_FOR_SNAKE 20

/* Constantes des pommes */

/*!
*
* @def MIN_POS_APPLE
* @brief Représente la position minimal autorisé pour l'apparition de la pomme
*
*/
#define MIN_POS_APPLE 1

/*!
*
* @def APPLE_CHAR
* @brief Le caractère utilisé pour représenter la pomme au moment de l'affichage
*
*/
#define APPLE_CHAR '6'


/********************************************
* Constantes liés aux saisies des contrôles *
*********************************************/

/*!
*
* @def STOP_CHAR
* @brief Le caractère pour l'arrêt du programme
*
*/
#define STOP_CHAR 'a'

/*!
*
* @def LEFT
* @brief Le caractère pour diriger le serpent vers la gauche
*
*/
#define LEFT 'q'

/*!
*
* @def RIGHT
* @brief Le caractère pour diriger le serpent vers la droite
*
*/
#define RIGHT 'd'

/*!
*
* @def UP
* @brief Le caractère pour diriger le serpent vers le haut
*
*/
#define UP 'z'

/*!
*
* @def DOWN
* @brief Le caractère pour diriger le serpent vers le bas
*
*/
#define DOWN 's'


/*******************************
* Constantes liés au programme *
********************************/

/*!
*
* @def BASE_SPEED
* @brief Valeur du timer mettant en pause le déroulement du programme
*
*/
#define BASE_SPEED 120000

/*!
*
* @def SPEED_TO_ADD
* @brief Valeur à retirer du timer pour accélérer le serpent
*
*/
#define SPEED_TO_ADD 10000

/*!
*
* @def NB_APPLE_TO_WIN
* @brief Nombre de pomme à manger pour terminer la partie
*
*/
#define NB_APPLE_TO_WIN 10



/********************************************************
*       Déclaration des Prototypes des procédures       *
*********************************************************/



/*************************
* Procédures principales *
**************************/

//Procédures de gestion d'affichage
void displayChar(int x, int y, char c);
void eraseChar(int x, int y);

//Procédure de la map/du plateau
void initMap();
void drawMap();
void addApple();

//Procédure du serpent
void drawSnake(int x[], int y[]);
void progress(int x[], int y[], char direction, bool * adrIsColliding, bool * adrHasEatApple);
void updateSnake(int x[], int y[], bool * adrHasEatApple, int * adrNbAppleEated, int * adrCurrentSpeed);

//Procédures liés à l'Input
char getInput();
void defDirection(char * currentDirection, char currentInput);
void exitSnake(bool * adrIsWorking, char currentInput, bool isColliding, int nbAppleEated);


/**********************
* Procédures externes *
***********************/

void gotoXY(int x, int y);
int kbhit();
void disableEcho();
void enableEcho(); 



/***********************************
*        Variables globales        *
************************************/



char gameMap[MAP_LIMIT_Y_MAX][MAP_LIMIT_X_MAX]; //Tableau à double entrée correspondant à la zone du jeu, y = pour les lignes et x = pour les colonnes

int currentAppleX; //Coordonnée X de l'actuelle pomme
int currentAppleY; //Coordonnée Y de l'actuelle pomme

int snakeX[MAX_SNAKE_LENGTH];
int snakeY[MAX_SNAKE_LENGTH]; 
int lastSnakeElemX; //Ancienne coordonnée X du dernier élément du serpent
int lastSnakeElemY; //Ancienne coordonnée Y du dernier élément du serpent

int currentSnakeLength = START_SNAKE_LENGTH;



/************************************
*        PROGRAMME PRINCIPAL        *
*************************************/



/*!                !!! A modifier !!!
*
* @fn int main()
* @brief Programme principal du jeu Snake
*
* @return Retourne 0 en cas de bon fonctionnement, -1 si il y a eu un problème
*
* Le programme dessine le plateau et le serpent à l'écran dans le terminal et le fait se déplacer vers la droite
* L'utilisateur peut ensuite dirigé le serpent vers le haut, le bas, la gauche et la droite
* Le serpent ne peut pas faire de demi-tour direct, et si il rentre en collision avec son corps, un pavé ou la bordure du plateau formé de '#'
* la partie se termine et le programme s'arrête
* En cas d'appuie sur la touche A, met fin à l'exécution du programme
*
*/
int main(){
    system("clear");
    disableEcho();

    bool isGameWorking = true;
    bool isSnakeColliding = false;
    bool hasSnakeEatApple = false;

    int currentSnakeSpeed = BASE_SPEED;

    int nbAppleEatedByPlayer = 0;

    char currentInput;
    char direction = 'd';

    //INITIALISATION
    srand(time(NULL));

    initMap();

    snakeX[0] = START_X_POSITION;
    snakeY[0] = START_Y_POSITION;
    
    /*Génération des éléments du corps du snake*/
    for (int i = 1; i < currentSnakeLength; i++){ 

        //Génère les éléments du corps du serpent en définissantles coordonnées selon la position de la tête, en les plaçant à sa droite
        snakeX[i] = snakeX[0] - i;         
        snakeY[i] = snakeY[0];
    }

    //TRAITEMENT & AFFICHAGE

    drawMap(); //Dessine le plateau avec la bordure et les pavés
    addApple();
    drawSnake(snakeX, snakeY); //Dessine le serpent une première fois aux coordonnées de départ

    /* Boucle du jeu */
    while (isGameWorking == true){
        usleep(currentSnakeSpeed);

        currentInput = getInput(); //récupère l'input de ce tour de boucle et fais ensuite les check sur cet input pour la direction et l'arrêt
        defDirection(&direction, currentInput); 

        progress(snakeX, snakeY, direction, &isSnakeColliding, &hasSnakeEatApple); //Déplace les coordonnées dans la direction rentré en paramètre
        drawSnake(snakeX, snakeY); //Affiche le serpent aux coordonnées rentrés en paramètres

        exitSnake(&isGameWorking, currentInput, isSnakeColliding, nbAppleEatedByPlayer);
        updateSnake(snakeX, snakeY, &hasSnakeEatApple, &nbAppleEatedByPlayer, &currentSnakeSpeed); //Met à jour les infos liés au serpent : sa vitesse, son nombre de pomme mangé et sa taille
    }

    enableEcho();
    return EXIT_SUCCESS;
}



/*******************************************************
*        Définitions des Procédures & Fonctions        *    
********************************************************/



/*!
*
* @fn void displayChar(int x, int y, char c)
* @brief Affiche un caractère dans le terminal à une position indiqué
*
* @param x : coordonnée x de la position à laquelle on souhaite afficher le caractère
* @param y : coordonnée y de la position à laquelle on souhaite afficher le caractère
* @param c : le caractère qu'on souhaite afficher
*
* Affiche le caractère c à la position (x, y) dans le terminal
*
*/
void displayChar(int x, int y, char c){

    gotoXY(x, y);
    printf("%c", c);
}


/*!
*
* @fn void eraseChar(int x, int y)
* @brief Efface un caractère dans le terminal à la position indiqué
*
* @param x : coordonnée x de la position à laquelle on souhaite effacer un caractère
* @param y : coordonnée y de la position à laquelle on souhaite effacer un caractère
*
* Efface un potentiel caractère à la position (x, y) en affichant un espace ' '
*
*/
void eraseChar(int x, int y){
    displayChar(x, y, EMPTY_CHAR);
}


/*!
*
* @fn void initMap()
* @brief Initialise le plateau du jeu en remplissant le tableau à double entrée le représentant 
*
* Crée d'abord la bordure haute du plateau, 
* puis crée chaque ligne du plateau en remplissant la bordure gauche et droite avec des espaces à l'intérieur du plateau,
* Enfin crée la bordure basse du plateau,
* 
* Crée ensuite les coordonnées des pavés selon certaines conditions puis les placent à l'intérieur du tableau
*
*/
void initMap(){

    /* Initialisation du cadre */
    for (int x = MAP_LIMIT_MIN; x < MAP_LIMIT_X_MAX; x++){  //Initialise la bordure haute
        gameMap[MAP_LIMIT_MIN][x] = WALL_CHAR;
    }

    for (int y = MAP_LIMIT_MIN + 1; y < MAP_LIMIT_Y_MAX - 1; y++){  //Initialise chaque ligne intérieur du tableau contenant un caractère # en début et fin de ligne 
                                                                    //et avec un nombre précis d'espace à l'intérieur
        gameMap[y][MAP_LIMIT_MIN] = WALL_CHAR;

        for (int x = MAP_LIMIT_MIN + 1; x < MAP_LIMIT_X_MAX - 1; x++){
            gameMap[y][x] = EMPTY_CHAR;
        }
        
        gameMap[y][MAP_LIMIT_X_MAX - 1] = WALL_CHAR;
    }

    for (int x = MAP_LIMIT_MIN; x < MAP_LIMIT_X_MAX; x++){ //Initialise la bordure basse
        gameMap[MAP_LIMIT_Y_MAX - 1][x] = WALL_CHAR;
    }

    /* Initialisation des portails */
    gameMap[MAP_LIMIT_MIN][MAP_LIMIT_X_MAX / 2] = EMPTY_CHAR; //Initialisation du portail du haut
    gameMap[MAP_LIMIT_Y_MAX - 1][MAP_LIMIT_X_MAX / 2] = EMPTY_CHAR; //Initialisation du portail du bas

    gameMap[MAP_LIMIT_Y_MAX / 2][MAP_LIMIT_MIN] = EMPTY_CHAR; //Initialisation du portail de gauche
    gameMap[MAP_LIMIT_Y_MAX / 2][MAP_LIMIT_X_MAX - 1] = EMPTY_CHAR;

    /* Initialisation des pavés */

    int currentBlockX;
    int currentBlockY;

    for (int i = 0; i < NB_BLOCK; i++){

        do{

            currentBlockX = (rand() % ((MAP_LIMIT_X_MAX - 1) - BLOCK_SIZE - SPACE_BORDER - (MIN_POS_BLOCK - 1))) + MIN_POS_BLOCK;
            currentBlockY = (rand() % ((MAP_LIMIT_Y_MAX - 1) - BLOCK_SIZE - SPACE_BORDER - (MIN_POS_BLOCK - 1))) + MIN_POS_BLOCK; 

        }while ((currentBlockX >= MIN_POS_X_BLOCK_FOR_SNAKE && currentBlockX <= MAX_POS_X_BLOCK_FOR_SNAKE) && (currentBlockY >= MIN_POS_Y_BLOCK_FOR_SNAKE && currentBlockY <= MAX_POS_Y_BLOCK_FOR_SNAKE));

        for (int i = 0; i < BLOCK_SIZE; i++){

            for (int j = 0; j < BLOCK_SIZE; j++){
                gameMap[currentBlockY + i][currentBlockX + j] = WALL_CHAR;
            }
        }
    }
}


/*!
*
* @fn void drawMap()
* @brief Affiche tout les éléments du tableau à double entrée correspondant au plateau du jeu
*
* Parcourt ligne par ligne et colonne par colonne pour afficher le caractère à l'indice des coordonnées
*
*/
void drawMap(){

    for (int y = MAP_LIMIT_MIN; y < MAP_LIMIT_Y_MAX; y++){

        for (int x = MAP_LIMIT_MIN; x < MAP_LIMIT_X_MAX; x++){
            printf("%c", gameMap[y][x]);
        }

        printf("\n");
    }
}

/*!
*
* @fn void addApple()
* @brief Génère les coordonnées X et Y de la pomme actuelle puis l'affiche dans le terminal
*
* La procédure génère d'abord une fois les 2 coordonnées, puis elle vérifie que les coordonnées ne correspondent pas à un élément de la bordure, de pavés ou du serpent
* Si elles correspondent, la procédure régénère de nouvelles valeurs jusqu'à ce qu'elles soient bonnes
* Une fois qu'elles sont bonnes, la procédure affiche la pomme dans le terminal 
*
*/
void addApple(){
    
    bool isGood = true;

    do{
        

        currentAppleX = (rand() % ((MAP_LIMIT_X_MAX) - MIN_POS_APPLE)) + MIN_POS_APPLE;
        currentAppleY = (rand() % ((MAP_LIMIT_Y_MAX) - MIN_POS_APPLE)) + MIN_POS_APPLE;

        for (int i = 0; i < currentSnakeLength; i++){
            if ((currentAppleX == snakeX[i]) && (currentAppleY == snakeY[i])){
                isGood = false;
            }
        }
    }while(gameMap[currentAppleY][currentAppleX] == WALL_CHAR || isGood != true);

    
    displayChar(currentAppleX, currentAppleY, APPLE_CHAR);
}


/*!
*
* @fn void drawSnake(int x[], int y[])
* @brief Affiche le serpent dans le terminal
*
* @param snakeX : tableau contenant les valeurs des positions X de chaque élément du serpent
* @param snakeY : tableau contenant les valeurs des positions Y de chaque élément du serpent
*
* Affiche d'abord chacun à son tour chaque élément du serpent à leurs positions respectives
* A condition qu'il n'y a pas un pavé affiché au même coordonée que l'élément du corps du snake
* Puis affiche la tête du serpent à ses coordonnées 
*
*/
void drawSnake(int snakeX[], int snakeY[]){

    for (int i = 1; i < currentSnakeLength; i++){ /*! boucle parcourant les éléments du serpent */
        
        if (gameMap[snakeY[i]][snakeX[i]] != WALL_CHAR){ 
            displayChar(snakeX[i], snakeY[i], SNAKE_BODY);
        }
    }

    displayChar(snakeX[0], snakeY[0], SNAKE_HEAD);
}


/*!
*                                                
* @fn void progress(int snakeX[], int snakeY[], char direction, int * adrIsColliding)
* @brief Fais avancer le serpent dans la direction indiqué en paramètre et vérifie les collisions du serpent
*
* @param snakeX : tableau contenant les valeurs des positions X de chaque élément du serpent
* @param snakeY : tableau contenant les valeurs des positions Y de chaque élément du serpent
* @param direction : caractère correspondant à la direction dans laquelle diriger le serpent
* @param adrIsColliding : entier "booléen" correspondant au statut de collision du serpent
*
* 1- Création de 2 tableaux pour contenir les nouvelles coordonnées
* 2- D'abord on efface le caractère à la position du dernier élément si il n'est pas sur la même position qu'un caractère de pavé
* 3- Puis selon la direction rentré en paramètre (soit gauche, droite, haut ou bas)
* 4- On va d'abord ajouter la valeur de la nouvelle position de la tête aux tableaux des nouvelles coordonnées
* 5- Ensuite on boucle sur le corps du serpent pour ajouter aux tableaux les nouvelles positions de chaque élément en se basant sur l'actuelle position
* de l'élément précédent
* 6- Enfin, on applique les déplacements des coordonnées en les appliquant aux tableaux des coordonnées des éléments du serpent
* 7- On vérifie les collisions de la tête du serpent avec un élément de pavé ou de la bordure
* 8- On vérifié les collisions de la tête du serpent avec un élément de son corps
* 9- On vérifie les collisions de la tête du serpent avec la pomme en comparant leurs coordonnées
*
*/
void progress(int snakeX[], int snakeY[], char direction, bool * adrIsColliding, bool * adrHasEatApple){

    /* Gestion du déplacement du serpent */

    //1.
    int newCoordX[currentSnakeLength];
    int newCoordY[currentSnakeLength];

    if ( gameMap[snakeY[currentSnakeLength - 1]][snakeX[currentSnakeLength - 1]] != WALL_CHAR ){

        eraseChar(snakeX[currentSnakeLength - 1], snakeY[currentSnakeLength - 1]); //2.
    }

    //3.
    if (direction == RIGHT){

        //4.
        newCoordX[0] = (snakeX[0] + 1 <= MAP_LIMIT_X_MAX - 1 ? snakeX[0] + 1 : MAP_LIMIT_MIN);
        newCoordY[0] = snakeY[0];

        for (int i = 1; i < currentSnakeLength; i++){

            //5.
            newCoordX[i] = snakeX[i - 1];
            newCoordY[i] = snakeY[i - 1];
        }
    }

    //3.
    else if (direction == LEFT){

        newCoordX[0] = (snakeX[0] - 1 >= MAP_LIMIT_MIN ? snakeX[0] - 1 : MAP_LIMIT_X_MAX - 1);
        newCoordY[0] = snakeY[0];

        for (int i = 1; i < currentSnakeLength; i++){

            //5.
            newCoordX[i] = snakeX[i - 1];
            newCoordY[i] = snakeY[i - 1];
        }
    }

    //3.
    else if (direction == UP){

        //4.
        newCoordX[0] = snakeX[0];
        newCoordY[0] = (snakeY[0] - 1 >= MAP_LIMIT_MIN ? snakeY[0] - 1 : MAP_LIMIT_Y_MAX - 1);

        for (int i = 1; i < currentSnakeLength; i++){

            //5.
            newCoordX[i] = snakeX[i - 1];
            newCoordY[i] = snakeY[i - 1];
        }
    }

    //3.
    else if (direction == DOWN){

        //4.
        newCoordX[0] = snakeX[0];
        newCoordY[0] = (snakeY[0] + 1 <= MAP_LIMIT_Y_MAX - 1 ? snakeY[0] + 1 : MAP_LIMIT_MIN);

        for (int i = 1; i < currentSnakeLength; i++){

            //5.
            newCoordX[i] = snakeX[i - 1];
            newCoordY[i] = snakeY[i - 1];
        }
    }

    /* Enregistrement de l'ancienne position du dernier élément du snake utilisé pour initialiser le nouveau segment à celles-ci */
    lastSnakeElemX = snakeX[currentSnakeLength - 1];
    lastSnakeElemY = snakeY[currentSnakeLength - 1];

    //6.
    for (int i = 0; i < currentSnakeLength; i++){
        snakeX[i] = newCoordX[i];
        snakeY[i] = newCoordY[i];
    }

    /* Gestion des collisions du serpent */
    

        /* Gestion des collisions avec le décor et lui-même */

    //7.
    if (gameMap[snakeY[0]][snakeX[0]] == WALL_CHAR){
        *adrIsColliding = true;
    }
    
    //8.
    for (int i = 1; i < currentSnakeLength; i++){

        if ((snakeX[0] == snakeX[i]) && (snakeY[0] == snakeY[i])){
            *adrIsColliding = true;
        }
    }

        /* Gestion des collisions avec la pomme */
    
    //9.
    if ((snakeX[0] == currentAppleX) && (snakeY[0] == currentAppleY)){
        *adrHasEatApple = true;
    }
   
    
}

/*!
*
* @fn updateSnake(int x[], int y[], bool * adrHasEatApple, int * adrNbAppleEated, int * adrCurrentSpeed)
* @brief Met à jour les informations du serpent lorsqu'il a mangé une pomme
*
* @param x : tableau contenant les valeurs des positions X de chaque élément du serpent
* @param y : tableau contenant les valeurs des positions Y de chaque élément du serpent
* @param adrHasEatApple : booléen correspondant à l'état du serpent si il a mangé une pomme
* @param adrNbAppleEated : entier correspondant au nombre de pomme mangés par le serpent
* @param adrCurrentSpeed : entier correspondant à la vitesse actuelle du serpent
*
* Si le serpent a mangé une pomme : la procédure incrémente le compteur de pomme mangé, repasse le booléen lié à l'état de manger une pomme à false
* puis ajoute un nouveau segment au serpent et augmente sa taille
* ensuite augmente sa vitesse puis enfin fait réapparaître une nouvelle pomme
*
*/
void updateSnake(int x[], int y[], bool * adrHasEatApple, int * adrNbAppleEated, int * adrCurrentSpeed){

    if (*adrHasEatApple == true){
        (*adrNbAppleEated) += 1;
        *adrHasEatApple = false;

        x[currentSnakeLength] = lastSnakeElemX;
        y[currentSnakeLength] = lastSnakeElemY;
        currentSnakeLength++;
        

        (*adrCurrentSpeed) -= SPEED_TO_ADD;

        if (*adrNbAppleEated < NB_APPLE_TO_WIN){
            addApple();
        }
    }
}


/*!
*
* @fn char getInput()
* @brief Check si une touche a été appuyé et récupère le caractère correspondant à l'appuie et le renvoie
*
* @return input - Renvoie le caractère lié à l'input d'une touche
*
*/
char getInput(){

    char input;

    if (kbhit()){
        input = getchar();
    }

    return input;
}


/*!
*
* @fn void defDirection(char * currentDirection, char currentInput)
* @brief Modifie la direction actuelle en la direction indiqué par le caractère lié à l'input rentré en paramètre
*
* La procédure vérifie si le caractère de l'input correspond au caractère de la constante lié
* Et vérifie aussi que la direction actuelle n'est pas l'opposé de la direction lié à l'input
* exemple : si la direction actuelle est la gauche, et que la direction indiqué par l'input est la droite : condition invalidé
*
* Si les conditions sont remplis, la direction actuelle est remplacé par la nouvelle
*
*/
void defDirection(char * currentDirection, char currentInput){

    if (currentInput == RIGHT && *currentDirection != LEFT){
        *currentDirection = RIGHT;
    }

    else if (currentInput == LEFT && *currentDirection != RIGHT){
        *currentDirection = LEFT;
    }

    else if (currentInput == UP && *currentDirection != DOWN){
        *currentDirection = UP;
    }

    else if (currentInput == DOWN && *currentDirection != UP){
        *currentDirection = DOWN;
    }
}


/*!
*
* @fn void exitSnake(int * adrIsWorking, char currentInput, int adrIsColliding)
* @brief Vérifie l'appuie d'une touche ou la collision avec un élément et modifie une variable de boucle
*
* @param adrIsWorking : variable entière servant de booléen qui agit comme condition dans une boucle
* @param currentInput : variable correspondant à l'input du tour de boucle actuelle lors de l'appel de la procédure
* @param isColliding : entier "booléen" correspondant au statut de collision du serpent
* @param nbAppleEated : entier correspondant au nombre de pommes mangés par le serpent
*
* Une Procédure qui regarde si l'input actuelle est touche A OU si il y a une collision avec un élément 
* OU si le nombre de pomme correspond au nombre de pomme que le serpent doit manger
* Si oui adrIsWorking sera modifié à 0 
*
*/
void exitSnake(bool * adrIsWorking, char currentInput, bool isColliding, int nbAppleEated){

    if (currentInput == STOP_CHAR || isColliding == true || nbAppleEated == NB_APPLE_TO_WIN){
        *adrIsWorking = false;
    }
}


/*!
*
* @fn void gotoXY(int x, int y)
* @brief Positionne le curseur du terminal à une position donné
*
* @param x : position X du curseur
* @param y : position Y du curseur
*
* Positionne le curseur de saisie du terminal à la position (x, y)
*
*/
void gotoXY(int x, int y) { 
    printf("\033[%d;%df", y, x);
}


/*!
*
* @fn int kbhit()
* @brief Fonction qui check la saisie d'un caractère au clavier sans bloquer l'exécution du programme
*
* @return 1 si un caractère a été saisie, 0 dans le cas contraire
*
* Utilise le mon non bloquant du terminal pour pouvoir réaliser des inputs et pouvoir les vérifier par la suite sans interrompre l'exécution du programme
*/
int kbhit(){
	// la fonction retourne :
	// 1 si un caractere est present
	// 0 si pas de caractere present
	
	int unCaractere=0;
	struct termios oldt, newt;
	int ch;
	int oldf;

	// mettre le terminal en mode non bloquant
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
	ch = getchar();

	// restaurer le mode du terminal
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
 
	if(ch != EOF){
		ungetc(ch, stdin);
		unCaractere=1;
	} 
	return unCaractere;
}


void disableEcho() {
    struct termios tty;

    // Obtenir les attributs du terminal
    if (tcgetattr(STDIN_FILENO, &tty) == -1) {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }

    // Desactiver le flag ECHO
    tty.c_lflag &= ~ECHO;

    // Appliquer les nouvelles configurations
    if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
}


void enableEcho() {
    struct termios tty;

    // Obtenir les attributs du terminal
    if (tcgetattr(STDIN_FILENO, &tty) == -1) {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }

    // Reactiver le flag ECHO
    tty.c_lflag |= ECHO;

    // Appliquer les nouvelles configurations
    if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
}
