# yahboom

Apps intended for the Yahboom Pico Sensor Kit:

* https://www.amazon.com/dp/B09C3NW8DX
* http://www.yahboom.net/study/Pico-Sensor-Kit
* https://github.com/YahboomTechnology/Pico-sensor-kit
  * backup: https://drive.google.com/file/d/11lvc1O39g4hefDUQW0L77SI8HteHyOC_/view?usp=sharing

## Setup

1. Put the Pico in MicroPython mode:
   https://www.raspberrypi.com/documentation/microcontrollers/micropython.html

1. Install Thonny:

   ```
   python3 -m venv venv
   source venv/bin/activate
   python3 -m pip install -r requirements.txt
   ```

2. Run Thonny:

   ```
   thonny
   ```

3. Copy the `//mpy/lib/yahboom/*.py` files one at a time via Thonny's
   **File** > **Save copy** > **Raspberry Pi Pico** workflow.

   (You could probably copy them all in one go via Pico's standard USB Mass
   Storage Device workflow.)
