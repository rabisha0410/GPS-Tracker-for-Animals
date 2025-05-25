# GPS Tracking System with LoRa and Node-RED

This project implements a GPS tracking system using two ESP32 microcontrollers, a LoRa module (SX1278) for wireless communication, and a GPS module for location data. The transmitter sends GPS coordinates via LoRa, which the receiver captures and sends to Node-RED. Node-RED processes the data, stores it in a SQLite database, and displays it on a dashboard with a map, gauges, a table, and geofence alerts for the Suryabinayak Jungle area. The transmitter uses deep sleep to save power, collecting data every 10 minutes outside the geofence and every 1 minute inside it.

## Prerequisites

### Hardware
- **ESP32 Development Board** (e.g., ESP32 DevKitC) - 2 units (one for transmitter, one for receiver).
- **SX1278 LoRa Module** (433 MHz) - 2 units.
- **GPS Module** (e.g., NEO-6M or NEO-7M with UART interface) - 1 unit for the transmitter.
- **USB-to-Serial Adapter** (e.g., FTDI or CP2102) if not integrated with the ESP32 board.
- **Jumper Wires** for connections.
- **Antennas** for LoRa modules (433 MHz, matching your region's regulations).
- **Computer** with USB ports for programming and running Node-RED.
- **Power Supply** (e.g., USB cable or battery for the transmitter, ensuring 3.3V or 5V compatibility).

### Software
- **Arduino IDE**: For programming the ESP32 boards.
- **Node.js**: Required to run Node-RED (version 14 or later recommended).
- **Node-RED**: For processing and visualizing data.
- **SQLite**: For storing GPS data (included with Node-RED's SQLite node).
- **Drivers**: USB drivers for your ESP32 board (e.g., CP2102 or CH340).
- **Web Browser**: For accessing the Node-RED dashboard (e.g., Chrome, Firefox).

## Step-by-Step Setup

### Step 1: Set Up the Hardware

#### Transmitter (ESP32 + LoRa + GPS)
1. **Connect the LoRa Module (SX1278)**:
   - Connect to the ESP32 using the following pins:
     - **NSS (SS)**: GPIO 5
     - **RST**: GPIO 14
     - **DIO0**: GPIO 2
     - **MOSI**: GPIO 23
     - **MISO**: GPIO 19
     - **SCK**: GPIO 18
     - **GND**: GND
     - **3.3V**: 3.3V (or 5V if your module supports it)
   - Attach a 433 MHz antenna to the LoRa module.
2. **Connect the GPS Module**:
   - Connect to the ESP32 UART2:
     - **GPS TX** to ESP32 GPIO 16 (RX2)
     - **GPS RX** to ESP32 GPIO 17 (TX2, optional)
     - **GND**: GND
     - **VCC**: 3.3V or 5V (check your GPS module's specifications)
   - Ensure the GPS module has a clear view of the sky for satellite acquisition.
3. **Power the ESP32**:
   - Connect the ESP32 to your computer via USB or use a battery (ensure proper voltage regulation).

#### Receiver (ESP32 + LoRa)
1. **Connect the LoRa Module (SX1278)**:
   - Use the same pin connections as the transmitter:
     - **NSS (SS)**: GPIO 5
     - **RST**: GPIO 14
     - **DIO0**: GPIO 2
     - **MOSI**: GPIO 23
     - **MISO**: GPIO 19
     - **SCK**: GPIO 18
     - **GND**: GND
     - **3.3V**: 3.3V (or 5V if supported)
   - Attach a 433 MHz antenna.
2. **Connect to Computer**:
   - Connect the receiver ESP32 to your computer via USB for serial communication with Node-RED.

**Note**: Verify that both LoRa modules operate at 433 MHz and comply with your region's radio frequency regulations.

### Step 2: Install Software

#### Install Arduino IDE
1. Download and install the Arduino IDE from [arduino.cc](https://www.arduino.cc/en/software).
2. Add ESP32 board support:
   - Open Arduino IDE, go to **File > Preferences**.
   - In "Additional Boards Manager URLs", add:
     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/master/package_esp32_index.json
     ```
   - Go to **Tools > Board > Boards Manager**, search for "ESP32", and install the **esp32** package by Espressif.
3. Install required libraries:
   - Go to **Sketch > Include Library > Manage Libraries**.
   - Install:
     - `LoRa` by Sandeep Mistry
     - `TinyGPSPlus` by Mikal Hart
     - `SPI` (included with Arduino IDE)
4. Select your ESP32 board:
   - Go to **Tools > Board > ESP32 Arduino** and select your board (e.g., "ESP32 Dev Module").
   - Choose the correct port under **Tools > Port**.

#### Install Node.js and Node-RED
1. Download and install Node.js from [nodejs.org](https://nodejs.org) (LTS version recommended).
2. Install Node-RED:
   - Open a terminal (Command Prompt on Windows, Terminal on macOS/Linux).
   - Run:
     ```
     npm install -g node-red
     ```
3. Start Node-RED:
   - Run:
     ```
     node-red
     ```
   - Open a web browser and go to `http://localhost:1880` to access the Node-RED editor.

#### Install Node-RED Dependencies
1. Install required Node-RED nodes:
   - In the Node-RED editor, go to **Menu (top-right) > Manage Palette > Install**.
   - Search for and install:
     - `node-red-node-sqlite`
     - `node-red-dashboard`
     - `node-red-contrib-web-worldmap`
2. Install SQLite:
   - On Windows, download SQLite from [sqlite.org](https://www.sqlite.org/download.html).
   - On macOS/Linux, verify installation with `sqlite3 --version`. If not installed:
     - macOS: `brew install sqlite`
     - Ubuntu/Debian: `sudo apt-get install sqlite3`

### Step 3: Program the ESP32 Boards

#### Transmitter Code
1. Open the Arduino IDE.
2. Create a new sketch and paste the following code:
 - Copy the transmitter code

3. **Update Geofence Parameters**:
   - `GEOFENCE_LAT` and `GEOFENCE_LON`: Set to `27.667079, 85.466263` (center of Suryabinayak Jungle).
   - `GEOFENCE_RADIUS`: Set to `1000` meters (adjust as needed).
4. **Upload the Code**:
   - Connect the transmitter ESP32 to your computer via USB.
   - Select the correct board and port in Arduino IDE.
   - Click **Upload**.

#### Receiver Code
1. Create a new sketch in Arduino IDE and paste the following code:
 - Copy the receiver code 

2. **Upload the Code**:
   - Connect the receiver ESP32 to your computer.
   - Select the correct board and port.
   - Click **Upload**.

### Step 4: Configure Node-RED

1. **Import the Node-RED Flow**:
   - Open Node-RED in your browser (`http://localhost:1880`).
   - Click **Menu > Import**.
   - Copy and paste the provided JSON flow (from your input).
   - Click **Import** and then **Deploy** (top-right red button).
2. **Configure the Serial Port**:
   - Locate the **Serial In** node labeled "Receiver Serial".
   - Double-click to open its settings.
   - Set the **Serial Port** to the port of your receiver ESP32 (e.g., `COM5` on Windows, `/dev/ttyUSB0` on Linux/macOS). Check the port in Arduino IDE under **Tools > Port**.
   - Ensure the baud rate is `115200`.
3. **Configure the SQLite Database**:
   - Locate the **SQLite** node labeled "Create Table".
   - Double-click to open its settings.
   - Set the **Database** path to a writable location, e.g.:
     - Windows: `C:\Users\YourUsername\gps_data.db`
     - macOS/Linux: `/home/yourusername/gps_data.db`
   - Ensure the directory exists (create it if necessary).
   - Repeat for the **Store Data** and **Query Altitude** SQLite nodes.
4. **Verify Geofence Coordinates**:
   - The flow defines a rectangular geofence for Suryabinayak Jungle in the **Dashboard Payload** and **Geo_fence** nodes:
     ```json
     [
       [27.667079, 85.465263],
       [27.667079, 85.467263],
       [27.669079, 85.467263],
       [27.669079, 85.465263]
     ]
     ```
   - To update, edit the **Geo_fence** node and the `geofence` array in the **Dashboard Payload** node.
5. **Deploy the Flow**:
   - Click **Deploy** to save and run the flow.

### Step 5: Run and Test the System

1. **Power the Transmitter**:
   - Connect the transmitter ESP32 to a power source (USB or battery).
   - Place the GPS module in an area with a clear view of the sky.
   - The transmitter sends GPS data every 10 minutes (or every 1 minute if within 1000 meters of the geofence center) and enters deep sleep to save power.
2. **Run the Receiver**:
   - Connect the receiver ESP32 to your computer.
   - Open the Arduino Serial Monitor (`Tools > Serial Monitor`, 115200 baud) to verify packets, e.g.:
     ```
     Received Packet (Size: 50 bytes): Pkt#0 Lat:27.667079 Lon:85.466263 Alt:100.0m Dist:500.00m (RSSI: -80 dBm, SNR: 5.0 dB)
     ```
3. **Access the Node-RED Dashboard**:
   - Open your browser and go to `http://localhost:1880/ui`.
   - The dashboard includes:
     - **Signal Metrics**: Gauges for RSSI (dBm) and SNR (dB).
     - **GPS Data**: A table showing packet number, latitude, longitude, altitude, RSSI, SNR, and timestamp.
     - **Map**: Displays the tracker's location, path (last 50 points), and geofence.
     - **Altitude Chart**: Plots altitude over time (last 50 points).
     - **Alert**: Shows if the tracker is inside or outside the geofence, with a toast notification.
4. **Test Geofence Alerts**:
   - Use the **Inject** nodes ("Inside the GeoFence" and "Outside the GeoFence") to simulate positions:
     - Inside: `{"lat": 27.66806, "lon": 85.43182}`
     - Outside: `{"lat": 27.6666, "lon": 85.42798}`
   - Click the inject nodes in Node-RED to test alerts.
5. **Verify Database Storage**:
   - Use a SQLite browser (e.g., DB Browser for SQLite) to check `gps_data.db`.
   - The `gps_data` table should have columns: `id`, `packetNum`, `lat`, `lon`, `alt`, `rssi`, `snr`, `timestamp`.

### Troubleshooting

- **No GPS Data**:
  - Ensure the GPS module has a clear view of the sky.
  - Verify wiring and power (check GPS module voltage requirements).
  - Increase the GPS timeout in the transmitter code (e.g., change `10000` to `30000` in `millis() - start < 10000`).
- **No LoRa Packets Received**:
  - Confirm both LoRa modules are on 433 MHz with matching parameters (spreading factor: 7, bandwidth: 125 kHz, coding rate: 4/5, sync word: 0x12).
  - Check antenna connections and ensure line-of-sight (LoRa range is 1-10 km depending on environment).
  - Verify pin connections.
- **Node-RED Issues**:
  - If the Serial In node fails, ensure the correct port is selected and no other application (e.g., Arduino Serial Monitor) is using it.
  - For SQLite errors, verify the database path and file permissions.
  - Ensure `node-red-node-sqlite`, `node-red-dashboard`, and `node-red-contrib-web-worldmap` are installed.
- **Dashboard Problems**:
  - If the map or gauges don’t display, check for JavaScript errors in the browser console.
  - Verify Node-RED dashboard nodes are installed and the flow is deployed.

### Additional Notes
- **Geofence Customization**: Update the geofence coordinates in the transmitter code (`GEOFENCE_LAT`, `GEOFENCE_LON`, `GEOFENCE_RADIUS`) and Node-RED flow to match your area.
- **Power Optimization**: The transmitter uses deep sleep. To further reduce power, add a transistor to power off the GPS and LoRa modules during sleep (requires hardware modification).
- **Database Management**: The SQLite database grows with each packet. Periodically back up or prune old data.
- **Security**: For public deployment, secure Node-RED with authentication (see Node-RED documentation).
- **Frequency Regulations**: Ensure 433 MHz is legal in your region or adjust to a permitted frequency (e.g., 915 MHz in the US).

## Support
For help, refer to:
- [Node-RED Documentation](https://nodered.org/docs)
- [Arduino Forum](https://forum.arduino.cc)
- [LoRa Library](https://github.com/sandeepmistry/arduino-LoRa)
- [TinyGPSPlus](https://github.com/mikalhart/TinyGPSPlus)

Enjoy tracking with your LoRa GPS system!