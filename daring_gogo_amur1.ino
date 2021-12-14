#include "included.h"

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

int main() {
  // отключение прерываний
  cli();

  // Настройка ножек на выход
  DDRE |= (1 << PE4) | (1 << PE5);

  // Работаем с таймером номер 3
  // выбираем режим работы FAST PWM (TOP = ICR3)
  TCCR3A = (1 << WGM31);
  TCCR3B = (1 << WGM32) | (1 << WGM33);

  // неинвертированный режим работы на каналах B и C
  TCCR3A |= (1 << COM3B1) | (1 << COM3C1);

  // настройка пределителя (prescaling) clk/1
  TCCR3B |= 1 << CS30;

  // записываем TOP value, таймер считает от 0 до TOP
  ICR3 = 3199;

  // Заполняем ШИМ (duty cycle) (от 0 до TOP)
  OCR3B = 1000;
  OCR3C = 1000;

  // разрешаем прерывания
  sei();

  while (true) {}
}

// DDRB - 0-вход 1-выход. Data Direction Register
// PORTB - В режиме выхода: Для установки выходного напряжения 0-low, 1-high.
// PORTB - В режиме входа:  Для подключения pull-up подтяжки 0-disabled, 1-enabled.
// PINb - для чтения напряжения

// Работа с АЦП с ручным вызовом преобразования

// int main()
// {
//   // запускаем АЦП
//   // По дефолту выравнивание ADCW идёт справа
//   ADCSRA = 1 << ADEN;

//   // тактовую частоту микроконтроллера делим на 128, частота АЦП 125 Кгц
//   ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

//   // Установили опорное напряжение
//   ADMUX = 1 << REFS0;

//   Serial.begin(115200);

//   while (true)
//   {
//     // помним о том что MUX5 нужно записывать в ADCSRA, а все остальные в ADMUX
//     // записываем 1 в бит для выбора аналогового порта ADC1
//     ADMUX |= 1 << MUX0;

//     // записываем 1 в бит для запуска преобразования
//     ADCSRA |= (1 << ADSC);

//     while (ADCSRA & (1 << ADSC)) {}

//     // альтернативный вариант написания, но он медленнее Serial.println(ADCL | (ADCH << 8));
//     Serial.print("ADC1: ");
//     Serial.println(ADCW);

//     // записываем 0 в бит для выбора аналогового порта ADC0
//     ADMUX &= ~(1 << MUX0);

//     // записываем 1 в бит для запуска преобразования
//     ADCSRA |= (1 << ADSC);

//     while (ADCSRA & (1 << ADSC)) {}

//     Serial.print("ADC0: ");
//     Serial.println(ADCW);
//   }
// }

// Работа с АЦП с автоматическим вызовом преобразования (цикличное преобразование)

// #define AMOUNT_OF_ADC_PINS 2

// int main()
// {
//   initADC();
//   Serial.begin(115200);
//   AdcReader::Init(AMOUNT_OF_ADC_PINS);
//   AdcReader *AdcReader = AdcReader::GetInstance();


//   while (true)
//   {
//     Serial.print("ADC0: ");
//     Serial.print(adcValuesFromLastFinishedMeasureCycle[0]);

//     Serial.print(", ADC1: ");
//     Serial.println(adcValuesFromLastFinishedMeasureCycle[1]);
//   }
// }

// ISR(ADC_vect)
// {
// }

// const uint8_t adcPinsToRead[AMOUNT_OF_ADC_PINS] = {
//     0,
//     1 << MUX0,
//     // 0,
//     // 0 | (1 << MUX0),
//     // 0 | (1 << MUX1),
//     // 0 | (1 << MUX0) | (1 << MUX1)
// };

// void initADC()
// {
//   // запускаем АЦП
//   ADCSRA = 1 << ADEN;

//   // выбираем тактовую частоту относительно микроконтроллера.
//   // Тактовую частоту делим на 128, частота АЦП 125 Кгц
//   ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

