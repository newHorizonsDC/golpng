//g++ -std=c++11 -lSDL pics.cpp lodepng.o

#include "lodepng.h"
#include <iostream>
#include <SDL/SDL.h>
#include "utils.h"
#include <cmath>

const int white = 0xffffffff;

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
    show(filename, &image[0], width, height, [](SDL_Surface* scr, unsigned w, unsigned h, unsigned char* rgba) -> int {
        for (unsigned i=1;i<h-1;++i)
          for (unsigned j=1;j<w-1;++j)
          {
            //std::cout << "\t"<< std::hex << ((int*)(scr->pixels))[i*w + j];
            int * pixel = ((int*)(scr->pixels))+i*w + j;
            int light = std::sqrt(pow(((*pixel)>>16)&0xff,2)+pow(((*pixel)>>8)&0xff,2)+pow((*pixel)&0xff,2));
            if (light>0xff)light=0xff;
            else if (light<0)light=0;
            *pixel = light*0x10101;
          }
      SDL_Event event;
      int done = 0;
      int steps = 0;
      bool iterate = false;
      bool gol = false;
      SDL_Surface* nscr;
      nscr = SDL_CreateRGBSurface(0, scr->w, scr->h, 32, 0, 0, 0, 0);
      //nscr->pixels = new unsigned char[scr->w * scr->h * scr->format->BytesPerPixel];
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
              else if(event.type == SDL_KEYDOWN)
              {
                  if (event.key.keysym.sym == SDLK_RIGHT)
                      iterate = true;
                  else if (event.key.keysym.sym == SDLK_DOWN)
                  {
                      sdl_to_lodepng(rgba, scr, w, h);
                      const char* filename = "resultimage.png";
                      lodepng::encode(filename, rgba, w, h);
                  }
                  else if (event.key.keysym.sym == SDLK_UP)
                      gol = !gol;
              }
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
                        if (laplace > 0xff/6) laplace=0xff;
                        else laplace=0;

                        *pixel = laplace;
                    }
                iterate = false;
            }
            else if(gol)
            {
                //nscr = SDL_ConvertSurface(scr, scr->format, 0);
                memcpy(nscr->pixels, scr->pixels, scr->w*scr->h*scr->format->BytesPerPixel);
                for (unsigned i=0;i<h;++i){
                  ((int*)(scr->pixels))[i*w] = 0;
                  ((int*)(scr->pixels))[i*w+w-1] = 0;
                }
                for (unsigned j=0;j<w;++j){
                  ((int*)(scr->pixels))[j] = 0;
                  ((int*)(scr->pixels))[(h-1)*w+j] = 0;
                }

                for (unsigned i=1;i<h-1;++i)
                    for (unsigned j=1;j<w-1;++j)
                    {
                        int * p = ((int*)(scr->pixels))+i*w+j;
                        //if (*p!=0)std::cout << std::hex << *p << " ";
                        int * np = ((int*)(nscr->pixels))+i*w+j;
                        int alive =
                        (*(p+w-1)&1) + (*(p+w)&1) + (*(p+w+1)&1)+
                        (*(p-1)  &1) + (*(p)  &1) + (*(p+1)  &1)+
                        (*(p-w-1)&1) + (*(p-w)&1) + (*(p-w+1)&1);
                        //if (*p!=0)std::cout << std::hex << alive << " ";

                        switch (alive){
                            case 3:
                              *np = white;;
                            break;
                            case 4:
                              *np = *p;
                            break;
                            //case 6:
                            //  *np = ~(*p);
                            //break;
                            default:
                              *np = 0;
                            break;
                        }
                    }
                memcpy(scr->pixels, nscr->pixels, scr->w*scr->h*scr->format->BytesPerPixel);
            }
            SDL_UpdateRect(scr, 0, 0, 0, 0);
            SDL_Delay(5);
      }
      return done;
    });
}
