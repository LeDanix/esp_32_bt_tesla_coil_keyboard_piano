# esp_32_bt_tesla_coil_keyboard_piano
Bluetooth communication between Tesla Coil and ESP32 microcontroler and jeyboard piano driver. 'C' powered

## Project target and description
This project was created in order to allow a bluetooth communication between a Tesla Coil module and a ESP32 DevKit device. In adittion, the source code allows to play piano notes into the tesla coil interacting with the keyboard.

To use it, the user will need a computer, a tesla coil bluetooth friendly, a ESP32 and a micro USB to USB wire.

### Computer
It will be conected to ESP32 through the USB wire and it will be the master device. It will send, via UART, the KeyBoard keys the user press.
In addition, it will be needed to flash the microcontroller &rarr; [Flashing proccess](#flashing).

### Tesla Coil
Tesla coil device which could generate some eletric arcs with specific frecuencies. In my case, I was using this one that came from [Aliexpress](https://es.aliexpress.com/item/1005005065300797.html). Sure you can find a cheapest one ;).

<p align="center">
<img src="https://user-images.githubusercontent.com/74117305/233836317-1033bd0f-f2e3-4456-ae69-a904ed450060.png"  width="200" height="200">
</p>

The most important thing to be into account is that the device is proveided with a Bluetooth module. 

### ESP32
This tiny microcontroller is a powerfull device provided with many features like Bluetooth or Wifi modules. In my case, I was using the DevKit ESP32 which provide a easy way of flashing, supplying and communication.

This device actuates like a reciver/sender. On the one hand, it is always listening from the UART port connected to the computer, computing the key selected and converting it into a nature frecuency. On the other hand, Bluetooth module enters into action and send the note sound array, created in the previous phase, to the Tesla Coil.

[Purchase link](https://es.aliexpress.com/item/33048658274.html?spm=a2g0o.order_list.order_list_main.40.7d90194dI1wGpE&gatewayAdapt=glo2esp)

<p align="center">
<img src="https://user-images.githubusercontent.com/74117305/233836698-23d3b0f9-e4fb-4bd9-a51f-18c740f2a857.png"  width="200" height="200">
</p>

## How to use

### Clone the repo
The first step is clone the repository into your local computer.

### Flashing
Once you have cloned the repository, you should flash the source code into your ESP32 Dev Kit. 
There are many tools to do this on the internet but I recommend you the Platformio IDE extension. This extension allows you to select a board to use, in this case, ESP32 DevKit and configurate your project with the requirements. In that case, the repository already contains the configuration needed.

[Youtube video explaining how to create and use projects in Platformio](https://www.youtube.com/watch?v=P5QbBnwn-G0)

Take into account that if you create a new project in Platformio, you should replace all the files created by the ones into this repository.

### Tesla Coil name
There is an important variable to be changed into the code.
* `device_name`: a string that contains the name of the Tesla Coil device. It use it in order to connect with it. You must replace the text value of the variable and rename to your Tesla Coil name. This variable could be founded into `main.cpp` file.

```c++
const char* device_name = "xxxxxx";  // where "xxxxxx" is the name of your tesla coil device
```

You could find this name using the bluetooth feature of your mobile phone.

### Piano play
The keyboard have been mapped between kays and musical notes. The following table represents the kays associated to each musical note from the fourth octave.

![image](https://user-images.githubusercontent.com/74117305/233836199-359452c2-4bdd-4a82-aada-e4f63e3ac45a.png)

If this keys diposition are wrong to you, be free to change it locally into `frec_map` modifying each case of the switch-case.
