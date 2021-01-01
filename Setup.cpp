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