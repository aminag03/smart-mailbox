# 📬 Smart Mailbox

Smart Mailbox is an **IoT solution** that allows users to receive real-time notifications about new mail through a web application. The application displays the mailbox status and communicates with **Firebase Realtime Database**.

## Installation and Running

1️⃣ **Clone the repository**

```bash
git clone https://github.com/aminag03/smart-mailbox.git
cd smart-mailbox
```

2️⃣ **Upload the code to NodeMCU ESP8266 via Arduino IDE**

Install the ESP8266 board package

Install the Firebase ESP Client library

Enter your Wi-Fi and Firebase credentials

3️⃣ **Enter your Firebase credentials in index.html**

4️⃣ **Run the web application**


## Features

✔ **Detection of new mail** using the TCRT5000 sensor

✔ **Sending mailbox status** ("Mail detected" or "Mailbox is empty") to Firebase

✔ **Displaying mailbox status** in the web application

✔ **Automatic activation of LED indicator** for three seconds when mail is detected

✔ **Automatic LED indicator reset** after the timeout

## Technologies Used

**Microcontroller**: NodeMCU ESP8266

**Sensor**: TCRT5000 IR sensor

**Cloud database**: Firebase Realtime Database

**Frontend**: Web application for mailbox status display (HTML and CSS)

**Programming languages**: C++ (Arduino), JavaScript (for the web application)

## Project Goal

Smart Mailbox enhances convenience and efficiency in mail reception, allowing users to know at any moment whether they have received mail without the need for physical mailbox checks. Additionally, the system facilitates communication between users and postal workers by providing a visual confirmation of successful delivery.
