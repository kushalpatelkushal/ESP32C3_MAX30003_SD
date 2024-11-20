// #include "C:\Users\patelkus\Documents\libraries\FS\src\FS.h"
// #include "SD.h"
// #include "SPI.h"
// #include "C:\Users\patelkus\Documents\libraries\ProtoCentral_MAX30003_ECG_AFE_Sensor_Library1\src\protocentral_Max30003.h"
// #include <WiFi.h>
// #include <WebServer.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "mbedtls/aes.h" //LIBRARY FOR AES128 METHOD FOR ENCRYPTION
// #include "mbedtls/base64.h"
// #include "KickFiltersRT.h"

// KickFiltersRT<signed long> filtersRT2;

// // #define data_size 25600
// // uint8_t BLE_data1[data_size];
// // int myValue;
// // bool deviceConnected = false;
// // bool oldDeviceConnected = false;
// // signed long ECG_filter;
// // int ECG_test = 0;
// // int data_i = 0;
// // bool ONLYSD = false;
// // bool READ_SD_ONCE = false;

// #define HIGHEST_PRIORITY (configMAX_PRIORITIES - 1)

// // Define the GPIO pin that is connected to the battery.
// const int GPIO_BATTERY = 1;
// const int GPIO_BUTTON = 10;
// #define DEBOUNCE_DELAY 50 // Define the debounce delay in milliseconds
// unsigned long previousMillis = 0;
// const long interval = 60000;             // 1 minutes in milliseconds
// uint8_t previousBatteryPercentage = 100; // Initial battery percentage
// bool firstRead = true;                   // Flag to check if it's the first read
// uint8_t battery_percentage, battery_percentage1;
// bool SD_PRESENT = false;

// MAX30003 max30003;

// const char *ssid = "ESP32_Hotspot"; // SSID for the Access Point
// const char *password = "12345678";  // Password for the Access Point

// WebServer server(80);

// TaskHandle_t ECGSDTaskHandle = NULL;

// SemaphoreHandle_t dataSemaphore;

// // Forward declarations
// void ECGSDTask(void *pvParameters);

// // Function to encrypt data using AES CBC mode
// void encryptBLEData(uint8_t *data, size_t dataLength, uint8_t *encryptedData)
// {
//   mbedtls_aes_context aes;
//   unsigned char key[16] = "itzkbgulrcsjmnv"; // Example key
//   key[15] = 'x';
//   unsigned char iv[16] = "abcdefghijklmno"; // Example initialization vector
//   iv[15] = 'p';

//   // Set up the encryption context
//   mbedtls_aes_init(&aes);
//   mbedtls_aes_setkey_enc(&aes, key, 128);

//   // Encrypt the data using AES in CBC mode
//   mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_ENCRYPT, dataLength, iv, data, encryptedData);

//   // Clean up the AES context
//   mbedtls_aes_free(&aes);
// }

// void byteArrayToBase64(const uint8_t *byteArray, size_t byteArrayLen, char *base64Str, size_t base64StrLen)
// {
//   size_t outputLen;
//   mbedtls_base64_encode((unsigned char *)base64Str, base64StrLen, &outputLen, byteArray, byteArrayLen);
//   base64Str[outputLen] = '\0'; // Null-terminate the base64 string
// }

// void handleRoot()
// {
//   server.send(200, "text/html", R"rawliteral(
//     <!DOCTYPE html>
//     <html>
//     <head>
//       <title>ESP32 File Server</title>
//       <style>
//         body {
//           font-family: Arial, sans-serif;
//           background-color: #f4f4f4;
//           margin: 0;
//           padding: 0;
//         }
//         .container {
//           width: 80%;
//           margin: auto;
//           overflow: hidden;
//         }
//         header {
//           background: #50b3a2;
//           color: #ffffff;
//           padding-top: 30px;
//           min-height: 70px;
//           border-bottom: #e8491d 3px solid;
//         }
//         header a {
//           color: #ffffff;
//           text-decoration: none;
//           text-transform: uppercase;
//           font-size: 16px;
//         }
//         header ul {
//           padding: 0;
//           list-style: none;
//         }
//         header li {
//           display: inline;
//           padding: 0 20px 0 20px;
//         }
//         header #branding {
//           float: left;
//         }
//         header #branding h1 {
//           margin: 0;
//         }
//         header nav {
//           float: right;
//           margin-top: 10px;
//         }
//         header .highlight, header .current a {
//           color: #e8491d;
//           font-weight: bold;
//         }
//         #showcase {
//           min-height: 400px;
//           text-align: center;
//           color: #ffffff;
//           background: #50b3a2;
//         }
//         #showcase h1, #showcase p {
//           background: #e8491d;
//           display: inline-block;
//           padding: 10px 20px;
//           border-radius: 5px;
//         }
//         #showcase h1 {
//           margin-top: 100px;
//           font-size: 55px;
//           margin-bottom: 10px;
//         }
//         #showcase p {
//           font-size: 20px;
//         }
//         .button {
//           display: inline-block;
//           color: #ffffff;
//           background: #e8491d;
//           padding: 10px 20px;
//           text-decoration: none;
//           border-radius: 5px;
//         }
//         .content {
//           padding: 20px;
//           background: #ffffff;
//           box-shadow: 0 0 10px rgba(0,0,0,0.1);
//           margin-top: 20px;
//         }
//         footer {
//           background: #333333;
//           color: #ffffff;
//           text-align: center;
//           padding: 20px;
//           margin-top: 20px;
//         }
//         footer p {
//           margin: 0;
//         }
//       </style>
//     </head>
//     <body>
//       <header>
//         <div class="container">
//           <div id="branding">
//             <h1><span class="highlight">ESP32</span> File Server</h1>
//           </div>
//           <nav>
//             <ul>
//               <li class="current"><a href="/">Home</a></li>
//               <li><a href="/list">List Files</a></li>
//             </ul>
//           </nav>
//         </div>
//       </header>
//       <section id="showcase">
//         <div class="container">
//           <h1>Welcome to the ESP32 File Server</h1>
//           <p>Access and manage files stored on the ESP32's SD card.</p>
//         </div>
//       </section>
//       <div class="container content">
//         <h2>Available Files:</h2>
//         <ul id="fileList"></ul>
//       </div>
//       <footer>
//         <p>ESP32 File Server &copy; 2024</p>
//       </footer>
//       <script>
//         async function fetchFiles() {
//           const response = await fetch('/list');
//           const files = await response.text();
//           document.getElementById('fileList').innerHTML = files;
//         }
//         fetchFiles();
//       </script>
//     </body>
//     </html>
//   )rawliteral");
// }

