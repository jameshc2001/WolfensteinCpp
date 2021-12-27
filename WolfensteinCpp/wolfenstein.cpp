#include <iostream>
#include <SDL.h>
#include <vec2.h>
#include <vector>
using namespace std;

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 800;

int map[24][24] =
{
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow("Woflenstein in C++", SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
	void *pixels;
	Uint8 *base;
	int pitch;

	vec2 pos = vec2(22, 12);
	vec2 dir = vec2(-1, 0);
	vec2 plane = vec2(0, 0.66);
	Uint64 time = SDL_GetPerformanceCounter();
	Uint64 oldTime = 0;

	SDL_Event event;
	bool quit = false;

	while (!quit)
	{
		oldTime = time;
		time = SDL_GetPerformanceCounter();
		double frameTime = (double)((time - oldTime) * 1000 / (double)SDL_GetPerformanceFrequency());
		double moveSpeed = frameTime * 0.01;
		double rotSpeed = frameTime * 0.004;

		while (SDL_PollEvent(&event) != 0)
		{
			switch (event.type)
			{
				case SDL_QUIT:
					quit = true;
					break;
			}
		}

		vec2 prevPos = pos;
		const Uint8* keyState = SDL_GetKeyboardState(NULL);
		if (keyState[SDL_SCANCODE_W]) { pos += dir * moveSpeed; }
		if (keyState[SDL_SCANCODE_S]) { pos -= dir * moveSpeed; }
		if (keyState[SDL_SCANCODE_A])
		{
			vec2 left = dir.rotateReturn(M_PI / 2.0);
			pos += left * moveSpeed;
		}
		if (keyState[SDL_SCANCODE_D])
		{
			vec2 right = dir.rotateReturn(-M_PI / 2.0);
			pos += right * moveSpeed;
		}
		if (keyState[SDL_SCANCODE_L]) { dir.rotate(-rotSpeed); plane.rotate(-rotSpeed); }
		if (keyState[SDL_SCANCODE_J]) { dir.rotate(rotSpeed); plane.rotate(rotSpeed); }
		if (map[int(pos.x())][int(pos.y())] != 0 && !keyState[SDL_SCANCODE_K]) pos = prevPos;

		SDL_LockTexture(texture, NULL, &pixels, &pitch);

		for (int x = 0; x < SCREEN_WIDTH; x++)
		{
			double camX = 2 * x / double(SCREEN_WIDTH) - 1;
			vec2 rayDir = dir + plane * camX;
			vec2 mapPos = vec2(int(pos.x()), int(pos.y()));

			//length of ray from current position to next x or y side
			vec2 sideDist;

			//length of ray from one x or y side to the next
			vec2 deltaDist = vec2(abs(1 / rayDir.x()), abs(1 / rayDir.y()));
			double perpWallDist;

			//what direction to step in
			vec2 step;

			int hit = 0; //was their a wall hit?
			int side; //x or y side?
			//calculate step and initial sideDist
			if (rayDir.x() < 0)
			{
				step.e[0] = -1;
				sideDist.e[0] = (pos.x() - mapPos.x()) * deltaDist.x();
			}
			else
			{
				step.e[0] = 1;
				sideDist.e[0] = (mapPos.x() + 1.0 - pos.x()) * deltaDist.x();
			}
			if (rayDir.y() < 0)
			{
				step.e[1] = -1;
				sideDist.e[1] = (pos.y() - mapPos.y()) * deltaDist.y();
			}
			else
			{
				step.e[1] = 1;
				sideDist.e[1] = (mapPos.y() + 1.0 - pos.y()) * deltaDist.y();
			}

			while (hit == 0)
			{
				//jump to next map square
				if (sideDist.x() < sideDist.y())
				{
					sideDist.e[0] += deltaDist.x();
					mapPos.e[0] += step.x();
					side = 0;
				}
				else
				{
					sideDist.e[1] += deltaDist.y();
					mapPos.e[1] += step.y();
					side = 1;
				}
				//check for hit
				if (map[int(mapPos.x())][int(mapPos.y())] > 0)
				{
					hit = 1;
				}
			}

			//calculate distance projected on camera direction
			if (side == 0) perpWallDist = (mapPos.x() - pos.x() + (1 - step.x()) / 2) / rayDir.x();
			else perpWallDist = (mapPos.y() - pos.y() + (1 - step.y()) / 2) / rayDir.y();

			//calculate height of line to draw on screen
			int lineheight = (int)(SCREEN_HEIGHT / perpWallDist);
			int drawStart = -lineheight / 2 + SCREEN_HEIGHT / 2;
			if (drawStart < 0) drawStart = 0;
			int drawEnd = lineheight / 2 + SCREEN_HEIGHT / 2;
			if (drawEnd >= SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT - 1;

			SDL_Color color; //RGBA
			switch (map[int(mapPos.x())][int(mapPos.y())])
			{
				case 0: color = { 0, 0, 0, 0 }; break;
				case 1: color = { 255, 0, 0, 255 }; break;
				case 2: color = { 0, 255, 0, 255 }; break;
				case 3: color = { 0, 0, 255, 255 }; break;
				case 4: color = { 255, 255, 255, 255 }; break;
				case 5: color = { 255, 255, 0, 255 }; break;
				default: color = { Uint8(map[int(mapPos.x())][int(mapPos.y())] % 255), Uint8(int(mapPos.x()) % 255), Uint8(int(mapPos.y()) % 255), 255 }; //RGBA
			}

			if (side == 1) color = {static_cast<unsigned char>(color.r / Uint32(2)),
				static_cast<unsigned char>(color.g / Uint32(2)),
				static_cast<unsigned char>(color.b / Uint32(2)), 255};

			//draw pixels
			for (int y = drawStart; y < drawEnd; y++)
			{
				base = ((Uint8 *)pixels) + (4 * (y * SCREEN_WIDTH + x));
				base[0] = color.r;
				base[1] = color.g;
				base[2] = color.b;
				base[3] = Uint8(255);
			}
		}

		SDL_UnlockTexture(texture);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
		memset(pixels, 86, SCREEN_WIDTH * SCREEN_HEIGHT * 4 * sizeof(Uint8));
		memset(pixels, 156, SCREEN_WIDTH * SCREEN_HEIGHT * 4 * sizeof(Uint8) / 2);
		cout << "\r" << pos.x() << " " << pos.y();
	}

	SDL_DestroyTexture(texture);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	return 0;
}