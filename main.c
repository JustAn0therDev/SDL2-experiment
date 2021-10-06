#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <stdlib.h>

// The triangles are currently in the bottom of the screen because of their origin points.
typedef struct line {
    float x1;
    float y1;
    float x2;
    float y2;
} Line;


#define INCREASE_LINE_BY 0.015


Line* MakeLine(float x1, float y1, float x2, float y2) {
    Line* line = (Line*)malloc(sizeof(Line));

    if (line == NULL) {
        printf("Not enough memory available.\n");
        exit(1);
    }

    line->x1 = x1 * 5;
    line->y1 = y1 * 5;
    line->x2 = x2 * 5;
    line->y2 = y2 * 5;

    return line;
}

void MoveLine(Line* line) {
    line->x1 += INCREASE_LINE_BY;
    line->x2 += INCREASE_LINE_BY;
    line->y1 -= INCREASE_LINE_BY;
    line->y2 -= INCREASE_LINE_BY;
}

void MakeLineBigger(Line* line) {
    line->x1 += 0.01;
    line->y1 += 0.01;
    line->x2 += 0.01;
    line->y2 += 0.01;
}

int main(int argc, char *argv[])
{
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    Line* first_line = MakeLine(320, 200, 300, 240);
    Line* second_line = MakeLine(300, 240, 340, 240);
    Line* third_line = MakeLine(340, 240, 320, 200);

    if (SDL_CreateWindowAndRenderer(1920, 1080, 0, &window, &renderer) == 0) {

        SDL_bool done = SDL_FALSE;

        while (!done) {
            SDL_Event event;

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderClear(renderer);

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawLine(renderer, first_line->x1, first_line->y1, first_line->x2, first_line->y2);
            SDL_RenderDrawLine(renderer, second_line->x1, second_line->y1, second_line->x2, second_line->y2);
            SDL_RenderDrawLine(renderer, third_line->x1, third_line->y1, third_line->x2, third_line->y2);
            SDL_RenderPresent(renderer);

            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    done = SDL_TRUE;
                }
            }
        }

    }

    free(first_line);
    free(second_line);
    free(third_line);

}
