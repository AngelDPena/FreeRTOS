#include <Arduino.h>

void IncDec(void *param);
void Selector(void *param);
void DelayAssign(void *param);
void l1(void *param);
void l2(void *param);
void l3(void *param);
void l4(void *param);
void print(void *param);

TaskHandle_t IncDecH;
TaskHandle_t SelectorH;
TaskHandle_t DelayAsignH;
TaskHandle_t l1H;
TaskHandle_t l2H;
TaskHandle_t l3H;
TaskHandle_t l4H;
TaskHandle_t printH;

bool down = false;
bool up = false;
bool sel = false;

int lbsDown = 0;
int lbsUp = 0;
int lbsSel = 0;

unsigned long ldtDown = 0;
unsigned long ldtup = 0;
unsigned long ldtSel = 0;
unsigned long dDelayIncDec = 100;
unsigned long dDelaySel = 1000;

int millSecs = 1000;
int secsL1 = 1000;
int secsL2 = 1000;
int secsL3 = 1000;
int secsL4 = 1000;
int ledPin = 1;

void setup()
{
  Serial.begin(115200);
  xTaskCreatePinnedToCore(IncDec, "IncDec", 1000, NULL, 3, &IncDecH, 0);
  xTaskCreatePinnedToCore(Selector, "Selector", 1000, NULL, 3, &SelectorH, 0);
  xTaskCreatePinnedToCore(DelayAssign, "DelayAssign", 1000, NULL, 2, &DelayAsignH, 0);
  xTaskCreatePinnedToCore(l1, "Led 1", 2000, NULL, 1, &l1H, 1);
  xTaskCreatePinnedToCore(l2, "Led 2", 2000, NULL, 1, &l2H, 1);
  xTaskCreatePinnedToCore(l3, "Led 3", 2000, NULL, 1, &l3H, 1);
  xTaskCreatePinnedToCore(l4, "Led 4", 2000, NULL, 1, &l4H, 1);
  xTaskCreatePinnedToCore(print, "print", 5000, NULL, 1, &printH, 0);

  pinMode(35, INPUT); // Decrease time in seconds
  pinMode(32, INPUT); // LedSelector
  pinMode(33, INPUT); // Increase time in seconds

  pinMode(13, OUTPUT); // Led 1
  pinMode(12, OUTPUT); // Led 2
  pinMode(14, OUTPUT); // Led 3
  pinMode(27, OUTPUT); // Led 4
}

void loop()
{
}
void print(void *param)
{
  (void)param;
  while (1)
  {
    Serial.print("L1 secs: ");
    Serial.println(secsL1);
    Serial.print("L2 secs: ");
    Serial.println(secsL2);
    Serial.print("L3 secs: ");
    Serial.println(secsL3);
    Serial.print("L4 secs: ");
    Serial.println(secsL4);
    delay(100);
    Serial.println(" ");
  }
}
void IncDec(void *param)
{
  (void)param;
  while (1)
  {
    if (digitalRead(35) == HIGH && millis() - lbsDown > dDelayIncDec)
    {
      lbsDown = millis();
      if (millSecs > 500)
        millSecs = millSecs - 10;
    }

    if (digitalRead(33) == HIGH && millis() - lbsUp > dDelayIncDec)
    {
      lbsUp = millis();
      millSecs = millSecs + 10;
    }
    vTaskDelay(10);
  }
}
void Selector(void *param)
{
  (void)param;
  while (1)
  {
    if (digitalRead(32) == HIGH && millis() - lbsSel > dDelaySel)
    {
      millSecs = 1000;
      lbsSel = millis();
      if (ledPin > 0 && ledPin < 4)
      {
        ledPin = ledPin + 1;
      }
      else
      {
        ledPin = 1;
      }
    }
    vTaskDelay(10);
  }
}
void DelayAssign(void *param)
{
  (void)param;
  while (1)
  {
    switch (ledPin)
    {
    case 1:
      secsL1 = millSecs;
      break;
    case 2:
      secsL2 = millSecs;
      break;
    case 3:
      secsL3 = millSecs;
      break;
    case 4:
      secsL4 = millSecs;
      break;

    default:
      break;
    }
    vTaskDelay(10);
  }
}
void l1(void *param) // Light 1
{
  (void)param;
  while (1)
  {
    digitalWrite(13, HIGH);
    vTaskDelay(secsL1 / portTICK_PERIOD_MS);
    digitalWrite(13, LOW);
    vTaskDelay(secsL1 / portTICK_PERIOD_MS);
  }
}
void l2(void *param) // Light 2
{
  (void)param;
  while (1)
  {
    digitalWrite(12, HIGH);
    vTaskDelay(secsL2 / portTICK_PERIOD_MS);
    digitalWrite(12, LOW);
    vTaskDelay(secsL2 / portTICK_PERIOD_MS);
  }
}
void l3(void *param) // Light 3
{
  (void)param;
  while (1)
  {
    digitalWrite(14, HIGH);
    vTaskDelay(secsL3 / portTICK_PERIOD_MS);
    digitalWrite(14, LOW);
    vTaskDelay(secsL3 / portTICK_PERIOD_MS);
  }
}
void l4(void *param) // Light 4
{
  (void)param;
  while (1)
  {
    digitalWrite(27, HIGH);
    vTaskDelay(secsL4 / portTICK_PERIOD_MS);
    digitalWrite(27, LOW);
    vTaskDelay(secsL4 / portTICK_PERIOD_MS);
  }
}
