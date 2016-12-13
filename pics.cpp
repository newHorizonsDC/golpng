#include "lodepng.h"
#include <iostream>
#include <SDL/SDL.h>
#include "utils.h"
#include <cmath>


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
      for (unsigned i=1;i<h-1;++i)
        for (unsigned j=1;j<w-1;++j)
        {
          //std::cout << "\t"<< std::hex << ((int*)(scr->pixels))[i*w + j];
          int * pixel = ((int*)(scr->pixels))+i*w + j;
          int light = std::sqrt(pow(((*pixel)>>16)&0xff,2)+pow(((*pixel)>>8)&0xff,2)+pow((*pixel)&0xff,2));
          if (light>0xff)light=0xff;
          *pixel = light*0x10101;
        }
    SDL_Event event;
    int done = 0;
    int steps = 0;
    bool iterate = false;
    while(done == 0)
    {
      /*if (steps<100){
        ++steps;
        SDL_Delay(5);
        continue;
      }else
        steps=0;*/
      while(SDL_PollEvent(&event))
      {
        if(event.type == SDL_QUIT) done = 2;
        else if(SDL_GetKeyState(NULL)[SDLK_ESCAPE]) done = 2;
        else if(event.type == SDL_KEYDOWN) iterate = true;
      }
      if (iterate)
      {
        for (unsigned i=1;i<h-1;++i)
          for (unsigned j=1;j<w-1;++j)
          {
            //std::cout << "\t"<< std::hex << ((int*)(scr->pixels))[i*w + j];
            int * pixel = ((int*)(scr->pixels))+i*w + j;
            char laplace = 4*((*pixel)&0xff) - (*(pixel-1))&0xff - (*(pixel+1))&0xff - (*(pixel-w))&0xff - (*(pixel+1))&0xff;
            if (laplace < 0) laplace = 0;
            else if (laplace > 0xff) laplace = 0xff;

            //Thresholding
            if (laplace > 0xff/4) laplace=0xff;
            else laplace=0;

            *pixel = 0x10101*laplace;
          }
        iterate = false;
      }
      SDL_UpdateRect(scr, 0, 0, 0, 0);
      SDL_Delay(5);
    }
    return done;
  });



  
}
