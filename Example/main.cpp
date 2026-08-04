#include <avr/io.h>
#include <util/delay.h>
#include <font.h>
#include <i2c.h>
#include <oled.h>

void demo_todas_las_funciones(void)
{
    // 1. LIMPIEZA INICIAL Y MARCO PERIMETRAL (Líneas y Rectángulos)
    oled_clear();
    oled_draw_rectangle(0, 0, 128, 64, OLED_WHITE); // Borde exterior

    // Encabezado alineado con texto pequeño
    oled_draw_string_aligned(0, 0, "AVR OLED DEMO", OLED_WHITE, TEXT_NORMAL, OLED_ALIGN_CENTER);
    oled_draw_string_aligned(0, 18, "V 1.0", OLED_WHITE, TEXT_BIG, OLED_ALIGN_CENTER);
    oled_draw_line(0, 14, 128, 14, OLED_WHITE); // Línea divisora
    oled_update();
    _delay_ms(600);

    // 2. EFECTO CARGA CONTINUA (Círculos, Triángulos y Rectángulos Redondeados)
    // Fondo geométrico combinado (Superposición)
    oled_draw_string_aligned(0, 0, "AVR OLED DEMO", OLED_BLACK, TEXT_NORMAL, OLED_ALIGN_CENTER);
    oled_draw_string_aligned(0, 18, "V 1.0", OLED_BLACK, TEXT_BIG, OLED_ALIGN_CENTER);
    oled_draw_circle(28, 36, 12, OLED_WHITE);               // Círculo hueco
    oled_draw_round_rect(50, 26, 24, 20, 4, OLED_WHITE);    // Rectángulo redondeado
    oled_draw_triangle(84, 46, 92, 26, 100, 46, OLED_WHITE); // Triángulo hueco

    // Animación de llenado progresivo sobre las figuras
    for (uint8_t r = 1; r <= 12; r += 1)
    {
        oled_draw_circle_filled(28, 36, r, OLED_WHITE);
        oled_update();
        _delay_ms(10);
    }

    oled_draw_round_rect_filled(50, 26, 24, 20, 4, OLED_WHITE); // Llenado interno
    oled_update();
    _delay_ms(150);

    oled_draw_triangle_filled(84, 46, 92, 26, 100, 46, OLED_WHITE); // Llenado interno
    oled_update();
    _delay_ms(200);

    // 4. BITMAP + TEXTO EN TEXT_MEGA (Superposición total)
    oled_clear();
    // Dibujamos bitmap de fondo
    oled_draw_round_rect_filled(3, 3, 122, 58, 6, OLED_WHITE); 
    oled_draw_bitmap(0, 0, epd_bitmap_nashe, 128, 64, OLED_BLACK);
    // Sobreponemos un banner central con fondo redondeado relleno y texto MEGA
    oled_update();
    _delay_ms(800);

    // 5. EFECTO DE PARPADEO E INVERSIÓN HARDWARE
    oled_invert_display();
    oled_update();
    _delay_ms(400);
    oled_invert_display();
    oled_update();
    _delay_ms(400);

    // 6. SCROLL POR BUFFER (Desplazamiento vertical en pantalla)
    for (uint8_t i = 0; i < 10; i++)
    {
        oled_scroll_pages(1, scroll_direction::Up);
        oled_update();
        _delay_ms(40);
    }

    // 7. SCROLL HARDWARE (Desplazamiento horizontal)
    oled_clear();
    oled_draw_string_aligned(0, 0, "HARDWARE SCROLL", OLED_WHITE, TEXT_NORMAL, OLED_ALIGN_CENTER);
    oled_update();

    oled_scroll(0, 7, 0x00, scroll_direction::Right, 0);
    _delay_ms(2500);
    oled_scroll_stop();
}

int main(void)
{
    i2c_init();
    oled_init(SSD1306, 128, 64);
    oled_set_contrast(0xFF);

    // Ejecuta la secuencia demostrativa
    demo_todas_las_funciones();

    // 8. BUCLE PRINCIPAL CON ANIMACIÓN DE SPRITE EN EL CENTRO
    oled_clear();
    oled_draw_rectangle(0, 0, 128, 64, OLED_WHITE);
    oled_draw_string_aligned(0, 4, "SPRITE TEST", OLED_WHITE, TEXT_NORMAL, OLED_ALIGN_CENTER);

    while (1)
    {
        // La animación corre de forma continua en el bucle principal
        oled_sprite_animation(56, 12, epd_bitmap_sprite, 32, 32, 16, 16, OLED_WHITE, 150);
    }

    return 0;
}