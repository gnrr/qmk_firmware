rebellious
========

![photo](docs/rebellious-photo.jpg)
[layout](docs/rebellious-keymap.pdf)

[schematic](docs/rebellious-sch.pdf)

rebellious - A keyboard with trackball and scroll ring

- Keyboard maintainer: hidsh
- Hardware supported: 
    - rebellious003 PCB
    - atmega32u4-5V
    - Kensington Orbit with Scroll Ring
- Hardware availability:
    - [In progress] DIY-Kit consists of PCBs and 3D-printed case

# Building the firmware

[Install the build tools.](https://docs.qmk.fm/#/getting_started_build_tools)

In the root directory of the repository, build the firmware with a command like:

```
make rebellious:default
```

and/or flashing firmware:

```
make rebellious:default:avrdude
```


For more information on the layout option and other ones, see the [`make` guide](https://docs.qmk.fm/#/getting_started_make_guide).

