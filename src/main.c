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

// prototype
void SDL_ExitWithError(const char *message);
void positionPion(int colonne1[], int colonne2[], int colonne3[], int colonne4[], int colonne5[], int colonne6[], int colonne7[], int colonne8[], SDL_Window *window, SDL_Renderer *renderer, SDL_Surface *picture, SDL_Texture *texture);
int captation(int colonne1[], int colonne2[], int colonne3[], int colonne4[], int colonne5[], int colonne6[], int colonne7[], int colonne8[], int cdnCX, int cdnCY, int *typeDePion, int *numeros);
void deplacement(int colonne1[], int colonne2[], int colonne3[], int colonne4[], int colonne5[], int colonne6[], int colonne7[], int colonne8[], int cdnCX, int cdnCY, int typeDePion, int numeros);
void afficherPlato(SDL_Renderer *renderer);

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
    afficherPlato(renderer);
    // images
    SDL_Surface *picture = SDL_LoadBMP("pionblanc.bmp");
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, picture);

    SDL_Rect dest_rect = {20,
                          110,
                          600,
                          600};

    SDL_RenderPresent(renderer); // permet d'afficher le rendu
    positionPion(colonne1, colonne2, colonne3, colonne4, colonne5, colonne6, colonne7, colonne8, window, renderer, picture, texture);
    // gestion des evènement

    double cdnCX = 10000,
           cdnCY = 10000, test = 0;
    int transi = 21, numeros = 66;

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
            case SDL_MOUSEBUTTONDOWN: // permet de gerer les clicks de la souris
            {
                cdnCX = event.button.x;
                cdnCY = event.button.y;
                // permet de savoir ou le click a été effectué
                if (event.button.button == SDL_BUTTON_LEFT) // click gauche pour prendre la pièce
                {
                    captation(colonne1, colonne2, colonne3, colonne4, colonne5, colonne6, colonne7, colonne8, cdnCX, cdnCY, &transi, &numeros);
                }
                else if (event.button.button == SDL_BUTTON_RIGHT) // click droit pour poser la piece
                {
                    deplacement(colonne1, colonne2, colonne3, colonne4, colonne5, colonne6, colonne7, colonne8, cdnCX, cdnCY, transi, numeros);
                    transi = 0; // permet de ne pas depaler plusieur fois la même pièce
                }
                printf("%d\n", colonne2[2]);
                // -------- permet de reactuliser toute les nouvelles positions
                afficherPlato(renderer);
                positionPion(colonne1, colonne2, colonne3, colonne4, colonne5, colonne6, colonne7, colonne8, window, renderer, picture, texture);
                // --------
                break;
            }

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

// fonction regardans sur chaque case ce qu'il y a et l'affiche
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

// afficher position positionPion(colonne1, colonne2, colonne3, colonne4, colonne5, colonne6, colonne7, colonne8, window, renderer, picture, texture);

