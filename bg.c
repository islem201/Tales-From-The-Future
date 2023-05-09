
#include "bg.h"

void initBack(Background* b, SDL_Surface* screen, const char* paths[], int numImages) {
    SDL_Surface* temp = NULL;
    for (int i = 0; i < numImages; i++) {
        temp = IMG_Load(paths[i]);
        if (temp == NULL) {
            // handle error here, e.g. print error message and return
            printf("Failed to load image %s: %s\n", paths[i], IMG_GetError());
            return;
        }
        b->images[i] = SDL_DisplayFormatAlpha(temp);
        SDL_FreeSurface(temp);
    }
    b->rect.x = 0;
    b->rect.y = 0;
    b->rect.w = screen->w;
    b->rect.h = screen->h;
    b->camera_pos.x = 0;
    b->camera_pos.y = 0;
    b->camera_pos.h = b->rect.h;
    b->camera_pos.w = b->rect.w;
    b->direction = 0;
    b->currentImageIndex = 0;
}



void afficherBack(Background b, SDL_Surface *screen) {
    SDL_BlitSurface(b.images[b.currentImageIndex], &b.camera_pos, screen,&b.rect);
}

void initMusic(char *music) {
    
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
        printf("Unable to initialize SDL_mixer: %s\n", Mix_GetError());
        exit(1);
    }

}



void scrolling(Background *b, int direction, int dx) {
    switch (direction) {
        case LEFT:
            if(b->camera_pos.x>0)
            b->camera_pos.x -= dx;
            break;
        case RIGHT:
            b->camera_pos.x += dx;
            break;
        case UP:
            b->camera_pos.y -= dx;
            break;
        case DOWN:
            b->camera_pos.y += dx;
            break;
        default:
            break;
            
        
    }

void saveScore(ScoreInfo s, char *fileName) {
    FILE *fp;
    fp = fopen(fileName, "a");
    if (fp == NULL) {
        printf("Error: could not open file.\n");
        return;
    }
    fprintf(fp, "%d %d %s\n", s.score, s.time, s.playerName);
    fclose(fp);
}


void draw_scores(char *filename, ScoreInfo t[],int score,Uint32 start_time) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    SDL_Surface *surface = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);
    TTF_Font *font = TTF_OpenFont("font.ttf", 28);

    FILE *fp;
    char line[100];
    int i = 0;
    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: could not open file.\n");
        return;
    }
    while (fgets(line, sizeof(line), fp) != NULL ) {
        sscanf(line, "%d %d %s", &t[i].score, &t[i].time, t[i].playerName);
        i++;
    }
    fclose(fp);
    int n = i;
    ScoreInfo temp;
    for (i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (t[j].score > t[i].score || (t[j].score == t[i].score && t[j].time < t[i].time)) {
                temp = t[i];
                t[i] = t[j];
                t[j] = temp;
            }
        }
    }
    SDL_Surface *textSurface;
    SDL_Rect rect;
    char scoreString[50];
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 0, 0));
    for (i = 0; i < 3 && i < n; i++) {
        sprintf(scoreString, "%d. %s %d %d", i + 1, t[i].playerName, t[i].score, t[i].time);
        textSurface = TTF_RenderText_Blended(font, scoreString, (SDL_Color) { 255, 255, 255 });
        rect.x = 50;
        rect.y = i * 50 + 50;
        SDL_BlitSurface(textSurface, NULL, surface, &rect);
        SDL_FreeSurface(textSurface);
    }
    SDL_UpdateRect(surface, 0, 0, 0, 0);


    SDL_Event event;
    while (SDL_WaitEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN) {
            break;
        }
    }
    Uint32 elapsed_time = get_elapsed_time(start_time);
    ScoreInfo si = { score,elapsed_time, "Player 1" };
    saveScore(si, "scores.txt");
    TTF_CloseFont(font);
    SDL_Quit();
    TTF_Quit();
}
void animerBack(Background *b,int imagenumber) {
    

    b->currentImageIndex++;

    if (b->currentImageIndex >= imagenumber) {
        b->currentImageIndex = 0;
    }
}


