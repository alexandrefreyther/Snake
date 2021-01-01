#include <iostream>
#include "sys/ioctl.h"

#include "stdlib.h"

#include "config.h"
#include "internals.h"
#include "display.h"
#include "food.h"

#include <fstream> //lecture et ecriture de fichier (pour le meilleur score)
#include <string> // pour l'écriture avec ofstream


void backgroundSetup(const int nx, const int ny, int bg[]) // va modifier le background initial
{   
    // on met les bords sur 1
    for (int j=0; j< nx; j++) // bords horizontaux
    {
        bg[j] = 1; 
        bg[nx*(ny-1)+j] = 1;
    }
    for (int i=1; i< ny-1; i++) // bords verticaux
    {
        bg[nx*i] = 1; 
        bg[nx*(i+1)-1] = 1;        
    }
}

void setupSnake(int snake[], int snake_len) 
{
    for (int i=0; i<snake_len; i++)
    {
        snake[i] = snake_len-i ; // le snake commence au bord gauche
        snake[snake_len+i] = 12; // à modifier en fonction de ny (ou rajouter une dépendance en ny dans la fonction)
    }
}



void add_snake(int snake[], int bg[], int snl, const int nx, const int ny) // on ajoute le snake au bg 
// snl = snake_len
{
    for (int i=0; i<snl; i++)
    {
        bg[snake[i]+snake[snl+i]*nx] = 1;
    }
}


void remove_snake(int snake[], int bg[], int snl, const int nx, const int ny) // on supprime le snake au bg 
{
    for (int i=0; i<snl; i++)
    {
        bg[snake[i]+snake[snl+i]*nx] = 0; 
    }
    if ((snake[0]==0) || (snake[0] == nx-1)) // alors le serpent est mort en fonçant dans le mur
    {
        bg[snake[0]+snake[snl]*nx] = 1;
    }
}

void snake_movement(char key, int dxdy[]) // directions == > ZQWS
{
    if (key=='z') // en haut
    {
        if ((dxdy[0]==0) && (dxdy[1]==-1))
        {
            // rien ne se passe ==> c'est comme si on n'avait pas vu la consigne
        }
        else
        {
            dxdy[0] = 0;
            dxdy[1] = -1; // et non 1 ! l'axe des y est orienté vers le bas
        }
    }
    if (key=='w') // en bas
    {
        if ((dxdy[0]==0) && (dxdy[1]==1))
        {
            // rien ne se passe ==> c'est comme si on n'avait pas vu la consigne
        }
        else
        {
            dxdy[0] = 0;
            dxdy[1] = 1; // et non -1 ! l'axe des y est orienté vers le bas
        }
    }
        if (key=='q') // à gauche
    {
        if ((dxdy[0]==1) && (dxdy[1]==0))
        {
            // rien ne se passe ==> c'est comme si on n'avait pas vu la consigne
        }
        else
        {
            dxdy[0] = -1;
            dxdy[1] = 0;
        }
    }
        if (key=='s') // à droite
    {
        if ((dxdy[0]==-1) && (dxdy[1]==0))
        {
            // rien ne se passe ==> c'est comme si on n'avait pas vu la consigne
        }
        else
        {
            dxdy[0] = 1;
            dxdy[1] = 0;
        }
    }
}

bool verifyBorder(int bg[], int snake[], int snl, const int nx, int dxdy[]) // prend en compte le cas où le snake se mord
{   // avec bg
    if (bg[(snake[snl]+dxdy[1])*nx+snake[0]+dxdy[0]]==1) // alors le snake meurt
    {
        return false;
    }
    return true;
}


void update_snake_coordinates(int snake[], int snl, int dxdy[], bool eat) // si on appelle cette fonction, le snake est encore en vie
{   
    if (eat==0)
    {
        for (int i=1; i<=snl-1; i++) 
        {
            snake[snl-i]=snake[snl-i-1]; 
            snake[2*snl-i]=snake[2*snl-i-1];
        }
    }
    else // alors un fruit a été mangé (snl n'est pas encore actualisé)
    // juste le 1er élément doit être modifié, le reste décalé
    // PROBLEME avec le y ==> je ne comprends pas
    // PROBLEME À PARTIR DE 9
    {   
        snl++; // on remplace snl par snl+1
        snake[snl+1] = snake[snl-1]; // avant qu'il soit modifié par la boucle for
        for (int i=1; i<=snl-2; i++) 
        {   
            snake[snl-i]=snake[snl-i-1]; 
            snake[2*snl-i]=snake[2*snl-i-2]; // bien faire attention au -2 du fait de l'incursion du dernier x
        }
        snake[1] = snake[0];
    }
    snake[0]=snake[1]+dxdy[0]; 
    snake[snl]=snake[snl+1]+dxdy[1]; 
}


void maj_record(int snl)
{
    std::ifstream fichier_record_lecture("record.txt"); // un seul élément 
    int meilleur_score;
    fichier_record_lecture >> meilleur_score;
    std::cout << "record : " << meilleur_score << std::endl;

    std::ofstream fichier_record_ecriture("record.txt"); 

    if (snl-3 > meilleur_score)
    {   
        meilleur_score = snl-3;
        fichier_record_ecriture << meilleur_score << std::endl;
    }
}


// déjà implémentée
void startGame(const int& lap, const int& nx, const int& ny, int& snl, int* snake, int* bg){
    char key;
    int dxdy[2] = {1,0}; // direction x, direction y
    int food[2] = {0,0};

    createFood( bg, food, nx, ny );

    while( true ) // quand peut on sortir de la boucle ?
    { 
        internal::frameSleep(lap);
        if( internal::keyEvent() ){
            std::cin >> key; 
            snake_movement(key, dxdy);
        }
        backgroundClear();
        add_snake( snake, bg, snl, nx, ny );
        printFrame(nx, ny, bg);
        remove_snake(snake, bg, snl, nx, ny);
        bool out =  verifyBorder(bg, snake, snl, nx, dxdy); // initialement sans snl et bg (et nx, ny au lieu de dxdy)
        if( out == false)
        {
            /* option : le snake disparrait quand on perd
            remove_snake(snake, bg, snl, nx, ny); 
            backgroundClear();
            printFrame(nx, ny, bg);
            */
            std::cerr << "Perdu" << std::endl;
            exit(1);
        }
        bool eat = eatFood(food, snake, snl);
        update_snake_coordinates(snake, snl, dxdy, eat); // on le passe avant le if
        if(eat)
        {
            createFood(bg, food, nx, ny);
            snl++;
        }
        std::cout << "score : " << snl-3 << std::endl;
        // maj_record(snl); COMPRENDRE LE PROBLEME
    }
}


// déjà implémentée
int main()
{   
    const int nx = 50;
    const int ny = 25;
    const int lap=200;

    int snake_len = 3;
    int background[nx*ny] ;
    for( int i=0; i<nx*ny; i++){
        background[i] = 0; //(au lieu de -1 initialement)
    }
    int snake[2*snake_len];

    backgroundSetup(nx, ny, background); // background initial
    setupSnake(snake, snake_len); // snake initial


    startGame(lap, nx, ny, snake_len, snake, background); // et met à jour à chaque étape

}