// permet de de savoir ou le click est effectuer
int captation(int colonne1[], int colonne2[], int colonne3[], int colonne4[], int colonne5[], int colonne6[], int colonne7[], int colonne8[], int cdnCX, int cdnCY, int *typeDePion, int *numeros)
{

    *numeros = 66;
    *typeDePion = 21;
    if (cdnCX >= 0 && (cdnCX <= (1.0 / 8.0) * TAILLX) && cdnCY >= 0 && (cdnCY <= (1.0 / 8.0) * TAILLY))
    {
        printf("A8\n");
        *typeDePion = colonne1[0];
        *numeros = 1;
    }
    else if ((cdnCX >= (1.0 / 8.0) * TAILLX) && (cdnCX <= (2.0 / 8.0) * TAILLX) && (cdnCY >= (0) * TAILLY) && (cdnCY <= (1.0 / 8.0) * TAILLY))
    {
        printf("B8\n");
        *typeDePion = colonne1[1];
        *numeros = 2;
    }
    else if ((cdnCX >= (2.0 / 8.0) * TAILLX) && (cdnCX <= (3.0 / 8.0) * TAILLX) && (cdnCY >= (0) * TAILLY) && (cdnCY <= (1.0 / 8.0) * TAILLY))
    {
        printf("C8\n");
        *typeDePion = colonne1[2];
        *numeros = 3;
    }
    else if ((cdnCX >= (3.0 / 8.0) * TAILLX) && (cdnCX <= (4.0 / 8.0) * TAILLX) && (cdnCY >= (0) * TAILLY) && (cdnCY <= (1.0 / 8.0) * TAILLY))
    {
        printf("D8\n");
        *typeDePion = colonne1[3];
        *numeros = 4;
    }
    else if ((cdnCX >= (4.0 / 8.0) * TAILLX) && (cdnCX <= (5.0 / 8.0) * TAILLX) && (cdnCY >= (0) * TAILLY) && (cdnCY <= (1.0 / 8.0) * TAILLY))
    {
        printf("E8\n");
        *typeDePion = colonne1[4];
        *numeros = 5;
    }
    else if ((cdnCX >= (5.0 / 8.0) * TAILLX) && (cdnCX <= (6.0 / 8.0) * TAILLX) && (cdnCY >= (0) * TAILLY) && (cdnCY <= (1.0 / 8.0) * TAILLY))
    {
        printf("F8\n");
        *typeDePion = colonne1[5];
        *numeros = 6;
    }
    else if ((cdnCX >= (6.0 / 8.0) * TAILLX) && (cdnCX <= (7.0 / 8.0) * TAILLX) && (cdnCY >= (0) * TAILLY) && (cdnCY <= (1.0 / 8.0) * TAILLY))
    {
        printf("G8\n");
        *typeDePion = colonne1[6];
        *numeros = 7;
    }
    else if ((cdnCX >= (7.0 / 8.0) * TAILLX) && (cdnCX <= (8.0 / 8.0) * TAILLX) && (cdnCY >= (0) * TAILLY) && (cdnCY <= (1.0 / 8.0) * TAILLY))
    {
        printf("H8\n");
        *typeDePion = colonne1[7];
        *numeros = 8;
    }
    else if ((cdnCX >= (0) * TAILLX) && (cdnCX <= (1.0 / 8.0) * TAILLX) && (cdnCY >= (1.0 / 8.0) * TAILLY) && (cdnCY <= (2.0 / 8.0) * TAILLY))
    {
        printf("A7\n");
        *typeDePion = colonne2[0];
        *numeros = 9;
    }
    else if ((cdnCX >= (1.0 / 8.0) * TAILLX) && (cdnCX <= (2.0 / 8.0) * TAILLX) && (cdnCY >= (1.0 / 8.0) * TAILLY) && (cdnCY <= (2.0 / 8.0) * TAILLY))
    {
        printf("B7\n");
        *typeDePion = colonne2[1];
        *numeros = 10;
    }
    else if ((cdnCX >= (2.0 / 8.0) * TAILLX) && (cdnCX <= (3.0 / 8.0) * TAILLX) && (cdnCY >= (1.0 / 8.0) * TAILLY) && (cdnCY <= (2.0 / 8.0) * TAILLY))
    {
        printf("C7\n");
        *typeDePion = colonne2[2];
        *numeros = 11;
    }
    else if ((cdnCX >= (3.0 / 8.0) * TAILLX) && (cdnCX <= (4.0 / 8.0) * TAILLX) && (cdnCY >= (1.0 / 8.0) * TAILLY) && (cdnCY <= (2.0 / 8.0) * TAILLY))
    {
        printf("D7\n");
        *typeDePion = colonne2[3];
        *numeros = 12;
    }
    else if ((cdnCX >= (4.0 / 8.0) * TAILLX) && (cdnCX <= (5.0 / 8.0) * TAILLX) && (cdnCY >= (1.0 / 8.0) * TAILLY) && (cdnCY <= (2.0 / 8.0) * TAILLY))
    {
        printf("E7\n");
        *typeDePion = colonne2[4];
        *numeros = 13;
    }
    else if ((cdnCX >= (5.0 / 8.0) * TAILLX) && (cdnCX <= (6.0 / 8.0) * TAILLX) && (cdnCY >= (1.0 / 8.0) * TAILLY) && (cdnCY <= (2.0 / 8.0) * TAILLY))
    {
        printf("F7\n");
        *typeDePion = colonne2[5];
        *numeros = 14;
    }
    else if ((cdnCX >= (6.0 / 8.0) * TAILLX) && (cdnCX <= (7.0 / 8.0) * TAILLX) && (cdnCY >= (1.0 / 8.0) * TAILLY) && (cdnCY <= (2.0 / 8.0) * TAILLY))
    {
        printf("G7\n");
        *typeDePion = colonne2[6];
        *numeros = 15;
    }
    else if ((cdnCX >= (7.0 / 8.0) * TAILLX) && (cdnCX <= (8.0 / 8.0) * TAILLX) && (cdnCY >= (1.0 / 8.0) * TAILLY) && (cdnCY <= (2.0 / 8.0) * TAILLY))
    {
        printf("H7\n");
        *typeDePion = colonne2[7];
        *numeros = 16;
    }
    else if ((cdnCX >= (0) * TAILLX) && (cdnCX <= (1.0 / 8.0) * TAILLX) && (cdnCY >= (2.0 / 8.0) * TAILLY) && (cdnCY <= (3.0 / 8.0) * TAILLY))
    {
        printf("A6\n");
        *typeDePion = colonne3[0];
        *numeros = 17;
    }
    else if ((cdnCX >= (1.0 / 8.0) * TAILLX) && (cdnCX <= (2.0 / 8.0) * TAILLX) && (cdnCY >= (2.0 / 8.0) * TAILLY) && (cdnCY <= (3.0 / 8.0) * TAILLY))
    {
        printf("B6\n");
        *typeDePion = colonne3[1];
        *numeros = 18;
    }
    else if ((cdnCX >= (2.0 / 8.0) * TAILLX) && (cdnCX <= (3.0 / 8.0) * TAILLX) && (cdnCY >= (2.0 / 8.0) * TAILLY) && (cdnCY <= (3.0 / 8.0) * TAILLY))
    {
        printf("C6\n");
        *typeDePion = colonne3[2];
        *numeros = 19;
    }
    else if ((cdnCX >= (3.0 / 8.0) * TAILLX) && (cdnCX <= (4.0 / 8.0) * TAILLX) && (cdnCY >= (2.0 / 8.0) * TAILLY) && (cdnCY <= (3.0 / 8.0) * TAILLY))
    {
        printf("D6\n");
        *typeDePion = colonne3[3];
        *numeros = 20;
    }
    else if ((cdnCX >= (4.0 / 8.0) * TAILLX) && (cdnCX <= (5.0 / 8.0) * TAILLX) && (cdnCY >= (2.0 / 8.0) * TAILLY) && (cdnCY <= (3.0 / 8.0) * TAILLY))
    {
        printf("E6\n");
        *typeDePion = colonne3[4];
        *numeros = 21;
    }
    else if ((cdnCX >= (5.0 / 8.0) * TAILLX) && (cdnCX <= (6.0 / 8.0) * TAILLX) && (cdnCY >= (2.0 / 8.0) * TAILLY) && (cdnCY <= (3.0 / 8.0) * TAILLY))
    {
        printf("F6\n");
        *typeDePion = colonne3[5];
        *numeros = 22;
    }
    else if ((cdnCX >= (6.0 / 8.0) * TAILLX) && (cdnCX <= (7.0 / 8.0) * TAILLX) && (cdnCY >= (2.0 / 8.0) * TAILLY) && (cdnCY <= (3.0 / 8.0) * TAILLY))
    {
        printf("G6\n");
        *typeDePion = colonne3[6];
        *numeros = 23;
    }
    else if ((cdnCX >= (7.0 / 8.0) * TAILLX) && (cdnCX <= (8.0 / 8.0) * TAILLX) && (cdnCY >= (2.0 / 8.0) * TAILLY) && (cdnCY <= (3.0 / 8.0) * TAILLY))
    {
        printf("H6\n");
        *typeDePion = colonne3[7];
        *numeros = 24;
    }
    else if ((cdnCX >= (0) * TAILLX) && (cdnCX <= (1.0 / 8.0) * TAILLX) && (cdnCY >= (3.0 / 8.0) * TAILLY) && (cdnCY <= (4.0 / 8.0) * TAILLY))
    {
        printf("A5\n");
        *typeDePion = colonne4[0];
        *numeros = 25;
    }
    else if ((cdnCX >= (1.0 / 8.0) * TAILLX) && (cdnCX <= (2.0 / 8.0) * TAILLX) && (cdnCY >= (3.0 / 8.0) * TAILLY) && (cdnCY <= (4.0 / 8.0) * TAILLY))
    {
        printf("B5\n");
        *typeDePion = colonne4[1];
        *numeros = 26;
    }
    else if ((cdnCX >= (2.0 / 8.0) * TAILLX) && (cdnCX <= (3.0 / 8.0) * TAILLX) && (cdnCY >= (3.0 / 8.0) * TAILLY) && (cdnCY <= (4.0 / 8.0) * TAILLY))
    {
        printf("C5\n");
        *typeDePion = colonne4[2];
        *numeros = 27;
    }
    else if ((cdnCX >= (3.0 / 8.0) * TAILLX) && (cdnCX <= (4.0 / 8.0) * TAILLX) && (cdnCY >= (3.0 / 8.0) * TAILLY) && (cdnCY <= (4.0 / 8.0) * TAILLY))
    {
        printf("D5\n");
        *typeDePion = colonne4[3];
        *numeros = 28;
    }
    else if ((cdnCX >= (4.0 / 8.0) * TAILLX) && (cdnCX <= (5.0 / 8.0) * TAILLX) && (cdnCY >= (3.0 / 8.0) * TAILLY) && (cdnCY <= (4.0 / 8.0) * TAILLY))
    {
        printf("E5\n");
        *typeDePion = colonne4[4];
        *numeros = 29;
    }
    else if ((cdnCX >= (5.0 / 8.0) * TAILLX) && (cdnCX <= (6.0 / 8.0) * TAILLX) && (cdnCY >= (3.0 / 8.0) * TAILLY) && (cdnCY <= (4.0 / 8.0) * TAILLY))
    {
        printf("F5\n");
        *typeDePion = colonne4[5];
        *numeros = 30;
    }
    else if ((cdnCX >= (6.0 / 8.0) * TAILLX) && (cdnCX <= (7.0 / 8.0) * TAILLX) && (cdnCY >= (3.0 / 8.0) * TAILLY) && (cdnCY <= (4.0 / 8.0) * TAILLY))
    {
        printf("G5\n");
        *typeDePion = colonne4[6];
        *numeros = 31;
    }
    else if ((cdnCX >= (7.0 / 8.0) * TAILLX) && (cdnCX <= (8.0 / 8.0) * TAILLX) && (cdnCY >= (3.0 / 8.0) * TAILLY) && (cdnCY <= (4.0 / 8.0) * TAILLY))
    {
        printf("H5\n");
        *typeDePion = colonne4[7];
        *numeros = 32;
    }
    else if ((cdnCX >= (0) * TAILLX) && (cdnCX <= (1.0 / 8.0) * TAILLX) && (cdnCY >= (4.0 / 8.0) * TAILLY) && (cdnCY <= (5.0 / 8.0) * TAILLY))
    {
        printf("A4\n");
        *typeDePion = colonne5[0];
        *numeros = 33;
    }
    else if ((cdnCX >= (1.0 / 8.0) * TAILLX) && (cdnCX <= (2.0 / 8.0) * TAILLX) && (cdnCY >= (4.0 / 8.0) * TAILLY) && (cdnCY <= (5.0 / 8.0) * TAILLY))
    {
        printf("B4\n");
        *typeDePion = colonne5[1];
        *numeros = 34;
    }
    else if ((cdnCX >= (2.0 / 8.0) * TAILLX) && (cdnCX <= (3.0 / 8.0) * TAILLX) && (cdnCY >= (4.0 / 8.0) * TAILLY) && (cdnCY <= (5.0 / 8.0) * TAILLY))
    {
        printf("C4\n");
        *typeDePion = colonne5[2];
        *numeros = 35;
    }
    else if ((cdnCX >= (3.0 / 8.0) * TAILLX) && (cdnCX <= (4.0 / 8.0) * TAILLX) && (cdnCY >= (4.0 / 8.0) * TAILLY) && (cdnCY <= (5.0 / 8.0) * TAILLY))
    {
        printf("D4\n");
        *typeDePion = colonne5[3];
        *numeros = 36;
    }
    else if ((cdnCX >= (4.0 / 8.0) * TAILLX) && (cdnCX <= (5.0 / 8.0) * TAILLX) && (cdnCY >= (4.0 / 8.0) * TAILLY) && (cdnCY <= (5.0 / 8.0) * TAILLY))
    {
        printf("E4\n");
        *typeDePion = colonne5[4];
        *numeros = 37;
    }
    else if ((cdnCX >= (5.0 / 8.0) * TAILLX) && (cdnCX <= (6.0 / 8.0) * TAILLX) && (cdnCY >= (4.0 / 8.0) * TAILLY) && (cdnCY <= (5.0 / 8.0) * TAILLY))
    {
        printf("F4\n");
        *typeDePion = colonne5[5];
        *numeros = 38;
    }
    else if ((cdnCX >= (6.0 / 8.0) * TAILLX) && (cdnCX <= (7.0 / 8.0) * TAILLX) && (cdnCY >= (4.0 / 8.0) * TAILLY) && (cdnCY <= (5.0 / 8.0) * TAILLY))
    {
        printf("G4\n");
        *typeDePion = colonne5[6];
        *numeros = 39;
    }
    else if ((cdnCX >= (7.0 / 8.0) * TAILLX) && (cdnCX <= (8.0 / 8.0) * TAILLX) && (cdnCY >= (4.0 / 8.0) * TAILLY) && (cdnCY <= (5.0 / 8.0) * TAILLY))
    {
        printf("H4\n");
        *typeDePion = colonne5[7];
        *numeros = 40;
    }
    else if ((cdnCX >= (0) * TAILLX) && (cdnCX <= (1.0 / 8.0) * TAILLX) && (cdnCY >= (5.0 / 8.0) * TAILLY) && (cdnCY <= (6.0 / 8.0) * TAILLY))
    {
        printf("A3\n");
        *typeDePion = colonne6[0];
        *numeros = 41;
    }
    else if ((cdnCX >= (1.0 / 8.0) * TAILLX) && (cdnCX <= (2.0 / 8.0) * TAILLX) && (cdnCY >= (5.0 / 8.0) * TAILLY) && (cdnCY <= (6.0 / 8.0) * TAILLY))
    {
        printf("B3\n");
        *typeDePion = colonne6[1];
        *numeros = 42;
    }
    else if ((cdnCX >= (2.0 / 8.0) * TAILLX) && (cdnCX <= (3.0 / 8.0) * TAILLX) && (cdnCY >= (5.0 / 8.0) * TAILLY) && (cdnCY <= (6.0 / 8.0) * TAILLY))
    {
        printf("C3\n");
        *typeDePion = colonne6[2];
        *numeros = 43;
    }
    else if ((cdnCX >= (3.0 / 8.0) * TAILLX) && (cdnCX <= (4.0 / 8.0) * TAILLX) && (cdnCY >= (5.0 / 8.0) * TAILLY) && (cdnCY <= (6.0 / 8.0) * TAILLY))
    {
        printf("D3\n");
        *typeDePion = colonne6[3];
        *numeros = 44;
    }
    else if ((cdnCX >= (4.0 / 8.0) * TAILLX) && (cdnCX <= (5.0 / 8.0) * TAILLX) && (cdnCY >= (5.0 / 8.0) * TAILLY) && (cdnCY <= (6.0 / 8.0) * TAILLY))
    {
        printf("E3\n");
        *typeDePion = colonne6[4];
        *numeros = 45;
    }
    else if ((cdnCX >= (5.0 / 8.0) * TAILLX) && (cdnCX <= (6.0 / 8.0) * TAILLX) && (cdnCY >= (5.0 / 8.0) * TAILLY) && (cdnCY <= (6.0 / 8.0) * TAILLY))
    {
        printf("F3\n");
        *typeDePion = colonne6[5];
        *numeros = 46;
    }
    else if ((cdnCX >= (6.0 / 8.0) * TAILLX) && (cdnCX <= (7.0 / 8.0) * TAILLX) && (cdnCY >= (5.0 / 8.0) * TAILLY) && (cdnCY <= (6.0 / 8.0) * TAILLY))
    {
        printf("G3\n");
        *typeDePion = colonne6[6];
        *numeros = 47;
    }
    else if ((cdnCX >= (7.0 / 8.0) * TAILLX) && (cdnCX <= (8.0 / 8.0) * TAILLX) && (cdnCY >= (5.0 / 8.0) * TAILLY) && (cdnCY <= (6.0 / 8.0) * TAILLY))
    {
        printf("H3\n");
        *typeDePion = colonne6[7];
        *numeros = 48;
    }
    else if ((cdnCX >= (0) * TAILLX) && (cdnCX <= (1.0 / 8.0) * TAILLX) && (cdnCY >= (6.0 / 8.0) * TAILLY) && (cdnCY <= (7.0 / 8.0) * TAILLY))
    {
        printf("A2\n");
        *typeDePion = colonne7[0];
        *numeros = 49;
    }
    else if ((cdnCX >= (1.0 / 8.0) * TAILLX) && (cdnCX <= (2.0 / 8.0) * TAILLX) && (cdnCY >= (6.0 / 8.0) * TAILLY) && (cdnCY <= (7.0 / 8.0) * TAILLY))
    {
        printf("B2\n");
        *typeDePion = colonne7[1];
        *numeros = 50;
    }
    else if ((cdnCX >= (2.0 / 8.0) * TAILLX) && (cdnCX <= (3.0 / 8.0) * TAILLX) && (cdnCY >= (6.0 / 8.0) * TAILLY) && (cdnCY <= (7.0 / 8.0) * TAILLY))
    {
        printf("C2\n");
        *typeDePion = colonne7[2];
        *numeros = 51;
    }
    else if ((cdnCX >= (3.0 / 8.0) * TAILLX) && (cdnCX <= (4.0 / 8.0) * TAILLX) && (cdnCY >= (6.0 / 8.0) * TAILLY) && (cdnCY <= (7.0 / 8.0) * TAILLY))
    {
        printf("D2\n");
        *typeDePion = colonne7[3];
        *numeros = 52;
    }
    else if ((cdnCX >= (4.0 / 8.0) * TAILLX) && (cdnCX <= (5.0 / 8.0) * TAILLX) && (cdnCY >= (6.0 / 8.0) * TAILLY) && (cdnCY <= (7.0 / 8.0) * TAILLY))
    {
        printf("E2\n");
        *typeDePion = colonne7[4];
        *numeros = 53;
    }
    else if ((cdnCX >= (5.0 / 8.0) * TAILLX) && (cdnCX <= (6.0 / 8.0) * TAILLX) && (cdnCY >= (6.0 / 8.0) * TAILLY) && (cdnCY <= (7.0 / 8.0) * TAILLY))
    {
        printf("F2\n");
        *typeDePion = colonne7[5];
        *numeros = 54;
    }
    else if ((cdnCX >= (6.0 / 8.0) * TAILLX) && (cdnCX <= (7.0 / 8.0) * TAILLX) && (cdnCY >= (6.0 / 8.0) * TAILLY) && (cdnCY <= (7.0 / 8.0) * TAILLY))
    {
        printf("G2\n");
        *typeDePion = colonne7[6];
        *numeros = 55;
    }
    else if ((cdnCX >= (7.0 / 8.0) * TAILLX) && (cdnCX <= (8.0 / 8.0) * TAILLX) && (cdnCY >= (6.0 / 8.0) * TAILLY) && (cdnCY <= (7.0 / 8.0) * TAILLY))
    {
        printf("H2\n");
        *typeDePion = colonne7[7];
        *numeros = 56;
    }
    else if ((cdnCX >= (0) * TAILLX) && (cdnCX <= (1.0 / 8.0) * TAILLX) && (cdnCY >= (7.0 / 8.0) * TAILLY) && (cdnCY <= (8.0 / 8.0) * TAILLY))
    {
        printf("A1\n");
        *typeDePion = colonne8[0];
        *numeros = 57;
    }
    else if ((cdnCX >= (1.0 / 8.0) * TAILLX) && (cdnCX <= (2.0 / 8.0) * TAILLX) && (cdnCY >= (7.0 / 8.0) * TAILLY) && (cdnCY <= (8.0 / 8.0) * TAILLY))
    {
        printf("B1\n");
        *typeDePion = colonne8[1];
        *numeros = 58;
    }
    else if ((cdnCX >= (2.0 / 8.0) * TAILLX) && (cdnCX <= (3.0 / 8.0) * TAILLX) && (cdnCY >= (7.0 / 8.0) * TAILLY) && (cdnCY <= (8.0 / 8.0) * TAILLY))
    {
        printf("C1\n");
        *typeDePion = colonne8[2];
        *numeros = 59;
    }
    else if ((cdnCX >= (3.0 / 8.0) * TAILLX) && (cdnCX <= (4.0 / 8.0) * TAILLX) && (cdnCY >= (7.0 / 8.0) * TAILLY) && (cdnCY <= (8.0 / 8.0) * TAILLY))
    {
        printf("D1\n");
        *typeDePion = colonne8[3];
        *numeros = 60;
    }
    else if ((cdnCX >= (4.0 / 8.0) * TAILLX) && (cdnCX <= (5.0 / 8.0) * TAILLX) && (cdnCY >= (7.0 / 8.0) * TAILLY) && (cdnCY <= (8.0 / 8.0) * TAILLY))
    {
        printf("E1\n");
        *typeDePion = colonne8[4];
        *numeros = 61;
    }
    else if ((cdnCX >= (5.0 / 8.0) * TAILLX) && (cdnCX <= (6.0 / 8.0) * TAILLX) && (cdnCY >= (7.0 / 8.0) * TAILLY) && (cdnCY <= (8.0 / 8.0) * TAILLY))
    {
        printf("F1\n");
        *typeDePion = colonne8[5];
        *numeros = 62;
    }
    else if ((cdnCX >= (6.0 / 8.0) * TAILLX) && (cdnCX <= (7.0 / 8.0) * TAILLX) && (cdnCY >= (7.0 / 8.0) * TAILLY) && (cdnCY <= (8.0 / 8.0) * TAILLY))
    {
        printf("G1\n");
        *typeDePion = colonne8[6];
        *numeros = 63;
    }
    else if ((cdnCX >= (7.0 / 8.0) * TAILLX) && (cdnCX <= (8.0 / 8.0) * TAILLX) && (cdnCY >= (7.0 / 8.0) * TAILLY) && (cdnCY <= (8.0 / 8.0) * TAILLY))
    {
        printf("H1\n");
        *typeDePion = colonne8[7];
        *numeros = 64;
    }
    return 0;
}

