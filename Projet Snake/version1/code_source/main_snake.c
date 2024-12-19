/*!
*
* @file main_snake.c
* @author Parveau le Saint Korentin
* @brief Programme de jeu Snake
* @version 1.0
* @date 2024-10-27
*
* Ce programme est une première version d'un jeu snake dans le terminal Linux. Cette première version consiste à demander à l'utilisateur
* la position où afficher le serpent. Ce dernier avance ensuite vers la droite jusqu'à ce que l'utilisateur appuie sur la touche 'a'
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
* @def SLEEP_TIME
* @brief Valeur du timer mettant en pause le déroulement du programme
*
*/
#define SLEEP_TIME 300000

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



/********************************************************
*       Déclaration des Prototypes des procédures       *
*********************************************************/



/*Procédures externes*/
void gotoXY(int x, int y);
int kbhit();

/*Procédures de Saisie*/
void verifSaisie();

/*Procédures principales*/
void afficherChar(int x, int y, char c);
void effacerChar(int x, int y);

void dessinerSerpent(int x[], int y[]);
void progresser(int x[], int y[]);

void exitSnake(int * adr_isWorking);



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
* Le programme commence avec l'affichage d'un message donnant des informations
* Il demande ensuite à l'utilisateur de saisir une valeur pour la position X et pour la position Y de la tête du serpent
* Il dessine ensuite le serpent à l'écran dans le terminal et le fait se déplacer vers la droite
* En cas d'appuie sur la touche A, met fin à l'exécution du programme
*
*/
int main(){
    //Intro
    system("clear");
    printf("Bienvenue ! Programme de jeu Snake version 1.0\nPour stopper le jeu, appuyez sur la touche A.\n\n");

    //Saisie
    int posX;
    printf("\nSaisissez la valeur pour la position X de départ (entre 1 et 40 inclus) : ");
    verifSaisie(&posX); 

    int posY;
    printf("\nSaisissez la valeur pour la position Y de départ (entre 1 et 40 inclus) : ");
    verifSaisie(&posY);

    //Initialisation
    int lesX[TAILLE];
    int lesY[TAILLE];
    lesX[0] = posX;
    lesY[0] = posY;

    system("clear");
    
    //Traitement & Affichage
    
    progresser(lesX, lesY);
    
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
    printf("\n");
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
* Affiche d'abord la tête du serpent à la position (lesX[0], lesY[0])
* Puis affiche chacun son tour chaque élément du serpent en se basant sur la position de la tête
* Enfin la procédure efface à la position derrière l'actuelle de chaque élément
*
*/
void dessinerSerpent(int lesX[], int lesY[]){
    afficherChar(lesX[0], lesY[0], SNAKE_HEAD); /*! Affiche la tête du serpent à sa position */

    for (int i = 1; i < TAILLE; i++){ /*! fais une boucle de TAILLE fois correspondant au nombre d'élément formant le serpent */
        if (lesX[0] - i > 0){ /*! si l'emplacement de la coordonnée est supérieur à 0, alors on affiche à l'écran le corps du serpent */
            afficherChar(lesX[0] - i, lesY[0], SNAKE_BODY);
            effacerChar(lesX[0] - i - 1, lesY[0]); /*! Efface enfin à la position derrière la position actuelle */
        }
    }
}


/*!
*
* @fn void progresser(int lesX[], int lesY[])
* @brief Fais avancer le serpent vers la droite jusqu'à l'appuie d'une touche
*
* @param lesX : tableau contenant les valeurs des positions X de chaque élément du serpent
* @param lesY : tableau contenant les valeurs des positions Y de chaque élément du serpent
*
* Dessine le serpent via les valeurs de lesX et lesY puis incrémente la position de la tête du serpent le faisant avancer de 1 vers la droite puis le redessine
* La procédure boucle sur la valeur d'une variable entière servant de booléen (si cette dernière = 1), en cas d'appuie sur la touche A la variable est modifié et la procédure sort donc de la boucle
*
*/
void progresser(int lesX[], int lesY[]){

    int isWorking = 1;

    while (isWorking == 1){
        dessinerSerpent(lesX, lesY);
        usleep(SLEEP_TIME);

        exitSnake(&isWorking);

        lesX[0] += 1;
    }
}


/*!
*
* @fn void verifSaisie(int * adr_pos)
* @brief Vérifie la saisie de la position X et Y
*
* @param adr_pos : Variable qui contiendra la valeur saisie lors de la procédure
*
* Procédure réalisant la saisie et vérifie que cette dernière s'agit bien d'un entier compris entre les bornes minimales et maximales (1 et 40 inclus) indiqués dans la procédure
*
*/
void verifSaisie(int * adr_pos){

    while (scanf("%d", adr_pos) != 1 || *adr_pos <= BORNE_MIN || *adr_pos > BORNE_MAX){

        printf("\nErreur de saisie, la valeur doit être entière et comprise entre 1 et 40 inclus. Veuillez ressaisir : ");
        while (getchar() != '\n');
    }
}


/*!
*
* @fn void exitSnake(int * adr_isWorking)
* @brief Vérifie l'appuie d'une touche et laquelle et modifie une variable de boucle
*
* @param adr_isWorking : variable entière servant de booléen qui agit comme condition dans une boucle
*
* Une Procédure qui regarde si une touche est appuyé lors de l'exécution du programme et si celle-ci est la touche A
* Si oui adr_isWorking sera modifié à 0 
*
*/
void exitSnake(int * adr_isWorking){
    if (kbhit() == 1 && getchar() == 'a'){
            *adr_isWorking = 0;
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