// void handleFileList()
// {
//   // Check if the "hello.txt" file exists
//   if (SD.exists("/hello.txt"))
//   {
//     // Generate the HTML response with a link to download the "hello.txt" file
//     String output = "<h2>Files:</h2><ul>";
//     output += "<li><a href='/download?file=/hello.txt'>hello.txt</a> - " + String(SD.open("/hello.txt").size()) + " bytes</li>";
//     output += "</ul>";
//     server.send(200, "text/html", output);
//   }
//   else
//   {
//     // If the "hello.txt" file doesn't exist, display a message indicating that it's not found
//     server.send(404, "text/plain", "404: hello.txt Not Found");
//   }
// }

// void handleFileDownload()
// {
//   if (server.hasArg("file"))
//   {
//     String fileName = server.arg("file");
//     Serial.print("Requested file to download: ");
//     Serial.println(fileName);

//     if (fileName.charAt(0) != '/')
//     {
//       fileName = "/" + fileName;
//     }

//     if (SD.exists(fileName))
//     {
//       File downloadFile = SD.open(fileName, FILE_READ);
//       size_t fileSize = downloadFile.size();
//       size_t totalBytesSent = 0;

//       // Send HTTP headers
//       server.setContentLength(fileSize);
//       server.sendHeader("Content-Type", "application/octet-stream");
//       server.sendHeader("Content-Disposition", "attachment; filename=" + fileName);
//       server.sendHeader("Connection", "close");
//       server.send(200, "application/octet-stream", "");

//       // Determine the buffer size dynamically
//       size_t bufferSize = 512; // Start with a default buffer size
//       // if (ESP.getFreeHeap() > bufferSize * 2) {
//       //   bufferSize = ESP.getFreeHeap() / 2; // Use half of the free heap for the buffer
//       // }

//       uint8_t *buffer = new uint8_t[bufferSize];
//       size_t bytesRead;

//       while ((bytesRead = downloadFile.read(buffer, bufferSize)) > 0)
//       {
//         server.client().write(buffer, bytesRead);
//         totalBytesSent += bytesRead;
//         Serial.printf("Bytes sent: %u/%u\n", totalBytesSent, fileSize);
//       }

//       delete[] buffer;
//       downloadFile.close();
//       Serial.println("File download completed.");
//     }
//     else
//     {
//       Serial.println("File not found: " + fileName);
//       server.send(404, "text/plain", "404: File Not Found");
//     }
//   }
//   else
//   {
//     Serial.println("File argument missing in request");
//     server.send(400, "text/plain", "400: Invalid Request");
//   }
// }

// void ECGSDTask(void *pvParameters)
// {
//   if (getCpuFrequencyMhz() != 20)
//   {
//     setCpuFrequencyMhz(20);
//   }

//   const TickType_t xFrequency = pdMS_TO_TICKS(8);
//   TickType_t xLastWakeTime = xTaskGetTickCount();

//   // if (SD.exists("/hello.txt"))
//   // {
//   //     deleteFile(SD, "/hello.txt");
//   // }
//   // myFile = SD.open("/hello.txt", FILE_APPEND);

//   // if (myFile)
//   // {
//   //     Serial.println("File opened successfully.");
//   // }
//   // else
//   // {
//   //     myFile = SD.open("/hello.txt", FILE_APPEND);
//   //     Serial.println("Error opening file.");
//   // }
//   // bool ONLYSD = true;
//   int Q = 0;
//   while (Q < 100)
//   {
//     max30003.getEcgSamples();
//     vTaskDelay(xFrequency);
//     Q++;
//   }

// #define data_size 32000
//   uint8_t BLE_data1[data_size];
//   int data_i = 0;

//   for (;;)
//   {
//     if (digitalRead(GPIO_BUTTON) == true)
//     {
//       while (true)
//       {
//         server.handleClient();
//       }
//     }
//     uint64_t t1 = micros();

//     // Serial.printf("Free stack6: %d\n", uxTaskGetStackHighWaterMark(NULL));

//     max30003.getEcgSamples();
//     // Serial.printf("Free stack7: %d\n", uxTaskGetStackHighWaterMark(NULL));
//     // ECG_filter = filtersRT2.movingAverage(max30003.ecgdata);
//     // signed long ECG_filter = filtersRT2.movingAverage(max30003.ecgdata);
//     // int16_t ECG_filter = max30003.ecgdata;
//     // ECG_test = round(ECG_filter / 1000);
//     // Serial.println(ECG_test);
//     if (data_i < data_size)
//     {

//       BLE_data1[data_i] = max30003.ecgdata;
//       BLE_data1[data_i + 1] = max30003.ecgdata >> 8;
//       // BLE_data1[data_i + 2] = ECG_filter >> 16;
//       // BLE_data1[data_i + 3] = ECG_filter >> 24;
//       data_i = data_i + 2;
//       // Serial.println(data_i);
//     }

//     // Serial.printf("Free stack: %d\n", uxTaskGetStackHighWaterMark(NULL));
//     // if (myFile && data_i == data_size)
//     if (data_i == data_size)
//     // if (data_i == data_size)
//     { // Serial.printf("Free stack: %d\n", uxTaskGetStackHighWaterMark(NULL));
//       size_t dataLength = sizeof(BLE_data1);

//       // uint8_t encryptedData[dataLength];
//       uint8_t *encryptedData = new uint8_t[dataLength];
//       // Encrypt the data
//       encryptBLEData(BLE_data1, dataLength, encryptedData);

//       // String fileContent;

//       // for (int l = 0; l < dataLength; l++)
//       // {

//       //   if (encryptedData[l] < 0x10)
//       //   {
//       //     fileContent += "0"; // Pad single hex digit with leading zero
//       //   }
//       //   fileContent += String(encryptedData[l], HEX);
//       // }

//       char base64Str[data_size * 2]; // Make sure this is large enough to hold the encoded string
//       byteArrayToBase64(encryptedData, dataLength, base64Str, sizeof(base64Str));
//       delete[] encryptedData;
//       // Serial.printf("Free stack: %d\n", uxTaskGetStackHighWaterMark(NULL));
//       // Serial.println(base64Str);
//       if (!SD.begin(3))
//       {
//         Serial.println("Card Mount Failed");
//         // return false;
//       }

