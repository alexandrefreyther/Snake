#include "stdlib.h"
#include "config.h"

void createFood(int* bg, int food[2], const int& nx, const int& ny) 
{   
    if (food[0]==0) // si la nourriture vient d'être mangée
    {
        while(bg[food[0]+nx*food[1]] != 0) // LA NOURRITURE NE DOIT PAS SE CRÉER SUR LE SNAKE (d'où la modification)
        {
            food[0] = rand()%(nx-2) + 1; // ici entre 1 et nx-2 (il y avait +2 à l'origine)
            food[1] = rand()%(ny-2) + 1; // (il y avait +2 à l'origine)
        
        }
    bg[food[1]*nx+food[0]] = 2;
    }
}

bool eatFood(int food[], int snake[], int snl){
    if( (food[0]== snake[0]) && (food[1] == snake[snl]) ){
        food[0] = 0;
        food[1] = 0;
        return true; // alors on appelera createFood
    }
    return false;
}