#include "../include/menu.h"


void update_game_solo_events(stGame *game){

}
void update_game_solo_events(stGame *game, SDL_Event event)
{
    const Uint8 *keyboard_state_array = SDL_GetKeyboardState(NULL);

	
	if(event.type == SDL_KEYDOWN || event.type == SDL_KEYUP )
    {
        if (keyboard_state_array[SDL_SCANCODE_UP] || (keyboard_state_array[SDL_SCANCODE_LEFT]))
        {
            if (game->game_mode > 0)
                game->game_mode = game->game_mode - 1;
        }
        if (keyboard_state_array[SDL_SCANCODE_DOWN] || keyboard_state_array[SDL_SCANCODE_RIGHT])
        {
            if (game->game_mode < 3)
                game->game_mode = game->game_mode + 1;
        }
        if (keyboard_state_array[SDL_SCANCODE_SPACE] || keyboard_state_array[SDL_SCANCODE_RETURN])
            game->actionKeyPressed = TRUE;
        if (keyboard_state_array[SDL_SCANCODE_ESCAPE] || event.type == SDL_QUIT){
            game->game_mode = 4;
            game->should_run = FALSE;
        }
	}
}

BOOL game_loop_init(stGame *game){
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
    for (int i = 0; i < 4; i++){
        SDL_Surface *temp = TTF_RenderText_Solid(game->pFont, game->text_menu[i], text_color);
        if (!temp){
            printf("Echec d'initialisation de surface : %s\n", SDL_GetError());
            SDL_FreeSurface(temp);
            return FALSE;
        }
        game->pTextMenu[i] = SDL_CreateTextureFromSurface(game->pRenderer, temp);
        SDL_FreeSurface(temp);
    }
    // Create Bold Text Texture 
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
    TTF_SetFontStyle(game->pFont, TTF_STYLE_NORMAL);
    printf("Menu init OK\n");
    return TRUE;
}

void draw_game(stGame *game){
    SDL_Rect textBox[4];
    SDL_Rect cursorBox;
    SDL_Rect backBox;
    backBox.x = 0;
    backBox.y = 0;
    backBox.w = SCREEN_WIDTH; 
    backBox.h = SCREEN_HEIGHT;

    // Nettoie l'écran
    SDL_SetRenderDrawColor(game->pRenderer, 0, 0, 0, 255);
    SDL_RenderClear(game->pRenderer);

    // Ajoute le background
    SDL_RenderCopy(game->pRenderer, game->pTextBackground, NULL, &backBox);
    // Ajoute le texte et le curseur
    for (int i = 0; i < 4; i++){
        textBox[i].x = (SCREEN_WIDTH/2) - 160;
        textBox[i].y = (SCREEN_HEIGHT/2) - 32 *(1 + i);
        textBox[i].w = 320; 
        textBox[i].h = 32;
        
        if(i == game->game_mode){
            cursorBox.x = (SCREEN_WIDTH/2) - 192;
            cursorBox.y = (SCREEN_HEIGHT/2) - 32 *(1 + i);
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

char *random_game_music(){
    if((rand()*100) > 50){
        return "./res/musics/pokemon-tcg-ost-club-master-battle-2020-stereo-mix.mp3";
    } else {
        return "./res/musics/pokemon-tcg-ost-water-rock-psychic-clubs-2020-stereo-mix.mp3";
    }
}

void game_loop_solo(stGame *game){
    SDL_Event event;
    Mix_Music *musique;
    Mix_Chunk *bip;
    // Init menu data + ttf logic
    if(!game_loop_init(game)){
        printf("Exiting on error\n");
        game->game_mode = 4;
        return;
    }

    // Musique et sons
    Mix_AllocateChannels(32);
    musique = Mix_LoadMUS(random_game_music());
    Mix_PlayMusic(musique, -1); //Jouer infiniment la musique
    bip = Mix_LoadWAV("./res/sounds/sfx_menu_move4.wav");
    Mix_VolumeChunk(bip, MIX_MAX_VOLUME/2);


    // Game loop
    draw_game(game);
    printf("first frame OK\n");
    while(game->should_run == TRUE){
        unsigned int elapsed;
        unsigned int lasttime = SDL_GetTicks();
        SDL_PollEvent(&event);

        update_game_solo_events(game, event);
        update_game_solo_data(game);
        draw_game(game);

        elapsed = SDL_GetTicks() - lasttime;
        if (elapsed < 20)
            SDL_Delay(20-elapsed);
    }
    printf("Unloading music\n");
    Mix_FreeMusic(musique);
    Mix_FreeChunk(bip);
    printf("gracefully exiting menu : game mode = %d", game->game_mode);
}