//       File myFile;

//       if (myFile)
//       {
//         Serial.println("File opened successfully.");
//       }
//       else
//       {
//         if (SD_PRESENT == false)
//         {
//           myFile = SD.open("/hello.txt", FILE_WRITE);
//           SD_PRESENT = true;
//         }
//         else
//         {
//           myFile = SD.open("/hello.txt", FILE_APPEND);
//         }
//       }

//       // Convert to seconds
//       uint64_t totalSeconds = t1 / 1000000;

//       // Calculate hours, minutes, and seconds
//       uint64_t hours = totalSeconds / 3600;
//       uint64_t minutes = (totalSeconds % 3600) / 60;
//       uint64_t seconds = totalSeconds % 60;
//       // Serial.println(String(hours) + ":" + String(minutes) + ":" + String(seconds));
//       myFile.println(base64Str);
//       myFile.println(String(hours) + ":" + String(minutes) + ":" + String(seconds));
//       myFile.flush(); // Ensure data is written to the SD card
//       myFile.close();
//       SD.end();

//       analogWrite(0, 255);
//       vTaskDelay(pdMS_TO_TICKS(200));
//       analogWrite(0, 0);
//       vTaskDelay(pdMS_TO_TICKS(200));

//       data_i = 0;
//       SD.end();
//     }
//     uint64_t t2 = micros();
//     Serial.print(data_i);
//     Serial.print(",");
//     Serial.println(t2 - t1);
//     // vTaskDelayUntil(&xLastWakeTime, xFrequency);

//     if ((8000 > (t2 - t1)))
//     {
//       // delayMicroseconds(8000 - (t2 - t1));
//       vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(8 - ((t2 - t1) / 1000)));
//       // vTaskDelay(pdMS_TO_TICKS(7812.5 - (t2 - t1)));
//     }

//     if (((t2 - t1) > 8000))
//     {
//       int remainder = (t2 - t1) % 8000;
//       // Serial.println(remainder);
//       vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(8 - (remainder / 1000)));
//       // delayMicroseconds(8000 - remainder);
//     }

//     // Serial.println("DATA WRITTEN TO SD");
//   }
// }

// void setup()
// {
//   Serial.begin(57600);
//   filtersRT2.initmovingAverage(4); // for ECG
//   pinMode(MAX30003_CS_PIN, OUTPUT);
//   digitalWrite(MAX30003_CS_PIN, HIGH); // disable device

//   SPI.begin();
//   SPI.setBitOrder(MSBFIRST);
//   SPI.setDataMode(SPI_MODE0);

//   bool ret = max30003.max30003ReadInfo();
//   if (ret)
//   {
//     Serial.println("Max30003 read ID Success");
//   }
//   else
//   {

//     while (!ret)
//     {
//       // stay here untill the issue is fixed.
//       ret = max30003.max30003ReadInfo();
//       Serial.println("Failed to read ID, please make sure all the pins are connected");
//       delay(10000);
//     }
//   }

//   Serial.println("Initialising the chip ...");
//   max30003.max30003Begin(); // initialize MAX30003
//   // pinMode(buttonPin, INPUT_PULLUP);
//   delay(50);
//   pinMode(GPIO_BUTTON, INPUT_PULLDOWN); // Initialize the button pin as input with pull-up resistor

//   // Set up Access Point
//   WiFi.softAP(ssid, password);
//   IPAddress IP = WiFi.softAPIP();
//   Serial.print("AP IP address: ");
//   Serial.println(IP);

//   delay(50);

//   // if (!SD.begin(3))
//   // {
//   //     Serial.println("Card Mount Failed");
//   //     // return false;
//   // }
//   // uint8_t cardType = SD.cardType();

//   // if (cardType == CARD_NONE)
//   // {
//   //     Serial.println("No SD card attached");
//   //     //   return false;
//   // }

//   // Serial.print("SD Card Type: ");
//   // if (cardType == CARD_MMC)
//   // {
//   //     Serial.println("MMC");
//   // }
//   // else if (cardType == CARD_SD)
//   // {
//   //     Serial.println("SDSC");
//   // }
//   // else if (cardType == CARD_SDHC)
//   // {
//   //     Serial.println("SDHC");
//   // }
//   // else
//   // {
//   //     Serial.println("UNKNOWN");
//   // }

//   // uint64_t cardSize = SD.cardSize() / (1024 * 1024);
//   // Serial.printf("SD Card Size: %lluMB\r\n", cardSize);

//   for (int w = 0; w < 10; w++)
//   {
//     analogWrite(0, 255);
//     delay(200);
//     analogWrite(0, 0);
//     delay(200);
//   }
//   // Define routes
//   server.on("/", HTTP_GET, handleRoot);
//   server.on("/list", HTTP_GET, handleFileList);
//   server.on("/download", HTTP_GET, handleFileDownload);

//   server.begin();
//   Serial.println("HTTP server started");

//   // Create a semaphore to synchronize tasks
//   dataSemaphore = xSemaphoreCreateBinary();

//   // Create tasks
//   // xTaskCreate(WEBTask, "WEB Task", 4096, NULL, 1, &WEBTaskHandle);
//   xTaskCreate(ECGSDTask, " ECG SD Task", data_size * 4, NULL, HIGHEST_PRIORITY, &ECGSDTaskHandle); // value 65536 is 2.5 times 25600
//                                                                                                    // xTaskCreate(BATTERYTask, "BATTERY Task", 16384, NULL, 2, &BATTERYTaskHandle);
//                                                                                                    // xTaskCreate(BLEBATTERYTask, "BLE BATTERY Task", 4096, NULL, 2, &BLEBATTERYTaskHandle);
// }

// void loop()
// {
//   // vTaskDelay(pdMS_TO_TICKS(1));
// }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------
// #include "C:\Users\patelkus\Documents\libraries\FS\src\FS.h"
// #include "SD.h"
// #include "SPI.h"
// #include "C:\Users\patelkus\Documents\libraries\ProtoCentral_MAX30003_ECG_AFE_Sensor_Library1\src\protocentral_Max30003.h"
// #include <WiFi.h>
// #include <WebServer.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "mbedtls/aes.h" //LIBRARY FOR AES128 METHOD FOR ENCRYPTION
// #include "mbedtls/base64.h"
// #include "KickFiltersRT.h"
// #include "esp_task_wdt.h"
// #include <freertos/semphr.h>

