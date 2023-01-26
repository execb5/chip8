# Chip8

![c workflow](https://github.com/execb5/chip8/actions/workflows/c.yml/badge.svg)

My take on a chip8 emulator.

---

https://user-images.githubusercontent.com/6979113/214741148-686045ed-f662-4373-82ff-09b5391c4fc4.mp4

---

## Dependencies

- [SDL](https://www.libsdl.org/) - For graphics.
- [cmocka](https://cmocka.org/) - For running the unit tests.

## Compiling

```bash
# With the dependencies properly installed
make
```

## Running

- Unit tests

```bash
make run_test
```

- main

```bash
# example for pong
make run ARGS="20 1 roms/pong.ch8"
```

## Notes

I really liked how the [`instructions.h`](inc/instructions.h) ended up,
since it has the proper documentation for every instruction.