// permet de faire depalcer les pions en replacant l'endoit initial du pion par rien(0) et en l'envoyant a l'endroit choisi
void deplacement(int colonne1[], int colonne2[], int colonne3[], int colonne4[], int colonne5[], int colonne6[], int colonne7[], int colonne8[], int cdnCX, int cdnCY, int typeDePion, int numero)
{
    if (cdnCX >= 0 && (cdnCX <= (1.0 / 8.0) * TAILLX) && cdnCY >= 0 && (cdnCY <= (1.0 / 8.0) * TAILLY))
    {
        colonne1[0] = typeDePion;
    }
    else if ((cdnCX >= (1.0 / 8.0) * TAILLX) && (cdnCX <= (2.0 / 8.0) * TAILLX) && (cdnCY >= (0) * TAILLY) && (cdnCY <= (1.0 / 8.0) * TAILLY))
    {
        colonne1[1] = typeDePion;
    }
    else if ((cdnCX >= (2.0 / 8.0) * TAILLX) && (cdnCX <= (3.0 / 8.0) * TAILLX) && (cdnCY >= (0) * TAILLY) && (cdnCY <= (1.0 / 8.0) * TAILLY))
    {
        colonne1[2] = typeDePion;
    }
    else if ((cdnCX >= (3.0 / 8.0) * TAILLX) && (cdnCX <= (4.0 / 8.0) * TAILLX) && (cdnCY >= (0) * TAILLY) && (cdnCY <= (1.0 / 8.0) * TAILLY))
    {
        colonne1[3] = typeDePion;
    }
    else if ((cdnCX >= (4.0 / 8.0) * TAILLX) && (cdnCX <= (5.0 / 8.0) * TAILLX) && (cdnCY >= (0) * TAILLY) && (cdnCY <= (1.0 / 8.0) * TAILLY))
    {
        colonne1[4] = typeDePion;
    }
    else if ((cdnCX >= (5.0 / 8.0) * TAILLX) && (cdnCX <= (6.0 / 8.0) * TAILLX) && (cdnCY >= (0) * TAILLY) && (cdnCY <= (1.0 / 8.0) * TAILLY))
    {
        colonne1[5] = typeDePion;
    }
    else if ((cdnCX >= (6.0 / 8.0) * TAILLX) && (cdnCX <= (7.0 / 8.0) * TAILLX) && (cdnCY >= (0) * TAILLY) && (cdnCY <= (1.0 / 8.0) * TAILLY))
    {
        colonne1[6] = typeDePion;
    }
    else if ((cdnCX >= (7.0 / 8.0) * TAILLX) && (cdnCX <= (8.0 / 8.0) * TAILLX) && (cdnCY >= (0) * TAILLY) && (cdnCY <= (1.0 / 8.0) * TAILLY))
    {
        colonne1[7] = typeDePion;
    }
    else if ((cdnCX >= (0) * TAILLX) && (cdnCX <= (1.0 / 8.0) * TAILLX) && (cdnCY >= (1.0 / 8.0) * TAILLY) && (cdnCY <= (2.0 / 8.0) * TAILLY))
    {
        colonne2[0] = typeDePion;
    }
    else if ((cdnCX >= (1.0 / 8.0) * TAILLX) && (cdnCX <= (2.0 / 8.0) * TAILLX) && (cdnCY >= (1.0 / 8.0) * TAILLY) && (cdnCY <= (2.0 / 8.0) * TAILLY))
    {
        colonne2[1] = typeDePion;
    }
    else if ((cdnCX >= (2.0 / 8.0) * TAILLX) && (cdnCX <= (3.0 / 8.0) * TAILLX) && (cdnCY >= (1.0 / 8.0) * TAILLY) && (cdnCY <= (2.0 / 8.0) * TAILLY))
    {
        colonne2[2] = typeDePion;
    }
    else if ((cdnCX >= (3.0 / 8.0) * TAILLX) && (cdnCX <= (4.0 / 8.0) * TAILLX) && (cdnCY >= (1.0 / 8.0) * TAILLY) && (cdnCY <= (2.0 / 8.0) * TAILLY))
    {
        colonne2[3] = typeDePion;
    }
    else if ((cdnCX >= (4.0 / 8.0) * TAILLX) && (cdnCX <= (5.0 / 8.0) * TAILLX) && (cdnCY >= (1.0 / 8.0) * TAILLY) && (cdnCY <= (2.0 / 8.0) * TAILLY))
    {
        colonne2[4] = typeDePion;
    }
    else if ((cdnCX >= (5.0 / 8.0) * TAILLX) && (cdnCX <= (6.0 / 8.0) * TAILLX) && (cdnCY >= (1.0 / 8.0) * TAILLY) && (cdnCY <= (2.0 / 8.0) * TAILLY))
    {
        colonne2[5] = typeDePion;
    }
    else if ((cdnCX >= (6.0 / 8.0) * TAILLX) && (cdnCX <= (7.0 / 8.0) * TAILLX) && (cdnCY >= (1.0 / 8.0) * TAILLY) && (cdnCY <= (2.0 / 8.0) * TAILLY))
    {
        colonne2[6] = typeDePion;
    }
    else if ((cdnCX >= (7.0 / 8.0) * TAILLX) && (cdnCX <= (8.0 / 8.0) * TAILLX) && (cdnCY >= (1.0 / 8.0) * TAILLY) && (cdnCY <= (2.0 / 8.0) * TAILLY))
    {
        colonne2[7] = typeDePion;
    }
    else if ((cdnCX >= (0) * TAILLX) && (cdnCX <= (1.0 / 8.0) * TAILLX) && (cdnCY >= (2.0 / 8.0) * TAILLY) && (cdnCY <= (3.0 / 8.0) * TAILLY))
    {
        colonne3[0] = typeDePion;
    }
    else if ((cdnCX >= (1.0 / 8.0) * TAILLX) && (cdnCX <= (2.0 / 8.0) * TAILLX) && (cdnCY >= (2.0 / 8.0) * TAILLY) && (cdnCY <= (3.0 / 8.0) * TAILLY))
    {
        colonne3[1] = typeDePion;
    }
    else if ((cdnCX >= (2.0 / 8.0) * TAILLX) && (cdnCX <= (3.0 / 8.0) * TAILLX) && (cdnCY >= (2.0 / 8.0) * TAILLY) && (cdnCY <= (3.0 / 8.0) * TAILLY))
    {
        colonne3[2] = typeDePion;
    }
    else if ((cdnCX >= (3.0 / 8.0) * TAILLX) && (cdnCX <= (4.0 / 8.0) * TAILLX) && (cdnCY >= (2.0 / 8.0) * TAILLY) && (cdnCY <= (3.0 / 8.0) * TAILLY))
    {
        colonne3[3] = typeDePion;
    }
    else if ((cdnCX >= (4.0 / 8.0) * TAILLX) && (cdnCX <= (5.0 / 8.0) * TAILLX) && (cdnCY >= (2.0 / 8.0) * TAILLY) && (cdnCY <= (3.0 / 8.0) * TAILLY))
    {
        colonne3[4] = typeDePion;
    }
    else if ((cdnCX >= (5.0 / 8.0) * TAILLX) && (cdnCX <= (6.0 / 8.0) * TAILLX) && (cdnCY >= (2.0 / 8.0) * TAILLY) && (cdnCY <= (3.0 / 8.0) * TAILLY))
    {
        colonne3[5] = typeDePion;
    }
    else if ((cdnCX >= (6.0 / 8.0) * TAILLX) && (cdnCX <= (7.0 / 8.0) * TAILLX) && (cdnCY >= (2.0 / 8.0) * TAILLY) && (cdnCY <= (3.0 / 8.0) * TAILLY))
    {
        colonne3[6] = typeDePion;
    }
    else if ((cdnCX >= (7.0 / 8.0) * TAILLX) && (cdnCX <= (8.0 / 8.0) * TAILLX) && (cdnCY >= (2.0 / 8.0) * TAILLY) && (cdnCY <= (3.0 / 8.0) * TAILLY))
    {
        colonne3[7] = typeDePion;
    }
    else if ((cdnCX >= (0) * TAILLX) && (cdnCX <= (1.0 / 8.0) * TAILLX) && (cdnCY >= (3.0 / 8.0) * TAILLY) && (cdnCY <= (4.0 / 8.0) * TAILLY))
    {
        colonne4[0] = typeDePion;
    }
    else if ((cdnCX >= (1.0 / 8.0) * TAILLX) && (cdnCX <= (2.0 / 8.0) * TAILLX) && (cdnCY >= (3.0 / 8.0) * TAILLY) && (cdnCY <= (4.0 / 8.0) * TAILLY))
    {
        colonne4[1] = typeDePion;
    }
    else if ((cdnCX >= (2.0 / 8.0) * TAILLX) && (cdnCX <= (3.0 / 8.0) * TAILLX) && (cdnCY >= (3.0 / 8.0) * TAILLY) && (cdnCY <= (4.0 / 8.0) * TAILLY))
    {
        colonne4[2] = typeDePion;
    }
    else if ((cdnCX >= (3.0 / 8.0) * TAILLX) && (cdnCX <= (4.0 / 8.0) * TAILLX) && (cdnCY >= (3.0 / 8.0) * TAILLY) && (cdnCY <= (4.0 / 8.0) * TAILLY))
    {
        colonne4[3] = typeDePion;
    }
    else if ((cdnCX >= (4.0 / 8.0) * TAILLX) && (cdnCX <= (5.0 / 8.0) * TAILLX) && (cdnCY >= (3.0 / 8.0) * TAILLY) && (cdnCY <= (4.0 / 8.0) * TAILLY))
    {
        colonne4[4] = typeDePion;
    }
    else if ((cdnCX >= (5.0 / 8.0) * TAILLX) && (cdnCX <= (6.0 / 8.0) * TAILLX) && (cdnCY >= (3.0 / 8.0) * TAILLY) && (cdnCY <= (4.0 / 8.0) * TAILLY))
    {
        colonne4[5] = typeDePion;
    }
    else if ((cdnCX >= (6.0 / 8.0) * TAILLX) && (cdnCX <= (7.0 / 8.0) * TAILLX) && (cdnCY >= (3.0 / 8.0) * TAILLY) && (cdnCY <= (4.0 / 8.0) * TAILLY))
    {
        colonne4[6] = typeDePion;
    }
    else if ((cdnCX >= (7.0 / 8.0) * TAILLX) && (cdnCX <= (8.0 / 8.0) * TAILLX) && (cdnCY >= (3.0 / 8.0) * TAILLY) && (cdnCY <= (4.0 / 8.0) * TAILLY))
    {
        colonne4[7] = typeDePion;
    }
    else if ((cdnCX >= (0) * TAILLX) && (cdnCX <= (1.0 / 8.0) * TAILLX) && (cdnCY >= (4.0 / 8.0) * TAILLY) && (cdnCY <= (5.0 / 8.0) * TAILLY))
    {
        colonne5[0] = typeDePion;
    }
    else if ((cdnCX >= (1.0 / 8.0) * TAILLX) && (cdnCX <= (2.0 / 8.0) * TAILLX) && (cdnCY >= (4.0 / 8.0) * TAILLY) && (cdnCY <= (5.0 / 8.0) * TAILLY))
    {
        colonne5[1] = typeDePion;
    }
    else if ((cdnCX >= (2.0 / 8.0) * TAILLX) && (cdnCX <= (3.0 / 8.0) * TAILLX) && (cdnCY >= (4.0 / 8.0) * TAILLY) && (cdnCY <= (5.0 / 8.0) * TAILLY))
    {
        colonne5[2] = typeDePion;
    }
    else if ((cdnCX >= (3.0 / 8.0) * TAILLX) && (cdnCX <= (4.0 / 8.0) * TAILLX) && (cdnCY >= (4.0 / 8.0) * TAILLY) && (cdnCY <= (5.0 / 8.0) * TAILLY))
    {
        colonne5[3] = typeDePion;
    }
    else if ((cdnCX >= (4.0 / 8.0) * TAILLX) && (cdnCX <= (5.0 / 8.0) * TAILLX) && (cdnCY >= (4.0 / 8.0) * TAILLY) && (cdnCY <= (5.0 / 8.0) * TAILLY))
    {
        colonne5[4] = typeDePion;
    }
    else if ((cdnCX >= (5.0 / 8.0) * TAILLX) && (cdnCX <= (6.0 / 8.0) * TAILLX) && (cdnCY >= (4.0 / 8.0) * TAILLY) && (cdnCY <= (5.0 / 8.0) * TAILLY))
    {
        colonne5[5] = typeDePion;
    }
    else if ((cdnCX >= (6.0 / 8.0) * TAILLX) && (cdnCX <= (7.0 / 8.0) * TAILLX) && (cdnCY >= (4.0 / 8.0) * TAILLY) && (cdnCY <= (5.0 / 8.0) * TAILLY))
    {
        colonne5[6] = typeDePion;
    }
    else if ((cdnCX >= (7.0 / 8.0) * TAILLX) && (cdnCX <= (8.0 / 8.0) * TAILLX) && (cdnCY >= (4.0 / 8.0) * TAILLY) && (cdnCY <= (5.0 / 8.0) * TAILLY))
    {
        colonne5[7] = typeDePion;
    }
    else if ((cdnCX >= (0) * TAILLX) && (cdnCX <= (1.0 / 8.0) * TAILLX) && (cdnCY >= (5.0 / 8.0) * TAILLY) && (cdnCY <= (6.0 / 8.0) * TAILLY))
    {
        colonne6[0] = typeDePion;
    }
    else if ((cdnCX >= (1.0 / 8.0) * TAILLX) && (cdnCX <= (2.0 / 8.0) * TAILLX) && (cdnCY >= (5.0 / 8.0) * TAILLY) && (cdnCY <= (6.0 / 8.0) * TAILLY))
    {
        colonne6[1] = typeDePion;
    }
    else if ((cdnCX >= (2.0 / 8.0) * TAILLX) && (cdnCX <= (3.0 / 8.0) * TAILLX) && (cdnCY >= (5.0 / 8.0) * TAILLY) && (cdnCY <= (6.0 / 8.0) * TAILLY))
    {
        colonne6[2] = typeDePion;
    }
    else if ((cdnCX >= (3.0 / 8.0) * TAILLX) && (cdnCX <= (4.0 / 8.0) * TAILLX) && (cdnCY >= (5.0 / 8.0) * TAILLY) && (cdnCY <= (6.0 / 8.0) * TAILLY))
    {
        colonne6[3] = typeDePion;
    }
    else if ((cdnCX >= (4.0 / 8.0) * TAILLX) && (cdnCX <= (5.0 / 8.0) * TAILLX) && (cdnCY >= (5.0 / 8.0) * TAILLY) && (cdnCY <= (6.0 / 8.0) * TAILLY))
    {
        colonne6[4] = typeDePion;
    }
    else if ((cdnCX >= (5.0 / 8.0) * TAILLX) && (cdnCX <= (6.0 / 8.0) * TAILLX) && (cdnCY >= (5.0 / 8.0) * TAILLY) && (cdnCY <= (6.0 / 8.0) * TAILLY))
    {
        colonne6[5] = typeDePion;
    }
    else if ((cdnCX >= (6.0 / 8.0) * TAILLX) && (cdnCX <= (7.0 / 8.0) * TAILLX) && (cdnCY >= (5.0 / 8.0) * TAILLY) && (cdnCY <= (6.0 / 8.0) * TAILLY))
    {
        colonne6[6] = typeDePion;
    }
    else if ((cdnCX >= (7.0 / 8.0) * TAILLX) && (cdnCX <= (8.0 / 8.0) * TAILLX) && (cdnCY >= (5.0 / 8.0) * TAILLY) && (cdnCY <= (6.0 / 8.0) * TAILLY))
    {
        colonne6[7] = typeDePion;
    }
    else if ((cdnCX >= (0) * TAILLX) && (cdnCX <= (1.0 / 8.0) * TAILLX) && (cdnCY >= (6.0 / 8.0) * TAILLY) && (cdnCY <= (7.0 / 8.0) * TAILLY))
    {
        colonne7[0] = typeDePion;
    }
    else if ((cdnCX >= (1.0 / 8.0) * TAILLX) && (cdnCX <= (2.0 / 8.0) * TAILLX) && (cdnCY >= (6.0 / 8.0) * TAILLY) && (cdnCY <= (7.0 / 8.0) * TAILLY))
    {
        colonne7[1] = typeDePion;
    }
    else if ((cdnCX >= (2.0 / 8.0) * TAILLX) && (cdnCX <= (3.0 / 8.0) * TAILLX) && (cdnCY >= (6.0 / 8.0) * TAILLY) && (cdnCY <= (7.0 / 8.0) * TAILLY))
    {
        colonne7[2] = typeDePion;
    }
    else if ((cdnCX >= (3.0 / 8.0) * TAILLX) && (cdnCX <= (4.0 / 8.0) * TAILLX) && (cdnCY >= (6.0 / 8.0) * TAILLY) && (cdnCY <= (7.0 / 8.0) * TAILLY))
    {
        colonne7[3] = typeDePion;
    }
    else if ((cdnCX >= (4.0 / 8.0) * TAILLX) && (cdnCX <= (5.0 / 8.0) * TAILLX) && (cdnCY >= (6.0 / 8.0) * TAILLY) && (cdnCY <= (7.0 / 8.0) * TAILLY))
    {
        colonne7[4] = typeDePion;
    }
    else if ((cdnCX >= (5.0 / 8.0) * TAILLX) && (cdnCX <= (6.0 / 8.0) * TAILLX) && (cdnCY >= (6.0 / 8.0) * TAILLY) && (cdnCY <= (7.0 / 8.0) * TAILLY))
    {
        colonne7[5] = typeDePion;
    }
    else if ((cdnCX >= (6.0 / 8.0) * TAILLX) && (cdnCX <= (7.0 / 8.0) * TAILLX) && (cdnCY >= (6.0 / 8.0) * TAILLY) && (cdnCY <= (7.0 / 8.0) * TAILLY))
    {
        colonne7[6] = typeDePion;
    }
    else if ((cdnCX >= (7.0 / 8.0) * TAILLX) && (cdnCX <= (8.0 / 8.0) * TAILLX) && (cdnCY >= (6.0 / 8.0) * TAILLY) && (cdnCY <= (7.0 / 8.0) * TAILLY))
    {
        colonne7[7] = typeDePion;
    }
    else if ((cdnCX >= (0) * TAILLX) && (cdnCX <= (1.0 / 8.0) * TAILLX) && (cdnCY >= (7.0 / 8.0) * TAILLY) && (cdnCY <= (8.0 / 8.0) * TAILLY))
    {
        colonne8[0] = typeDePion;
    }
    else if ((cdnCX >= (1.0 / 8.0) * TAILLX) && (cdnCX <= (2.0 / 8.0) * TAILLX) && (cdnCY >= (7.0 / 8.0) * TAILLY) && (cdnCY <= (8.0 / 8.0) * TAILLY))
    {
        colonne8[1] = typeDePion;
    }
    else if ((cdnCX >= (2.0 / 8.0) * TAILLX) && (cdnCX <= (3.0 / 8.0) * TAILLX) && (cdnCY >= (7.0 / 8.0) * TAILLY) && (cdnCY <= (8.0 / 8.0) * TAILLY))
    {
        colonne8[2] = typeDePion;
    }
    else if ((cdnCX >= (3.0 / 8.0) * TAILLX) && (cdnCX <= (4.0 / 8.0) * TAILLX) && (cdnCY >= (7.0 / 8.0) * TAILLY) && (cdnCY <= (8.0 / 8.0) * TAILLY))
    {
        colonne8[3] = typeDePion;
    }
    else if ((cdnCX >= (4.0 / 8.0) * TAILLX) && (cdnCX <= (5.0 / 8.0) * TAILLX) && (cdnCY >= (7.0 / 8.0) * TAILLY) && (cdnCY <= (8.0 / 8.0) * TAILLY))
    {
        colonne8[4] = typeDePion;
    }
    else if ((cdnCX >= (5.0 / 8.0) * TAILLX) && (cdnCX <= (6.0 / 8.0) * TAILLX) && (cdnCY >= (7.0 / 8.0) * TAILLY) && (cdnCY <= (8.0 / 8.0) * TAILLY))
    {
        colonne8[5] = typeDePion;
    }
    else if ((cdnCX >= (6.0 / 8.0) * TAILLX) && (cdnCX <= (7.0 / 8.0) * TAILLX) && (cdnCY >= (7.0 / 8.0) * TAILLY) && (cdnCY <= (8.0 / 8.0) * TAILLY))
    {
        colonne8[6] = typeDePion;
    }
    else if ((cdnCX >= (7.0 / 8.0) * TAILLX) && (cdnCX <= (8.0 / 8.0) * TAILLX) && (cdnCY >= (7.0 / 8.0) * TAILLY) && (cdnCY <= (8.0 / 8.0) * TAILLY))
    {
        colonne8[7] = typeDePion;
    }
    switch (numero) // permet de faire disparaitre le pion depalcer
    {
    case 1:
        colonne1[0] = 0;
        break;
    case 2:
        colonne1[1] = 0;
        break;
    case 3:
        colonne1[2] = 0;
        break;
    case 4:
        colonne1[3] = 0;
        break;
    case 5:
        colonne1[4] = 0;
        break;
    case 6:
        colonne1[5] = 0;
        break;
    case 7:
        colonne1[6] = 0;
        break;
    case 8:
        colonne1[7] = 0;
        break;
    case 9:
        colonne2[0] = 0;
        break;
    case 10:
        colonne2[1] = 0;
        break;
    case 11:
        colonne2[2] = 0;
        break;
    case 12:
        colonne2[3] = 0;
        break;
    case 13:
        colonne2[4] = 0;
        break;
    case 14:
        colonne2[5] = 0;
        break;
    case 15:
        colonne2[6] = 0;
        break;
    case 16:
        colonne2[7] = 0;
        break;
    case 17:
        colonne3[0] = 0;
        break;
    case 18:
        colonne3[1] = 0;
        break;
    case 19:
        colonne3[2] = 0;
        break;
    case 20:
        colonne3[3] = 0;
        break;
    case 21:
        colonne3[4] = 0;
        break;
    case 22:
        colonne3[5] = 0;
        break;
    case 23:
        colonne3[6] = 0;
        break;
    case 24:
        colonne3[7] = 0;
        break;
    case 25:
        colonne4[0] = 0;
        break;
    case 26:
        colonne4[1] = 0;
        break;
    case 27:
        colonne4[2] = 0;
        break;
    case 28:
        colonne4[3] = 0;
        break;
    case 29:
        colonne4[4] = 0;
        break;
    case 30:
        colonne4[5] = 0;
        break;
    case 31:
        colonne4[6] = 0;
        break;
    case 32:
        colonne4[7] = 0;
        break;
    case 33:
        colonne5[0] = 0;
        break;
    case 34:
        colonne5[1] = 0;
        break;
    case 35:
        colonne5[2] = 0;
        break;
    case 36:
        colonne5[3] = 0;
        break;
    case 37:
        colonne5[4] = 0;
        break;
    case 38:
        colonne5[5] = 0;
        break;
    case 39:
        colonne5[6] = 0;
        break;
    case 40:
        colonne5[7] = 0;
        break;
    case 41:
        colonne6[0] = 0;
        break;
    case 42:
        colonne6[1] = 0;
        break;
    case 43:
        colonne6[2] = 0;
        break;
    case 44:
        colonne6[3] = 0;
        break;
    case 45:
        colonne6[4] = 0;
        break;
    case 46:
        colonne6[5] = 0;
        break;
    case 47:
        colonne6[6] = 0;
        break;
    case 48:
        colonne6[7] = 0;
        break;
    case 49:
        colonne7[0] = 0;
        break;
    case 50:
        colonne7[1] = 0;
        break;
    case 51:
        colonne7[2] = 0;
        break;
    case 52:
        colonne7[3] = 0;
        break;
    case 53:
        colonne7[4] = 0;
        break;
    case 54:
        colonne7[5] = 0;
        break;
    case 55:
        colonne7[6] = 0;
        break;
    case 56:
        colonne7[7] = 0;
        break;
    case 57:
        colonne8[0] = 0;
        break;
    case 58:
        colonne8[1] = 0;
        break;
    case 59:
        colonne8[2] = 0;
        break;
    case 60:
        colonne8[3] = 0;
        break;
    case 61:
        colonne8[4] = 0;
        break;
    case 62:
        colonne8[5] = 0;
        break;
    case 63:
        colonne8[6] = 0;
        break;
    case 64:
        colonne8[7] = 0;
        break;

    default:
        break;
    }
}

// permet de refaire afficher le plato pour reinitialiser l'image
void afficherPlato(SDL_Renderer *renderer)
{
    SDL_Rect rectangle;
    int color = 0;
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
}

// dest_rect.x = 0;
//                 dest_rect.y = 0;
//                 picture = SDL_LoadBMP("src/damier.bmp");
//                 if (picture == NULL)
//                     SDL_ExitWithError("inmpossible de cree la fenetre et le rendu");
//                 texture = SDL_CreateTextureFromSurface(renderer, picture);
//                 SDL_FreeSurface(picture);
//                 if (texture == NULL)
//                     SDL_ExitWithError("inmpossible de cree la fenetre et le rendu3");
//                 if (SDL_QueryTexture(texture, NULL, NULL, &dest_rect.w, &dest_rect.h) != 0)
//                     SDL_ExitWithError("inmpossible de cree la fenetre et le rendu2");
//                 if (SDL_RenderCopy(renderer, texture, NULL, &dest_rect) != 0)
//                     SDL_ExitWithError("inmpossible de cree la fenetre et le rendu1");