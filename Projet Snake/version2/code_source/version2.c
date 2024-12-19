/*!
*
* @file version2.c
* @author Parveau le Saint Korentin
* @brief Programme de jeu Snake
* @version 2.0
* @date 2024-11-10
*
* Ce programme est une deuxième version d'un jeu snake dans le terminal Linux. Cette deuxième version consiste au
* déplacement dans 4 direction du serpent. Le programme démarre sur le serpent se dirigeant vers la droite.
* L'utilisateur peut ensuite le diriger vers le haut, le bas, la gauche ou la droite avec les touches 
* zqsd et peut mettre fin au programme en appuyant sur la touche a
*
*/

/*Déclaration des bibliothèques*/
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>



/********************************************************
*        Déclaration des constantes du programme        *
*********************************************************/



/*!
*
* @def TAILLE
* @brief Taille du serpent
*
*/
#define TAILLE 10

/*!
*
* @def BORNE_MIN
* @brief Représente la valeur de borne minimal pour l'affichage de la zone de jeu
*
*/
#define BORNE_MIN 0

/*!
*
* @def BORNE_MAX
* @brief Représente la valeur de borne maximal pour l'affichage de la zone de jeu
*
*/
#define BORNE_MAX 40

/*!
*
* @def START_POSITION
* @brief Représente la coordonnée de départ où la tête du snake s'affiche en début de programme
*
*/
#define START_POSITION 20

/*!
*
* @def SLEEP_TIME
* @brief Valeur du timer mettant en pause le déroulement du programme
*
*/
#define SLEEP_TIME 125000

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

/*!
*
* @def CHAR_ARRET
* @brief Le caractère pour l'arrêt du programme
*
*/
#define CHAR_ARRET 'a'

/*!
*
* @def GAUCHE
* @brief Le caractère pour diriger le serpent vers la gauche
*
*/
#define GAUCHE 'q'

/*!
*
* @def DROITE
* @brief Le caractère pour diriger le serpent vers la droite
*
*/
#define DROITE 'd'

/*!
*
* @def HAUT
* @brief Le caractère pour diriger le serpent vers le haut
*
*/
#define HAUT 'z'

/*!
*
* @def BAS
* @brief Le caractère pour diriger le serpent vers le bas
*
*/
#define BAS 's'



/********************************************************
*       Déclaration des Prototypes des procédures       *
*********************************************************/



/*************************
* Procédures principales *
**************************/

//Procédures de gestion d'affichage
void afficherChar(int x, int y, char c);
void effacerChar(int x, int y);

//Procédure du serpent
void dessinerSerpent(int x[], int y[]);
void progresser(int x[], int y[], char direction);

//Procédures liés à l'Input
char getInput();
void defDirection(char * currentDirection, char currentInput);
void exitSnake(int * adrIsWorking, char currentInput);


/**********************
* Procédures externes *
***********************/

void gotoXY(int x, int y);
int kbhit();
void disableEcho();
void enableEcho(); 



/************************************
*        PROGRAMME PRINCIPAL        *
*************************************/



