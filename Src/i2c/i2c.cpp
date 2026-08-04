#include <i2c.h>

void i2c_init()
{
  TWSR = 0x00;                          // Sin prescaler
  TWBR = ((F_CPU / 100000UL) - 16) / 2; // Ajustar a 100kHz
};
void i2c_start()
{
  TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT); // Iniciar condición START
  while (!(TWCR & (1 << TWINT)))
    ; // Esperar a que la transmisión termine
};
void i2c_stop()
{
  TWCR = (1 << TWSTO) | (1 << TWEN) | (1 << TWINT); // Iniciar condición STOP
};
void i2c_write(uint8_t data)
{
  TWDR = data;                       // Cargar los datos a enviar
  TWCR = (1 << TWEN) | (1 << TWINT); // Iniciar la transmisión
  while (!(TWCR & (1 << TWINT)))
    ; // Esperar a que termine
};