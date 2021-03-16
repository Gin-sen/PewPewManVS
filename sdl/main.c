#include "./include/myheader.h"

// bomb *bomble_handled(SDL_Renderer *renderer,bomb *bombs,character *character) 
// {
// 	bomb *newBomb;
//     newBomb = malloc(sizeof(*newBomb));
//     SDL_Surface *bomb_img  = IMG_Load("./res/bombe.png");
//     newBomb->image  = SDL_CreateTextureFromSurface(renderer, bomb_img);
//     SDL_Rect bomb_rect;
//     bomb_rect.w = 32;
//     bomb_rect.h = 32;
//     bomb_rect.x = character->character_rect.x;
//     bomb_rect.y = character->character_rect.y;
//     newBomb->bomb_rect = bomb_rect;
//     newBomb->force = 1;
//     newBomb->next = bombs;
//     bombs = newBomb;
//     return bombs;
// }

// character *create_character(char *name,char *image,SDL_Renderer *renderer) 
// {
// 	character *character;
//     character = malloc(sizeof(*character));
//     SDL_Surface *character_img  = IMG_Load(image);
//     character->image  = SDL_CreateTextureFromSurface(renderer, character_img);
//     SDL_Rect character_rect;
//     character_rect.w = 32;
//     character_rect.h = 32;
//     character_rect.x = 0;
//     character_rect.y = 0;
//     character->character_rect = character_rect;
//     character->name = name;
//     return character;
// }

// int display(SDL_Renderer *renderer,SDL_Texture *texture,SDL_Rect rect) 
// {
//     SDL_RenderCopy(renderer, texture,NULL, &rect);
//     return 0;
// }

// int display_bombs(bomb *bombs,SDL_Renderer *renderer) 
// {
// 	if (bombs != NULL)
//     {
//         while (bombs != 0)
//         {
//             display(renderer,bombs->image,bombs->bomb_rect);
//             bombs = bombs->next;
//         }
//     }
//     return (0);
// }

// int main(void)
// {
//     /*Background*/
//     SDL_Window *window;
//     SDL_Rect background;
//     background.w = SCREEN_WIDTH;
//     background.h = SCREEN_HEIGHT;
//     background.x = 0;
//     background.y = 0;



//     SDL_Surface *background_image = IMG_Load("./res/ground.png");
//     SDL_Init(SDL_INIT_VIDEO);
//     window = SDL_CreateWindow("Bomberman", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT, SDL_WINDOW_OPENGL );
//     SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
//     SDL_Texture *textu_background = SDL_CreateTextureFromSurface(renderer, background_image);
    


//     /*PERSONNAGE*/
//     character *character;
//     character = create_character("player 1","./res/perso.png",renderer);
//     bomb *bombs;

//     if (window == NULL) {
//         printf("Could not create window: %s\n", SDL_GetError());
//         return 1;
//     }
//     SDL_bool program_launched = SDL_TRUE;
//     while(program_launched) {
//         display(renderer,textu_background,background);
//         display(renderer,character->image,character->character_rect);
//         display_bombs(bombs,renderer);
//         SDL_Event event;
//         while (SDL_PollEvent(&event))
//         {
//             switch (event.type) {
//                 case SDL_QUIT:
//                     program_launched = SDL_FALSE;
//                     break;
//                 default:
//                     break;

//                 case SDL_KEYDOWN:
//                     switch (event.key.keysym.sym) {
//                         case SDLK_ESCAPE:
//                             program_launched = SDL_FALSE;
//                             break;
//                         case SDLK_RIGHT:
// 	                        if (character->character_rect.x + 5 + 32 <= SCREEN_WIDTH)
//                             	character->character_rect.x = character->character_rect.x + 5;
//                             break;
//                         case SDLK_LEFT:
//                         	if (character->character_rect.x - 5  >= 0)
//                             	character->character_rect.x = character->character_rect.x - 5;
//                             break;

//                         case SDLK_UP:
//                         	if (character->character_rect.y - 5  >= 0)
//                             	character->character_rect.y = character->character_rect.y - 5;
//                             break;

//                         case SDLK_DOWN:
//                         	if (character->character_rect.y + 5 + 32  <= SCREEN_HEIGHT)
//                             	character->character_rect.y = character->character_rect.y + 5;
//                             break;
//                         case SDLK_SPACE:
//                             bombs = bomble_handled(renderer,bombs,character);
//                             break;
//                         default:
//                             break;
//                     }
//                     break;
//             }
//         }
//         SDL_RenderPresent(renderer);
//     }

// 	if (bombs != NULL)
//     {
//         while (bombs != 0)
//         {
//             SDL_DestroyTexture(bombs->image);
//             bombs = bombs->next;
//         }
//     }
//     free(bombs);
//     if (NULL != renderer) {
//         SDL_DestroyRenderer(renderer);
//     }
//     if (NULL != textu_background) {
//         SDL_DestroyTexture(textu_background);
//     }
//     SDL_DestroyTexture(character->image);
//     free(character);
//     SDL_DestroyWindow(window);
//     SDL_Quit();
//     return 0;
// }


int main(void){
    stGame *game = game_init();
    if (!game){
        game_destroy(game);
        return (1);
    }
    printf("Game init OK\n");

    while(game->should_run){
        menu(game);

        switch (game->game_mode){
            case 0:
            //      Mode Solo
                printf("Solo game start ! (no server)\n");
                //game_loop_solo(game);
                break;
            case 1:
            //      Mode Multi (crée une room)
                printf("Reaching for server ...\n");
                printf("Server reached.\n");
                printf("Lobby created ! \n");
                printf("Waiting for other players ...\n");
                break;
            case 2:
            //      Mode Multi (join une room)
                printf("Reaching for server ...\n");
                printf("Server reached.\n");
                printf("Joining Lobby ! \n");
                printf("Waiting for other players ...\n");
                break;
            case 3:
            //      Mode Debug (pour tester des trucs)
                printf("Debug mode :\n");
                break;
            default:
            // Escape or error
                printf("Exiting gracefully\n");
                game->should_run = FALSE;
                break;            
        }
        //pthread_create game_loop + pthread connection handler

    }

    game_destroy(game);

    return (EXIT_SUCCESS);
}