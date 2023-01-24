# golpng

Game of life applied on png images with edge detection.

![illustration](golpng.gif)


## prerequisites:

tup, sdl1

## Build

```bash
tup
```

## Run

```bash
./build-default/src/pics/run <path-to-image.png>
```

while running, press right to apply edge detection and saturate, up to start/stop game of life and down to save picture as resultimage.png . Press i to only do one iteration of game of life.

The edge detection is necessary on photos, but maybe not so much on computer generated images with large areas with the same color.