/*!
*
* @fn int main()
* @brief Programme principal du jeu Snake
*
* @return Retourne 0 en cas de bon fonctionnement, -1 si il y a eu un problème
*
* Le programme dessine le serpent à l'écran dans le terminal et le fait se déplacer vers la droite
* L'utilisateur peut ensuite dirigé le serpent vers le haut, le bas, la gauche et la droite
* Le serpent ne peut pas faire un demi-tour direct, et disparaît lorsqu'il quitte les limites définis en constantes
* En cas d'appuie sur la touche A, met fin à l'exécution du programme
*
*/
int main(){
    system("clear");
    disableEcho();

    int isWorking = 1;
    char currentInput;
    char direction = 'd';

    //INITIALISATION
    int lesX[TAILLE];
    int lesY[TAILLE]; 
    lesX[0] = START_POSITION;
    lesY[0] = START_POSITION;
    
    //TRAITEMENT & AFFICHAGE
    
    /*Génération des éléments du corps du snake*/
    for (int i = 1; i < TAILLE; i++){ 

        //Génère les éléments du corps du serpent en définissantles coordonnées selon la position de la tête, en les plaçant à sa droite
        lesX[i] = lesX[0] - i;         
        lesY[i] = lesY[0];
    }

    dessinerSerpent(lesX, lesY); //Dessine le serpent une première fois aux coordonnées de départ

    /* Boucle du jeu */
    while (isWorking == 1){
        usleep(SLEEP_TIME);

        currentInput = getInput(); //récupère l'input de ce tour de boucle et fais ensuite les check sur cet input pour la direction et l'arrêt
        defDirection(&direction, currentInput); 
        exitSnake(&isWorking, currentInput);

        progresser(lesX, lesY, direction); //Déplace les coordonnées dans la direction rentré en paramètre
        dessinerSerpent(lesX, lesY); //Affiche le serpent aux coordonnées rentrés en paramètres
    }

    enableEcho();
    return EXIT_SUCCESS;
}



/*******************************************************
*        Définitions des Procédures & Fonctions        *    
********************************************************/



/*!
*
* @fn void afficherChar(int x, int y, char c)
* @brief Affiche un caractère dans le terminal à une position indiqué
*
* @param x : coordonnée x de la position à laquelle on souhaite afficher le caractère
* @param y : coordonnée y de la position à laquelle on souhaite afficher le caractère
* @param c : le caractère qu'on souhaite afficher
*
* Affiche le caractère c à la position (x, y) dans le terminal
*
*/
void afficherChar(int x, int y, char c){
    gotoXY(x, y);
    printf("%c", c);
}


/*!
*
* @fn void effacerChar(int x, int y)
* @brief Efface un caractère dans le terminal à la position indiqué
*
* @param x : coordonnée x de la position à laquelle on souhaite effacer un caractère
* @param y : coordonnée y de la position à laquelle on souhaite effacer un caractère
*
* Efface un potentiel caractère à la position (x, y) en affichant un espace ' '
*
*/
void effacerChar(int x, int y){
    afficherChar(x, y, ' ');
}


/*!
*
* @fn void dessinerSerpent(int x[], int y[])
* @brief Affiche le serpent dans le terminal
*
* @param lesX : tableau contenant les valeurs des positions X de chaque élément du serpent
* @param lesY : tableau contenant les valeurs des positions Y de chaque élément du serpent
*
* Affiche d'abord chacun à son tour chaque élément du serpent à leurs positions respectives
* A condition que ces coordonnées sont comprises entre les bornes définis
* Puis affiche la tête du serpent à ses coordonnées à condition qu'elles sont comprises entre les bornes
*
*/
void dessinerSerpent(int lesX[], int lesY[]){

    for (int i = 1; i < TAILLE; i++){ /*! boucle parcourant les éléments du serpent */
        
        /*! si l'emplacement de la coordonnée est comprise dans les bornes, alors on affiche à l'écran le corps du serpent */
        if (lesX[i] > BORNE_MIN && lesY[i] > BORNE_MIN && lesX[i] <= BORNE_MAX && lesY[i] <= BORNE_MAX){ 
            afficherChar(lesX[i], lesY[i], SNAKE_BODY);
        }
    }

    /*! si l'emplacement de la coordonnée est comprise dans les bornes, alors on affiche à l'écran sa tête */
    if (lesX[0] > BORNE_MIN && lesY[0] > BORNE_MIN && lesX[0] <= BORNE_MAX && lesY[0] <= BORNE_MAX){
        afficherChar(lesX[0], lesY[0], SNAKE_HEAD);
    }
}


