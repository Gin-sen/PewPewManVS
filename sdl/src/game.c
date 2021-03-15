#include "../include/game.h"

stGame *game_create(){
    srand(time(NULL));

    stGame *game = NULL;
    game = malloc(sizeof(stGame));
    game->screenSize.x = SCREEN_WIDTH;
    game->screenSize.y = SCREEN_HEIGHT;
    game->pWindow = NULL;
    game->pRenderer = NULL;

    // Textures
        // Menu
    game->pTextBackground = NULL;
    game->pTextCursor = NULL;
    for (int i = 0; i < 4; i++){
        game->pTextMenu[i] = NULL;
        game->pTextMenuBold[i] = NULL;
    }
        //Game

    // Font
    game->pFont = NULL;

    // variables de logique
    game->should_run = TRUE;
    game->game_mode = 0;
    game->actionKeyPressed = FALSE;

    game->input_delay = 100;

    return game;
}


// Init SDL et jeu
stGame *game_init(){
    stGame *game = game_create();

    // cumuler avec flag audio si implémenté
    if (SDL_Init(SDL_INIT_VIDEO)){
        printf("Echec de l'initialisationde la SDL : %s\n", SDL_GetError());
        game_destroy(game);
        return NULL;
    }

    if (TTF_Init() != 0){
        printf("Echec de l'initialisation de la SDL_TTF (%s)\n", TTF_GetError());
        game_destroy(game);
        return NULL;
    }

    // Chargement de la police
    game->pFont = TTF_OpenFont("./res/arial.ttf", 25);
    if (!game->pFont){
        printf("Echec de chargement de la police : %s\n", TTF_GetError());
        game_destroy(game);
        return NULL;
    }


    game->pWindow = SDL_CreateWindow("BombermanVS", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        game->screenSize.x, game->screenSize.y, SDL_WINDOW_SHOWN);

    // si création d'une window, on crée un renderer
    if (game->pWindow){
        printf("mem Leak here \n"); //    mem leak
        //game->pRenderer = SDL_CreateRenderer(game->pWindow, -1, SDL_RENDERER_ACCELERATED);
        game->pRenderer = SDL_CreateRenderer(game->pWindow, -1, SDL_RENDERER_SOFTWARE);
        //si renderer échoue => erreur
        if (!game->pRenderer){
            printf("Impossible d'initialiser le renderer SDL : %s\n", SDL_GetError());
            game_destroy(game);
            return NULL;
        }
    } else {
        // si window échoue
        printf("Impossible d'initialiser une fenetre : %s\n", SDL_GetError());
        game_destroy(game);
        return NULL;
    }
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) //Initialisation de l'API Mixer
    {
        printf("%s", Mix_GetError());
    }

    return game;
}

// Détruit le jeu puis la sdl
void game_destroy(stGame *game){
    if (game){
        if (game->pTextBackground) {
            SDL_DestroyTexture(game->pTextBackground);
        }
        if (game->pTextCursor) {
            SDL_DestroyTexture(game->pTextCursor);
        }
        if (game->pTextMenu) {
            for (int i = 0; i < 4; i++){
                SDL_DestroyTexture(game->pTextMenu[i]);
            }
        }
        if (game->pTextMenuBold) {
            for (int i = 0; i < 4; i++){
                SDL_DestroyTexture(game->pTextMenuBold[i]);
            }
        }
        if (game->pFont) {
            TTF_CloseFont(game->pFont);
        }
        if (game->pRenderer) {
            SDL_DestroyRenderer(game->pRenderer);
        }
        if (game->pWindow) {
            SDL_DestroyWindow(game->pWindow);
        }
        // if (game->x){
        //     free(game->x);
        // }
        free(game);
    }
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
}