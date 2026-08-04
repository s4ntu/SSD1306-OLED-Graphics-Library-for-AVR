/******************************************************************************
 * @file    oled.cpp
 * @brief   Implementación de la librería OLED SSD1306.
 *
 * Autor: Santiago Gomez
 * Plataforma: AVR
 * Controlador: SSD1306
 * Comunicación: I2C
 ******************************************************************************/

#include <oled.h>
#define WIDTH 128
#define HEIGHT 64
#define OLED_BLACK 0
#define OLED_WHITE 1
#define SWAP(a, b)            \
  do                          \
  {                           \
    __typeof__(a) temp = (a); \
    (a) = (b);                \
    (b) = temp;               \
  } while (0)
uint8_t oled_buffer[WIDTH * HEIGHT / 8];

//=====================================================
// Funciones privadas
//=====================================================

static void oled_send_data(const uint8_t *data, size_t length)
{
  i2c_start();
  i2c_write(SSD1306_ADDR << 1); // Dirección con bit de escritura
  i2c_write(SSD1306_DATA);      // Enviar datos

  for (size_t i = 0; i < length; i++)
  {
    i2c_write(data[i]);
  }

  i2c_stop();
};

static void oled_set_page_address(uint8_t x, uint8_t y)
{
  i2c_start();
  i2c_write(SSD1306_ADDR << 1); // Dirección con bit de escritura
  i2c_write(SSD1306_COMMAND);   // Modo de comando

  // Configurar la columna baja y alta
  i2c_write(0x00 | (x & 0x0F));        // Dirección columna baja
  i2c_write(0x10 | ((x >> 4) & 0x0F)); // Dirección columna alta

  // Configurar la página (dividir 'y' entre 8 porque cada página es de 8 píxeles)
  i2c_write(0xB0 | (y / 8)); // Página (0 a 7)

  i2c_stop();
};

static void oled_send_command(uint8_t cmd)
{
  i2c_start();
  i2c_write(SSD1306_ADDR << 1);
  i2c_write(SSD1306_COMMAND);
  i2c_write(cmd);
  i2c_stop();
}

//=====================================================
// Inicialización y Gestión del framebuffer
//=====================================================

void oled_init()
{
  i2c_start();
  i2c_write(SSD1306_ADDR << 1); // Dirección con bit de escritura
  i2c_write(SSD1306_COMMAND);   // Enviar comando
  i2c_write(0xAE);              // Apagar el display
  i2c_write(0xD5);              // Configurar la frecuencia del reloj
  i2c_write(0x80);              // Valor de configuración
  i2c_write(0xA8);              // Configurar el multiplex
  i2c_write(HEIGHT - 1);        // Valor de multiplex
  i2c_write(0xD3);              // Desplazamiento vertical
  i2c_write(0x00);              // Sin desplazamiento
  i2c_write(0x40);              // Configurar el inicio de la línea
  i2c_write(0x8D);              // Habilitar la carga de pre-carga
  i2c_write(0x14);              // Habilitar
  i2c_write(0x20);              // Establecer modo de dirección
  i2c_write(0x00);              // Horizontal
  i2c_write(0xA1);              // Configurar el segmento
  i2c_write(0xC8);              // Configurar la salida
  i2c_write(0xDA);              // Configurar la resistencia de tiras
  i2c_write(0x12);              // Configurar
  i2c_write(0x81);              // Configurar el contraste
  i2c_write(0x7F);              // Valor de contraste
  i2c_write(0xA4);              // Habilitar el modo de salida
  i2c_write(0xAF);              // Encender el display
  i2c_stop();
};

void oled_update(void)
{
  oled_set_page_address(0, 0);
  oled_send_data(oled_buffer, sizeof(oled_buffer));
}

void oled_clear(void)
{
  memset(oled_buffer, 0, sizeof(oled_buffer));
}

