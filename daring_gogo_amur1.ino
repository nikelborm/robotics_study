#define __AVR_ATmega2560__

#include <avr/io.h>
#include <avr/common.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL

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

// int main()
// {
//   // записываем 1 на pb5, то есть устанавливаем на выход (светодиод)
//   DDRB |= 1 << PB5;

//   // записываем 0 на pb2, то есть устанавливаем на выход (кнопка)
//   DDRB &= ~(1 << PB4); // !00100000 == 11011111
//   // 00100010 & 11011111 = 00000010

//   // включаем подтяжку к питанию (pull-up)
//   PORTB |= 1 << PB4;

//   while (1)
//   {
//     if (PINB & (1 << PB4))
//       PORTB &= ~(1 << PB5);
//     else
//       PORTB |= 1 << PB5;
//     _delay_ms(500);
//   }
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
//   OCR3B = 1000;
//   OCR3C = 1000;

//   // разрешаем прерывания
//   sei();

//   while (true) {}
// }

// DDRB - 0-вход 1-выход. Data Direction Register
// PORTB - В режиме выхода: Для установки выходного напряжения 0-low, 1-high.
// PORTB - В режиме входа:  Для подключения pull-up подтяжки 0-disabled, 1-enabled.
// PINb - для чтения напряжения

// Работа с АЦП с ручным вызовом преобразования

int main()
{
  // запускаем АЦП
  // По дефолту выравнивание ADCW идёт справа
  ADCSRA = 1 << ADEN;

  // тактовую частоту микроконтроллера делим на 128, частота АЦП 125 Кгц
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

  // Установили опорное напряжение
  ADMUX = 1 << REFS0;

  Serial.begin(115200);

  while (true)
  {
    // помним о том что MUX5 нужно записывать в ADCSRA, а все остальные в ADMUX
    // записываем 1 в бит для выбора аналогового порта ADC1
    ADMUX |= 1 << MUX0;

    // записываем 1 в бит для запуска преобразования
    ADCSRA |= (1 << ADSC);

    while (ADCSRA & (1 << ADSC)) {}

    // альтернативный вариант написания, но он медленнее Serial.println(ADCL | (ADCH << 8));
    Serial.print("ADC1: ");
    Serial.println(ADCW);

    // записываем 0 в бит для выбора аналогового порта ADC0
    ADMUX &= ~(1 << MUX0);

    // записываем 1 в бит для запуска преобразования
    ADCSRA |= (1 << ADSC);

    while (ADCSRA & (1 << ADSC)) {}

    Serial.print("ADC0: ");
    Serial.println(ADCW);
  }
}