//   // Установили опорное напряжение (на котором работает ардуино)
//   ADMUX = 1 << REFS0;

//   // Сделали возможным прерывание
//   ADMUX = 1 << ADIF;

//   // Режим непрерывных преобразований
//   ADCSRA |= 1 << ADATE;

//   setPinForNextAdcReading(0);
//   // записываем 1 в бит для запуска преобразования
//   ADCSRA |= 1 << ADSC;
// }

// void setPinForNextAdcReading(byte knownPinsArrayIndex)
// {
//   turnOffBits(&ADMUX, (1 << MUX0) | (1 << MUX1) | (1 << MUX2) | (1 << MUX3) | (1 << MUX4));
//   turnOnBits(&ADMUX, adcPinsToRead[knownPinsArrayIndex]);
// }

// // class AdcReader
// // {
// // public:
// //   static AdcReader &getInstance()
// //   {
// //     static AdcReader instance;
// //     return instance;
// //   }

// //   void getLastAdcMeasurement()
// //   {
// //   }

// //   void addNewValue()
// //   {
// //     if (current_adc_index + 1 == AMOUNT_OF_ADC_PINS)
// //     {
// //       adcValuesFromCurrentMeasureCycle[current_adc_index] = ADCW;
// //       current_adc_index++;
// //       setPinForNextAdcReading(current_adc_index);
// //     }
// //     else
// //     {
// //       current_adc_index = 0;
// //       setPinForNextAdcReading(0);
// //       for (int i = 0; i < AMOUNT_OF_ADC_PINS; i++)
// //       {
// //         adcValuesFromLastFinishedMeasureCycle[i] = adcValuesFromCurrentMeasureCycle[i];
// //       }
// //     }
// //   }

// // private:
// //   AdcReader() {}

// //   volatile uint8_t current_adc_index = 0;
// //   volatile uint16_t adcValuesFromCurrentMeasureCycle[AMOUNT_OF_ADC_PINS];
// //   volatile uint16_t adcValuesFromLastFinishedMeasureCycle[AMOUNT_OF_ADC_PINS];

// // public:
// //   AdcReader(const AdcReader &) = delete;
// //   AdcReader(AdcReader &&) = delete;
// //   AdcReader &operator=(const AdcReader &) = delete;
// //   AdcReader &operator=(AdcReader &&) = delete;
// // };

// class AdcReader
// {
// protected:
//   AdcReader(const byte amountOfAdcPins) : amountOfAdcPins_(amountOfAdcPins)
//   {
//   }

//   static AdcReader *instance;

//   byte amountOfAdcPins_;

// public:
//   AdcReader(AdcReader &other) = delete;
//   void operator=(const AdcReader &) = delete;

//   static AdcReader *GetInstance();
//   static AdcReader *Init(const byte amountOfAdcPins);

//   void SomeBusinessLogic()
//   {
//     // ...
//   }

//   byte amountOfAdcPins() const
//   {
//     return amountOfAdcPins_;
//   }
// };

// AdcReader *AdcReader::instance = nullptr;

// AdcReader *AdcReader::Init(const byte amountOfAdcPins)
// {
//   if (instance != nullptr)
//   {
//     throw "Can`t initialize AdcReader twice";
//   }
//   instance = new AdcReader(amountOfAdcPins);
// }

// AdcReader *AdcReader::GetInstance()
// {
//   if (instance == nullptr)
//   {
//     throw "AdcReader not initialized";
//   }
//   return instance;
// }

// void setBitIntoRegister(volatile uint8_t *reg, unsigned pos, bool val)
// {
//   if (val)
//   {
//     turnOnBits(reg, 1 << pos);
//   }
//   else
//   {
//     turnOffBits(reg, 1 << pos);
//   }
// }

// void turnOnBits(volatile uint8_t *reg, unsigned mask)
// {
//   *reg = (*reg) | (mask);
// }

// void turnOffBits(volatile uint8_t *reg, unsigned mask)
// {
//   *reg = (*reg) & ~(mask);
// }
