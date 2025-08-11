#include "DotRenderer.h"
#include <SDL3/SDL_render.h>
#include <SDL3/SDL.h>
#include <cmath> 

#include "glm/glm.hpp"
#include "game.h"


DotRenderer::DotRenderer(SDL_Window* window) : _sdlRenderer(nullptr)
{
	_sdlRenderer = SDL_CreateRenderer(window, nullptr);
	if (!_sdlRenderer) return;


	int surfaceWidth = 64;
	int surfaceHeight = 64;
	SDL_Surface* circleSurface = SDL_CreateSurface(surfaceWidth, surfaceHeight, SDL_PIXELFORMAT_RGBA32);
	glm::vec2 circleCentre{ surfaceWidth / 2, surfaceHeight / 2 };
	for (int x = 0; x < surfaceWidth; x++) {
		for (int y = 0; y < surfaceHeight; y++) 
		{
			SetPixel(circleSurface, x, y, 255, 255, 255, 255);
		}
	}
	_circleTexture = SDL_CreateTextureFromSurface(_sdlRenderer, circleSurface);
}

void DotRenderer::SetPixel(SDL_Surface* surface, int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	SDL_LockSurface(surface);

	// surface->format is now an enum SDL_PixelFormat  so we have to to get BytesPerPixel in a different way
	SDL_PixelFormat format = surface->format;
	const SDL_PixelFormatDetails* format_details = SDL_GetPixelFormatDetails(format);

	uint8_t* pixelArray = (uint8_t*)surface->pixels;
	pixelArray[y * surface->pitch + x * format_details->bytes_per_pixel + 0] = r;
	pixelArray[y * surface->pitch + x * format_details->bytes_per_pixel + 1] = g;
	pixelArray[y * surface->pitch + x * format_details->bytes_per_pixel + 2] = b;
	pixelArray[y * surface->pitch + x * format_details->bytes_per_pixel + 3] = a;

	SDL_UnlockSurface(surface);
}

DotRenderer::~DotRenderer()
{
	if (_sdlRenderer)
	{
		SDL_DestroyRenderer(_sdlRenderer);
		_sdlRenderer = nullptr;
	}
	if (_circleTexture)
	{
		SDL_DestroyTexture(_circleTexture);
		_circleTexture = nullptr;
	}
}

void DotRenderer::SetDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	if (_sdlRenderer)
	{
		SDL_SetRenderDrawColor(_sdlRenderer, r, g, b, a);
	}
}

void DotRenderer::Clear()
{
	if (_sdlRenderer)
	{
		SDL_RenderClear(_sdlRenderer);
	}
}

void DotRenderer::Present()
{
	if (_sdlRenderer)
	{
		SDL_RenderPresent(_sdlRenderer);
	}
}

void DotRenderer::DrawPoint(int x, int y)
{
	if (_sdlRenderer)
	{
		SDL_RenderPoint(_sdlRenderer, x, y);
	}
}

void DotRenderer::DrawCircle(int centerX, int centerY, int radius)
{
	if (!_sdlRenderer) return;

	int x = radius;
	int y = 0;
	int err = 0;

	while (x >= y) 
	{
		DrawPoint(centerX + x, centerY + y);
		DrawPoint(centerX + y, centerY + x);
		DrawPoint(centerX - y, centerY + x);
		DrawPoint(centerX - x, centerY + y);
		DrawPoint(centerX - x, centerY - y);
		DrawPoint(centerX - y, centerY - x);
		DrawPoint(centerX + y, centerY - x);
		DrawPoint(centerX + x, centerY - y);

		if (err <= 0) 
		{
			y++;
			err += 2 * y + 1;
		}
		if (err > 0) 
		{
			x--;
			err -= 2 * x + 1;
		}
	}
}

void DotRenderer::DrawFilledCircle(int centerX, int centerY, int radius, float totalTime)
{
	if (!_sdlRenderer) return;
	
	SDL_FRect destinationRect{centerX - radius, centerY - radius, radius * 2, radius * 2};
	RenderTexture(_circleTexture, NULL, &destinationRect);

	return;
	//Old render stuff below
	float redColor = (glm::cos((totalTime) * 0.1f + (centerX / SCREEN_WIDTH)) * 0.5f + 0.5f) * 255.0f;
	float greenColor = (glm::cos((totalTime) * 0.9f + (centerY / SCREEN_HEIGHT)) * 0.5f + 0.5f) * 255.0f;
	float blueColor = (glm::cos(totalTime * 0.4f) * 0.5f + 0.5f) * 255.0f;

	SetDrawColor(255, 255, 255, 255);

	for (int y = -radius; y <= radius; y++) 
	{
		int x = static_cast<int>(std::sqrt(radius * radius - y * y));
		SDL_RenderLine(_sdlRenderer, centerX - x, centerY + y, centerX + x, centerY + y);
	}
}

bool DotRenderer::DrawLine(float startX, float startY, float endX, float endY)
{
	if (!_sdlRenderer) return false;
	return SDL_RenderLine(_sdlRenderer, startX, startY, endX, endY);
}

void DotRenderer::RenderTexture(SDL_Texture* texture, const SDL_FRect* srcRect, const SDL_FRect* dstRect)
{
	if (_sdlRenderer && texture)
	{
		SDL_RenderTexture(_sdlRenderer, texture, srcRect, dstRect);
	}
}