// // SET_LOOP_TASK_STACK_SIZE(128*1024);
// #define HIGHEST_PRIORITY (configMAX_PRIORITIES - 1)
// KickFiltersRT<signed long> filtersRT2;

// #define data_size 32000
// uint8_t BLE_data1[data_size];

// // Define the GPIO pin that is connected to the battery.
// const int GPIO_BATTERY = 0;
// unsigned long previousMillis = 0;
// const long interval = 60000;             // 1 minutes in milliseconds
// uint8_t previousBatteryPercentage = 100; // Initial battery percentage
// bool firstRead = true;                   // Flag to check if it's the first read
// uint8_t battery_percentage, battery_percentage1;
// int SD_PRESENT = false; // flag for first time data write to sd card
// MAX30003 max30003;

// const char *ssid = "ESP32_Hotspot"; // SSID for the Access Point
// const char *password = "12345678";  // Password for the Access Point

// WebServer server(80);

// TaskHandle_t ECGSDTaskHandle = NULL;

// SemaphoreHandle_t dataSemaphore;

// // Forward declarations
// void ECGSDTask(void *pvParameters);
// hw_timer_t *My_timer = NULL;
// SemaphoreHandle_t xSemaphore = NULL;

// void IRAM_ATTR onTimer()
// {
//   BaseType_t xHigherPriorityTaskWoken = pdFALSE;
//   xSemaphoreGiveFromISR(xSemaphore, &xHigherPriorityTaskWoken);
//   if (xHigherPriorityTaskWoken == pdTRUE)
//   {
//     portYIELD_FROM_ISR(); // No arguments needed
//                           // Serial.println("ALARM");
//   }
// }

// // Function to encrypt data using AES CBC mode
// void encryptBLEData(uint8_t *data, size_t dataLength, uint8_t *encryptedData)
// {
//   mbedtls_aes_context aes;
//   unsigned char key[16] = "itzkbgulrcsjmnv"; // Example key
//   key[15] = 'x';
//   unsigned char iv[16] = "abcdefghijklmno"; // Example initialization vector
//   iv[15] = 'p';

//   // Set up the encryption context
//   mbedtls_aes_init(&aes);
//   mbedtls_aes_setkey_enc(&aes, key, 128);

//   // Encrypt the data using AES in CBC mode
//   mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_ENCRYPT, dataLength, iv, data, encryptedData);

//   // Clean up the AES context
//   mbedtls_aes_free(&aes);
// }

// void byteArrayToBase64(const uint8_t *byteArray, size_t byteArrayLen, char *base64Str, size_t base64StrLen)
// {
//   size_t outputLen;
//   mbedtls_base64_encode((unsigned char *)base64Str, base64StrLen, &outputLen, byteArray, byteArrayLen);
//   base64Str[outputLen] = '\0'; // Null-terminate the base64 string
// }

// void handleRoot()
// {
//   server.send(200, "text/html", R"rawliteral(
//     <!DOCTYPE html>
//     <html>
//     <head>
//       <title>ESP32 File Server</title>
//       <style>
//         body {
//           font-family: Arial, sans-serif;
//           background-color: #f4f4f4;
//           margin: 0;
//           padding: 0;
//         }
//         .container {
//           width: 80%;
//           margin: auto;
//           overflow: hidden;
//         }
//         header {
//           background: #50b3a2;
//           color: #ffffff;
//           padding-top: 30px;
//           min-height: 70px;
//           border-bottom: #e8491d 3px solid;
//         }
//         header a {
//           color: #ffffff;
//           text-decoration: none;
//           text-transform: uppercase;
//           font-size: 16px;
//         }
//         header ul {
//           padding: 0;
//           list-style: none;
//         }
//         header li {
//           display: inline;
//           padding: 0 20px 0 20px;
//         }
//         header #branding {
//           float: left;
//         }
//         header #branding h1 {
//           margin: 0;
//         }
//         header nav {
//           float: right;
//           margin-top: 10px;
//         }
//         header .highlight, header .current a {
//           color: #e8491d;
//           font-weight: bold;
//         }
//         #showcase {
//           min-height: 400px;
//           text-align: center;
//           color: #ffffff;
//           background: #50b3a2;
//         }
//         #showcase h1, #showcase p {
//           background: #e8491d;
//           display: inline-block;
//           padding: 10px 20px;
//           border-radius: 5px;
//         }
//         #showcase h1 {
//           margin-top: 100px;
//           font-size: 55px;
//           margin-bottom: 10px;
//         }
//         #showcase p {
//           font-size: 20px;
//         }
//         .button {
//           display: inline-block;
//           color: #ffffff;
//           background: #e8491d;
//           padding: 10px 20px;
//           text-decoration: none;
//           border-radius: 5px;
//         }
//         .content {
//           padding: 20px;
//           background: #ffffff;
//           box-shadow: 0 0 10px rgba(0,0,0,0.1);
//           margin-top: 20px;
//         }
//         footer {
//           background: #333333;
//           color: #ffffff;
//           text-align: center;
//           padding: 20px;
//           margin-top: 20px;
//         }
//         footer p {
//           margin: 0;
//         }
//       </style>
//     </head>
//     <body>
//       <header>
//         <div class="container">
//           <div id="branding">
//             <h1><span class="highlight">ESP32</span> File Server</h1>
//           </div>
//           <nav>
//             <ul>
//               <li class="current"><a href="/">Home</a></li>
//               <li><a href="/list">List Files</a></li>
//             </ul>
//           </nav>
//         </div>
//       </header>
//       <section id="showcase">
//         <div class="container">
//           <h1>Welcome to the ESP32 File Server</h1>
//           <p>Access and manage files stored on the ESP32's SD card.</p>
//         </div>
//       </section>
//       <div class="container content">
//         <h2>Available Files:</h2>
//         <ul id="fileList"></ul>
//       </div>
//       <footer>
//         <p>ESP32 File Server &copy; 2024</p>
//       </footer>
//       <script>
//         async function fetchFiles() {
//           const response = await fetch('/list');
//           const files = await response.text();
//           document.getElementById('fileList').innerHTML = files;
//         }
//         fetchFiles();
//       </script>
//     </body>
//     </html>
//   )rawliteral");
// }

