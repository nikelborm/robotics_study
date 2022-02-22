#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL
#define M_RIGHT 49
#define PWM_RIGHT 2
#define M_LEFT 51
#define PWM_LEFT 3

#define RA 0
#define RB 1
#define LA 2
#define LB 3
volatile byte motorsState;

volatile int32_t cntLeft = 0;
volatile int32_t cntRight = 0;

#define MAX_SPEED 3199;
volatile float rad = 6.28; // используем радианы

int main()
{
  initHardware();

  while (true)
  {
    // Задаем скорость колёсам
    spinRightWheel(3199, (cntRight / 2435.f) * 3.1415 * 6.55);
    spinLeftWheel(3199, (cntLeft / 2435.f) * 3.1415 * 6.55);
  }
}

ISR(INT0_vect)
{
  motorInterruption(LA, LB, PD0, PD1, &cntLeft);
}
ISR(INT1_vect)
{
  motorInterruption(LA, LB, PD0, PD1, &cntLeft);
}
ISR(INT2_vect)
{
  motorInterruption(RA, RB, PD3, PD2, &cntRight);
}
ISR(INT3_vect)
{
  motorInterruption(RA, RB, PD3, PD2, &cntRight);
}

void initHardware()
{
  cli();

  //настройка ножек на вход
  DDRD &= ~((1 << PD3) | (1 << PD2));
  DDRB &= ~(1 << PB5);
  DDRB |= (1 << PB6);
  DDRD &= ~((1 << PD1) | (1 << PD0));
  //настройка внешних прерываний int1,2,3,4 change
  EICRA |= ((1 << ISC20) | (1 << ISC30));
  EICRA |= ((1 << ISC10) | (1 << ISC00));
  //включаем внешние прерывания int2, int3, int 1, int 0
  EIMSK |= (1 << INT3) | (1 << INT2);
  EIMSK |= (1 << INT1) | (1 << INT0);
  Serial.begin(9600);
  //настройка ножек на выход
  DDRE |= (1 << PE4) | (1 << PE5);
  //Задаем пинам 49 и 51 значение ВЫХОД
  DDRL |= (1 << PL0);
  DDRB |= (1 << PB2);

  // режим работы таймера fast PWM (TOP = ICR3)
  TCCR3A = (1 << WGM31);
  TCCR3B = (1 << WGM32) | (1 << WGM33);
  // неинвертированный режим работы на каналах В и С
  TCCR3A |= (1 << COM3B1) | (1 << COM3C1);
  //настраиваем прескалер clk/1
  TCCR3B |= (1 << CS30);

  // записывает TOP value, таймер считает от 0 до ТОР
  ICR3 = MAX_SPEED;
  //разрешаем прерывания
  sei();
}

volatile void motorInterruption(byte bitA, byte bitB, byte pinA, byte pinB, volatile int32_t *encoder_counter)
{
  bool pA = motorsState & (1 << bitA);
  bool pB = motorsState & (1 << bitB);
  bool A = (PIND & (1 << pinA)) != 0;
  bool B = (PIND & (1 << pinB)) != 0;
  setBit(&motorsState, bitA, A);
  setBit(&motorsState, bitB, B);

  if (
      ((A == 1) & (B == 1) & (pA == 1) & (pB == 0)) ||
      ((A == 0) & (B == 1) & (pA == 1) & (pB == 1)) ||
      ((A == 0) & (B == 0) & (pA == 0) & (pB == 1)) ||
      ((A == 1) & (B == 0) & (pA == 0) & (pB == 0)))
    (*encoder_counter)++;

  if (
      ((A == 0) & (B == 0) & (pA == 1) & (pB == 0)) ||
      ((A == 0) & (B == 1) & (pA == 0) & (pB == 0)) ||
      ((A == 1) & (B == 1) & (pA == 0) & (pB == 1)) ||
      ((A == 1) & (B == 0) & (pA == 1) & (pB == 1)))
    (*encoder_counter)--;
}

void spinRightWheel(int velocity, float range)
{
  // Если пройденное расстояние правого колеса меньше заданной радианы, то колесо крутится
  if (rad >= 0)
  {
    if (fabs(range) < fabs(15 * rad))
    {
      digitalWrite(M_RIGHT, HIGH);
      analogWrite(PWM_RIGHT, velocity);
    }
    else
    {
      digitalWrite(M_RIGHT, LOW);
      analogWrite(PWM_RIGHT, 0);
    }
  }
  else
  {
    if (fabs(range) < fabs(15 * rad))
    {
      digitalWrite(M_RIGHT, LOW);
      analogWrite(PWM_RIGHT, 0);
    }
    else
    {
      digitalWrite(M_RIGHT, HIGH);
      analogWrite(PWM_RIGHT, 0);
    }
  }
}

void spinLeftWheel(int velocity, float range)
{
  // Если пройденное расстояние левого колеса меньше заданной радианы, то колесо крутится
  if (rad >= 0)
  {
    if (fabs(range) < fabs(15 * rad))
    {
      digitalWrite(M_LEFT, LOW);
      analogWrite(PWM_LEFT, 3199 - velocity);
    }
    else
    {
      digitalWrite(M_LEFT, HIGH);
      analogWrite(PWM_LEFT, 0);
    }
  }
  else
  {
    if (fabs(range) < fabs(15 * rad))
    {
      digitalWrite(M_LEFT, HIGH);
      analogWrite(PWM_LEFT, velocity);
    }
    else
    {
      digitalWrite(M_LEFT, LOW);
      analogWrite(PWM_LEFT, 0);
    }
  }
}

void setBit(volatile unsigned char *bytelink, unsigned pos, bool val)
{
  if (val)
  {
    *bytelink = (*bytelink) | (1 << pos);
  }
  else
  {
    *bytelink = (*bytelink) & ~(1 << pos);
  }
}
