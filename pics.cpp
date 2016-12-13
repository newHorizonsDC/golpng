#include "lodepng.h"
#include <iostream>
#include <SDL/SDL.h>
#include "utils.h"


int main(int argc, char *argv[])
{
  const char* filename = argc > 1 ? argv[1] : "test.png";

  std::vector<unsigned char> image; //the raw pixels
  unsigned width=0, height=0;

  unsigned error = lodepng::decode(image, width, height, filename);
  if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

  for (unsigned i=0; i<height; i++)
    for (unsigned j=0; j<width/4; j++)
    {
      //std::cout << "\t"<< std::hex << *((int*)(&image[i*width + 4*j]));
    }
  show(filename, &image[0], width, height, [](SDL_Surface* scr, unsigned w, unsigned h) -> int {
    SDL_Event event;
    int done = 0;
    while(done == 0)
    {
      while(SDL_PollEvent(&event))
      {
        if(event.type == SDL_QUIT) done = 2;
        else if(SDL_GetKeyState(NULL)[SDLK_ESCAPE]) done = 2;
        else if(event.type == SDL_KEYDOWN) done = 1;
      }
      for (unsigned i=0;i<h;++i)
        for (unsigned j=0;j<w;++j)
        {
          //std::cout << "\t"<< std::hex << ((int*)(scr->pixels))[i*w + j];
          //((int*)(scr->pixels))[i*w + j] = 0x000000;
        }
      SDL_UpdateRect(scr, 0, 0, 0, 0);
      SDL_Delay(5);
    }
    return done;
  });



  
}
