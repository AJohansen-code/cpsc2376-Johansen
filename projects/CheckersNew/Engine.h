#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_mixer.h>
#include <string>

class Engine {
public:
    // Constructor: Initializes SDL, SDL_ttf, SDL_mixer, creates window/renderer, and loads font/sound.
    Engine(const std::string& title = "Engine Window",
        int width = 700,
        int height = 700,
        const std::string& fontPath = "Ubuntu-Bold.ttf",
        int fontSize = 24,
        const std::string& soundPath = "move.wav");  // Path to sound file

    // Destructor: Cleans up resources.
    ~Engine();

    void drawCircle(int centerX, int centerY, int radius,
        SDL_Color color = { 255,0,0,255 });

    void clear(SDL_Color color = { 0,0,0,255 });
    void flip();

    void drawRectangle(int centerX, int centerY, int rectWidth = 100, int rectHeight = 100,
        SDL_Color color = { 0,0,255,255 });

    void drawText(const std::string& text, int centerX, int centerY,
        SDL_Color color = { 255,255,255,255 });

    SDL_Renderer* getRenderer() const;

    // Play loaded sound
    void playSound();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    Mix_Chunk* sound;
}; 
