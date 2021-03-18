#include "../include/menu.h"


void update_menu_events(stGame *game, SDL_Event event, Mix_Chunk **bip)
{
    while (SDL_PollEvent(&event)){
        switch (event.type) {
            case SDL_QUIT:
                game->should_run = FALSE;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode) {
                    case SDL_SCANCODE_UP:
                    case SDL_SCANCODE_Z:
                    case SDL_SCANCODE_LEFT:
                    case SDL_SCANCODE_Q:
                        if (game->game_mode > 0) {
                            game->game_mode = game->game_mode - 1;
                            Mix_PlayChannel(-1, bip[0], 0);
                        }
                        break;
                    case SDL_SCANCODE_DOWN:
                    case SDL_SCANCODE_S:
                    case SDL_SCANCODE_RIGHT:
                    case SDL_SCANCODE_D:
                        if (game->game_mode < 3) {
                            game->game_mode = game->game_mode + 1;
                            Mix_PlayChannel(-1, bip[0], 0);
                        }
                        break;
                    case SDL_SCANCODE_ESCAPE:
                        game->should_run = FALSE;
                        break;
                    case SDL_SCANCODE_RETURN:
                    case SDL_SCANCODE_SPACE:
                        game->actionKeyPressed = TRUE;
                        break;
                    case SDL_SCANCODE_UNKNOWN:
                        printf("Scancode unknown %s\n", SDL_GetError());
                        game->should_run = FALSE;
                        break;
                    default:
                        break;
                }
                break;
        }
    }
}

BOOL generate_text_texture(stGame *game, SDL_Color text_color, BOOL is_bold){
    if(is_bold)
        TTF_SetFontStyle(game->pFont, TTF_STYLE_BOLD);

    for (int i = 0; i < 4; i++){
        SDL_Surface *temp = TTF_RenderText_Solid(game->pFont, game->text_menu[i], text_color);
        if (!temp){
            printf("Echec d'initialisation de surface : %s\n", SDL_GetError());
            SDL_FreeSurface(temp);
            return FALSE;
        }
        game->pTextMenuBold[i] = SDL_CreateTextureFromSurface(game->pRenderer, temp);
        SDL_FreeSurface(temp);
    }
    if(is_bold)
        TTF_SetFontStyle(game->pFont, TTF_STYLE_NORMAL);
    return TRUE;
}

BOOL menu_init(stGame *game){
    // Nettoie l'écran
    SDL_SetRenderDrawColor(game->pRenderer, 0, 0, 0, 255);
    SDL_RenderClear(game->pRenderer);

    // Load les images Background et curseur
    SDL_Surface *surface_background = IMG_Load("./res/background_menu.jpg");
    if (!surface_background){
        printf("Impossible de charger l'image : %s\n", SDL_GetError());
        return FALSE;
    } else {
        game->pTextBackground = SDL_CreateTextureFromSurface(game->pRenderer, surface_background);
        if (!game->pTextBackground){
            printf("Could not Create Texture from Surface : %s", SDL_GetError());
            SDL_FreeSurface(surface_background);
            return FALSE;
        }
        SDL_FreeSurface(surface_background);
    }
    SDL_Surface *surface_cursor = IMG_Load("./res/perso.png");
    if (!surface_cursor){
        printf("Impossible de charger l'image : %s\n", SDL_GetError());
        return FALSE;
    } else {
        game->pTextCursor = SDL_CreateTextureFromSurface(game->pRenderer, surface_cursor);
        if (!game->pTextCursor){
            printf("Could not Create Texture from Surface : %s", SDL_GetError());
            SDL_FreeSurface(surface_background);
            SDL_FreeSurface(surface_cursor);
            return FALSE;
        }
        SDL_FreeSurface(surface_cursor);
    }
        // Set Text
    SDL_Color text_color = {255, 255, 255, 255};
    game->text_menu[0] = "Story mode";
    game->text_menu[1] = "Create Mulplayer Lobby";
    game->text_menu[2] = "Join Mulplayer Lobby";
    game->text_menu[3] = "Debug Mode";
    // Create Normal Text Texture 
    if (!generate_text_texture(game, text_color, FALSE)){
        printf("Could not load normal text texture\n");
        return FALSE;
    }

    // Create Bold Text Texture 
    if (!generate_text_texture(game, text_color, TRUE)){
        printf("Could not load bold text texture\n");
        return FALSE;
    }
    printf("Menu init OK\n");
    return TRUE;
}