// void handleFileList()
// {
//   if (!SD.begin(3))
//   {

//     Serial.println("Card Mount Failed");
//     // return false;
//   }
//   // Check if the "hello.txt" file exists
//   if (SD.exists("/hello.txt"))
//   {
//     // Generate the HTML response with a link to download the "hello.txt" file
//     String output = "<h2>Files:</h2><ul>";

//     output += "<li><a href='/download?file=/hello.txt'>hello.txt</a> - " + String(SD.open("/hello.txt").size()) + " bytes</li>";
//     output += "</ul>";
//     server.send(200, "text/html", output);
//   }
//   else
//   {
//     // If the "hello.txt" file doesn't exist, display a message indicating that it's not found
//     server.send(404, "text/plain", "404: hello.txt Not Found");
//   }
// }

// void handleFileDownload()
// {
//   if (server.hasArg("file"))
//   {
//     String fileName = server.arg("file");
//     Serial.print("Requested file to download: ");
//     Serial.println(fileName);

//     if (fileName.charAt(0) != '/')
//     {
//       fileName = "/" + fileName;
//     }

//     if (SD.exists(fileName))
//     {
//       File downloadFile = SD.open(fileName, FILE_READ);
//       size_t fileSize = downloadFile.size();
//       size_t totalBytesSent = 0;

//       // Send HTTP headers
//       server.setContentLength(fileSize);
//       server.sendHeader("Content-Type", "application/octet-stream");
//       server.sendHeader("Content-Disposition", "attachment; filename=" + fileName);
//       server.sendHeader("Connection", "close");
//       server.send(200, "application/octet-stream", "");

//       // Determine the buffer size dynamically
//       size_t bufferSize = 512; // Start with a default buffer size
//       // if (ESP.getFreeHeap() > bufferSize * 2) {
//       //   bufferSize = ESP.getFreeHeap() / 2; // Use half of the free heap for the buffer
//       // }

//       uint8_t *buffer = new uint8_t[bufferSize];
//       size_t bytesRead;

//       while ((bytesRead = downloadFile.read(buffer, bufferSize)) > 0)
//       {
//         server.client().write(buffer, bytesRead);
//         totalBytesSent += bytesRead;
//         Serial.printf("Bytes sent: %u/%u\n", totalBytesSent, fileSize);
//       }

//       delete[] buffer;
//       downloadFile.close();
//       Serial.println("File download completed.");
//     }
//     else
//     {
//       Serial.println("File not found: " + fileName);
//       server.send(404, "text/plain", "404: File Not Found");
//     }
//   }
//   else
//   {
//     Serial.println("File argument missing in request");
//     server.send(400, "text/plain", "400: Invalid Request");
//   }
// }

// void ECGSDTask(void *pvParameters)
// {
//   if (getCpuFrequencyMhz() != 20)
//   {
//     setCpuFrequencyMhz(20);
//   }

//   int data_i = 0;
//   TickType_t xLastWakeTime = xTaskGetTickCount();

//   for (;;)
//   {
//     // Wait for the semaphore from the ISR
//     if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE)
//     {
//       // Serial.printf("Free stack2: %d\n", uxTaskGetStackHighWaterMark(NULL));
//       uint64_t t1 = micros();
//       // Sample the ECG data (this is now done inside the task)
//       max30003.getEcgSamples();
//       // Serial.println(max30003.ecgdata);
//       // Serial.println(max30003.data);
//       int16_t ECG_filter = max30003.ecgdata;
//       // unsigned long ECG_ETAG = max30003.ETAG;
//       // Serial.println(ECG_ETAG);

//       if (data_i < data_size)
//       {
//         BLE_data1[data_i] = ECG_filter;
//         BLE_data1[data_i + 1] = ECG_filter >> 8;
//         // BLE_data1[data_i + 2] = ECG_filter >> 16;
//         // BLE_data1[data_i + 3] = ECG_filter >> 24;
//         data_i = data_i + 2;
//       }
//       Serial.println(data_i);
//       if (data_i == data_size)
//       {
//         // unsigned long t1 = micros();
//         size_t dataLength = sizeof(BLE_data1);
//         uint8_t *encryptedData = new uint8_t[dataLength];
//         // Encrypt the data
//         encryptBLEData(BLE_data1, dataLength, encryptedData);

//         char base64Str[data_size * 2]; // Make sure this is large enough to hold the encoded string
//         byteArrayToBase64(encryptedData, dataLength, base64Str, sizeof(base64Str));
//         delete[] encryptedData;

//         if (!SD.begin(3))
//         {
//           Serial.println("Card Mount Failed");
//         }
//         File myFile;
//         if (SD_PRESENT == false)
//         {

//           if (SD.exists("/hello.txt"))
//           {
//             myFile = SD.open("/hello.txt", FILE_WRITE);
//             SD_PRESENT = true;
//           }
//         }

//         myFile = SD.open("/hello.txt", FILE_APPEND);

//         if (!myFile)
//         {
//           Serial.println("File open failed");
//         }

//         // Convert to seconds
//         uint64_t totalSeconds = t1 / 1000000;

//         // Calculate hours, minutes, and seconds
//         uint64_t hours = totalSeconds / 3600;
//         uint64_t minutes = (totalSeconds % 3600) / 60;
//         uint64_t seconds = totalSeconds % 60;
//         // Serial.println(String(hours) + ":" + String(minutes) + ":" + String(seconds));
//         myFile.println(base64Str);
//         myFile.println(String(hours) + ":" + String(minutes) + ":" + String(seconds));
//         myFile.flush(); // Ensure data is written to the SD card
//         myFile.close();
//         SD.end();

//         analogWrite(0, 255);
//         vTaskDelay(pdMS_TO_TICKS(200));
//         analogWrite(0, 0);
//         vTaskDelay(pdMS_TO_TICKS(200));

//         data_i = 0;
//       }

//       // if (myValue == 3)
//       // {
//       //     uint8_t BLE_data[5];
//       //     unsigned long currentMillis = millis();

//       //     if (firstRead || currentMillis - previousMillis >= interval)
//       //     {
//       //         previousMillis = currentMillis; // Save the last time the battery was read

