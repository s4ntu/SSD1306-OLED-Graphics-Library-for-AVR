#include <avr/io.h>        // Librería para operaciones de entrada y salida en microcontroladores AVR
#include <util/delay.h>    // Librería para retrasos en AVR
#include <font.h>          // Posiblemente una librería personalizada para el manejo de fuentes (texto) en el OLED
#include <i2c.h>           // Librería para la comunicación I2C
#include <oled.h>          // Librería para manejar el display OLED

int main()
{
  i2c_init();
  oled_init();

  oled_set_contrast(0xFF);

      // ============================
      // TEST CLEAR + PIXEL
      // ============================

      oled_clear();

      oled_draw_pixel(10, 10, OLED_WHITE);
      oled_draw_pixel(20, 20, OLED_WHITE);

      oled_update();

      _delay_ms(1000);


      // ============================
      // TEST LINEAS
      // ============================

      oled_clear();

      oled_draw_line(0, 0, 127, 63, OLED_WHITE);
      oled_draw_line(0, 63, 127, 0, OLED_WHITE);
      oled_draw_line(0, 32, 127, 32, OLED_WHITE);
      oled_draw_line(64, 0, 64, 63, OLED_WHITE);

      oled_update();

      _delay_ms(1000);


      // ============================
      // TEST RECTANGULOS
      // ============================

      oled_clear();

      oled_draw_rectangle(10, 22, 40, 20, OLED_WHITE);
      oled_draw_rectangle_filled(70, 22, 40, 20, OLED_WHITE);

      oled_update();

      _delay_ms(1000);


      // ============================
      // TEST CIRCULOS
      // ============================

      oled_clear();

      oled_draw_circle(32, 32, 20, OLED_WHITE);
      oled_draw_circle_filled(96, 32, 15, OLED_WHITE);

      oled_update();

      _delay_ms(1000);


      // ============================
      // TEST TRIANGULOS
      // ============================

      oled_clear();

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

      oled_update();

      _delay_ms(1000);


      // ============================
      // TEST TEXTO
      // ============================

      oled_clear();

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

      oled_update();

      _delay_ms(1000);


      // ============================
      // TEST BITMAP
      // ============================

      oled_clear();

      oled_draw_bitmap(
          0,
          0,
          epd_bitmap_nashe,
          128,
          64,
          OLED_WHITE
      );

      oled_update();

      _delay_ms(2000);


      // ============================
      // TEST CLEAR AREA
      // ============================

      oled_clear();

      oled_draw_triangle_filled(
          30, 55,
          64, 20,
          100, 55,
          OLED_WHITE
      );

      oled_update();

      _delay_ms(1000);

      oled_clear_area(
          64,
          12,
          40,
          53
      );

      oled_update();

      _delay_ms(1000);


      // ============================
      // TEST INVERT DISPLAY
      // ============================

      oled_clear();

      oled_draw_string(
          10,
          25,
          "INVERT",
          OLED_WHITE,
          TEXT_BIG
      );

      oled_update();
      _delay_ms(200);
      oled_invert_display();
      oled_update();
      _delay_ms(200);
          oled_invert_display();
      oled_update();
      _delay_ms(200);
          oled_invert_display();
      oled_update();
      _delay_ms(200);
      oled_invert_display();
      oled_update();



      // ============================
      // TEST SCROLL HARDWARE
      // ============================

      oled_clear();

      oled_draw_string(
          0,
          20,
          "SCROLL",
          OLED_WHITE,
          TEXT_BIG
      );

      oled_update();

      _delay_ms(500);


      oled_scroll(
          0,
          7,
          0x00,
          scroll_direction::Right,
          0
      );


      _delay_ms(1500);

      oled_scroll_stop();


      // ============================
      // TEST SCROLL BUFFER
      // ============================

      oled_clear();

      oled_draw_string(
          32,
          42,
          "BUFFER",
          OLED_WHITE,
          TEXT_BIG
      );

      oled_update();

      for (size_t i = 0; i < 4; i++)
      {
         oled_scroll_pages(
              1,
              scroll_direction::Up
          );

          oled_update();

          _delay_ms(200);
      } 
      // ============================
      // TEST SPRITE ANIMATION
      // ============================

      oled_clear();

      while (1)
          {     
        oled_sprite_animation(56, 24, epd_bitmap_sprite, 32,32, 16, 16, OLED_WHITE, 200);

      }  
      return 0;
}