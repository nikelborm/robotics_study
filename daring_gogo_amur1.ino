#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL

int main()
{
  Serial.begin(115200);

  while (true)
  {
    Serial.println("123");
    _delay_ms(1000);
  }
}
