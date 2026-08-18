
# 🚶‍♂️ Light Motion Control System (PIR + PIC Microcontroller)

## 📖 Overview
This project implements a **Light Motion Control System** using a **PIR (Passive Infrared) motion sensor** and a **PIC microcontroller**.  
The system automatically turns ON a light when motion is detected and turns it OFF after a certain delay when no motion is present.

This helps in **energy saving** and is widely used in smart lighting systems.

---

## 🚀 Features
- Motion detection using PIR sensor  
- Automatic light ON/OFF control  
- Adjustable delay (software-based)  
- Relay-based AC load switching  
- LCD display for system status (optional)  

---

## 🧰 Components Used
- PIC Microcontroller (e.g., PIC18F4520)  
- PIR Motion Sensor (HC-SR501 or similar)  
- BC337 Transistor  
- 12V Relay  
- Diode (1N4007)  
- 16x2 LCD (LM016L) *(optional)*  
- Resistors  
- Power Supply  

---

## ⚙️ How It Works
1. The **PIR sensor** detects motion (infrared changes from human/animal movement).  
2. When motion is detected, the sensor outputs a HIGH signal.  
3. The **PIC microcontroller** reads this signal.  
4. Based on the input:
   - 🟢 **Motion Detected → Relay ON → Light ON**  
   - 🔴 **No Motion → Delay → Relay OFF → Light OFF**  
5. The delay ensures the light remains ON briefly after motion stops.  
6. LCD (if used) displays motion status.

---

## 🔌 Circuit Description
- PIR sensor output is connected to a **digital input pin** of the PIC.  
- Microcontroller processes the signal and controls output.  
- A **BC337 transistor** drives the relay.  
- A **flyback diode (1N4007)** protects the circuit from voltage spikes.  
- LCD displays system status (optional).

---

## 💡 Applications
- Automatic room lighting  
- Smart home systems  
- Security lighting  
- Corridor and stair lighting  

---

## 🔧 Future Improvements
- Adjustable delay using buttons  
- Sensitivity control for motion detection  
- IoT integration (remote monitoring)  
- Combine with LDR for day/night detection  

---

## 👨‍💻 Author
**Anushka Bakare**  