void oled_clear_area(uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{
  for (uint8_t i = 0; i < height; i++)
  {
    for (uint8_t j = 0; j < width; j++)
    {
      oled_draw_pixel(x + j, y + i, OLED_BLACK);
    }
  }
}

void oled_fill(uint8_t color)
{
  memset(oled_buffer, color ? 0xFF : 0x00, sizeof(oled_buffer));
}

void oled_scroll_pages(uint8_t pages, scroll_direction direction)
{
  if(direction == scroll_direction::Down){
     if (pages == 0 || pages >= HEIGHT / 8)
        return;

    uint8_t total_pages = HEIGHT / 8;

    // Mover páginas hacia abajo
    for (int8_t page = total_pages - 1; page >= pages; page--)
    {
        for (uint8_t x = 0; x < WIDTH; x++)
        {
            oled_buffer[page * WIDTH + x] =
                oled_buffer[(page - pages) * WIDTH + x];
        }
    }

    // Limpiar las páginas nuevas del inicio
    for (uint8_t page = 0; page < pages; page++)
    {
        for (uint8_t x = 0; x < WIDTH; x++)
        {
            oled_buffer[page * WIDTH + x] = 0x00;
        }
    }
  }else if (direction == scroll_direction::Up)
  {
    if (pages == 0 || pages >= HEIGHT / 8)
        return;

    uint8_t total_pages = HEIGHT / 8;

    // Mover páginas hacia arriba
    for (uint8_t page = 0; page < total_pages - pages; page++)
    {
        for (uint8_t x = 0; x < WIDTH; x++)
        {
            oled_buffer[page * WIDTH + x] =
                oled_buffer[(page + pages) * WIDTH + x];
        }
    }

    // Limpiar las páginas nuevas del final
    for (uint8_t page = total_pages - pages; page < total_pages; page++)
    {
        for (uint8_t x = 0; x < WIDTH; x++)
        {
            oled_buffer[page *  WIDTH + x] = 0x00;
        }
    }
  }else return;
    
}

//=====================================================
// Primitivas de dibujo
//=====================================================

void oled_draw_pixel(uint8_t x, uint8_t y, uint8_t color)
{
  // Verificar límites
  if (x >= WIDTH || y >= HEIGHT)
    return;

  // Calcular el byte correspondiente
  uint16_t byte_index = (y / 8) * WIDTH + x;

  // Bit dentro del byte
  uint8_t bit_mask = (1 << (y % 8));

  if (color == OLED_WHITE)
  {
    // Encender el píxel
    oled_buffer[byte_index] |= bit_mask;
  }
  else
  {
    // Apagar el píxel
    oled_buffer[byte_index] &= ~bit_mask;
  }
}

void oled_draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color)
{
  int dx = abs(x1 - x0);
  int dy = -abs(y1 - y0);
  int sx = (x0 < x1) ? 1 : -1;
  int sy = (y0 < y1) ? 1 : -1;
  int err = dx + dy;

  while (true)
  {
    oled_draw_pixel(x0, y0, color);
    if (x0 == x1 && y0 == y1)
      break;
    int e2 = 2 * err;
    if (e2 >= dy)
    {
      err += dy;
      x0 += sx;
    }
    if (e2 <= dx)
    {
      err += dx;
      y0 += sy;
    }
  }
}

void oled_draw_rectangle(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height, uint8_t color)
{
  oled_draw_line(x0, y0, x0 + width - 1, y0, color);                           // Línea superior
  oled_draw_line(x0, y0 + height - 1, x0 + width - 1, y0 + height - 1, color); // Línea inferior
  oled_draw_line(x0, y0, x0, y0 + height - 1, color);                          // Línea izquierda
  oled_draw_line(x0 + width - 1, y0, x0 + width - 1, y0 + height - 1, color);  // Línea derecha
}

void oled_draw_rectangle_filled(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height, uint8_t color)
{
  for (int y = y0; y < y0 + height; y++)
  {
    oled_draw_line(x0, y, x0 + width - 1, y, color);
  }
}

void oled_draw_circle(uint8_t x0, uint8_t y0, uint8_t radius, uint8_t color)
{
  int x = 0;
  int y = radius;
  int d = 3 - 2 * radius;

  while (y >= x)
  {
    oled_draw_pixel(x0 + x, y0 + y, color);
    oled_draw_pixel(x0 - x, y0 + y, color);
    oled_draw_pixel(x0 + x, y0 - y, color);
    oled_draw_pixel(x0 - x, y0 - y, color);

    oled_draw_pixel(x0 + y, y0 + x, color);
    oled_draw_pixel(x0 - y, y0 + x, color);
    oled_draw_pixel(x0 + y, y0 - x, color);
    oled_draw_pixel(x0 - y, y0 - x, color);

    x++;

    if (d > 0)
    {
      y--;
      d += 4 * (x - y) + 10;
    }
    else
    {
      d += 4 * x + 6;
    }
  }
}

