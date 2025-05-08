#include "Engine.h"
#include <iostream>
#include <stdexcept>

Engine::Engine(const std::string& title, int width, int height,
    const std::string& fontPath, int fontSize, const std::string& soundPath)
    : window(nullptr), renderer(nullptr), font(nullptr), sound(nullptr)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: "
            << SDL_GetError() << std::endl;
        throw std::runtime_error("SDL_Init failed");
    }

    if (TTF_Init() == -1) {
        std::cerr << "TTF could not initialize! TTF_Error: "
            << TTF_GetError() << std::endl;
        SDL_Quit();
        throw std::runtime_error("TTF_Init failed");
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! Mix_Error: "
            << Mix_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        throw std::runtime_error("Mix_OpenAudio failed");
    }

    window = SDL_CreateWindow(title.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: "
            << SDL_GetError() << std::endl;
        Mix_Quit();
        TTF_Quit();
        SDL_Quit();
        throw std::runtime_error("SDL_CreateWindow failed");
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: "
            << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        Mix_Quit();
        TTF_Quit();
        SDL_Quit();
        throw std::runtime_error("SDL_CreateRenderer failed");
    }

    font = TTF_OpenFont(fontPath.c_str(), fontSize);
    if (!font) {
        std::cerr << "Failed to load font! TTF_Error: "
            << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        Mix_Quit();
        TTF_Quit();
        SDL_Quit();
        throw std::runtime_error("TTF_OpenFont failed");
    }

    sound = Mix_LoadWAV(soundPath.c_str());
    if (!sound) {
        std::cerr << "Failed to load sound! Mix_Error: "
            << Mix_GetError() << std::endl;
        // Do not throw, continue without sound.
    }
}


Engine::~Engine()
{
    if (sound)
        Mix_FreeChunk(sound);
    if (font)
        TTF_CloseFont(font);
    if (renderer)
        SDL_DestroyRenderer(renderer);
    if (window)
        SDL_DestroyWindow(window);
    Mix_CloseAudio();
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();
}


void Engine::drawCircle(int centerX, int centerY, int radius,
    SDL_Color color)
{
    filledCircleRGBA(renderer, centerX, centerY, radius, color.r, color.g, color.b, color.a);
    aacircleRGBA(renderer, centerX, centerY, radius, color.r, color.g, color.b, color.a);
}

void Engine::drawRectangle(int centerX, int centerY, int rectWidth, int rectHeight,
    SDL_Color color)
{
    SDL_Rect rect;
    rect.x = centerX - rectWidth / 2;
    rect.y = centerY - rectHeight / 2;
    rect.w = rectWidth;
    rect.h = rectHeight;

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}

void Engine::drawText(const std::string& text, int centerX, int centerY,
    SDL_Color color)
{
    ;
    // Render the text to a surface using the loaded font.
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), color);
    if (!textSurface) {
        std::cerr << "Unable to render text surface! TTF_Error: "
            << TTF_GetError() << std::endl;
        return;
    }

    // Create texture from surface.
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        std::cerr << "Unable to create texture from rendered text! SDL_Error: "
            << SDL_GetError() << std::endl;
        SDL_FreeSurface(textSurface);
        return;
    }

    // Calculate destination rectangle to center the text.
    SDL_Rect destRect;
    destRect.w = textSurface->w;
    destRect.h = textSurface->h;
    destRect.x = centerX - destRect.w / 2;
    destRect.y = centerY - destRect.h / 2;

    SDL_RenderCopy(renderer, textTexture, nullptr, &destRect);

    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}

void Engine::clear(SDL_Color color)
{
    // Clear screen (set background to black).
    SDL_SetRenderDrawColor(getRenderer(), color.r, color.g, color.b, color.a);
    SDL_RenderClear(getRenderer());
}
void Engine::flip()
{

    SDL_RenderPresent(getRenderer());
}

SDL_Renderer* Engine::getRenderer() const {
    return renderer;
}

void Engine::playSound()
{
    if (sound)
        Mix_PlayChannel(-1, sound, 0);
}