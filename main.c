#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <SDL2/SDL.h>

#define USE_MANHATTAN

double min(double a, double b)
{
	return a < b ? a : b;
}

uint32_t window_width = 640;
uint32_t window_height = 480;

void draw_star(SDL_Surface* target, 
                uint32_t x, uint32_t y, 
                uint32_t inten, 
                uint8_t red, uint8_t green, uint8_t blue)
{
	uint32_t* data = (uint32_t *) target -> pixels;

        uint32_t pix_w = window_width / target -> w;
        uint32_t pix_h = window_height / target -> h;

        SDL_PixelFormat* fmt = target -> format;
	for(uint_fast32_t i = 0; i < target -> w; i++) {
		for(uint_fast32_t j = 0; j < target -> h; j++) {
			int coord = i + j * target -> w;

			int dx = (i - x) * pix_w;
			int dy = (j - y) * pix_h;
			
			#ifdef USE_MANHATTAN
			double dist = abs(dx) + abs(dy);
			#else
			double dist = sqrt(dx * dx + dy * dy);
			#endif

			uint8_t oldr, oldg, oldb;
                        SDL_GetRGB(data[coord], fmt, &oldr, &oldg, &oldb);
                        
                        uint8_t r = min((inten * red / (dist + 1)), 255 - oldr);
			uint8_t g = min((inten * green / (dist + 1)), 255 - oldg);
			uint8_t b = min((inten * blue / (dist + 1)), 255 - oldb);
                        uint32_t pix = SDL_MapRGB(fmt, r, g, b); 
			data[coord] += pix;

		}
	}
}

void step_clear(SDL_Surface* target)
{
        uint32_t* data = (uint32_t *) target -> pixels;

        for(int i = 0; i < target -> w * target -> h; i++) {
                if(data[i] > 1) data[i] -= 2;
        }
}

SDL_Surface* create_surface(uint32_t width, uint32_t height)
{
        uint32_t rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        rmask = 0xff000000;
        gmask = 0x00ff0000;
        bmask = 0x0000ff00;
        amask = 0x000000ff;
#else
        rmask = 0x000000ff;
        gmask = 0x0000ff00;
        bmask = 0x00ff0000;
        amask = 0xff000000;
#endif

        if(rmask == 0xff000000) {
                printf("Created big endian surface\n");
        } else if(rmask == 0x000000ff) {
                printf("Created little endian surface\n");
        } else {
                printf("Unknown byte order\n");
        }
        SDL_Surface* ret = SDL_CreateRGBSurface(0, width, height, 32, rmask, gmask, bmask, amask);
        return ret;
}


int main(int argc, int argv)
{
        const int width = window_width;
        const int height = window_height;
        int x = 100, y = 100;
        
        SDL_Window* window = SDL_CreateWindow("Stars", 100, 100, width, height, SDL_WINDOW_SHOWN);
        SDL_Surface* surf = SDL_GetWindowSurface(window);
        SDL_Surface* back = create_surface(160, 120);

        int running = 1;
        while(running) {
        	SDL_Event event;
        	while(SDL_PollEvent(&event)) {
        		if(event.type == SDL_QUIT) {
        			running = 0;
        		}
        		if(event.type == SDL_MOUSEMOTION) {
        			x = event.motion.x * ((float)back -> w / width);
        			y = event.motion.y * ((float)back -> h / height);
        		}
        	}

                uint32_t black = SDL_MapRGB(surf -> format, 0, 0, 0);
        	SDL_FillRect(back, NULL, black);

                draw_star(back, 80, 60, 60, 255, 128, 255);
                draw_star(back, x, y, 50, 255, 128, 1);
                draw_star(back, x - 55, y, 25, 120, 251, 22);
                draw_star(back, x + 55, y, 25, 1, 25, 255);
                draw_star(back, x, y - 55, 25, 20, 201, 21);
                draw_star(back, x, y + 55, 25, 201, 251, 231);                
                SDL_BlitScaled(back, NULL, surf, NULL);       
        	SDL_UpdateWindowSurface(window);
        }
}

 
