void oled_draw_circle_filled(uint8_t x0, uint8_t y0, uint8_t radius, uint8_t color)
{
  int x = 0;
  int y = radius;
  int d = 3 - 2 * radius;

  while (y >= x)
  {
    oled_draw_line(x0 - x, y0 + y, x0 + x, y0 + y, color);
    oled_draw_line(x0 - x, y0 - y, x0 + x, y0 - y, color);

    oled_draw_line(x0 - y, y0 + x, x0 + y, y0 + x, color);
    oled_draw_line(x0 - y, y0 - x, x0 + y, y0 - x, color);

    x++;

    if (d > 0)
    {
      y--;
      d += 4 * (x - y) + 10;
    }
    else
    {
      d += 4 * x + 6;
    }
  }
}

void oled_draw_triangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color)
{
  oled_draw_line(x0, y0, x1, y1, color);
  oled_draw_line(x1, y1, x2, y2, color);
  oled_draw_line(x2, y2, x0, y0, color);
}

void oled_draw_triangle_filled(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color)
{
  // Ordenar vértices por Y (y0 <= y1 <= y2)
  if (y0 > y1)
  {
    SWAP(y0, y1);
    SWAP(x0, x1);
  }

  if (y1 > y2)
  {
    SWAP(y1, y2);
    SWAP(x1, x2);
    SWAP(x1, x2);
  }

  if (y0 > y1)
  {
    SWAP(y0, y1);
    SWAP(x0, x1);
  }

  // Caso especial: todos los puntos en la misma línea
  if (y0 == y2)
  {
    int a = x0;
    int b = x0;

    if (x1 < a)
      a = x1;
    if (x1 > b)
      b = x1;
    if (x2 < a)
      a = x2;
    if (x2 > b)
      b = x2;

    oled_draw_line(a, y0, b, y0, color);
    return;
  }

  int dx01 = x1 - x0;
  int dy01 = y1 - y0;

  int dx02 = x2 - x0;
  int dy02 = y2 - y0;

  int dx12 = x2 - x1;
  int dy12 = y2 - y1;

  int sa = 0;
  int sb = 0;

  int y;
  int last = (y1 == y2) ? y1 : (y1 - 1);

  // Parte superior
  for (y = y0; y <= last; y++)
  {
    int a = x0 + sa / dy01;
    int b = x0 + sb / dy02;

    sa += dx01;
    sb += dx02;

    if (a > b)
      SWAP(a, b);

    oled_draw_line(a, y, b, y, color);
  }

  // Parte inferior
  sa = dx12 * (y - y1);
  sb = dx02 * (y - y0);

  for (; y <= y2; y++)
  {
    int a = x1 + sa / dy12;
    int b = x0 + sb / dy02;

    sa += dx12;
    sb += dx02;

    if (a > b)
      SWAP(a, b);

    oled_draw_line(a, y, b, y, color);
  }
}

//=====================================================================
// Texto
//=====================================================================

void oled_draw_char(uint8_t x, uint8_t y, char c, uint8_t color, text_size scale)
{
    if (c < 32 || c > 127)
        return;

    for (uint8_t i = 0; i < 6; i++)
    {
        uint8_t line = pgm_read_byte(&font[c - 32][i]);

        for (uint8_t j = 0; j < 8; j++)
        {
            if ((line >> j) & 1)
            {
                for (uint8_t dx = 0; dx < scale; dx++)
                {
                    for (uint8_t dy = 0; dy < scale; dy++)
                    {
                        oled_draw_pixel(
                            x + (i * scale) + dx,
                            y + (j * scale) + dy,
                            color
                        );
                    }
                }
            }
        }
    }
}

void oled_draw_string(uint8_t x, uint8_t y, const char *str, uint8_t color, text_size scale)
{
    while (*str != '\0')
    {
        oled_draw_char(x, y, *str, color, scale);

        x += 6 * scale;

        if (x >= WIDTH)
        {
            x = 0;
            y += 8 * scale;

            if (y >= HEIGHT)
            {
                y = 0;
            }
        }

        str++;
    }
}

//=====================================================================
// Bitmaps y sprites
//=====================================================================

