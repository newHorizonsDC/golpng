#include "lodepng.h"
#include <SDL/SDL.h>
#include <iostream>

template<typename Lambda>
int show(const std::string& caption, unsigned char* rgba, unsigned w, unsigned h, Lambda&& action)
{
  //init SDL
  if(SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    std::cout << "error, SDL video init failed" << std::endl;
    return 0;
  }
  SDL_Surface* scr = SDL_SetVideoMode(w, h, 32, SDL_HWSURFACE);
  if(!scr)
  {
    std::cout << "error, no SDL screen" << std::endl;
    return 0;
  }
  SDL_WM_SetCaption(caption.c_str(), NULL); //set window caption

  //plot the pixels of the PNG file
  for(unsigned y = 0; y < h; ++y)
  for(unsigned x = 0; x < w; ++x)
  {
    //get RGBA components
    Uint32 r = rgba[4 * y * w + 4 * x + 0]; //red
    Uint32 g = rgba[4 * y * w + 4 * x + 1]; //green
    Uint32 b = rgba[4 * y * w + 4 * x + 2]; //blue
    Uint32 a = rgba[4 * y * w + 4 * x + 3]; //alpha

    //make translucency visible by placing checkerboard pattern behind image
    int checkerColor = 191 + 64 * (((x / 16) % 2) == ((y / 16) % 2));
    r = (a * r + (255 - a) * checkerColor) / 255;
    g = (a * g + (255 - a) * checkerColor) / 255;
    b = (a * b + (255 - a) * checkerColor) / 255;

    //give the color value to the pixel of the screenbuffer
    int* bufp = (int*)scr->pixels + (y * scr->pitch / 4) + x;
    *bufp = 65536 * r + 256 * g + b;
  }

  int done = std::forward<Lambda>(action)(scr,w,h, rgba);
  SDL_Quit();
  return done == 2 ? 1 : 0;
}

void sdl_to_lodepng(unsigned char* rgba, const SDL_Surface* scr, unsigned w, unsigned h)
{
  for (unsigned i=0; i<h; ++i)
    for (unsigned j=0; j<w; ++j)
    {
      int* pixel = ((int*)(scr->pixels))+i*w + j;
      ((int*)(&rgba[0]))[i*w + j] = (*pixel)|0xff000000;
    }
}