//       //         int analogvalue = analogRead(GPIO_BATTERY);
//       //         analogvalue = constrain(analogvalue, 2900, 3700);
//       //         battery_percentage1 = map(analogvalue, 2900, 3700, 1, 100);

//       //         if (battery_percentage1 <= previousBatteryPercentage)
//       //         {
//       //             battery_percentage = battery_percentage1;
//       //             previousBatteryPercentage = battery_percentage1;
//       //         }
//       //         firstRead = false;
//       //     }

//       //     BLE_data[0] = ECG_filter;
//       //     BLE_data[1] = ECG_filter >> 8;
//       //     BLE_data[2] = ECG_filter >> 16;
//       //     BLE_data[3] = ECG_filter >> 24;
//       //     BLE_data[4] = battery_percentage;

//       //     pCharacteristic->setValue(BLE_data, 5);
//       //     pCharacteristic->notify(); // Send the value to the app!
//       //                                // Serial.printf("Free stack1: %d\n", uxTaskGetStackHighWaterMark(NULL));
//       //                                // Serial.println("data sent");
//       // }

//       // if (myValue != 3)
//       // {
//       //     ONLYSD = false;
//       // }

//       // if (myValue == 4)
//       // {
//       //     READ_SD_ONCE = true;
//       //     server.handleClient();
//       // }

//       uint64_t t2 = micros();

//       // Adjust the delay to maintain the 8ms interval
//       int elapsedTime = (t2 - t1);
//       // Serial.println(elapsedTime);
//       int remainingTime = 8000 - elapsedTime;
//       if (remainingTime > 0)
//       {
//         vTaskDelay(pdMS_TO_TICKS(remainingTime / 1000));
//       }
//       else
//       {
//         // If elapsed time exceeds the interval, calculate overrun and adjust next wake time
//         int overrunTime = elapsedTime - 8000;
//         vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(8000 - (overrunTime % 8000) / 1000));
//       }
//     }
//   }
// }

// void setup()
// {
//   Serial.begin(57600);
//   Serial.printf("Free stack0: %d\n", uxTaskGetStackHighWaterMark(NULL));
//   filtersRT2.initmovingAverage(2); // for ECG
//   pinMode(MAX30003_CS_PIN, OUTPUT);
//   digitalWrite(MAX30003_CS_PIN, HIGH); // disable device

//   SPI.begin();
//   SPI.setBitOrder(MSBFIRST);
//   SPI.setDataMode(SPI_MODE0);

//   bool ret = max30003.max30003ReadInfo();
//   if (ret)
//   {
//     Serial.println("Max30003 read ID Success");
//   }
//   else
//   {

//     while (!ret)
//     {
//       // stay here untill the issue is fixed.
//       ret = max30003.max30003ReadInfo();
//       Serial.println("Failed to read ID, please make sure all the pins are connected");
//       delay(10000);
//     }
//   }

//   Serial.println("Initialising the chip ...");
//   max30003.max30003Begin(); // initialize MAX30003

//   delay(50);
//   WiFi.softAP(ssid, password);
//   IPAddress IP = WiFi.softAPIP();
//   Serial.print("AP IP address: ");
//   Serial.println(IP);

//   delay(50);

//   // Define routes
//   server.on("/", HTTP_GET, handleRoot);
//   server.on("/list", HTTP_GET, handleFileList);
//   server.on("/download", HTTP_GET, handleFileDownload);

//   server.begin();
//   Serial.println("HTTP server started");
//   esp_task_wdt_init(100, true); // Increase timeout to 10 seconds

//   xSemaphore = xSemaphoreCreateBinary();

//   // Initialize the hardware timer
//   My_timer = timerBegin(0, 20, true); // Timer 0, prescaler 20, count up

//   // Attach the onTimer ISR to the timer
//   timerAttachInterrupt(My_timer, &onTimer, true);

//   // Set the timer to trigger the ISR every 8 milliseconds (8000 microseconds)
//   timerAlarmWrite(My_timer, 8000, true);

//   // Enable the timer alarm
//   timerAlarmEnable(My_timer);

//   // Create a semaphore to synchronize tasks
//   dataSemaphore = xSemaphoreCreateBinary();
//   Serial.printf("Free stack1: %d\n", uxTaskGetStackHighWaterMark(NULL));

//   for (int e = 0; e < 5; e++)
//   {
//     analogWrite(0, 255);
//     vTaskDelay(pdMS_TO_TICKS(200));
//     analogWrite(0, 0);
//     vTaskDelay(pdMS_TO_TICKS(200));
//   }
//   // Create tasks
//   // xTaskCreate(WEBTask, "WEB Task", 4096, NULL, 1, &WEBTaskHandle);
//   xTaskCreate(ECGSDTask, " ECG SD Task", 68000, NULL, HIGHEST_PRIORITY, &ECGSDTaskHandle); // value 65536 is 2.5 times 25600
//                                                                                            // xTaskCreate(BATTERYTask, "BATTERY Task", 16384, NULL, 2, &BATTERYTaskHandle);
//                                                                                            // xTaskCreate(BLEBATTERYTask, "BLE BATTERY Task", 4096, NULL, 2, &BLEBATTERYTaskHandle);
// }

// void loop()
// {
// }

//-------------------------------------------------------------------------------------------------------------------------

#include "C:\Users\patelkus\Documents\libraries\FS\src\FS.h"
#include "SD.h"
#include "SPI.h"
#include "C:\Users\patelkus\Documents\libraries\ProtoCentral_MAX30003_ECG_AFE_Sensor_Library1\src\protocentral_Max30003.h"
#include <WiFi.h>
#include <WebServer.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "mbedtls/aes.h" //LIBRARY FOR AES128 METHOD FOR ENCRYPTION
#include "mbedtls/base64.h"
#include "KickFiltersRT.h"

SET_LOOP_TASK_STACK_SIZE(90 * 1024);

KickFiltersRT<signed long> filtersRT2;

const int data_size = 32000;
uint8_t BLE_data1[data_size];
signed long index1 = 0;
int data_i = 0;
bool ONLYSD = false;
unsigned long progressbar_count = 0;
bool READ_SD_ONCE = false;
int ii = 0;
int16_t ECG_filter;
bool FILE_open = false;

float analogvalue;

