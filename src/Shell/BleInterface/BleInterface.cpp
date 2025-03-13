#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include "BleInterface.h"

// Define UUIDs for the service and characteristics
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define COMMAND_CHAR_UUID   "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define RESPONSE_CHAR_UUID  "beb5483e-36e1-4688-b7f5-ea07361b26a9"

BLEServer* pServer = nullptr;
BLECharacteristic* pCommandCharacteristic = nullptr;
BLECharacteristic* pResponseCharacteristic = nullptr;
bool deviceConnected = false;

// Callback class for handling connection events
class ServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        deviceConnected = true;
        Serial.println("Device connected");
    }

    void onDisconnect(BLEServer* pServer) {
        deviceConnected = false;
        Serial.println("Device disconnected");
        // Restart advertising to allow new connections
        pServer->startAdvertising();
    }
};

// Callback class for handling command characteristic events
class CommandCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        std::string value = pCharacteristic->getValue();
        if (value.length() > 0) {
            Serial.print("Received command: ");
            Serial.println(value.c_str());
            
            // Process the command and prepare response
            String response = processCommand(value.c_str());
            
            // Send response back to the client
            pResponseCharacteristic->setValue(response.c_str());
            pResponseCharacteristic->notify();
        }
    }
};

// Function to process commands
String processCommand(const char* command) {
    String cmd = String(command);
    String response = "ERROR: Unknown command";
    
    // Command format: CMD:PARAM1:PARAM2...
    if (cmd.startsWith("GET_STATUS")) {
        response = "OK:SYSTEM_RUNNING";
    }
    else if (cmd.startsWith("SET_VALVE:")) {
        // Example: SET_VALVE:1:ON
        int valve = cmd.substring(10, cmd.lastIndexOf(":")).toInt();
        String state = cmd.substring(cmd.lastIndexOf(":") + 1);
        // Add your valve control logic here
        response = "OK:VALVE_" + String(valve) + "_" + state;
    }
    // Add more commands as needed
    
    return response;
}

void setupBLE() {
    // Initialize BLE
    BLEDevice::init("ShamBE Controller");
    
    // Create the BLE Server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new ServerCallbacks());

    // Create the BLE Service
    BLEService *pService = pServer->createService(SERVICE_UUID);

    // Create BLE Characteristics
    pCommandCharacteristic = pService->createCharacteristic(
        COMMAND_CHAR_UUID,
        BLECharacteristic::PROPERTY_WRITE
    );
    
    pResponseCharacteristic = pService->createCharacteristic(
        RESPONSE_CHAR_UUID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
    );

    // Add descriptors
    pResponseCharacteristic->addDescriptor(new BLE2902());
    
    // Set callbacks for command characteristic
    pCommandCharacteristic->setCallbacks(new CommandCallbacks());

    // Start the service
    pService->start();

    // Start advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
    
    Serial.println("BLE server ready. Waiting for connections...");
}

void handleBLE() {
    // Handle BLE tasks in the main loop if needed
    if (deviceConnected) {
        // Add any periodic tasks here
        delay(10);
    }
}