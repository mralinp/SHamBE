#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

// Scan duration in seconds
#define SCAN_DURATION 5

// Callback class for handling discovered devices
class EddystoneScanCallback: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
        // Check if the device is an Eddystone beacon
        if (advertisedDevice.haveManufacturerData()) {
            std::string manufacturerData = advertisedDevice.getManufacturerData();
            uint8_t* manufacturerDataPtr = (uint8_t*)manufacturerData.data();
            
            // Check for Eddystone manufacturer ID (0x004C for Apple)
            if (manufacturerDataPtr[0] == 0x4C && manufacturerDataPtr[1] == 0x00) {
                Serial.println("Found Eddystone Beacon:");
                Serial.print("Device Name: ");
                Serial.println(advertisedDevice.getName().c_str());
                Serial.print("RSSI: ");
                Serial.println(advertisedDevice.getRSSI());
                Serial.print("Address: ");
                Serial.println(advertisedDevice.getAddress().toString().c_str());
                Serial.print("Manufacturer Data: ");
                for (int i = 0; i < manufacturerData.length(); i++) {
                    Serial.printf("%02X ", manufacturerDataPtr[i]);
                }
                Serial.println("\n");
            }
        }
    }
};

void setup() {
    Serial.begin(9600);
    Serial.println("Starting BLE Scanner for Eddystone Beacons...");
    
    // Initialize BLE
    BLEDevice::init("");
    BLEScan* pBLEScan = BLEDevice::getScan();
    
    // Set up callback
    pBLEScan->setAdvertisedDeviceCallbacks(new EddystoneScanCallback());
    
    // Set active scan (this will get more data from devices)
    pBLEScan->setActiveScan(true);
    
    // Set scan interval and window
    pBLEScan->setInterval(1349);
    pBLEScan->setWindow(449);
}

void loop() {
    Serial.println("Scanning for Eddystone beacons...");
    
    // Start scanning
    BLEScan* pBLEScan = BLEDevice::getScan();
    pBLEScan->start(SCAN_DURATION, false);
    
    // Wait for scan to complete
    delay(SCAN_DURATION * 1000);
    
    // Stop scanning
    pBLEScan->stop();
    
    // Clear results for next scan
    pBLEScan->clearResults();
    
    // Wait before next scan
    delay(2000);
}