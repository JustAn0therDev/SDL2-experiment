#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <map>

// The triangles are currently at the bottom of the screen because of their origin points.
typedef struct line {
    float x1;
    float y1;
    float x2;
    float y2;
} Line;

constexpr float INCREASE_LINE_BY = 0.015;
constexpr int TEXT_SIZE = 2048;
constexpr int FONT_SIZE = 48;
constexpr int WINDOW_WIDTH = 1920;
constexpr int WINDOW_HEIGHT = 1080;


Line* MakeLine(float x1, float y1, float x2, float y2) {
    Line* line = (Line*)malloc(sizeof(Line));
    
    if (line == NULL) {
        printf("Not enough memory available.\n");
        exit(1);
    }
    
    line->x1 = x1 * 4;
    line->y1 = y1 * 4;
    line->x2 = x2 * 4;
    line->y2 = y2 * 4;
    
    return line;
}

// Makes the line move by a macro called INCREASE_LINE_BY.
void MoveLine(Line* line) {
    line->x1 += INCREASE_LINE_BY;
    line->x2 += INCREASE_LINE_BY;
    line->y1 -= INCREASE_LINE_BY;
    line->y2 -= INCREASE_LINE_BY;
}

// Makes the line bigger by one percent.
void MakeLineBigger(Line* line) {
    line->x1 += 0.01;
    line->y1 += 0.01;
    line->x2 += 0.01;
    line->y2 += 0.01;
}


void InitKeysInMap(std::map<SDL_Keycode, const char*>& keys) {
    keys[SDLK_a] = "a";
    keys[SDLK_b] = "b";
    keys[SDLK_c] = "c";
    keys[SDLK_d] = "d";
    keys[SDLK_e] = "e";
    keys[SDLK_f] = "f";
    keys[SDLK_g] = "g";
    keys[SDLK_h] = "h";
    keys[SDLK_i] = "i";
    keys[SDLK_j] = "j";
    keys[SDLK_k] = "k";
    keys[SDLK_l] = "l";
    keys[SDLK_m] = "m";
    keys[SDLK_n] = "n";
    keys[SDLK_o] = "o";
    keys[SDLK_p] = "p";
    keys[SDLK_q] = "q";
    keys[SDLK_r] = "r";
    keys[SDLK_s] = "s";
    keys[SDLK_t] = "t";
    keys[SDLK_u] = "u";
    keys[SDLK_v] = "v";
    keys[SDLK_w] = "w";
    keys[SDLK_x] = "x";
    keys[SDLK_y] = "y";
    keys[SDLK_z] = "z";
    keys[SDLK_PERIOD] = ".";
}


int main()
{
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    std::map<SDL_Keycode, const char*> keys;

    InitKeysInMap(keys);
    
    TTF_Init();
    TTF_Font* openSans = TTF_OpenFont("OpenSans-Regular.ttf", FONT_SIZE);
    
    SDL_Rect rect;
    rect.h = FONT_SIZE * 2;
    rect.w = FONT_SIZE;
    rect.x = 0;
    rect.y = 0;
    
    SDL_SetTextInputRect(&rect);
    SDL_StartTextInput();
    
    Line* first_line = MakeLine(320, 200, 300, 240);
    Line* second_line = MakeLine(300, 240, 340, 240);
    Line* third_line = MakeLine(340, 240, 320, 200);
    
    SDL_Color Black = { 0, 0, 0 };
    
    char* text = (char*)malloc(sizeof(char) * TEXT_SIZE);
    
    if (SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer) == 0) {
        
        SDL_bool done = SDL_FALSE;
        float surface_message_size = 0;
        float previous_surface_message_size = 0;
        
        while (!done) {
            SDL_Event event;
            
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
            SDL_RenderClear(renderer);
            
            SDL_Surface* surfaceMessage = TTF_RenderText_Blended_Wrapped(openSans, text, Black, 200);
            
            SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
            
            SDL_RenderCopy(renderer, Message, NULL, &rect);
            SDL_RenderPresent(renderer);

            if (surfaceMessage != NULL) {
                surface_message_size = surfaceMessage->h;
            }

            free(surfaceMessage);
            
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    done = SDL_TRUE;
                } else if (event.key.type == SDL_KEYDOWN) {
                    if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
                        text = strcat(text, " ");
                    } else {
                        text = strcat(text, keys[event.key.keysym.sym]);
                        rect.w += FONT_SIZE / 2;
                    }

                    if (surface_message_size != previous_surface_message_size) {
                        previous_surface_message_size = surface_message_size;
                        rect.h += surface_message_size / 2;
                    }
                }
            }
        }

        SDL_DestroyWindow(window);

    }
    
    free(first_line);
    free(second_line);
    free(third_line);
    
}
