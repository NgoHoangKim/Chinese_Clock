#include <LiquidCrystal.h>
#define BT_1 2
#define BT_2 3
#define bientro A0
#define quangtro A1
LiquidCrystal lcd_1(9, 8, 7, 6, 5, 4);

struct Clock {
  unsigned long time;
  unsigned long hour;
  unsigned long minute;
  unsigned long second;
  char str[16];

  Clock(unsigned long timeIn) {
    time = timeIn;
    hour = (time / 3600) % 24;
    minute = (time / 60) % 60;
    second = (time % 60);
    sprintf(str, "%02lu:%02lu:%02lu", hour, minute, second);
  }

  void increase(unsigned increaseTime) {
    time = (time + increaseTime) % 86400;
    hour = (time / 3600) % 24;
    minute = (time / 60) % 60;
    second = (time % 60);
    sprintf(str, "%02lu:%02lu:%02lu", hour, minute, second);
  }
};

unsigned long startTime = 00 * 3600L + 00 * 60 + 00;
unsigned long alarmTime = 00 * 3600L;

int mode = 0;
int NUM_OF_MODE = 5;

int bt1_state = 1;
int bt2_state = 1;

Clock nowClock = Clock(startTime);
Clock alarmClock = Clock(alarmTime);

void scanButton() {
  nowClock = Clock(millis() / 1000 + startTime);

  bt1_state = digitalRead(BT_1);
  bt2_state = digitalRead(BT_2);

  if (bt1_state == 0 && bt2_state == 1) {
    while (digitalRead(BT_1) == 0);
    mode = (mode + 1) % NUM_OF_MODE;
    Serial.println(mode);
  }

  if (bt1_state == 1 && bt2_state == 0) {
    Serial.println("Bam nut 2");
    delay(200);
    switch (mode) {
      case 1:
        alarmClock.increase(3600);
        break;
      case 2:
        alarmClock.increase(60);
        break;
      case 3:
        alarmClock.increase(1);
      default:
        break;
    }
  }


  if (Serial.available()) {

    String command = Serial.readStringUntil('/');

    if (command.equals("Mode0")) {
      mode = 0;
    }
    if (command.equals("Mode1")) {
      mode = 1;
    }
    if (command.equals("Mode2")) {
      mode = 2;
    }
    if (command.equals("Mode3")) {
      mode = 3;
    }
    if (command.equals("Mode4")) {
      mode = 4;
    }

    if (command.startsWith("Now=")) {
      unsigned long h = (unsigned long)command.substring(4, 6).toInt();
      unsigned long m = (unsigned long)command.substring(7, 9).toInt();
      unsigned long s = (unsigned long)command.substring(10, 12).toInt();
      startTime = h * 3600L + m * 60 + s;
      nowClock = Clock(startTime);
    }

    if (command.startsWith("Alarm=")) {
      unsigned long h = (unsigned long)command.substring(6, 8).toInt();
      unsigned long m = (unsigned long)command.substring(9, 11).toInt();
      unsigned long s = (unsigned long)command.substring(12, 14).toInt();
      unsigned long newAlarm = h * 3600L + m * 60 + s;
      alarmClock = Clock(newAlarm);
    }
  }
}


int giatri;
int giatri1;
int alpha;
void display() {
  switch (mode) {
    case 0:
      lcd_1.setCursor(0, 0);
      lcd_1.print("Time:  ");
      lcd_1.print(nowClock.str);
      lcd_1.setCursor(0, 1);
      lcd_1.print("Alarm: ");
      lcd_1.print(alarmClock.str);
      break;
    case 1:
      lcd_1.setCursor(0, 0);
      lcd_1.print("Alarm: ");
      lcd_1.print(alarmClock.str);
      if (nowClock.time % 2 && bt2_state != 0) {
        lcd_1.setCursor(7, 0);
        lcd_1.print("  ");
      }
      lcd_1.setCursor(0, 1);
      lcd_1.print("               ");
      lcd_1.setCursor(7, 1);
      break;
    case 2:
      // lcd_1.clear();
      lcd_1.setCursor(0, 0);
      lcd_1.print("Alarm: ");
      lcd_1.print(alarmClock.str);
      if (nowClock.time % 2 && bt2_state != 0) {
        lcd_1.setCursor(10, 0);
        lcd_1.print("  ");
      }
      lcd_1.setCursor(0, 1);
      lcd_1.print("               ");
      lcd_1.setCursor(10, 1);
      break;

    case 3:
      lcd_1.setCursor(0, 0);
      lcd_1.print("Alarm: ");
      lcd_1.print(alarmClock.str);
      if (nowClock.time % 2 && bt2_state != 0) {
        lcd_1.setCursor(13, 0);
        lcd_1.print("  ");
      }
      lcd_1.setCursor(0, 1);
      lcd_1.print("               ");
      lcd_1.setCursor(13, 1);
      break;
    case 4:
    lcd_1.setCursor(0, 0);
     lcd_1.print("Ngo Hoang Kim  ");
    lcd_1.setCursor(0, 1);
    lcd_1.print("HE163588");
      
    default:
      break;
  }
}

void setup() {
  pinMode(BT_1, INPUT_PULLUP);
  pinMode(BT_2, INPUT_PULLUP);
  pinMode(11, OUTPUT);
  lcd_1.begin(16, 2);
  Serial.begin(9600);
}


void loop() {
  // Quet nut dong thoi tang thoi gian
  scanButton();
  // Nhay man hinh trong 10s
  if (nowClock.time - 10 <= alarmClock.time && nowClock.time >= alarmClock.time && mode == 0) {
    lcd_1.clear();
  }
  // Hien thi
  display();
} 