// Define the GPIO pin that is connected to the battery.
const int GPIO_BATTERY = 35;
unsigned long previousMillis = 0;
const long interval = 60000;             // 1 minutes in milliseconds
uint8_t previousBatteryPercentage = 100; // Initial battery percentage
uint8_t battery_percentage, battery_percentage1;
File myFile;
int downloadInProgress = 0; // Flag to track whether file download is in progress
bool FILEHASDATA = false;

MAX30003 max30003;

const char *ssid = "FIBRIART-L";   // SSID for the Access Point
const char *password = "12345678"; // Password for the Access Point

WebServer server(80);

// Function to encrypt data using AES CBC mode
void encryptBLEData(uint8_t *data, size_t dataLength, uint8_t *encryptedData)
{
  mbedtls_aes_context aes;
  unsigned char key[16] = "itzkbgulrcsjmnv"; // Example key
  key[15] = 'x';
  unsigned char iv[16] = "abcdefghijklmno"; // Example initialization vector
  iv[15] = 'p';

  // Set up the encryption context
  mbedtls_aes_init(&aes);
  mbedtls_aes_setkey_enc(&aes, key, 128);

  // Encrypt the data using AES in CBC mode
  mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_ENCRYPT, dataLength, iv, data, encryptedData);

  // Clean up the AES context
  mbedtls_aes_free(&aes);
}

void byteArrayToBase64(const uint8_t *byteArray, size_t byteArrayLen, char *base64Str, size_t base64StrLen)
{
  size_t outputLen;
  mbedtls_base64_encode((unsigned char *)base64Str, base64StrLen, &outputLen, byteArray, byteArrayLen);
  base64Str[outputLen] = '\0'; // Null-terminate the base64 string
}

void handleRoot()
{
  server.send(200, "text/html", R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
      <title>ESP32 File Server</title>
      <style>
        body {
          font-family: Arial, sans-serif;
          background-color: #f4f4f4;
          margin: 0;
          padding: 0;
        }
        .container {
          width: 80%;
          margin: auto;
          overflow: hidden;
        }
        header {
          background: #50b3a2;
          color: #ffffff;
          padding-top: 30px;
          min-height: 70px;
          border-bottom: #e8491d 3px solid;
        }
        header a {
          color: #ffffff;
          text-decoration: none;
          text-transform: uppercase;
          font-size: 16px;
        }
        header ul {
          padding: 0;
          list-style: none;
        }
        header li {
          display: inline;
          padding: 0 20px 0 20px;
        }
        header #branding {
          float: left;
        }
        header #branding h1 {
          margin: 0;
        }
        header nav {
          float: right;
          margin-top: 10px;
        }
        header .highlight, header .current a {
          color: #e8491d;
          font-weight: bold;
        }
        #showcase {
          min-height: 400px;
          text-align: center;
          color: #ffffff;
          background: #50b3a2;
        }
        #showcase h1, #showcase p {
          background: #e8491d;
          display: inline-block;
          padding: 10px 20px;
          border-radius: 5px;
        }
        #showcase h1 {
          margin-top: 100px;
          font-size: 55px;
          margin-bottom: 10px;
        }
        #showcase p {
          font-size: 20px;
        }
        .button {
          display: inline-block;
          color: #ffffff;
          background: #e8491d;
          padding: 10px 20px;
          text-decoration: none;
          border-radius: 5px;
        }
        .content {
          padding: 20px;
          background: #ffffff;
          box-shadow: 0 0 10px rgba(0,0,0,0.1);
          margin-top: 20px;
        }
        footer {
          background: #333333;
          color: #ffffff;
          text-align: center;
          padding: 20px;
          margin-top: 20px;
        }
        footer p {
          margin: 0;
        }
      </style>
    </head>
    <body>
      <header>
        <div class="container">
          <div id="branding">
            <h1><span class="highlight">FIBRIART-L</span> File Server</h1>
          </div>
          <nav>
            <ul>
              <li class="current"><a href="/">Home</a></li>
              <li><a href="/list">List Files</a></li>
            </ul>
          </nav>
        </div>
      </header>
      <section id="showcase">
        <div class="container">
          <h1>Welcome to the FIBRIART-L File Server</h1>
          <p>Access and manage files stored on the FIBRIART-L's SD card.</p>
        </div>
      </section>
      <div class="container content">
        <h2>Available Files:</h2>
        <ul id="fileList"></ul>
      </div>
      <footer>
        <p>ESP32 File Server &copy; 2024</p>
      </footer>
      <script>
        async function fetchFiles() {
          const response = await fetch('/list');
          const files = await response.text();
          document.getElementById('fileList').innerHTML = files;
        }
        fetchFiles();
      </script>
    </body>
    </html>
  )rawliteral");
}

void handleFileList()
{

  // Attempt to mount the SD card
  while (!SD.begin(3))
  {
    Serial.println("Card Mount Failed11");
    return;
  }

  // myFile = SD.open("/hello.txt", FILE_READ);
  // Serial.println(myFile);
  // Check if the "hello.txt" file exists
  if (SD.exists("/hello.txt"))
  {
    // Generate the HTML response with a link to download the "hello.txt" file
    String output = "<h2>Files:</h2><ul>";
    output += "<li><a href='/download?file=/hello.txt'>hello.txt</a> - " + String(SD.open("/hello.txt").size()) + " bytes</li>";
    output += "</ul>";
    server.send(200, "text/html", output);
  }
  else
  {
    // If the "hello.txt" file doesn't exist, display a message indicating that it's not found
    server.send(404, "text/plain", "404: hello.txt Not Found");
  }
}

