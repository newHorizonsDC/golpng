# golpng
simulate game of life with greyscaled / laplace saturated pngs as input

supports debian based systems

prerequisites:

install tup:
```
./scripts/getTup.sh
```
install sdl2:
```
sudo apt-get install libsdl2-dev
```

build:
```bash
tup
```

run:
```bash
./build-default/src/run <image.png>
```

while running, press right to apply laplace and saturate, up to start/stop game of life and down to save picture as resultimage.png
