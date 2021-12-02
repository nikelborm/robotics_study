#include <avr/io.h>
#include <avr/common.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL
#define __AVR_ATmega2560__


// void setup() {
//   Serial.begin(115200);
// }

// void loop()
// {
//   int adc = analogRead(A0);
//   Serial.println(adc);

//   int adc2 = analogRead(A1);
//   Serial.println(adc2);
//   delay(1000);
// }

// int main() {
//   // отключение прерываний
//   cli();

//   // Настройка ножек на выход
//   DDRE |= (1 << PE4) | (1 << PE5);

//   // Работаем с таймером номер 3
//   // выбираем режим работы FAST PWM (TOP = ICR3)
//   TCCR3A = (1 << WGM31);
//   TCCR3B = (1 << WGM32) | (1 << WGM33);

//   // неинвертированный режим работы на каналах B и C
//   TCCR3A |= (1 << COM3B1) | (1 << COM3C1);

//   // настройка пределителя (prescaling) clk/1
//   TCCR3B |= 1 << CS30;

//   // записываем TOP value, таймер считает от 0 до TOP
//   ICR3 = 3199;

//   // Заполняем ШИМ (duty cycle) (от 0 до TOP)
//   OCR3B = 100;
//   OCR3C = 100;

//   // разрешаем прерывания
//   sei();

//   while (true) {}
// }


// DDRB - 0-вход 1-выход. Data Direction Register
// PORTB - В режиме выхода: Для установки выходного напряжения 0-low, 1-high.
// PORTB - В режиме входа:  Для подключения pull-up подтяжки 0-disabled, 1-enabled.
// PINb - для чтения напряжения



int main() {
  

  
  while (true) {}
}
