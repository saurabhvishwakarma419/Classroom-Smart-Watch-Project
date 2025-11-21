#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

// Communication status
enum CommStatus {
    COMM_SUCCESS,
    COMM_WIFI_ERROR,
    COMM_SERVER_ERROR,
    COMM_TIMEOUT,
    COMM_AUTH_ERROR
};

// Function declarations
bool initWiFi();
bool connectWiFi();
void disconnectWiFi();
bool isWiFiConnected();
String getDeviceMacAddress();

CommStatus sendHTTPPost(const String &endpoint, const String &jsonData);
CommStatus sendHTTPGet(const String &endpoint, String &response);
bool syncDataWithServer();
void handleIncomingData();

bool initBluetooth();
bool sendViaBluetooth(const String &data);
void checkBluetoothMessages();

void displayConnectionStatus();
int getSignalStrength();

#endif