void handleFileDownload()
{
  downloadInProgress++; // Set the flag to true when the file download starts
  if (server.hasArg("file"))
  {
    String fileName = server.arg("file");
    Serial.print("Requested file to download: ");
    Serial.println(fileName);

    if (fileName.charAt(0) != '/')
    {
      fileName = "/" + fileName;
    }

    if (SD.exists(fileName))
    {
      File downloadFile = SD.open(fileName, FILE_READ);
      size_t fileSize = downloadFile.size();
      size_t totalBytesSent = 0;

      // Send HTTP headers
      server.setContentLength(fileSize);
      server.sendHeader("Content-Type", "application/octet-stream");
      server.sendHeader("Content-Disposition", "attachment; filename=" + fileName);
      server.sendHeader("Connection", "close");
      server.send(200, "application/octet-stream", "");

      // Determine the buffer size dynamically
      size_t bufferSize = uxTaskGetStackHighWaterMark(NULL) / 2; // Start with a default buffer size
      // if (ESP.getFreeHeap() > bufferSize * 2) {
      //   bufferSize = ESP.getFreeHeap() / 2; // Use half of the free heap for the buffer
      // }

      uint8_t *buffer = new uint8_t[bufferSize];
      size_t bytesRead;

      while ((bytesRead = downloadFile.read(buffer, bufferSize)) > 0)
      {
        server.client().write(buffer, bytesRead);
        totalBytesSent += bytesRead;
        Serial.printf("Bytes sent: %u/%u\n", totalBytesSent, fileSize);
      }

      delete[] buffer;
      downloadFile.close();
      Serial.println("File download completed.");
      if (SD.remove("/hello.txt"))
      {
        Serial.println("File successfully deleted");
      }
      else
      {
        Serial.println("ERROR in DELETION");
      }
      downloadInProgress++;
    }
    else
    {
      Serial.println("File not found: " + fileName);
      server.send(404, "text/plain", "404: File Not Found");
    }
  }
  else
  {
    Serial.println("File argument missing in request");
    server.send(400, "text/plain", "400: Invalid Request");
  }
}

void DatatoSD()
{
  if (!SD.begin(3))
  {
    Serial.println("Card Mount Failed");
  }

  size_t dataLength = sizeof(BLE_data1);

  uint8_t *encryptedData = new uint8_t[dataLength]; // Dynamically allocate memory for encrypted data
  // Encrypt the data
  encryptBLEData(BLE_data1, dataLength, encryptedData);

  char base64Str[data_size * 2]; // Make sure this is large enough to hold the encoded string
  byteArrayToBase64(encryptedData, dataLength, base64Str, sizeof(base64Str));
  delete[] encryptedData;

  if (SD.open("/hello.txt").size() == 0)
  {
    myFile = SD.open("/hello.txt", FILE_WRITE);
    FILE_open = true;
  }
  else
  {
    myFile = SD.open("/hello.txt", FILE_APPEND);
  }

  if (!myFile)
  {
    Serial.println("File open failed");
  }

  myFile.println(base64Str);
  // myFile.println(analogRead(1));
  myFile.flush(); // Ensure data is written to the SD card

  myFile.close();
  SD.end();
  FILEHASDATA = true;
  data_i = 0;
}

void setup()
{
  Serial.begin(57600);
  // delay(500);
  Serial.printf("Free stack0: %d\n", uxTaskGetStackHighWaterMark(NULL));
  pinMode(MAX30003_CS_PIN, OUTPUT);
  digitalWrite(MAX30003_CS_PIN, HIGH); // disable device

  filtersRT2.initmovingAverage(2); // for ECG

  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);

  bool ret = max30003.max30003ReadInfo();
  if (ret)
  {
    Serial.println("Max30003 read ID Success");
  }
  else
  {

    if (!ret)
    {
      // stay here untill the issue is fixed.
      ret = max30003.max30003ReadInfo();
      Serial.println("Failed to read ID, please make sure all the pins are connected");
      delay(10000);
    }
  }

  Serial.println("Initialising the chip ...");
  max30003.max30003Begin(); // initialize MAX30003
  delay(50);
  if (!SD.begin(3))
  {
    Serial.println("Card Mount Failed");
  }
  delay(50);
  // // Set up Access Point
  // WiFi.softAP(ssid, password);
  // IPAddress IP = WiFi.softAPIP();
  // Serial.print("AP IP address: ");
  // Serial.println(IP);

  // delay(50);

  // // Define routes
  // server.on("/", HTTP_GET, handleRoot);
  // server.on("/list", HTTP_GET, handleFileList);
  // server.on("/download", HTTP_GET, handleFileDownload);

  // server.begin();
  // Serial.println("HTTP server started");
  // delay(2000);
  // Serial.printf("Free stack0: %d\n", uxTaskGetStackHighWaterMark(NULL));
  // analogReadResolution(12);       // Set ADC resolution to 12 bits
  // analogSetAttenuation(ADC_11db); // Set attenuation to handle the full 0-3.3V range
  for (int e = 0; e < 5; e++)
  {
    analogWrite(0, 255);
    vTaskDelay(pdMS_TO_TICKS(1000));
    analogWrite(0, 0);
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void loop()
{

  if (getCpuFrequencyMhz() != 10)
  {
    setCpuFrequencyMhz(10);
  }
  unsigned long t1 = micros();

  max30003.getEcgSamples();

  ECG_filter = filtersRT2.movingAverage(max30003.ecgdata);
  Serial.println(ECG_filter);
  if (data_i < data_size)
  {

    BLE_data1[data_i] = ECG_filter;
    BLE_data1[data_i + 1] = ECG_filter >> 8;
    // BLE_data1[data_i + 2] = ECG_filter >> 16;
    // BLE_data1[data_i + 3] = ECG_filter >> 24;
    data_i = data_i + 2;
  }
  // Serial.printf("Free stack0: %d\n", uxTaskGetStackHighWaterMark(NULL));
  // Serial.println(data_i);
  if (data_i == data_size)
  {
    DatatoSD();
    Serial.println("data written to sd card");
  }

  if (digitalRead(10) == true && FILEHASDATA)
  {
    delay(100);
    if (digitalRead(10) == true)
    {
      if (getCpuFrequencyMhz() != 80)
      {
        setCpuFrequencyMhz(80);
      }

      // Set up Access Point
      WiFi.softAP(ssid, password);
      IPAddress IP = WiFi.softAPIP();
      Serial.print("AP IP address: ");
      Serial.println(IP);

      delay(50);

      // Define routes
      server.on("/", HTTP_GET, handleRoot);
      server.on("/list", HTTP_GET, handleFileList);
      server.on("/download", HTTP_GET, handleFileDownload);

      server.begin();
      Serial.println("HTTP server started");
      while (true)
      {
        Serial.println("adv");
        server.handleClient();
        if (downloadInProgress == 2)
        {
          Serial.println("adv stopped");
          downloadInProgress = 0;
          server.stop();
          WiFi.softAPdisconnect(true);
          FILEHASDATA = false;
          break; // Exit the loop when a client is connected
        }
      }
    }
  }

  unsigned long t2 = micros();
  // Serial.println((t2 - t1));
  if ((7812.5 > (t2 - t1)))
  {
    delayMicroseconds(7812.5 - (t2 - t1));
  }
}
