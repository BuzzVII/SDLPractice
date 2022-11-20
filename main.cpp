#include <iostream>
#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define SCREEN_WIDTH   1280
#define SCREEN_HEIGHT  720
#define PLAYER_WIDTH    50
#define PLAYER_HEIGHT   50

struct SDLWindowDestroyer
{
    void operator()(SDL_Window* w) const
    {
        SDL_DestroyWindow(w);
    }
};

struct SDLRendererDestroyer
{
    void operator()(SDL_Renderer* r) const
    {
        SDL_DestroyRenderer(r);
    }
};

struct SDLTextureDestroyer
{
    void operator()(SDL_Texture* t) const
    {
        SDL_DestroyTexture(t);
    }
};

int main(int argc, char* argv[])
{
    if (argc <= 1) {
        std::cout << "Usage: \n";
        std::cout << argv[0];
        std::cout << " PNG_FILE" << std::endl;
        return -1;
    }
    std::unique_ptr<SDL_Window, SDLWindowDestroyer> window;
    std::unique_ptr<SDL_Renderer, SDLRendererDestroyer> renderer;
    std::unique_ptr<SDL_Texture, SDLTextureDestroyer> texture;
    std::unique_ptr<SDL_Rect> player = std::make_unique<SDL_Rect>();
    player->w = PLAYER_WIDTH;
    player->h = PLAYER_HEIGHT;
    player->x = (SCREEN_WIDTH + PLAYER_WIDTH) / 2;
    player->y = (SCREEN_HEIGHT + PLAYER_HEIGHT) / 2;
    std::unique_ptr<SDL_Rect> background  = std::make_unique<SDL_Rect>();
    background->w = SCREEN_WIDTH;
    background->h = SCREEN_HEIGHT;

    SDL_Event event;
    bool running = true;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "SDL init error: " << SDL_GetError() << std::endl;
        return -1;
    }
    window.reset(SDL_CreateWindow("TEST", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0));
    renderer.reset(SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED));
    SDL_RenderPresent(renderer.get());

    texture.reset(IMG_LoadTexture(renderer.get(), argv[1]));
    if (!texture)
        SDL_Log("Couldn't load %s: %s\n", argv[1], SDL_GetError());
    else
        SDL_QueryTexture(texture.get(), nullptr, nullptr, &(player->w), &(player->h));


    while (running)
    {
        while ( SDL_PollEvent(&event) ) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.scancode) {
                        // Arrow keys for stepping player
                        case SDL_SCANCODE_UP:
                            player->y -= 10;
                            break;
                        case SDL_SCANCODE_DOWN:
                            player->y += 10;
                            break;
                        case SDL_SCANCODE_LEFT:
                            player->x -= 10;
                            break;
                        case SDL_SCANCODE_RIGHT:
                            player->x += 10;
                            break;
                        case SDL_SCANCODE_ESCAPE:
                            running = false;
                            break;
                        default:
                            break;
                    }
                default:
                    break;
            }
        }

        // Restrict movement to screen
        if ((player->x + player->w) > SCREEN_WIDTH) player->x = SCREEN_WIDTH - player->w;
        else if ((player->x) < 0) player->x = 0;
        if ((player->y + player->h) > SCREEN_HEIGHT) player->y = SCREEN_HEIGHT - player->h;
        else if ((player->y) < 0) player->y = 0;

        SDL_RenderClear(renderer.get());
        SDL_SetRenderDrawColor(renderer.get(), 0x00, 0x80, 0xAA, 0xff);
        SDL_RenderFillRect(renderer.get(), background.get());
        if (!texture) {
            SDL_SetRenderDrawColor(renderer.get(), 0xAA, 0x00, 0x80, 0xff);
            SDL_RenderFillRect(renderer.get(), player.get());
        } else SDL_RenderCopy(renderer.get(), texture.get(), nullptr, player.get());
        SDL_RenderPresent(renderer.get());

        SDL_Delay(1000/60); // 60 fps in ms
    }
    SDL_Quit();
    return 0;
}