void draw_menu(stGame *game){
    SDL_Rect textBox[4];
    SDL_Rect cursorBox;
    SDL_Rect backBox;
    backBox.x = 0;
    backBox.y = 0;
    backBox.w = SCREEN_WIDTH; 
    backBox.h = SCREEN_HEIGHT;

    // Nettoie l'écran
    SDL_RenderClear(game->pRenderer);

    // Ajoute le background
    SDL_RenderCopy(game->pRenderer, game->pTextBackground, NULL, &backBox);
    // Ajoute le texte et le curseur
    for (int i = 0; i < 4; i++){
        textBox[i].x = (SCREEN_WIDTH/2) - 160;
        textBox[i].y = (SCREEN_HEIGHT/2) + 32 *(0 + (2 * i)); // remplacer 0 par le padding final
        textBox[i].w = 288; 
        textBox[i].h = 32;
        
        if(i == game->game_mode){
            cursorBox.x = (SCREEN_WIDTH/2) - 192;
            cursorBox.y = (SCREEN_HEIGHT/2) + 32 *(0 + (2 * i)); // remplacer 0 par le padding final
            cursorBox.w = 32; 
            cursorBox.h = 32;
            SDL_RenderCopy(game->pRenderer, game->pTextMenuBold[i], NULL, &textBox[i]);
            SDL_RenderCopy(game->pRenderer, game->pTextCursor, NULL, &cursorBox);    
        } else {
            SDL_RenderCopy(game->pRenderer, game->pTextMenu[i], NULL, &textBox[i]);
        }
    }
    // Affiche le tout
    SDL_RenderPresent(game->pRenderer);
}

char *random_menu_music(){
    if((rand()*100) > 50){ // NOLINT(cert-msc50-cpp)
        return "./res/musics/pokemon-tcg-ost-club-master-battle-2020-stereo-mix.mp3";
    } else {
        return "./res/musics/pokemon-tcg-ost-water-rock-psychic-clubs-2020-stereo-mix.mp3";
    }
}

void menu(stGame *game){
    unsigned int elapsed;
    unsigned int lasttime;
    Mix_Music *musique;
    Mix_Chunk *bips[2];
    //Mix_Chunk *boup;

    // Init menu data + ttf logic
    if(!menu_init(game)){
        printf("Exiting on error\n");
        game->game_mode = 4;
        return;
    }

    // Musique et sons
    Mix_AllocateChannels(32);
    musique = Mix_LoadMUS(random_menu_music());
    Mix_PlayMusic(musique, -1); //Jouer infiniment la musique
    bips[0] = Mix_LoadWAV("./res/sounds/sfx_menu_move4.wav");
    bips[1] = Mix_LoadWAV("./res/sounds/sfx_coin_cluster3.wav");
    Mix_VolumeChunk(bips[0], MIX_MAX_VOLUME/2);
    Mix_VolumeChunk(bips[1], MIX_MAX_VOLUME/2);

    // Boucle d'event et d'affichage
    draw_menu(game);
    while(game->should_run == TRUE && game->actionKeyPressed == FALSE){
        lasttime = SDL_GetTicks();
        SDL_Event event;

        update_menu_events(game, event, bips);

        draw_menu(game);

        elapsed = SDL_GetTicks() - lasttime;
        if (elapsed < 20)
            SDL_Delay(20-elapsed);
    }
    if (game->actionKeyPressed){
        printf("Game mode selected\n");
        game->actionKeyPressed = FALSE;
    }

    printf("Unloading music\n");
    Mix_FreeMusic(musique);
    Mix_FreeChunk(bips[0]);
    Mix_FreeChunk(bips[1]);
    printf("gracefully exiting menu : game mode = %d\n", game->game_mode);
}