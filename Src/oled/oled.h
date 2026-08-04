/******************************************************************************
 * @file    oled.h
 * @brief   Librería gráfica para displays OLED SSD1306.
 ******************************************************************************/

#ifndef OLED_H
#define OLED_H

#include <util/delay.h>
#include <stddef.h>
#include <stdlib.h>
#include <avr/io.h>
#include <i2c.h>
#include <font.h>
#include <string.h>
#include <avr/pgmspace.h>

/**
 * @file oled.h
 * @brief Librería gráfica para displays OLED SSD1306 mediante I2C.
 *
 * La librería proporciona funciones para inicializar el display,
 * dibujar primitivas gráficas, texto, bitmaps, sprites y utilizar
 * las funciones de desplazamiento por hardware del controlador SSD1306.
 */

enum oled_config : uint8_t
{
    SSD1306 = 0,
    SH1106 = 1,
    SSD1306_ADDR = 0x3C,    ///< Dirección I2C del SSD1306.
    SSD1306_COMMAND = 0x00, ///< Byte de control para comandos.
    SSD1306_DATA = 0x40     ///< Byte de control para datos.
};

/**
 * @brief Cuadrantes para rectangulos.
 */

enum oled_quadrant : uint8_t
{
    OLED_QUADRANT_TOP_LEFT = 0x01,
    OLED_QUADRANT_TOP_RIGHT = 0x02,
    OLED_QUADRANT_BOTTOM_LEFT = 0x04,
    OLED_QUADRANT_BOTTOM_RIGHT = 0x08
};

/**
 * @brief Dimensiones del display Maximas.
 */

enum oled_dimensions : uint8_t
{
    OLED_MAX_WIDTH = 128,
    OLED_MAX_HEIGHT = 64
};

/**
 * @brief Color de dibujo.
 */

enum oled_color : uint8_t
{
    OLED_BLACK = 0, ///< Apaga el píxel.
    OLED_WHITE = 1  ///< Enciende el píxel.
};

/**
 * @brief Dirección del desplazamiento.
 */
enum scroll_direction : uint8_t
{
    Right = 0x26,         ///< Scroll horizontal hacia la derecha.
    Left = 0x27,          ///< Scroll horizontal hacia la izquierda.
    VerticalRight = 0x29, ///< Scroll diagonal hacia la derecha.
    VerticalLeft = 0x2A,  ///< Scroll diagonal hacia la izquierda.

    Up = 0,  ///< Scroll por software hacia arriba.
    Down = 1 ///< Scroll por software hacia abajo.
};

/**
 * @brief Tamaño del texto.
 */
enum text_size : uint8_t
{
    TEXT_NORMAL = 1, ///< Escala x1.
    TEXT_BIG = 2,    ///< Escala x2.
    TEXT_MEGA = 3,   ///< Escala x3.
    TEXT_HYPER = 4   ///< Escala x4.
};

enum oled_align_t : uint8_t
{
    OLED_ALIGN_TOP_LEFT,
    OLED_ALIGN_TOP_CENTER,
    OLED_ALIGN_TOP_RIGHT,

    OLED_ALIGN_CENTER_LEFT,
    OLED_ALIGN_CENTER,
    OLED_ALIGN_CENTER_RIGHT,

    OLED_ALIGN_BOTTOM_LEFT,
    OLED_ALIGN_BOTTOM_CENTER,
    OLED_ALIGN_BOTTOM_RIGHT
};

/**
 * @brief Inicializa el display OLED.
 */
void oled_init(oled_config config, uint8_t width, uint8_t height);

/**
 * @brief Envía el contenido del framebuffer al display.
 */
void oled_update(void);

/**
 * @brief Borra completamente el framebuffer.
 *
 * @note Es necesario llamar a oled_update() para reflejar los cambios.
 */
void oled_clear(void);

/**
 * @brief Borra un área rectangular del framebuffer.
 *
 * @param x Coordenada X.
 * @param y Coordenada Y.
 * @param width Ancho del área.
 * @param height Alto del área.
 */
void oled_clear_area(uint8_t x, uint8_t y, uint8_t width, uint8_t height);

/**
 * @brief Ajusta el contraste del display.
 *
 * @param contrast Valor entre 0x00 y 0xFF.
 */
void oled_set_contrast(uint8_t contrast);

/**
 * @brief Dibuja un píxel.
 */
void oled_draw_pixel(uint8_t x, uint8_t y, uint8_t color);

/**
 * @brief Dibuja una línea.
 */
void oled_draw_line(uint8_t x0,
                    uint8_t y0,
                    uint8_t x1,
                    uint8_t y1,
                    uint8_t color);

void oled_draw_round_rect(
    uint8_t x,
    uint8_t y,
    uint8_t width,
    uint8_t height,
    uint8_t radius,
    uint8_t color);
/**
 * @brief Dibuja un rectángulo.
 */
