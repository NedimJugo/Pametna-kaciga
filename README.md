# Smart Helmet

Smart Helmet is a web and hardware-based IoT project for real-time monitoring and control of environmental data, such as temperature, humidity, air quality, and LEDs, using Firebase and ESP8266.

---

## Features

- Real-time monitoring of temperature, humidity, and air quality.
- Remote control of RGB LED colors and individual LED diode.
- Interactive charts for environmental data.
- Buzzer alerts for specific conditions.
- Light and dark theme toggle.
- Firebase integration for real-time data synchronization.

---

## Prerequisites

### Hardware Components

1. **ESP8266 Wi-Fi Module**: Main microcontroller for data transmission.
2. **DHT11 Sensor**: Measures temperature and humidity.
3. **MQ135 Sensor**: Detects air quality.
4. **RGB LED**: Multi-color LED for visual indication.
5. **Single LED Diode**: Additional control for on/off state.
6. **Buzzer**: Alerts for critical conditions.
7. **Resistors**: 220Ω for LEDs.
8. **Jumper Wires**: For connections.
9. **Breadboard/PCB**: For assembling components.

### Software Requirements

- Arduino IDE with the following libraries installed:
  - `Firebase ESP8266`
  - `DHT`
  - `ESP8266WiFi`
- Firebase account and project setup.
- Web browser for accessing the frontend.

---

## Setup Guide

### Hardware Connections

1. **ESP8266**: Connect all components to the appropriate GPIO pins as shown below.

   | Component       | Pin Connections on ESP8266 |
   |-----------------|----------------------------|
   | **DHT11**       | `VCC` -> 3.3V, `GND` -> GND, `Data` -> GPIO 4 |
   | **MQ135**       | `VCC` -> 3.3V, `GND` -> GND, `Analog Out` -> A0 |
   | **RGB LED**     | `Red` -> GPIO 14, `Green` -> GPIO 13, `Blue` -> GPIO 12 (with 220Ω resistors) |
   | **Single LED**  | `Anode` -> GPIO 5 (with 220Ω resistor), `Cathode` -> GND |
   | **Buzzer**      | `Positive` -> GPIO 15, `Negative` -> GND |

2. **Breadboard Setup**:
   - Use a breadboard or soldered PCB to organize components.
   - Ensure power connections are stable.

---

### Firebase Configuration

1. **Create a Firebase Project**:
   - Go to [Firebase Console](https://console.firebase.google.com/).
   - Create a new project.

2. **Database Setup**:
   - Add a Real-Time Database and structure it as:
     ```json
     {
       "DHT_11": {
         "Temperature": 0,
         "Humidity": 0,
         "Air_Quality": 0,
         "LED_Color": "Green",
         "LED_Control": "OFF",
         "LED_Diode": "OFF",
         "Buzzer": "OFF"
       }
     }
     ```

3. **Web App Integration**:
   - Copy your Firebase configuration into the `scripts.js` file:
     ```javascript
     export const firebaseConfig = {
         apiKey: "YOUR_API_KEY",
         authDomain: "YOUR_AUTH_DOMAIN",
         databaseURL: "YOUR_DATABASE_URL",
         projectId: "YOUR_PROJECT_ID",
         storageBucket: "YOUR_STORAGE_BUCKET",
         messagingSenderId: "YOUR_MESSAGING_SENDER_ID",
         appId: "YOUR_APP_ID"
     };
     ```

---

### Arduino ESP8266 Configuration

1. Install required libraries via the Arduino IDE:
   - **Firebase ESP8266**
   - **DHT**
   - **ESP8266WiFi**

2. Configure Wi-Fi and Firebase settings in the Arduino sketch:
   ```cpp
   #define WIFI_SSID "Your_SSID"
   #define WIFI_PASSWORD "Your_Password"
   #define API_KEY "Your_Firebase_API_Key"
   #define DATABASE_URL "Your_Firebase_Database_URL"
   
3. Add code for hardware control:
- DHT Sensor: Read temperature and humidity.
- MQ135 Sensor: Monitor air quality.
- LED Control: Update states from Firebase.
- Buzzer Alerts: Activate on specific conditions.
  
4. Upload the code to your ESP8266.

---

### Usage Instructions
- Power on the ESP8266 with the connected components.
- Access the web interface via index.html.
- Monitor real-time data and control LEDs or buzzer.
- Use the Firebase database to view live updates.

---

### Additional Notes
- LED Diode Control: The single LED diode can be toggled between ON and OFF states from the web interface.
- RGB LED: Set any color using the control panel.
- Buzzer: Alerts can be automated for specific conditions (e.g., high temperature or poor air quality).

---

### Future Enhancements
Add push notifications for critical alerts.
Expand sensor compatibility (e.g., CO2 or sound sensors).
Optimize the mobile user interface.