void draw_hearts(SDL_Surface *surface, int lives) {
    SDL_Surface *full_heart = IMG_Load("heart1.png");
    SDL_Surface *half_heart = IMG_Load("heart2.png");
    SDL_Surface *empty_heart = IMG_Load("heart3.png");

    int heart_width = full_heart->w;
    int heart_height = full_heart->h;

    int x = 10;
    int y = 10;

    for (int i = 0; i < 3; i++) {
        SDL_Rect dest_rect = { x, y, heart_width, heart_height };

        if (lives >= 1) {
            SDL_BlitSurface(full_heart, NULL, surface, &dest_rect);
        } else {
            SDL_BlitSurface(empty_heart, NULL, surface, &dest_rect);
        }

        x += heart_width + 10;
        lives -= 1.0;
    }

    SDL_FreeSurface(full_heart);
    SDL_FreeSurface(half_heart);
    SDL_FreeSurface(empty_heart);
}

Uint32 get_elapsed_time(Uint32 start_time) {
    Uint32 current_time = SDL_GetTicks();
    Uint32 elapsed_time = current_time - start_time;
    return elapsed_time;
}

/*void playSinglePlayer(SDL_Surface *screen, Background *b, const char **paths) {
    
    initBack(b, screen, paths, 4);
    int gameover = 0;
    int score = 0;
    int level = 1;
    int lives = 3;
    SDL_Event event;
    Uint32 start_time = SDL_GetTicks();
    ScoreInfo topScores[3] = {
    {0, 0, ""},
    {0, 0, ""},
    {0, 0, ""}
    };
    
    while (!gameover) {
        
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    gameover = 1;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            gameover = 1;
                            break;
                        case SDLK_LEFT:
                            b->direction = LEFT;
                            scrolling(b, LEFT, 5);
                            break;
                        case SDLK_RIGHT:
                            b->direction = RIGHT;
                            scrolling(b, RIGHT, 5);
                            break;
                        case SDLK_UP:
                            b->direction = UP;
                            scrolling(b, UP, 5);
                            break;
                        case SDLK_DOWN:
                            b->direction = DOWN;
                            scrolling(b, DOWN, 5);
                            break;
                        case SDLK_m:
                            draw_scores("scores.txt", topScores, score,start_time);
                    }
                    break;
            }
        }
        score += level;
        if (score >= 10 * level) {
            level++;
        }
        animerBack(b,&screen);
        draw_hearts(screen, lives, b);

        SDL_Flip(screen);
        SDL_Delay(20);
    }
    Uint32 elapsed_time = get_elapsed_time(start_time);
    printf("Elapsed time: %u milliseconds\n", elapsed_time);
    ScoreInfo si = { score,elapsed_time, "Player 1" };
    saveScore(si, "scores.txt");
}



*/
/*void init_Back(Background bg){
bg.image = SDL_LoadBMP("background.bmp"); // Load the background image
bg.posBack1.x = 0;
bg.posBack1.y = 0;
bg.posBack1.w = bg.image->w / 2; // Set the width of the first camera to half of the image width
bg.posBack1.h = bg.image->h;
bg.posBack2.x = bg.posBack1.w; // Set the x position of the second camera to the end of the first camera
bg.posBack2.y = 0;
bg.posBack2.w = bg.image->w / 2; // Set the width of the second camera to half of the image width
bg.posBack2.h = bg.image->h;
bg.posScreen1.x = 0;
bg.posScreen1.y = 0;
bg.posScreen1.w = bg.posBack1.w;
bg.posScreen1.h = bg.posBack1.h;
bg.posScreen2.x = bg.posBack1.w;
bg.posScreen2.y = 0;
bg.posScreen2.w = bg.posBack2.w;
bg.posScreen2.h = bg.posBack2.h;
bg.lineY = bg.image->h / 2; // Set the Y position of the horizontal line to the middle of the image height
}
void playmultiplayer(Background *bg, SDL_Surface *screen) {
    // Blit the first part of the background
    SDL_BlitSurface(bg->image, &bg->posBack1, screen, &bg->posScreen1);

    // Blit the second part of the background
    SDL_BlitSurface(bg->image, &bg->posBack2, screen, &bg->posScreen2);

    // Draw the horizontal line
    SDL_Rect lineRect = {0, bg->lineY, screen->w, 1};
    Uint32 lineColor = SDL_MapRGB(screen->format, 255, 255, 255); // White color
    SDL_FillRect(screen, &lineRect, lineColor);
}*/