void oled_draw_rectangle(uint8_t x0, uint8_t y0,
                         uint8_t width, uint8_t height,
                         uint8_t color);

void oled_draw_round_rect_filled(
    uint8_t x,
    uint8_t y,
    uint8_t width,
    uint8_t height,
    uint8_t radius,
    uint8_t color);

/**
 * @brief Dibuja un rectángulo relleno.
 */
void oled_draw_rectangle_filled(uint8_t x0, uint8_t y0,
                                uint8_t width, uint8_t height,
                                uint8_t color);

/**
 * @brief Dibuja un círculo.
 */
void oled_draw_circle(uint8_t x0, uint8_t y0,
                      uint8_t radius,
                      uint8_t color);

/**
 * @brief Dibuja un círculo relleno.
 */
void oled_draw_circle_filled(uint8_t x0, uint8_t y0,
                             uint8_t radius,
                             uint8_t color);

/**
 * @brief Dibuja un triángulo.
 */
void oled_draw_triangle(uint8_t x0, uint8_t y0,
                        uint8_t x1, uint8_t y1,
                        uint8_t x2, uint8_t y2,
                        uint8_t color);

/**
 * @brief Dibuja un triángulo relleno.
 */
void oled_draw_triangle_filled(uint8_t x0,
                               uint8_t y0,
                               uint8_t x1,
                               uint8_t y1,
                               uint8_t x2,
                               uint8_t y2,
                               uint8_t color);

/**
 * @brief Dibuja un carácter.
 *
 * @param invert Invierte el color del carácter.
 * @param size Escala del texto.
 */
void oled_draw_char(uint8_t x, uint8_t y,
                    char c,
                    uint8_t invert,
                    text_size size);

/**
 * @brief Dibuja una cadena de texto.
 */
void oled_draw_string(uint8_t x, uint8_t y,
                      const char *str,
                      uint8_t color,
                      text_size size);

/**
 * @brief Dibuja una cadena de texto alineada.
 */
void oled_draw_string_aligned(int8_t padding_x,
                              int8_t padding_y,
                              const char *str,
                              uint8_t color,
                              text_size scale,
                              oled_align_t align);

/**
 * @brief Dibuja un bitmap.
 */
void oled_draw_bitmap(uint8_t x, uint8_t y,
                      const uint8_t *bitmap,
                      uint8_t width,
                      uint8_t height,
                      uint8_t color);

/**
 * @brief Dibuja un frame de un sprite sheet.
 *
 * @param sprite_x Posición X del frame dentro del bitmap.
 * @param sprite_y Posición Y del frame dentro del bitmap.
 * @param bitmap_width Ancho total del sprite sheet.
 * @param frame_width Ancho del frame.
 * @param frame_height Alto del frame.
 */
void oled_draw_sprite_frame(uint8_t x,
                            uint8_t y,
                            uint8_t sprite_x,
                            uint8_t sprite_y,
                            const uint8_t *bitmap,
                            uint8_t bitmap_width,
                            uint8_t frame_width,
                            uint8_t frame_height,
                            uint8_t color);

/**
 * @brief Invierte los colores del display.
 */
void oled_invert_display(void);

/**
 * @brief Inicia el desplazamiento por hardware del SSD1306.
 *
 * @param start_page Página inicial.
 * @param end_page Página final.
 * @param speed Velocidad del desplazamiento (0x00 - 0x07).
 * @param direction Dirección del scroll.
 * @param vertical_offset Desplazamiento vertical utilizado únicamente
 *        en los modos VerticalRight y VerticalLeft.
 */
void oled_scroll(uint8_t start_page,
                 uint8_t end_page,
                 uint8_t speed,
                 scroll_direction direction,
                 uint8_t vertical_offset);

/**
 * @brief Detiene el desplazamiento por hardware.
 */
void oled_scroll_stop(void);

/**
 * @brief Desplaza el framebuffer por páginas.
 *
 * @param pages Cantidad de páginas (8 píxeles cada una).
 * @param direction Dirección del desplazamiento.
 */
void oled_scroll_pages(uint8_t pages,
                       scroll_direction direction);

/**
 * @brief Reproduce una animación recorriendo todos los frames de un sprite sheet.
 *
 * @warning Esta función utiliza _delay_ms(), por lo que bloquea la ejecución
 * del microcontrolador durante la reproducción.
 *
 * @note Se incluye únicamente como ejemplo de uso. Para aplicaciones reales
 * se recomienda actualizar la animación mediante un temporizador o desde
 * el bucle principal del programa.
 */
void oled_sprite_animation(uint8_t x,
                           uint8_t y,
                           const uint8_t *bitmap,
                           uint8_t bitmap_width,
                           uint8_t bitmap_height,
                           uint8_t frame_width,
                           uint8_t frame_height,
                           uint8_t color,
                           uint16_t delay_ms);

#endif