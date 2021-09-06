#include <Preferences.h>
Preferences prefs;

typedef struct {
  uint8_t hour;
  uint8_t minute;
  uint8_t setting1;
  uint8_t setting2;
} schedule_t;

void setup() {
  Serial.begin(115200);
  prefs.begin("schedule");
  uint8_t content[] = {9, 30, 235, 255, 20, 15, 0, 0};
  prefs.putBytes("schedule", content, 8);
  size_t schLen = prefs.getBytes("schedule", NULL, NULL);
  
  char buffer[schLen];
  prefs.getBytes("schedule", buffer, schLen);
  if (schLen % sizeof(schedule_t)) {
    log_e("Data is not correct size!");
    return;
  }
  schedule_t *schedule = (schedule_t *) buffer;
  Serial.printf("%02d:%02d %d/%d\n", 
    schedule[1].hour, schedule[1].minute,
    schedule[1].setting1, schedule[1].setting2);
  prefs.putBytes("schedule", schedule, sizeof(schedule));
  buffer[0] = 0;
  prefs.getBytes("schedule", buffer, schLen);
  for (int x=0; x<8; x++) Serial.printf("%02X ", buffer[x]);
  Serial.println(); 
}

void loop() {}
