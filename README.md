#   ESP32-Based Android App-Controlled Car with Live Camera Streaming and Distance Sensing


## Personal note
This project was part of my final-year coursework in the "Computers" specialization at university,  in the "Planning and Programming Embedded Systems" course.
I considered it to be the most advanced project in my class; no one else had attempted anything involving an Android app and live streaming.
This project was an upgrade to a project I did as a hobby back in 2017 when I was learning Arduino for fun before even beginning university. My professor was highly impressed, and I received a  score of 97 for this project."
[Here's the link to the blog post where I described the 2017 project. ](https://andreypodlesni.blogspot.com/2021/11/arduino-wifi-rc-car-project.html)
## Features

-   Movement in multiple directions: forward, backward, left, and right.
-   Ultrasound distance sensor to automatically halt before encountering obstacles.
-   Equipped with a camera for real-time streaming,
-   Servo motor for adjusting the camera and distance sensor horizontally.
-   Speed regulation through PWM (Pulse Width Modulation).
- Full car control, tweaking speed, and watching live video, through a custom Android app using WiFi.

## Video
[![enter image description here](https://i.ibb.co/x1fTmcF/Thumbnail.jpg)](https://youtu.be/eb2qzxag-yg)
## Pictures
![enter image description here](https://github.com/bbas121/Wifi_remote_control_carr/blob/main/Images/1.jpg?raw=true)
![enter image description here](https://github.com/bbas121/Wifi_remote_control_carr/blob/main/Images/2.jpg?raw=true)
![enter image description here](https://github.com/bbas121/Wifi_remote_control_carr/blob/main/Images/3.jpg?raw=true)
## Hardware

 - ESP32 WROOM-32
- Ultrasonic Sensor HC-SR04
- SG90 _Servo_
- LM596 Step Down Converter
- 2x 18650 Batteries Holder Case
- 2x 18650 Batteries
-  DRV8833 Dual H-Bridge Motor Driver
-  2x DC Motor Size 130
-  4WD Robot Smart Car Chassis
- ESP32 CAM
## Scehmatics
![enter image description here](https://github.com/bbas121/Wifi_remote_control_carr/blob/main/Images/Schematics.jpg?raw=true)
  
1.  **Motor Control Pins:**
    -   GPIO 18 is connected to `BIN1` on the  Motor Driver.
    -   GPIO 19 is connected to `BIN2` on the  Motor Driver.
    -   GPIO 15 is connected to `AIN1` on the  Motor Driver.
    -   GPIO 2 is connected to `AIN2` on the  Motor Driver.
    - `BOUT 1` is connected to `V-` on the motor 1
    - `BOUT 2` is connected to `V+` on the motor 1
    - `AOUT 2` is connected to `V-` on the motor 2
    - `AOUT 1` is connected to `V+` on the motor 2
2.  **Ultrasonic Sensor (HC-SR04) Pins:**
    
    -   GPIO 23 is connected to `trigPin` (Trigger Pin).
    -   GPIO 22 is connected to `echoPin` (Echo Pin).
3.  **Servo Motor Pin:**
    -   GPIO 26 is connected to `servoPin` for controlling the servo motor.
    
    All other pins are voltage pins
     

## Android RC Car Controller App"
![enter image description here](https://github.com/bbas121/Wifi_remote_control_carr/blob/main/Images/APPedited.jpg?raw=true)

 - This application was developed using the MIT App Inventor platform.
 - In the video stream windows it is supposed to show the video stream because I disassembled this project to use the microcontrollers for another project i cannot show it in the screenshot but you can see the app in action in the video
 - the PWM++ and PWM-- buttons were for testing, the speed control itself is using the faster and slower buttons.
 - I initially intended to include a passive buzzer in the setup, as shown in the photos. That is why there is a BipBip Button However, I encountered issues with signal interference between the buzzer's frequency and that of the motor. buzzer so I abandoned that idea.
 - Where is the Written obstacle detected that is the text that is printed when receiving a response from the ESP32 as you can see in the video.
# Development process
The initial project utilized a 4 motor setup and 2x L298 motor driver, which had a built-in step-down to 5V. these motor drivers were gone a long time ago so I bought what I could get in Israel which was the two DRV8833 drivers it didn't have a step-down converter which is the reason I had to use an external M596 Step Down Converter that I've had.

i had some issues with 4 motors so i I switched to two motors

##   Libraries Utilized in This Project
 -   WiFi
 -   Web Server
 -   Servo
## Working Principle of the Code
Due to differences in hardware architecture, the ESP32 doesn't utilize the `analogWrite()` function for PWM (Pulse Width Modulation) like other boards. Instead, it employs a more sophisticated method known as LEDC (LED Control), providing greater precision and more channels. To use PWM on the ESP32, you first configure the frequency, resolution, and channels with:
  
      ledcSetup(channel, frequency, resolution);
Then, you associate the GPIO pins with the specified channel using:

    ledcAttachPin(GPIO_pin, channel);
To maneuver the motors, there are five distinct functions corresponding to each movement direction: forward, backward, left, right, and stop. Whenever the web server receives a request specifying a particular direction, it triggers the relevant function. Each function configures the PWM channels to align with the desired motor direction. Additionally, after setting the motor's direction, the function readjusts the servo's position, as the motor movement tends to affect the servo's orientation.

The function returns a successful HTTP response with the current position or status to the client.

Upon receiving a request for "Camera Left" or "Camera Right", the ESP32 triggers the corresponding function to shift the camera's servo in the specified direction. After adjusting the servo's position, it sends the updated angle back to the client.

The speed adjustment functions are simple: whenever a "slower" or "faster" request is received by the web server, it either increments or decrements the speed variable by 10, ensuring the speed stays within the 0 to 100 range. The direction functions translate the speed variable into a PWM value ranging from 0 to 255. using `map()` function.

To measure the distance in the loop function, it measures the distance every 100ms using some simple calculations that involve taking the time difference between the emission and reception of the ultrasonic pulse. The distance is calculated by multiplying this time interval by the speed of sound in air, and then dividing by two since the sound wave travels to the object and back.
When the distance is less than  20cm and the car is not moving backwards the stop function gets activated.


## Webcam Implementation

I utilized an ESP32 CAM for the video streaming . The webserver code for handling the video stream was directly adopted from the Arduino example "CameraWebServer" without any modifications.
This "CameraWebServer"  provides a webpage that displays a live feed from the camera.


## Android App Development
![enter image description here](https://github.com/bbas121/Wifi_remote_control_carr/blob/main/Images/appInventor.jpg?raw=true)
The app was created using MIT App Inventor, which allows for app layout design through a user-friendly drag-and-drop interface and programming via block-based coding. Upon initialization, the app requires the user to input the IP addresses of both the ESP32 and the ESP32 CAM. This leads the user to the main control page of the car. For displaying the video feed, a web component is integrated that shows the live stream from the camera. Control commands are executed through button presses in the app, each sending a specific web request (corresponding to the pressed button's function) to the ESP32's IP address. Additionally, releasing any of the steering buttons triggers a stop request.
[  
URL to the image of the full code blocks.](https://github.com/bbas121/Wifi_remote_control_carr/blob/main/Images/blocks.png?raw=true)

    


