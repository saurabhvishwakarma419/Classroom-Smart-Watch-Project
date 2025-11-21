#ifndef EMERGENCY_H
#define EMERGENCY_H

#include <Arduino.h>

// Emergency alert structure
struct EmergencyAlert {
    String studentId;
    String alertType;
    float latitude;
    float longitude;
    String location;
    unsigned long timestamp;
    String deviceMac;
};

// Emergency states
enum EmergencyState {
    NORMAL,
    ALERT_TRIGGERED,
    ALERT_SENT,
    ALERT_ACKNOWLEDGED
};

// Function declarations
void initEmergency();
void checkEmergencyButton();
bool triggerEmergencyAlert();
bool sendEmergencyToServer(const EmergencyAlert &alert);
void displayEmergencyScreen();
EmergencyState getEmergencyState();
void resetEmergencyState();
bool isEmergencyButtonPressed();
void vibrate(int duration);

#endif