void oled_draw_bitmap(uint8_t x, uint8_t y, const uint8_t *bitmap, uint8_t width, uint8_t height, uint8_t color)
{
    uint16_t bytes_per_row = (width + 7) / 8;

    for (uint8_t j = 0; j < height; j++)
    {
        for (uint8_t i = 0; i < width; i++)
        {
            uint16_t byte_index = (j * bytes_per_row) + (i / 8);

            uint8_t bit_mask = 0x80 >> (i % 8); 

            uint8_t current_byte = pgm_read_byte(&bitmap[byte_index]);

            if (current_byte & bit_mask)
            {
                oled_draw_pixel(x + i, y + j, color);
            }
        }
    }
}

void oled_sprite_animation(uint8_t x, uint8_t y,const uint8_t *bitmap,uint8_t bitmap_width,
uint8_t bitmap_height,uint8_t frame_width,uint8_t frame_height,uint8_t color, uint16_t delay_ms)
{
    // Cantidad de sprites por fila
    uint8_t frames_per_row = bitmap_width / frame_width;

    // Cantidad de filas de sprites
    uint8_t rows = bitmap_height / frame_height;

    // Total de sprites
    uint16_t total_frames = frames_per_row * rows;

    for (uint16_t frame = 0; frame < total_frames; frame++)
    {
        uint8_t sprite_x = (frame % frames_per_row) * frame_width;
        uint8_t sprite_y = (frame / frames_per_row) * frame_height;

        oled_clear_area(x, y, frame_width, frame_height);

        oled_draw_bitmap_frame(
            x,
            y,
            sprite_x,
            sprite_y,
            bitmap,
            bitmap_width,
            frame_width,
            frame_height,
            color);

        oled_update();

        _delay_ms(delay_ms);
    }
}

void oled_draw_bitmap_frame(uint8_t x,uint8_t y,uint8_t sprite_x,uint8_t sprite_y,const uint8_t *bitmap,uint8_t bitmap_width,uint8_t frame_width,uint8_t frame_height, uint8_t color)
{
    uint16_t bitmap_bytes_per_row = (bitmap_width + 7) / 8;

    for (uint8_t j = 0; j < frame_height; j++)
    {
        for (uint8_t i = 0; i < frame_width; i++)
        {
            // Coordenadas dentro del bitmap completo
            uint16_t bitmap_x = sprite_x + i;
            uint16_t bitmap_y = sprite_y + j;

            // Byte donde está ese píxel
            uint16_t byte_index =
                bitmap_y * bitmap_bytes_per_row +
                (bitmap_x / 8);

            // Bit correspondiente
            uint8_t bit_mask = 0x80 >> (bitmap_x % 8);

            uint8_t current_byte = pgm_read_byte(&bitmap[byte_index]);

            if (current_byte & bit_mask)
            {
                oled_draw_pixel(x + i, y + j, color);
            }
        }
    }
}

//=====================================================
// Funciones de utilidad
//=====================================================

void oled_set_contrast(uint8_t contrast)
{
    oled_send_command(0x81); // Comando para establecer el contraste
    oled_send_command(contrast);
}

void oled_invert_display(void)
{
    static bool invert = true;

    if (invert) {
        oled_send_command(0xA7);
    } else {
        oled_send_command(0xA6);
    }

    invert = !invert;
}

void oled_scroll(uint8_t start_page,uint8_t end_page,uint8_t speed,scroll_direction direction,uint8_t vertical_offset)
{
    // Si es scroll diagonal
    if (direction == scroll_direction::VerticalRight ||
        direction == scroll_direction::VerticalLeft)
    {
        oled_send_command(0xA3);
        oled_send_command(0x00);      // Top Fixed Area
        oled_send_command(HEIGHT);      // Scroll Area (64 líneas)
    }

    oled_send_command(static_cast<uint8_t>(direction));

    oled_send_command(0x00);
    oled_send_command(start_page);
    oled_send_command(speed);
    oled_send_command(end_page);

    if (direction == scroll_direction::VerticalRight ||
        direction == scroll_direction::VerticalLeft)
    {
        oled_send_command(vertical_offset);
    }
    else
    {
        oled_send_command(0x00);
        oled_send_command(0xFF);
    }

    oled_send_command(0x2F);
}

void oled_scroll_stop(void){
    oled_send_command(0x2E);
}
