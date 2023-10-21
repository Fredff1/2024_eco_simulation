#include <SDL2/SDL.h>

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Event event;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int BALL_RADIUS = 20;

int main(int args, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("SDL2 Circle Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Log("Unable to create window: %s", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_Log("Unable to create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    int x = SCREEN_WIDTH / 2;
    int y = SCREEN_HEIGHT / 2;
    int xVel = 6, yVel = 2;

    bool quit = false;
    while (!quit)
    {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        x += xVel;
        y += yVel;

        if (x > SCREEN_WIDTH - BALL_RADIUS || x < BALL_RADIUS) {
            xVel = -xVel;
        }

        if (y > SCREEN_HEIGHT - BALL_RADIUS || y < BALL_RADIUS) {
            yVel = -yVel;
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White background
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 10, 200, 120, 255); // Green circle
        for (int w = 0; w < BALL_RADIUS * 2; w++)
        {
            for (int h = 0; h < BALL_RADIUS * 2; h++)
            {
                int dx = BALL_RADIUS - w;
                int dy = BALL_RADIUS - h;
                if ((dx * dx + dy * dy) <= (BALL_RADIUS * BALL_RADIUS))
                {
                    SDL_RenderDrawPoint(renderer, x + dx, y + dy);
                }
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay((1.0 / 60) * 1000);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
