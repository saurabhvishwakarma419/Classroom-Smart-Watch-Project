#include "emergency.h"
#include "config.h"
#include "communication.h"
#include <ArduinoJson.h>

EmergencyState currentEmergencyState = NORMAL;
unsigned long emergencyButtonPressTime = 0;
const unsigned long EMERGENCY_HOLD_TIME = 3000;  // 3 seconds

void initEmergency() {
    Serial.println("Initializing emergency system...");
    pinMode(BTN_EMERGENCY, INPUT_PULLUP);
    currentEmergencyState = NORMAL;
    Serial.println("Emergency system initialized");
}

void checkEmergencyButton() {
    bool buttonPressed = (digitalRead(BTN_EMERGENCY) == LOW);
    
    if (buttonPressed) {
        if (emergencyButtonPressTime == 0) {
            // Button just pressed
            emergencyButtonPressTime = millis();
            Serial.println("Emergency button pressed...");
        } else {
            // Button is being held
            unsigned long holdDuration = millis() - emergencyButtonPressTime;
            
            if (holdDuration >= EMERGENCY_HOLD_TIME && currentEmergencyState == NORMAL) {
                // Trigger emergency alert
                Serial.println("EMERGENCY ALERT TRIGGERED!");
                triggerEmergencyAlert();
                vibrate(500);  // Haptic feedback
            }
        }
    } else {
        // Button released
        if (emergencyButtonPressTime > 0) {
            unsigned long holdDuration = millis() - emergencyButtonPressTime;
            
            if (holdDuration < EMERGENCY_HOLD_TIME) {
                Serial.println("Emergency button released (too short)");
            }
            
            emergencyButtonPressTime = 0;
        }
    }
}

bool triggerEmergencyAlert() {
    if (currentEmergencyState != NORMAL) {
        Serial.println("Emergency already in progress");
        return false;
    }
    
    currentEmergencyState = ALERT_TRIGGERED;
    
    EmergencyAlert alert;
    alert.studentId = STUDENT_ID;
    alert.alertType = "SOS";
    alert.latitude = 0.0;  // Would be GPS coordinates in production
    alert.longitude = 0.0;
    alert.location = "Classroom";
    alert.timestamp = millis();
    alert.deviceMac = getDeviceMacAddress();
    
    bool success = sendEmergencyToServer(alert);
    
    if (success) {
        currentEmergencyState = ALERT_SENT;
        displayEmergencyScreen();
        Serial.println("Emergency alert sent successfully!");
    } else {
        currentEmergencyState = NORMAL;
        Serial.println("Failed to send emergency alert!");
    }
    
    return success;
}

bool sendEmergencyToServer(const EmergencyAlert &alert) {
    if (!isWiFiConnected()) {
        Serial.println("WiFi not connected - cannot send emergency alert!");
        // In production, should queue for later or use alternative communication
        return false;
    }
    
    // Create JSON payload
    StaticJsonDocument<512> doc;
    doc["studentId"] = alert.studentId;
    doc["alertType"] = alert.alertType;
    doc["location"] = alert.location;
    doc["deviceMac"] = alert.deviceMac;
    doc["timestamp"] = alert.timestamp;
    
    JsonObject gps = doc.createNestedObject("gpsCoordinates");
    gps["latitude"] = alert.latitude;
    gps["longitude"] = alert.longitude;
    
    String jsonData;
    serializeJson(doc, jsonData);
    
    Serial.print("Sending emergency alert: ");
    Serial.println(jsonData);
    
    CommStatus status = sendHTTPPost(API_ENDPOINT_EMERGENCY, jsonData);
    
    if (status == COMM_SUCCESS) {
        Serial.println("Emergency alert delivered!");
        return true;
    } else {
        Serial.println("Emergency alert failed to send!");
        return false;
    }
}

void displayEmergencyScreen() {
    // This would display emergency screen on OLED
    // Shows "SOS SENT" message with flashing indicator
    Serial.println("===================");
    Serial.println("   SOS ALERT SENT  ");
    Serial.println("   HELP IS COMING  ");
    Serial.println("===================");
}

EmergencyState getEmergencyState() {
    return currentEmergencyState;
}

void resetEmergencyState() {
    currentEmergencyState = NORMAL;
    emergencyButtonPressTime = 0;
    Serial.println("Emergency state reset");
}

bool isEmergencyButtonPressed() {
    return (digitalRead(BTN_EMERGENCY) == LOW);
}

void vibrate(int duration) {
    // Placeholder for haptic feedback
    // Would control a vibration motor on GPIO pin
    // For now, just log it
    Serial.print("Vibrating for ");
    Serial.print(duration);
    Serial.println(" ms");
    
    // In production:
    // digitalWrite(VIBRATION_PIN, HIGH);
    // delay(duration);
    // digitalWrite(VIBRATION_PIN, LOW);
}
