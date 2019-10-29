DME - dactyl-manuform-egured 
========

![layout](docs/deck1_keymap.svg)

[schematic](docs/deck1_sch.pdf)

DECK1 - A keyboard with trackball and scroll ring

- Keyboard maintainer: hidsh
- Hardware supported: 
    - bastet001 PCB
    - adafruit ItsyBitsy32u4 5V
    - Kensington OrbitTrackball with Scroll Ring
- Hardware availability:
    - not yet

# Building the firmware

[Install the build tools.](https://docs.qmk.fm/#/getting_started_build_tools)

In the root directory of the repository, build the firmware with a command like:

```
make handwired/deck1:default
```

and/or flashing firmware:

```
make handwired/deck1:default:avrdude
```


For more information on the layout option and other ones, see the [`make` guide](https://docs.qmk.fm/#/getting_started_make_guide).

