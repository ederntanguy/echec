/*
    GNU/Linux et MacOS
        > gcc main.c $(sdl2-config --cflags --libs) -o prog
        > gcc *.c $(sdl2-config --cflags --libs) -o prog
        > gcc src/(mettre etoile).c $(sdl2-config --cflags --libs) -o prog
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        // do nothing
    }
*/

/*
    SDL_RENDERER_SOFTWARE
    SDL_RENDERER_ACCELERATED
    SDL_RENDERER_PRESENTCSYNC
    SDL_RENDERER_TARGETTEXTURE

*/

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#define TAILLX 800.0
#define TAILLY 800.0

void SDL_ExitWithError(const char *message);
void positionPion(int colonne1[], int colonne2[], int colonne3[], int colonne4[], int colonne5[], int colonne6[], int colonne7[], int colonne8[], SDL_Window *window, SDL_Renderer *renderer, SDL_Surface *picture, SDL_Texture *texture);

int main(int argc, char *argv[])
{
    int color = 0;
    // de la structur SDL_Window
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    // tout cela permet d'initialiser et de voir si elle a fonctionner
    if (SDL_Init(SDL_INIT_VIDEO) != 0) // pour en mettre plusieurs faut separer par | ex : (SDL_INIT_VIDEO | SDL_INIT_AUDIO)
        SDL_ExitWithError("Initialisation SDL");

    // Execution du programme
    // cree la fenetre et rendu
    window = SDL_CreateWindow("echec", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, TAILLX, TAILLY, 0);
    if (window == NULL)
        SDL_ExitWithError("creation fenetre echouee");

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL)
        SDL_ExitWithError("creation rendu echouee");

    //-------------------------------------------------------------

    /*1 on gere l'ouvertur et fermetur du projet*/
    if (SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE) != 0)
        SDL_ExitWithError("Impossible de changer la couleur pour le rendu");

    // initialisation des tableau de l'echequier
    int colonne1[8] = {6, 7, 8, 9, 10, 8, 7, 6}, colonne2[8] = {1, 1, 1, 1, 1, 1, 1, 1}, colonne3[8] = {0, 0, 0, 0, 0, 0, 0, 0}, colonne4[8] = {0, 0, 0, 0, 0, 0, 0, 0}, colonne5[8] = {0, 0, 0, 0, 0, 0, 0, 0}, colonne6[8] = {0, 0, 0, 0, 0, 0, 0, 0}, colonne7[8] = {2, 2, 2, 2, 2, 2, 2, 2}, colonne8[8] = {12, 14, 16, 18, 20, 16, 14, 12};
    // initialisation des carre du jeux
    SDL_Rect rectangle;
    rectangle.x = 0;
    rectangle.y = 0;
    rectangle.w = TAILLX / 8;
    rectangle.h = TAILLY / 8;

    // positionnement des carres
    int changementCouleur = 0;
    for (size_t i = 0; i < 8; i++)
    {
        for (size_t i = 0; i < 8; i++)
        {
            if (SDL_RenderFillRect(renderer, &rectangle) != 0) // cordonner (x,y)
                SDL_ExitWithError("ça ne s'est pas dessiner");
            if (color == 0)
            {

                if (changementCouleur == 0)
                    SDL_SetRenderDrawColor(renderer, 22, 184, 78, SDL_ALPHA_OPAQUE);
                else if (changementCouleur == 1)
                    SDL_SetRenderDrawColor(renderer, 245, 245, 220, SDL_ALPHA_OPAQUE);

                color++;
            }
            else if (color == 1)
            {
                if (changementCouleur == 0)
                    SDL_SetRenderDrawColor(renderer, 245, 245, 220, SDL_ALPHA_OPAQUE);
                else if (changementCouleur == 1)
                    SDL_SetRenderDrawColor(renderer, 22, 184, 78, SDL_ALPHA_OPAQUE);

                color--;
            }
            if (changementCouleur == 0)
                rectangle.x += TAILLX / 8;

            else if (changementCouleur == 1)
                rectangle.x -= TAILLX / 8;
        }
        if (changementCouleur == 0)
            changementCouleur++;
        else if (changementCouleur == 1)
            changementCouleur--;
        rectangle.y += TAILLY / 8;
    }

    rectangle.x = 0;
    rectangle.y = 0;
    for (size_t i = 0; i < 8; i++)
    {
        if (SDL_RenderFillRect(renderer, &rectangle) != 0) // cordonner (x,y)
            SDL_ExitWithError("ça ne s'est pas dessiner");
        if (color == 0)
        {

            if (changementCouleur == 0)
                SDL_SetRenderDrawColor(renderer, 22, 184, 78, SDL_ALPHA_OPAQUE);
            else if (changementCouleur == 1)
                SDL_SetRenderDrawColor(renderer, 245, 245, 220, SDL_ALPHA_OPAQUE);

            color++;
        }
        else if (color == 1)
        {
            if (changementCouleur == 0)
                SDL_SetRenderDrawColor(renderer, 245, 245, 220, SDL_ALPHA_OPAQUE);
            else if (changementCouleur == 1)
                SDL_SetRenderDrawColor(renderer, 22, 184, 78, SDL_ALPHA_OPAQUE);

            color--;
        }
        rectangle.y += TAILLY / 8;
    }
    SDL_SetRenderDrawColor(renderer, 245, 245, 220, SDL_ALPHA_OPAQUE);
    rectangle.x = 0;
    rectangle.y = 0;
    if (SDL_RenderFillRect(renderer, &rectangle) != 0) // cordonner (x,y)
        SDL_ExitWithError("ça ne s'est pas dessiner");
    // images
    SDL_Surface *picture = SDL_LoadBMP("pionblanc.bmp");
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, picture);

    SDL_Rect dest_rect = {20,
                          110,
                          600,
                          600};

    SDL_RenderPresent(renderer); // permet d'afficher le rendu

    // gestion des evènement
    unsigned int frame_limit = 0;

    frame_limit = SDL_GetTicks() + 16;

    double cdnCX = 10000,
           cdnCY = 10000, test = 0;

    SDL_bool program_launched = SDL_TRUE; // permet de laisser la fenetre ouverte
    while (program_launched)              // temps que c'est true
    {
        SDL_Event event;                   // initialisation des évènements
        while (SDL_PollEvent(&event) == 1) // permet de rentrer dans la gestion des evenements
        {
            switch (event.type) // peremt de renrter das l'utilisation des events
            {
            case SDL_QUIT: // permet de faire ferme la fenetre grace a la croix
                program_launched = SDL_FALSE;
                break;
            case SDL_KEYDOWN:                 // permet de gerer l'appui sur une touche du clavier
                switch (event.key.keysym.sym) // le chemin pour acceder a la position des touche
                {
                case SDLK_b: // nom pour dir la touche b
                    SDL_Log("vous avez appuye sur b\n");
                    break;

                default:
                    break;
                }
            case SDL_MOUSEBUTTONDOWN:        // permet de gerer les clicks de la souris
                switch (event.button.clicks) // d'aller a l'endorit ou pourrons etre gerer les clics
                {
                case SDL_BUTTON_LEFT: // le bouton de gauche de la souris
                    cdnCX = event.button.x;
                    cdnCY = event.button.y;
                    // permet de savoir ou le click a été effectué
                    if (cdnCX >= 0 && (cdnCX <= (1.0 / 8.0) * TAILLX) && cdnCY >= 0 && (cdnCY <= (1.0 / 8.0) * TAILLY))
                        printf("A8\n");
                    else if ((cdnCX >= (1.0 / 8.0) * TAILLX) && (cdnCX <= (2.0 / 8.0) * TAILLX) && (cdnCY >= (0) * TAILLY) && (cdnCY <= (1.0 / 8.0) * TAILLY))
                        printf("B8\n");
                    else if ((cdnCX >= (2.0 / 8.0) * TAILLX) && (cdnCX <= (3.0 / 8.0) * TAILLX) && (cdnCY >= (0) * TAILLY) && (cdnCY <= (1.0 / 8.0) * TAILLY))
                        printf("C8\n");
                    else if ((cdnCX >= (3.0 / 8.0) * TAILLX) && (cdnCX <= (4.0 / 8.0) * TAILLX) && (cdnCY >= (0) * TAILLY) && (cdnCY <= (1.0 / 8.0) * TAILLY))
                        printf("D8\n");
                    else if ((cdnCX >= (4.0 / 8.0) * TAILLX) && (cdnCX <= (5.0 / 8.0) * TAILLX) && (cdnCY >= (0) * TAILLY) && (cdnCY <= (1.0 / 8.0) * TAILLY))
                        printf("E8\n");
                    else if ((cdnCX >= (5.0 / 8.0) * TAILLX) && (cdnCX <= (6.0 / 8.0) * TAILLX) && (cdnCY >= (0) * TAILLY) && (cdnCY <= (1.0 / 8.0) * TAILLY))
                        printf("F8\n");
                    else if ((cdnCX >= (6.0 / 8.0) * TAILLX) && (cdnCX <= (7.0 / 8.0) * TAILLX) && (cdnCY >= (0) * TAILLY) && (cdnCY <= (1.0 / 8.0) * TAILLY))
                        printf("G8\n");
                    else if ((cdnCX >= (7.0 / 8.0) * TAILLX) && (cdnCX <= (8.0 / 8.0) * TAILLX) && (cdnCY >= (0) * TAILLY) && (cdnCY <= (1.0 / 8.0) * TAILLY))
                        printf("H8\n");
                    else if ((cdnCX >= (0) * TAILLX) && (cdnCX <= (1.0 / 8.0) * TAILLX) && (cdnCY >= (1.0 / 8.0) * TAILLY) && (cdnCY <= (2.0 / 8.0) * TAILLY))
                        printf("A7\n");
                    else if ((cdnCX >= (1.0 / 8.0) * TAILLX) && (cdnCX <= (2.0 / 8.0) * TAILLX) && (cdnCY >= (1.0 / 8.0) * TAILLY) && (cdnCY <= (2.0 / 8.0) * TAILLY))
                        printf("B7\n");
                    else if ((cdnCX >= (2.0 / 8.0) * TAILLX) && (cdnCX <= (3.0 / 8.0) * TAILLX) && (cdnCY >= (1.0 / 8.0) * TAILLY) && (cdnCY <= (2.0 / 8.0) * TAILLY))
                        printf("C7\n");
                    else if ((cdnCX >= (3.0 / 8.0) * TAILLX) && (cdnCX <= (4.0 / 8.0) * TAILLX) && (cdnCY >= (1.0 / 8.0) * TAILLY) && (cdnCY <= (2.0 / 8.0) * TAILLY))
                        printf("D7\n");
                    else if ((cdnCX >= (4.0 / 8.0) * TAILLX) && (cdnCX <= (5.0 / 8.0) * TAILLX) && (cdnCY >= (1.0 / 8.0) * TAILLY) && (cdnCY <= (2.0 / 8.0) * TAILLY))
                        printf("E7\n");
                    else if ((cdnCX >= (5.0 / 8.0) * TAILLX) && (cdnCX <= (6.0 / 8.0) * TAILLX) && (cdnCY >= (1.0 / 8.0) * TAILLY) && (cdnCY <= (2.0 / 8.0) * TAILLY))
                        printf("F7\n");
                    else if ((cdnCX >= (6.0 / 8.0) * TAILLX) && (cdnCX <= (7.0 / 8.0) * TAILLX) && (cdnCY >= (1.0 / 8.0) * TAILLY) && (cdnCY <= (2.0 / 8.0) * TAILLY))
                        printf("G7\n");
                    else if ((cdnCX >= (7.0 / 8.0) * TAILLX) && (cdnCX <= (8.0 / 8.0) * TAILLX) && (cdnCY >= (1.0 / 8.0) * TAILLY) && (cdnCY <= (2.0 / 8.0) * TAILLY))
                        printf("H7\n");
                    else if ((cdnCX >= (0) * TAILLX) && (cdnCX <= (1.0 / 8.0) * TAILLX) && (cdnCY >= (2.0 / 8.0) * TAILLY) && (cdnCY <= (3.0 / 8.0) * TAILLY))
                        printf("A6\n");
                    else if ((cdnCX >= (1.0 / 8.0) * TAILLX) && (cdnCX <= (2.0 / 8.0) * TAILLX) && (cdnCY >= (2.0 / 8.0) * TAILLY) && (cdnCY <= (3.0 / 8.0) * TAILLY))
                        printf("B6\n");
                    else if ((cdnCX >= (2.0 / 8.0) * TAILLX) && (cdnCX <= (3.0 / 8.0) * TAILLX) && (cdnCY >= (2.0 / 8.0) * TAILLY) && (cdnCY <= (3.0 / 8.0) * TAILLY))
                        printf("C6\n");
                    else if ((cdnCX >= (3.0 / 8.0) * TAILLX) && (cdnCX <= (4.0 / 8.0) * TAILLX) && (cdnCY >= (2.0 / 8.0) * TAILLY) && (cdnCY <= (3.0 / 8.0) * TAILLY))
                        printf("D6\n");
                    else if ((cdnCX >= (4.0 / 8.0) * TAILLX) && (cdnCX <= (5.0 / 8.0) * TAILLX) && (cdnCY >= (2.0 / 8.0) * TAILLY) && (cdnCY <= (3.0 / 8.0) * TAILLY))
                        printf("E6\n");
                    else if ((cdnCX >= (5.0 / 8.0) * TAILLX) && (cdnCX <= (6.0 / 8.0) * TAILLX) && (cdnCY >= (2.0 / 8.0) * TAILLY) && (cdnCY <= (3.0 / 8.0) * TAILLY))
                        printf("F6\n");
                    else if ((cdnCX >= (6.0 / 8.0) * TAILLX) && (cdnCX <= (7.0 / 8.0) * TAILLX) && (cdnCY >= (2.0 / 8.0) * TAILLY) && (cdnCY <= (3.0 / 8.0) * TAILLY))
                        printf("G6\n");
                    else if ((cdnCX >= (7.0 / 8.0) * TAILLX) && (cdnCX <= (8.0 / 8.0) * TAILLX) && (cdnCY >= (2.0 / 8.0) * TAILLY) && (cdnCY <= (3.0 / 8.0) * TAILLY))
                        printf("H6\n");
                    else if ((cdnCX >= (0) * TAILLX) && (cdnCX <= (1.0 / 8.0) * TAILLX) && (cdnCY >= (3.0 / 8.0) * TAILLY) && (cdnCY <= (4.0 / 8.0) * TAILLY))
                        printf("A5\n");
                    else if ((cdnCX >= (1.0 / 8.0) * TAILLX) && (cdnCX <= (2.0 / 8.0) * TAILLX) && (cdnCY >= (3.0 / 8.0) * TAILLY) && (cdnCY <= (4.0 / 8.0) * TAILLY))
                        printf("B5\n");
                    else if ((cdnCX >= (2.0 / 8.0) * TAILLX) && (cdnCX <= (3.0 / 8.0) * TAILLX) && (cdnCY >= (3.0 / 8.0) * TAILLY) && (cdnCY <= (4.0 / 8.0) * TAILLY))
                        printf("C5\n");
                    else if ((cdnCX >= (3.0 / 8.0) * TAILLX) && (cdnCX <= (4.0 / 8.0) * TAILLX) && (cdnCY >= (3.0 / 8.0) * TAILLY) && (cdnCY <= (4.0 / 8.0) * TAILLY))
                        printf("D5\n");
                    else if ((cdnCX >= (4.0 / 8.0) * TAILLX) && (cdnCX <= (5.0 / 8.0) * TAILLX) && (cdnCY >= (3.0 / 8.0) * TAILLY) && (cdnCY <= (4.0 / 8.0) * TAILLY))
                        printf("E5\n");
                    else if ((cdnCX >= (5.0 / 8.0) * TAILLX) && (cdnCX <= (6.0 / 8.0) * TAILLX) && (cdnCY >= (3.0 / 8.0) * TAILLY) && (cdnCY <= (4.0 / 8.0) * TAILLY))
                        printf("F5\n");
                    else if ((cdnCX >= (6.0 / 8.0) * TAILLX) && (cdnCX <= (7.0 / 8.0) * TAILLX) && (cdnCY >= (3.0 / 8.0) * TAILLY) && (cdnCY <= (4.0 / 8.0) * TAILLY))
                        printf("G5\n");
                    else if ((cdnCX >= (7.0 / 8.0) * TAILLX) && (cdnCX <= (8.0 / 8.0) * TAILLX) && (cdnCY >= (3.0 / 8.0) * TAILLY) && (cdnCY <= (4.0 / 8.0) * TAILLY))
                        printf("H5\n");
                    else if ((cdnCX >= (0) * TAILLX) && (cdnCX <= (1.0 / 8.0) * TAILLX) && (cdnCY >= (4.0 / 8.0) * TAILLY) && (cdnCY <= (5.0 / 8.0) * TAILLY))
                        printf("A4\n");
                    else if ((cdnCX >= (1.0 / 8.0) * TAILLX) && (cdnCX <= (2.0 / 8.0) * TAILLX) && (cdnCY >= (4.0 / 8.0) * TAILLY) && (cdnCY <= (5.0 / 8.0) * TAILLY))
                        printf("B4\n");
                    else if ((cdnCX >= (2.0 / 8.0) * TAILLX) && (cdnCX <= (3.0 / 8.0) * TAILLX) && (cdnCY >= (4.0 / 8.0) * TAILLY) && (cdnCY <= (5.0 / 8.0) * TAILLY))
                        printf("C4\n");
                    else if ((cdnCX >= (3.0 / 8.0) * TAILLX) && (cdnCX <= (4.0 / 8.0) * TAILLX) && (cdnCY >= (4.0 / 8.0) * TAILLY) && (cdnCY <= (5.0 / 8.0) * TAILLY))
                        printf("D4\n");
                    else if ((cdnCX >= (4.0 / 8.0) * TAILLX) && (cdnCX <= (5.0 / 8.0) * TAILLX) && (cdnCY >= (4.0 / 8.0) * TAILLY) && (cdnCY <= (5.0 / 8.0) * TAILLY))
                        printf("E4\n");
                    else if ((cdnCX >= (5.0 / 8.0) * TAILLX) && (cdnCX <= (6.0 / 8.0) * TAILLX) && (cdnCY >= (4.0 / 8.0) * TAILLY) && (cdnCY <= (5.0 / 8.0) * TAILLY))
                        printf("F4\n");
                    else if ((cdnCX >= (6.0 / 8.0) * TAILLX) && (cdnCX <= (7.0 / 8.0) * TAILLX) && (cdnCY >= (4.0 / 8.0) * TAILLY) && (cdnCY <= (5.0 / 8.0) * TAILLY))
                        printf("G4\n");
                    else if ((cdnCX >= (7.0 / 8.0) * TAILLX) && (cdnCX <= (8.0 / 8.0) * TAILLX) && (cdnCY >= (4.0 / 8.0) * TAILLY) && (cdnCY <= (5.0 / 8.0) * TAILLY))
                        printf("H4\n");
                    else if ((cdnCX >= (0) * TAILLX) && (cdnCX <= (1.0 / 8.0) * TAILLX) && (cdnCY >= (5.0 / 8.0) * TAILLY) && (cdnCY <= (6.0 / 8.0) * TAILLY))
                        printf("A3\n");
                    else if ((cdnCX >= (1.0 / 8.0) * TAILLX) && (cdnCX <= (2.0 / 8.0) * TAILLX) && (cdnCY >= (5.0 / 8.0) * TAILLY) && (cdnCY <= (6.0 / 8.0) * TAILLY))
                        printf("B3\n");
                    else if ((cdnCX >= (2.0 / 8.0) * TAILLX) && (cdnCX <= (3.0 / 8.0) * TAILLX) && (cdnCY >= (5.0 / 8.0) * TAILLY) && (cdnCY <= (6.0 / 8.0) * TAILLY))
                    {
                        printf("C3\n");
                        positionPion(colonne1, colonne2, colonne3, colonne4, colonne5, colonne6, colonne7, colonne8, window, renderer, picture, texture);
                    }
                    else if ((cdnCX >= (3.0 / 8.0) * TAILLX) && (cdnCX <= (4.0 / 8.0) * TAILLX) && (cdnCY >= (5.0 / 8.0) * TAILLY) && (cdnCY <= (6.0 / 8.0) * TAILLY))
                        printf("D3\n");
                    else if ((cdnCX >= (4.0 / 8.0) * TAILLX) && (cdnCX <= (5.0 / 8.0) * TAILLX) && (cdnCY >= (5.0 / 8.0) * TAILLY) && (cdnCY <= (6.0 / 8.0) * TAILLY))
                        printf("E3\n");
                    else if ((cdnCX >= (5.0 / 8.0) * TAILLX) && (cdnCX <= (6.0 / 8.0) * TAILLX) && (cdnCY >= (5.0 / 8.0) * TAILLY) && (cdnCY <= (6.0 / 8.0) * TAILLY))
                        printf("F3\n");
                    else if ((cdnCX >= (6.0 / 8.0) * TAILLX) && (cdnCX <= (7.0 / 8.0) * TAILLX) && (cdnCY >= (5.0 / 8.0) * TAILLY) && (cdnCY <= (6.0 / 8.0) * TAILLY))
                        printf("G3\n");
                    else if ((cdnCX >= (7.0 / 8.0) * TAILLX) && (cdnCX <= (8.0 / 8.0) * TAILLX) && (cdnCY >= (5.0 / 8.0) * TAILLY) && (cdnCY <= (6.0 / 8.0) * TAILLY))
                        printf("H3\n");
                    else if ((cdnCX >= (0) * TAILLX) && (cdnCX <= (1.0 / 8.0) * TAILLX) && (cdnCY >= (6.0 / 8.0) * TAILLY) && (cdnCY <= (7.0 / 8.0) * TAILLY))
                        printf("A2\n");
                    else if ((cdnCX >= (1.0 / 8.0) * TAILLX) && (cdnCX <= (2.0 / 8.0) * TAILLX) && (cdnCY >= (6.0 / 8.0) * TAILLY) && (cdnCY <= (7.0 / 8.0) * TAILLY))
                        printf("B2\n");
                    else if ((cdnCX >= (2.0 / 8.0) * TAILLX) && (cdnCX <= (3.0 / 8.0) * TAILLX) && (cdnCY >= (6.0 / 8.0) * TAILLY) && (cdnCY <= (7.0 / 8.0) * TAILLY))
                        printf("C2\n");
                    else if ((cdnCX >= (3.0 / 8.0) * TAILLX) && (cdnCX <= (4.0 / 8.0) * TAILLX) && (cdnCY >= (6.0 / 8.0) * TAILLY) && (cdnCY <= (7.0 / 8.0) * TAILLY))
                        printf("D2\n");
                    else if ((cdnCX >= (4.0 / 8.0) * TAILLX) && (cdnCX <= (5.0 / 8.0) * TAILLX) && (cdnCY >= (6.0 / 8.0) * TAILLY) && (cdnCY <= (7.0 / 8.0) * TAILLY))
                        printf("E2\n");
                    else if ((cdnCX >= (5.0 / 8.0) * TAILLX) && (cdnCX <= (6.0 / 8.0) * TAILLX) && (cdnCY >= (6.0 / 8.0) * TAILLY) && (cdnCY <= (7.0 / 8.0) * TAILLY))
                        printf("F2\n");
                    else if ((cdnCX >= (6.0 / 8.0) * TAILLX) && (cdnCX <= (7.0 / 8.0) * TAILLX) && (cdnCY >= (6.0 / 8.0) * TAILLY) && (cdnCY <= (7.0 / 8.0) * TAILLY))
                        printf("G2\n");
                    else if ((cdnCX >= (7.0 / 8.0) * TAILLX) && (cdnCX <= (8.0 / 8.0) * TAILLX) && (cdnCY >= (6.0 / 8.0) * TAILLY) && (cdnCY <= (7.0 / 8.0) * TAILLY))
                        printf("H2\n");
                    else if ((cdnCX >= (0) * TAILLX) && (cdnCX <= (1.0 / 8.0) * TAILLX) && (cdnCY >= (7.0 / 8.0) * TAILLY) && (cdnCY <= (8.0 / 8.0) * TAILLY))
                        printf("A1\n");
                    else if ((cdnCX >= (1.0 / 8.0) * TAILLX) && (cdnCX <= (2.0 / 8.0) * TAILLX) && (cdnCY >= (7.0 / 8.0) * TAILLY) && (cdnCY <= (8.0 / 8.0) * TAILLY))
                        printf("B1\n");
                    else if ((cdnCX >= (2.0 / 8.0) * TAILLX) && (cdnCX <= (3.0 / 8.0) * TAILLX) && (cdnCY >= (7.0 / 8.0) * TAILLY) && (cdnCY <= (8.0 / 8.0) * TAILLY))
                        printf("C1\n");
                    else if ((cdnCX >= (3.0 / 8.0) * TAILLX) && (cdnCX <= (4.0 / 8.0) * TAILLX) && (cdnCY >= (7.0 / 8.0) * TAILLY) && (cdnCY <= (8.0 / 8.0) * TAILLY))
                        printf("D1\n");
                    else if ((cdnCX >= (4.0 / 8.0) * TAILLX) && (cdnCX <= (5.0 / 8.0) * TAILLX) && (cdnCY >= (7.0 / 8.0) * TAILLY) && (cdnCY <= (8.0 / 8.0) * TAILLY))
                        printf("E1\n");
                    else if ((cdnCX >= (5.0 / 8.0) * TAILLX) && (cdnCX <= (6.0 / 8.0) * TAILLX) && (cdnCY >= (7.0 / 8.0) * TAILLY) && (cdnCY <= (8.0 / 8.0) * TAILLY))
                        printf("F1\n");
                    else if ((cdnCX >= (6.0 / 8.0) * TAILLX) && (cdnCX <= (7.0 / 8.0) * TAILLX) && (cdnCY >= (7.0 / 8.0) * TAILLY) && (cdnCY <= (8.0 / 8.0) * TAILLY))
                        printf("G1\n");
                    else if ((cdnCX >= (7.0 / 8.0) * TAILLX) && (cdnCX <= (8.0 / 8.0) * TAILLX) && (cdnCY >= (7.0 / 8.0) * TAILLY) && (cdnCY <= (8.0 / 8.0) * TAILLY))
                        printf("H1\n");
                    frame_limit = SDL_GetTicks() + 16;

                    break;

                default:
                    break;
                }
                break;
            default:
                break;
            }
        }
    }
    /*1 on gere l'ouvertur et fermetur du projet*/
    //-------------------------------------------------------------

    // pour liberer la memeoir !! dans le sens inverse de leurs initialisation
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit(); // TOUJOUR FERMER LA SDL

    return EXIT_SUCCESS;
}