/*!
*                                                     
* @fn void progresser(int lesX[], int lesY[], char direction)
* @brief Fais avancer le serpent dans la direction indiqué en paramètre
*
* @param lesX : tableau contenant les valeurs des positions X de chaque élément du serpent
* @param lesY : tableau contenant les valeurs des positions Y de chaque élément du serpent
* @param direction : caractère correspondant à la direction dans laquelle diriger le serpent
*
* 1- Création de 2 tableaux pour contenir les nouvelles coordonnées
* 2- D'abord on efface le caractère à la position du dernier élément si il est affiché (si sa position est comprise dans les bornes)
* 3- Puis selon la direction rentré en paramètre (soit gauche, droite, haut ou bas)
* 4- On va d'abord ajouter la valeur de la nouvelle position de la tête aux tableaux des nouvelles coordonnées
* 5- Ensuite on boucle sur le corps du serpent pour ajouter aux tableaux les nouvelles positions de chaque élément en se basant sur l'actuelle position
* de l'élément précédent
* 6- Enfin, on applique les déplacements des coordonnées en les appliquant aux tableaux des coordonnées des éléments du serpent
*
*/
void progresser(int lesX[], int lesY[], char direction){

    //1.
    int newCoordX[TAILLE];
    int newCoordY[TAILLE];

    if (lesX[TAILLE - 1] > BORNE_MIN && lesY[TAILLE - 1] > BORNE_MIN && lesX[TAILLE - 1] <= BORNE_MAX && lesY[TAILLE - 1] <= BORNE_MAX){
        effacerChar(lesX[TAILLE - 1], lesY[TAILLE - 1]); //2.
    }

    //3.
    if (direction == DROITE){

        //4.
        newCoordX[0] = lesX[0] + 1;
        newCoordY[0] = lesY[0];

        for (int i = 1; i < TAILLE; i++){

            //5.
            newCoordX[i] = lesX[i - 1];
            newCoordY[i] = lesY[i - 1];
        }
    }

    //3.
    else if (direction == GAUCHE){

        //4.
        newCoordX[0] = lesX[0] - 1;
        newCoordY[0] = lesY[0];

        for (int i = 1; i < TAILLE; i++){

            //5.
            newCoordX[i] = lesX[i - 1];
            newCoordY[i] = lesY[i - 1];
        }
    }

    //3.
    else if (direction == HAUT){

        //4.
        newCoordX[0] = lesX[0];
        newCoordY[0] = lesY[0] - 1;

        for (int i = 1; i < TAILLE; i++){

            //5.
            newCoordX[i] = lesX[i - 1];
            newCoordY[i] = lesY[i - 1];
        }
    }

    //3.
    else if (direction == BAS){

        //4.
        newCoordX[0] = lesX[0];
        newCoordY[0] = lesY[0] + 1;

        for (int i = 1; i < TAILLE; i++){

            //5.
            newCoordX[i] = lesX[i - 1];
            newCoordY[i] = lesY[i - 1];
        }
    }

    //6.
    for (int i = 0; i < TAILLE; i++){
        lesX[i] = newCoordX[i];
        lesY[i] = newCoordY[i];
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

    if (currentInput == DROITE && *currentDirection != GAUCHE){
        *currentDirection = DROITE;
    }

    else if (currentInput == GAUCHE && *currentDirection != DROITE){
        *currentDirection = GAUCHE;
    }

    else if (currentInput == HAUT && *currentDirection != BAS){
        *currentDirection = HAUT;
    }

    else if (currentInput == BAS && *currentDirection != HAUT){
        *currentDirection = BAS;
    }
}


/*!
*
* @fn void exitSnake(int * adrIsWorking)
* @brief Vérifie l'appuie d'une touche et laquelle et modifie une variable de boucle
*
* @param adrIsWorking : variable entière servant de booléen qui agit comme condition dans une boucle
*
* Une Procédure qui regarde si une touche est appuyé lors de l'exécution du programme et si celle-ci est la touche A
* Si oui adrIsWorking sera modifié à 0 
*
*/
void exitSnake(int * adrIsWorking, char currentInput){

    if (currentInput == CHAR_ARRET){
            *adrIsWorking = 0;
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
