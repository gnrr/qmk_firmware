DME - dactyl-manuform-egured 
========

![layout](docs/dme_keymap.svg)

[schematic](docs/dme_sch.pdf)

The DME - dactyl-manuform-egured is a modified version of the Dactyl Manuform. It has different firmware based on ErgoDone.

- Keyboard maintainer: hidsh
- Hardware supported: 
    - dme 001
- Hardware availability:
    under constrution

# Building the firmware

[Install the build tools.](https://docs.qmk.fm/#/getting_started_build_tools)

In the root directory of the repository, build the firmware with a command like:

```
make handwired/dme:4x5
```

and/or flashing firmware:

```
make handwired/dme:default:avrdude
```


For more information on the layout option and other ones, see the [`make` guide](https://docs.qmk.fm/#/getting_started_make_guide).