void SDL_ExitWithError(const char *message)
{
    SDL_Log("ERREUR : %s > %s\n", message, SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}
// if ((cdnCX >= (3 / (TAILLX / 100) * TAILLX) && cdnCX <= 400) && (cdnCY >= 400 && cdnCY <= 500))

void positionPion(int colonne1[], int colonne2[], int colonne3[], int colonne4[], int colonne5[], int colonne6[], int colonne7[], int colonne8[], SDL_Window *window, SDL_Renderer *renderer, SDL_Surface *picture, SDL_Texture *texture)
{
    SDL_Rect dest_rect = {20,
                          110,
                          600,
                          600};
    for (size_t i = 0; i < 1; i++)
    {

        for (size_t a = 0; a < 8; a++)
        {
            dest_rect.y = 10;
            switch (colonne1[a])
            {
            case 1:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/pionnoir.bmp");
                break;
            case 2:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/pionblanc.bmp");
                break;
            case 6:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/tournoir.bmp");
                break;
            case 7:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/cavaliernoir.bmp");
                break;
            case 8:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/founoir.bmp");
                break;
            case 9:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/damenoir.bmp");
                break;
            case 10:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/roinoir.bmp");
                break;
            case 12:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/tourblanc.bmp");
                break;
            case 14:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/cavalierblanc.bmp");
                break;
            case 16:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/foublanc.bmp");
                break;
            case 18:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/dameblanc.bmp");
                break;
            case 20:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/roiblanc.bmp");
                break;
            case 0:
                continue;
                break;

            default:
                break;
            }

            if (picture == NULL)
                SDL_ExitWithError("inmpossible de cree la fenetre et le rendu");
            texture = SDL_CreateTextureFromSurface(renderer, picture);
            SDL_FreeSurface(picture);
            if (texture == NULL)
                SDL_ExitWithError("inmpossible de cree la fenetre et le rendu3");
            if (SDL_QueryTexture(texture, NULL, NULL, &dest_rect.w, &dest_rect.h) != 0)
                SDL_ExitWithError("inmpossible de cree la fenetre et le rendu2");
            if (SDL_RenderCopy(renderer, texture, NULL, &dest_rect) != 0)
                SDL_ExitWithError("inmpossible de cree la fenetre et le rendu1");
            dest_rect.x += 100;
        }
        for (size_t a = 0; a < 8; a++)
        {
            dest_rect.y = 110;
            switch (colonne2[a])
            {
            case 1:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/pionnoir.bmp");
                break;
            case 2:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/pionblanc.bmp");
                break;
            case 6:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/tournoir.bmp");
                break;
            case 7:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/cavaliernoir.bmp");
                break;
            case 8:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/founoir.bmp");
                break;
            case 9:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/damenoir.bmp");
                break;
            case 10:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/roinoir.bmp");
                break;
            case 12:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/tourblanc.bmp");
                break;
            case 14:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/cavalierblanc.bmp");
                break;
            case 16:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/foublanc.bmp");
                break;
            case 18:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/dameblanc.bmp");
                break;
            case 20:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/roiblanc.bmp");
                break;
            case 0:
                continue;
                break;

            default:
                break;
            }

            if (picture == NULL)
                SDL_ExitWithError("inmpossible de cree la fenetre et le rendu");
            texture = SDL_CreateTextureFromSurface(renderer, picture);
            SDL_FreeSurface(picture);
            if (texture == NULL)
                SDL_ExitWithError("inmpossible de cree la fenetre et le rendu3");
            if (SDL_QueryTexture(texture, NULL, NULL, &dest_rect.w, &dest_rect.h) != 0)
                SDL_ExitWithError("inmpossible de cree la fenetre et le rendu2");
            if (SDL_RenderCopy(renderer, texture, NULL, &dest_rect) != 0)
                SDL_ExitWithError("inmpossible de cree la fenetre et le rendu1");
            dest_rect.x += 100;
        }
        for (size_t a = 0; a < 8; a++)
        {
            dest_rect.y = 210;
            switch (colonne3[a])
            {
            case 1:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/pionnoir.bmp");
                break;
            case 2:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/pionblanc.bmp");
                break;
            case 6:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/tournoir.bmp");
                break;
            case 7:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/cavaliernoir.bmp");
                break;
            case 8:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/founoir.bmp");
                break;
            case 9:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/damenoir.bmp");
                break;
            case 10:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/roinoir.bmp");
                break;
            case 12:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/tourblanc.bmp");
                break;
            case 14:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/cavalierblanc.bmp");
                break;
            case 16:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/foublanc.bmp");
                break;
            case 18:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/dameblanc.bmp");
                break;
            case 20:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/roiblanc.bmp");
                break;
            case 0:
                continue;
                break;

            default:
                break;
            }

            if (picture == NULL)
                SDL_ExitWithError("inmpossible de cree la fenetre et le rendu");
            texture = SDL_CreateTextureFromSurface(renderer, picture);
            SDL_FreeSurface(picture);
            if (texture == NULL)
                SDL_ExitWithError("inmpossible de cree la fenetre et le rendu3");
            if (SDL_QueryTexture(texture, NULL, NULL, &dest_rect.w, &dest_rect.h) != 0)
                SDL_ExitWithError("inmpossible de cree la fenetre et le rendu2");
            if (SDL_RenderCopy(renderer, texture, NULL, &dest_rect) != 0)
                SDL_ExitWithError("inmpossible de cree la fenetre et le rendu1");
            dest_rect.x += 100;
        }
        for (size_t a = 0; a < 8; a++)
        {
            dest_rect.y = 310;
            switch (colonne4[a])
            {
            case 1:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/pionnoir.bmp");
                break;
            case 2:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/pionblanc.bmp");
                break;
            case 6:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/tournoir.bmp");
                break;
            case 7:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/cavaliernoir.bmp");
                break;
            case 8:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/founoir.bmp");
                break;
            case 9:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/damenoir.bmp");
                break;
            case 10:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/roinoir.bmp");
                break;
            case 12:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/tourblanc.bmp");
                break;
            case 14:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/cavalierblanc.bmp");
                break;
            case 16:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/foublanc.bmp");
                break;
            case 18:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/dameblanc.bmp");
                break;
            case 20:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/roiblanc.bmp");
                break;
            case 0:
                continue;
                break;

            default:
                break;
            }

            if (picture == NULL)
                SDL_ExitWithError("inmpossible de cree la fenetre et le rendu");
            texture = SDL_CreateTextureFromSurface(renderer, picture);
            SDL_FreeSurface(picture);
            if (texture == NULL)
                SDL_ExitWithError("inmpossible de cree la fenetre et le rendu3");
            if (SDL_QueryTexture(texture, NULL, NULL, &dest_rect.w, &dest_rect.h) != 0)
                SDL_ExitWithError("inmpossible de cree la fenetre et le rendu2");
            if (SDL_RenderCopy(renderer, texture, NULL, &dest_rect) != 0)
                SDL_ExitWithError("inmpossible de cree la fenetre et le rendu1");
            dest_rect.x += 100;
        }
        for (size_t a = 0; a < 8; a++)
        {
            dest_rect.y = 410;
            switch (colonne5[a])
            {
            case 1:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/pionnoir.bmp");
                break;
            case 2:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/pionblanc.bmp");
                break;
            case 6:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/tournoir.bmp");
                break;
            case 7:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/cavaliernoir.bmp");
                break;
            case 8:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/founoir.bmp");
                break;
            case 9:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/damenoir.bmp");
                break;
            case 10:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/roinoir.bmp");
                break;
            case 12:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/tourblanc.bmp");
                break;
            case 14:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/cavalierblanc.bmp");
                break;
            case 16:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/foublanc.bmp");
                break;
            case 18:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/dameblanc.bmp");
                break;
            case 20:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/roiblanc.bmp");
                break;
            case 0:
                continue;
                break;

            default:
                break;
            }

            if (picture == NULL)
                SDL_ExitWithError("inmpossible de cree la fenetre et le rendu");
            texture = SDL_CreateTextureFromSurface(renderer, picture);
            SDL_FreeSurface(picture);
            if (texture == NULL)
                SDL_ExitWithError("inmpossible de cree la fenetre et le rendu3");
            if (SDL_QueryTexture(texture, NULL, NULL, &dest_rect.w, &dest_rect.h) != 0)
                SDL_ExitWithError("inmpossible de cree la fenetre et le rendu2");
            if (SDL_RenderCopy(renderer, texture, NULL, &dest_rect) != 0)
                SDL_ExitWithError("inmpossible de cree la fenetre et le rendu1");
            dest_rect.x += 100;
        }
        for (size_t a = 0; a < 8; a++)
        {
            dest_rect.y = 510;
            switch (colonne6[a])
            {
            case 1:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/pionnoir.bmp");
                break;
            case 2:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/pionblanc.bmp");
                break;
            case 6:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/tournoir.bmp");
                break;
            case 7:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/cavaliernoir.bmp");
                break;
            case 8:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/founoir.bmp");
                break;
            case 9:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/damenoir.bmp");
                break;
            case 10:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/roinoir.bmp");
                break;
            case 12:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/tourblanc.bmp");
                break;
            case 14:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/cavalierblanc.bmp");
                break;
            case 16:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/foublanc.bmp");
                break;
            case 18:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/dameblanc.bmp");
                break;
            case 20:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/roiblanc.bmp");
                break;
            case 0:
                continue;
                break;

            default:
                break;
            }

            if (picture == NULL)
                SDL_ExitWithError("inmpossible de cree la fenetre et le rendu");
            texture = SDL_CreateTextureFromSurface(renderer, picture);
            SDL_FreeSurface(picture);
            if (texture == NULL)
                SDL_ExitWithError("inmpossible de cree la fenetre et le rendu3");
            if (SDL_QueryTexture(texture, NULL, NULL, &dest_rect.w, &dest_rect.h) != 0)
                SDL_ExitWithError("inmpossible de cree la fenetre et le rendu2");
            if (SDL_RenderCopy(renderer, texture, NULL, &dest_rect) != 0)
                SDL_ExitWithError("inmpossible de cree la fenetre et le rendu1");
            dest_rect.x += 100;
        }
        for (size_t a = 0; a < 8; a++)
        {
            dest_rect.y = 610;
            switch (colonne7[a])
            {
            case 1:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/pionnoir.bmp");
                break;
            case 2:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/pionblanc.bmp");
                break;
            case 6:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/tournoir.bmp");
                break;
            case 7:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/cavaliernoir.bmp");
                break;
            case 8:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/founoir.bmp");
                break;
            case 9:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/damenoir.bmp");
                break;
            case 10:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/roinoir.bmp");
                break;
            case 12:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/tourblanc.bmp");
                break;
            case 14:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/cavalierblanc.bmp");
                break;
            case 16:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/foublanc.bmp");
                break;
            case 18:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/dameblanc.bmp");
                break;
            case 20:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/roiblanc.bmp");
                break;
            case 0:
                continue;
                break;

            default:
                break;
            }

            if (picture == NULL)
                SDL_ExitWithError("inmpossible de cree la fenetre et le rendu");
            texture = SDL_CreateTextureFromSurface(renderer, picture);
            SDL_FreeSurface(picture);
            if (texture == NULL)
                SDL_ExitWithError("inmpossible de cree la fenetre et le rendu3");
            if (SDL_QueryTexture(texture, NULL, NULL, &dest_rect.w, &dest_rect.h) != 0)
                SDL_ExitWithError("inmpossible de cree la fenetre et le rendu2");
            if (SDL_RenderCopy(renderer, texture, NULL, &dest_rect) != 0)
                SDL_ExitWithError("inmpossible de cree la fenetre et le rendu1");
            dest_rect.x += 100;
        }
        for (size_t a = 0; a < 8; a++)
        {
            dest_rect.y = 710;
            switch (colonne8[a])
            {
            case 1:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/pionnoir.bmp");
                break;
            case 2:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/pionblanc.bmp");
                break;
            case 6:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/tournoir.bmp");
                break;
            case 7:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/cavaliernoir.bmp");
                break;
            case 8:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/founoir.bmp");
                break;
            case 9:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/damenoir.bmp");
                break;
            case 10:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/roinoir.bmp");
                break;
            case 12:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/tourblanc.bmp");
                break;
            case 14:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/cavalierblanc.bmp");
                break;
            case 16:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/foublanc.bmp");
                break;
            case 18:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/dameblanc.bmp");
                break;
            case 20:
                dest_rect.x = 20 + 100 * a;
                picture = SDL_LoadBMP("src/roiblanc.bmp");
                break;
            case 0:
                continue;
                break;

            default:
                break;
            }

            if (picture == NULL)
                SDL_ExitWithError("inmpossible de cree la fenetre et le rendu");
            texture = SDL_CreateTextureFromSurface(renderer, picture);
            SDL_FreeSurface(picture);
            if (texture == NULL)
                SDL_ExitWithError("inmpossible de cree la fenetre et le rendu3");
            if (SDL_QueryTexture(texture, NULL, NULL, &dest_rect.w, &dest_rect.h) != 0)
                SDL_ExitWithError("inmpossible de cree la fenetre et le rendu2");
            if (SDL_RenderCopy(renderer, texture, NULL, &dest_rect) != 0)
                SDL_ExitWithError("inmpossible de cree la fenetre et le rendu1");
            dest_rect.x += 100;
        }
        SDL_RenderPresent(renderer); // permet d'afficher le rendu
    }
}
