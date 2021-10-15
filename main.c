#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>

typedef struct keyMap {
    struct keyMap* nextKeyMap; 
    SDL_Keycode key_code;
    const char* character;
} KeyMap;

typedef struct rendered_chars {
    struct rendered_chars* previous;
    struct rendered_chars* next;
    char* currentChar;
} RenderedChars;

KeyMap* GetKeyMapLinkedList();
KeyMap* GetPointerToKeyCodeInLinkedList(KeyMap* keys, SDL_Keycode *key_code);
void SetCharsFromRenderedChars(char* text, RenderedChars *first_item_pointer);
void AddToRenderedCharList(const char* charTyped, RenderedChars** chars);
void RemoveLastItemFromCharList(RenderedChars** chars, SDL_Rect* rect);

const float INCREASE_LINE_BY = 0.015;
const int TEXT_SIZE = 2048;
const int FONT_SIZE = 48;
const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;
const int POSSIBLE_KEY_CODES_AND_CHARACTERS_LIMIT = 26;

int main()
{
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    KeyMap* keys = GetKeyMapLinkedList();
    RenderedChars *chars = (RenderedChars*)malloc(sizeof(RenderedChars));
    RenderedChars *first_item_pointer = chars;

    TTF_Init();
    TTF_Font* openSans = TTF_OpenFont("OpenSans-Regular.ttf", FONT_SIZE);
    
    SDL_Rect rect;
    rect.h = FONT_SIZE * 2;
    rect.w = FONT_SIZE;
    rect.x = 0;
    rect.y = 0;
    
    SDL_SetTextInputRect(&rect);
    SDL_StartTextInput();
    
    SDL_Color White = { 255, 255, 255 };
    
    if (SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer) == 0) {
        
        SDL_bool done = SDL_FALSE;

        // These are used to control the size of the rendered characters.
        float surface_message_size = 0;
        float previous_surface_message_size = 0;
        
        while (!done) {
            
            SDL_Event event;
            
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderClear(renderer);

            SDL_Surface* surfaceMessage = TTF_RenderText_Blended_Wrapped(openSans, "INSERT TEXT HERE !!1", White, 500);

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
                        AddToRenderedCharList(" ", &chars);
                    } else if (event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE) {
                        RemoveLastItemFromCharList(&chars, &rect);
                    } else {
                        KeyMap* possible_key = GetPointerToKeyCodeInLinkedList(keys, &event.key.keysym.sym);

                        if (possible_key != NULL) {
                            AddToRenderedCharList(possible_key->character, &chars);
                            rect.w += FONT_SIZE / 2;
                        }
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

}

KeyMap* GetKeyMapLinkedList() {

    KeyMap* tmp;

    const char* possible_characters[26] = {"b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "."};
    SDL_Keycode possible_key_codes[26] = { SDLK_b, SDLK_c, SDLK_d, SDLK_e, SDLK_f, SDLK_g, SDLK_h, SDLK_i, SDLK_j, SDLK_k, SDLK_l, SDLK_m, SDLK_n, SDLK_o, SDLK_p, SDLK_q, SDLK_r, SDLK_s, SDLK_t, SDLK_u, SDLK_v, SDLK_w, SDLK_x, SDLK_y, SDLK_z, SDLK_PERIOD };

    KeyMap *firstKey = (KeyMap*)malloc(sizeof(KeyMap));

    // The first pointer is defined as the "a" and "SDLK_a", so that the next items on the list
    // have the subsequent characters attached to them.
    firstKey->character = "a";
    firstKey->key_code = SDLK_a;
    firstKey->nextKeyMap = (KeyMap*)malloc(sizeof(KeyMap));

    tmp = firstKey->nextKeyMap;

    for (int i = 1; i < POSSIBLE_KEY_CODES_AND_CHARACTERS_LIMIT; i++) {

        if (tmp == NULL) {
            tmp = (KeyMap*)malloc(sizeof(KeyMap));
        }

        tmp->character = possible_characters[i];
        tmp->key_code = possible_key_codes[i];
        tmp->nextKeyMap = (KeyMap*)malloc(sizeof(KeyMap));

        tmp = tmp->nextKeyMap;

    }

    return firstKey;

}

KeyMap* GetPointerToKeyCodeInLinkedList(KeyMap* keys, SDL_Keycode *key_code) {

    KeyMap* tmp = keys;

    while (tmp != NULL) {

        if (tmp->key_code == *key_code) {
            return tmp;
        }

        tmp = tmp->nextKeyMap;
    }

    return NULL;

}

void SetCharsFromRenderedChars(char* text, RenderedChars *first_item_pointer) {
    // text = (char*)realloc(text, sizeof(char) * 50);
    text = (char*)malloc(sizeof(char) * 50);

    while (first_item_pointer != NULL) {

        if (first_item_pointer->currentChar != NULL)
            printf("%i\n", first_item_pointer->currentChar != NULL);
            text = strcat(text, first_item_pointer->currentChar);

        printf("first_item_pointer = first_item_pointer->next;\n");
        first_item_pointer = first_item_pointer->next;

    }
}

void AddToRenderedCharList(const char* charTyped, RenderedChars** chars) {
    // Set the character in the next RenderedChars,
    // And set this one as the previous of the next;
    (*chars)->next = (RenderedChars*)malloc(sizeof(RenderedChars));
    (*chars)->next->previous = *chars;
    (*chars)->next->currentChar = (char*)malloc(sizeof(char) * 2);
    strcpy((*chars)->next->currentChar, charTyped);

    // After setting everything up, lets move one item along the linked list.
    *chars = (*chars)->next;
}

void RemoveLastItemFromCharList(RenderedChars** chars, SDL_Rect* rect) {
    if (chars != NULL) {
        RenderedChars* previous = (*chars)->previous;

        if (previous != NULL) {
            free((*chars)->currentChar);
            free(*chars);
            
            previous->next = NULL;
            *chars = previous;
            rect->w -= FONT_SIZE / 2;
        }
    }
}