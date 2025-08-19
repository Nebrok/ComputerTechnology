#pragma once
#include <SDL3/SDL.h>
#include <string>
#include <vector>
#include "Game.h"


class DotRenderer
{

private:
    SDL_Renderer* _sdlRenderer;
    SDL_Texture* _circleTexture;
    SDL_Texture* _screenTexture;

    std::vector<uint32_t> _pixelBuffer;

    const SDL_PixelFormatDetails* _pixelFormatDetails;
    const int HALF_WIDTH = SCREEN_WIDTH / 2;
    const int HALF_HEIGHT = SCREEN_HEIGHT / 2;


public:
    DotRenderer(SDL_Window* window);
    ~DotRenderer();

    SDL_Renderer* GetSDLRenderer() const { return _sdlRenderer; }

    void SetDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    void Clear();
    void Present();

    void DrawCircle(int centerX, int centerY, int radius);
    void DrawFilledCircle(int centerX, int centerY, int radius, float totalTime);
    void DrawCircleTexture(int centerX, int centerY, int radius, float totalTime);
    bool DrawLine(float startX, float startY, float endX, float endY);

    void ClearBuffer();
    void DrawToPixelBuffer(int centerX, int centerY, int radius, float totalTime);
    void DrawPixelBuffer();


    void RenderTexture(SDL_Texture* texture, const SDL_FRect* srcRect, const SDL_FRect* dstRect);

private:
    void DrawPoint(int x, int y);

    DotRenderer(const DotRenderer&) = delete;
    DotRenderer& operator=(const DotRenderer&) = delete;


    void SetPixel(SDL_Surface* surface, int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
};

