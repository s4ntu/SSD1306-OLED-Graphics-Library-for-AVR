# SSD1306 OLED Graphics Library

A lightweight graphics library for SSD1306 OLED displays over I²C.

Designed for AVR microcontrollers with simplicity, portability and readability in mind.

---

<p align="center">
  <img src="Demo/Demo.gif" width="500">
</p>

## Features

✔ SSD1306 initialization

✔ Framebuffer rendering

✔ Pixel drawing

✔ Lines

✔ Rectangles

✔ Filled rectangles

✔ Circles

✔ Filled circles

✔ Triangles

✔ Filled triangles

✔ Text rendering

✔ Text scaling (x1, x2, x3, x4)

✔ Bitmap rendering

✔ Sprite sheet support

✔ Sprite animation example

✔ Hardware scrolling

✔ Software scrolling

✔ Display inversion

✔ Contrast control

---

## Supported Display

SSD1306

128×64 | 128x32

I²C Interface

---

## Example

```cpp
i2c_init();

oled_init();

oled_clear();

oled_draw_string(
    0,
    0,
    "Hello World",
    OLED_WHITE,
    TEXT_BIG
);

oled_update();
```

---

## Hardware Scroll

```cpp
oled_scroll(
    0,
    7,
    0x00,
    scroll_direction::Right,
    0
);
```

---

## Text Example

```cpp
oled_draw_char(
          0,
          0,
          'A',
          OLED_WHITE,
          TEXT_NORMAL
      );

      oled_draw_string(
          0,
          20,
          "SSD1306 :)",
          OLED_WHITE,
          TEXT_BIG
      );

      oled_draw_string(
          0,
          44,
          "Test OK",
          OLED_WHITE,
          TEXT_MEGA
      );
```
---

## Draw Triangles Example

```cpp
      oled_draw_triangle(
          20, 50,
          60, 10,
          100, 50,
          OLED_WHITE
      );

      oled_draw_triangle_filled(
          30, 55,
          64, 20,
          100, 55,
          OLED_WHITE
      );
```
---

## Sprite Example

```cpp
oled_draw_bitmap_frame(
    32,
    32,
    sprite_x,
    sprite_y,
    sprite,
    32,
    16,
    16,
    OLED_WHITE
);
```

---

## Sprite Anmiation Example

```cpp
oled_sprite_animation(
    56,
    24,
    epd_bitmap_sprite,
    32,
    32,
    16,
    16,
    OLED_WHITE,
    200
);
```
---

## Documentation

The library is fully documented using Doxygen-style comments inside **oled.h**.

---

## Examples

The repository includes several example projects:

- Basic usage
- Text rendering
- Drawing primitives
- Bitmaps
- Sprites
- Hardware scrolling
- Software scrolling
- Full API demonstration

---

## Notes

The function

```cpp
oled_sprite_animation()
```

uses `_delay_ms()` and is intended only as a demonstration.

For production applications it is recommended to update animations using timers or from the main loop.

---

# Español

Biblioteca gráfica para pantallas OLED SSD1306 mediante I²C.

Incluye funciones para:

- Inicialización
- Framebuffer
- Texto escalable
- Figuras geométricas
- Bitmaps
- Sprites
- Scroll por hardware
- Scroll por software
- Control de contraste
- Inversión del display

La librería está pensada para ser simple, ligera y fácil de integrar en proyectos AVR.

Se incluyen ejemplos completos para cada funcionalidad.

La función

```cpp
oled_sprite_animation()
```

es únicamente una demostración de uso y utiliza `_delay_ms()`.

Para aplicaciones reales se recomienda controlar las animaciones mediante un temporizador o desde el bucle principal.

---

## License

MIT License
