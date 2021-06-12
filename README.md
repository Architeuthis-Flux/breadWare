# breadWare v0.2


Here's the second board revision. I was showing the [last version](breadWare/v0.1-alpha/) off to someone and they offhandedly mentioned that it would be cool if the whole thing fit under the breadboard. I knew this was my plan all along, but it forced me to finally design it before the code gets too specific to the original off-the-cuff design and I get stuck with my poor ~~life~~ design choices. 

## Matrix Board Changes

![2021-06-10-0002-3](https://user-images.githubusercontent.com/20519442/121673097-24e05280-cae3-11eb-87dd-462c0c393a41.jpg)

![MTmatrix](https://user-images.githubusercontent.com/20519442/121671360-0a0cde80-cae1-11eb-84ec-18e80a302420.png)

I found that the most valuable resource when finding paths between rows that aren't directly connected (so they need to make an intermediate hop) are "virtual" breadboard rows that aren't connected to the actual board but connect to the control board below and basically act as the general purpose nodes to do everything else (like reading voltages, connecting to the GPIO on the microcontroller, connecting to a second board, etc.). I put 4 of them in (on chips A, D, E, H), and I don't think that's nearly enough if I want to be able to do a bunch of special function things at once without making the user use some particular set of rows to do certain things. I would really like to make it as simple as "put anything wherever and the board will figure it out."

I changed the logical arrangement of the [MT8816](https://www.futurlec.com/Datasheet/Zarlink/MT8816AE.pdf) crosspoint switches to make the it easier to just hop over the middle space. In the original layout if I wanted to connect top row 5 to bottom row 5, it would always take 2 hops to do that (and these switches each have an on resistance of about 65Ω). Now the bottom rows are flipped so internally the bottom left is 62 and counts down to 33 on the right (note: these are abstracted away to the user by using b1-b30 instead of bigger numbers.)

And just to add some glam I added some reverse mounted LEDs that shine through the top and bottom power rails to show when that rail's power supply is on.

## Control Board Changes

![2021-06-10-0001-3](https://user-images.githubusercontent.com/20519442/121673107-2742ac80-cae3-11eb-9ab2-53451da856ce.jpg)

![breadControl](https://user-images.githubusercontent.com/20519442/121671448-1db84500-cae1-11eb-932b-a84a85dba7f8.png)


I almost entirely redesigned the controller board so it has a ton of whatever the hardware equivalent of syntactic sugar is. The original was kind of thrown together to make sure the concept of using crosspoint switches like that wasn't fundamentally stupid (still not entirely ruling that out, but so far it looks promising.) I also just realized I never did a writeup for the first iteration, and I'm not about to go back and do that just to prove that this board is an improvement.

### Power Stuff
Because I'm going to such great lengths to make everything analog, and I already need a negative supply for the crosspoint switches; I decided to make the adjustable power supply for the rails also be able to do negative voltages. The power stuff still needs some work, right now it gets pretty warm when it's running and it really doesn't like it when I remove the matrix board with it on. Presumably because it needs some current draw but I'm not exactly sure why it cares as much as it does (I burned a tantalum cap shaped brand into my finger, you could almost read the markings in it.) 

Also, the range of voltages available to each of the 4 rails is fixed. Next revision might include some way to multiplex the power rails (allowing any of them to be negative, right now only the bottom negative rail can do that and it can be changed with a solder jumper) but having looked into it for a while, there doesn't seem to be a way to do it simply enough to outweigh the small utility gained from it. If anyone knows a better way to do this, let me know.

I'm using an [LT1054](https://www.ti.com/lit/ds/symlink/lt1054.pdf?ts=1623386605661&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FLT1054%253Futm_source%253Dgoogle%2526utm_medium%253Dcpc%2526utm_campaign%253Dapp-null-null-GPN_EN-cpc-pf-google-sa%2526utm_content%253DLT1054%2526ds_k%253DLT1054%2526DCM%253Dyes%2526gclid%253DCj0KCQjw8IaGBhCHARIsAGIRRYqazWa1HYDDSNdHtIH7dqU-yqDqCnm6CfqRnDNBBC_sS4i2s7MQtgcaAuJ-EALw_wcB%2526gclsrc%253Daw.ds) switched capacitor bipolar voltage converter that takes in +5V and doubles it to +10V and also flips that to -10V. That powers the MT8816s directly and the op amps I'm using for the adjustable voltage supplies. The previous board used inductors for that, and aside from the electrical noise, you could hear them running. I know the frequency they run at is way higher than 20kHz but they must have been resonating with something (or I'm actually part bat) because it was super annoying.

The way I decided to make an adjustable supply that can be controlled by the mcu might be really dumb, but so far it seems to work. I didn't find any well documented examples of people doing it this way, so I'd love to hear from anyone who has or has a good reason why it will cause problems. 

I used an [MCP4728](http://ww1.microchip.com/downloads/en/devicedoc/22187e.pdf) DAC that's controlled by I2C and feeds it's 4 outputs into 2 [L272D](https://www.st.com/resource/en/datasheet/l272.pdf) power op amps that can apparently supply 1A each. I used the inverting arrangement with a gain of 1.6 to make the negative supply able to supply 0V - -8V from the 0V - +5V output from the DAC. I put a bunch of low voltage drop schottkys (or is it schottkies? weird) so whatever is happening on the power rails they're supplying shouldn't affect them too much. 

What's cool about this setup is that the DAC is fast enough to use this whole circuit as a DDS function generator. That's why I put solder jumpers to disconnect all the bypass caps. I haven't written the code to try this out yet though. 

### Special Functions

On the control board there's another 8x16 crosspoint switch that I'm using to multiplex the 8 passthrough connections (1 per chip on the top matrix board, but only 4 of them aren't connected to a breadboard pin) to the special functions on the bottom control board. 

The special functions are (a checkmark means the firmware is written enough to use it):
 [  ] 2 ADC channels that can be read by the microcontroller 
 [x] 2 digital potentiometer channels using an [MCP4661](https://ww1.microchip.com/downloads/en/DeviceDoc/22107B.pdf) 
 [  ] 5 GPIO pins directly connected to the [ATmega4809](http://ww1.microchip.com/downloads/en/DeviceDoc/ATmega4808-4809-Data-Sheet-DS40002173A.pdf)
 [x] 2 adjustable power supply channels (0 - 5V, 0 - -8V)
 [x] 1 ground

The 2 ADC channels go through two 4:1 voltage dividers to bring a possible ±10V input down to ±2.5V and the bottom of those dividers are connected to the V_REF pin on the microcontroller so analog signals can be biased to 0-5V to be read by the 10-bit ADC. 

There's an [MCP4661](https://ww1.microchip.com/downloads/en/DeviceDoc/22107B.pdf) dual 8 bit digital potentiometer that has all 3 pins on 2 independent pots connected to the special function switch. I need to come up with a better interface for setting it in the terminal menu. When I eventually get around to making a GUI for this it will definitely make these more useful. These also come in 4 different ranges (5K, 10K, 50K, 100K) with the same pinout, I currently have the 50K model in there so that means each step is 195 ohms.

The GPIO pins are super interesting, for some reason the [ATmega4809](http://ww1.microchip.com/downloads/en/DeviceDoc/ATmega4808-4809-Data-Sheet-DS40002173A.pdf) datasheet never mentions the clear superiority of the first 4 pins in port C. 
![Port C and E](https://user-images.githubusercontent.com/20519442/121649874-90b6c100-cacb-11eb-92d7-67cbc01a31eb.jpg)
PC0-3 can do I2C, U(S)ART, 16-bit Timer/Counter, PWM, SPI, and CCL-LUT! 
The CCL-LUT means Configurable Custom Logic - LookUp Table, so that means it an simulate an arbitrary logic gate. The analog stuff is covered by pin PE3. 

Now the tricky part is coming up with a nice way to use those functions through the serial connection. I could probably somehow port the Bus Pirate firmware into some sort of shell, so you could use their awesome Community Firmware (and the red PCB really sells the aesthetic.) 

Also, if anyone from Dangerous Prototypes is reading this; I'm dying for updates on (or just an official crushing of my dreams of one day owning) the [Bus Pirate Ultra](http://dangerousprototypes.com/forum/index.php?topic=10316.0)

### Communicating With Other Things

On each corner of the board I put a 2x2 header so you can connect these side-by-side and make these into longer board. They communicate via UART with Tx Rx in at the top left and Tx Rx out at the top right. The bottom corners have the 4 unconnected pins from the top matrix on both sides. So you could use these to make connections from one board to the other. As it is currently, having only 4 of these general purpose matrix connections is a huge bottleneck for doing just about anything so I probably need to come up with a way to have more of these free pins in the next revision. 

To communicate with the computer, I'm using an [MCP2221A](http://ww1.microchip.com/downloads/en/devicedoc/20005565b.pdf) to talk to the mcu via a dedicated UART. That chip is also connected to the I2C lines but I'm not sure what I'm going to use that for, maybe I'll have the I2C traffic passed along in some sort of debug mode. It also can send a reset signal from one of it's 4 GPIO lines to the ATmega4809 (with a solder jumper in case that causes problems), which notably is a missing feature of this chip. 
I could pretend that my choice to to use this was some principled stand against FTDI to cover for the fact that I actually don't remember why I chose this over and FTDI232.

## Making It Do Stuff

The interface currently sort of follows the general format that a Bus Pirate (and I'm sure a bunch of other things) use; mostly single character commands that present a sub menu. It's amazing how quickly you run out of letters that make sense for a particular command, especially when you have some command you want to be available globally. 

Making connections on the board just requires you to type in the two rows you want to connect, separated by a comma. Rows on the bottom are preceded by a 'b' so if you want to connect the 12th row on the top to the 24th row on the bottom you just type
 > 12,b24

In the near future I intend to make a full blown GUI for this, because that's the part that will make this thing actually make your life easier.

Here are some screenshots the interface:

Connecting Nodes
<img width="511" alt="Screen Shot 2021-06-11 at 4 58 42 PM" src="https://user-images.githubusercontent.com/20519442/121670695-4c81eb80-cae0-11eb-837f-d2716257b874.png">

Disconnecting Nodes
<img width="536" alt="Screen Shot 2021-06-11 at 4 57 02 PM" src="https://user-images.githubusercontent.com/20519442/121671306-f6fa0e80-cae0-11eb-80f4-6cc81e22b78a.png">



Turning on the power supplies
![Screen Shot 2021-06-11 at 5 00 05 PM](https://user-images.githubusercontent.com/20519442/121670839-76d3a900-cae0-11eb-8140-a3e641796f26.png)

Using the potentiometer
![Screen Shot 2021-06-11 at 5 03 24 PM](https://user-images.githubusercontent.com/20519442/121670934-8d7a0000-cae0-11eb-9c04-dff28cab8096.png)

Showing the frontend connections you've made
![Screen Shot 2021-06-11 at 5 04 14 PM](https://user-images.githubusercontent.com/20519442/121671034-aaaece80-cae0-11eb-97e7-26575fdda108.png)

Showing the backend (chip to chip) connections 
![Screen Shot 2021-06-11 at 5 04 45 PM](https://user-images.githubusercontent.com/20519442/121671140-c9ad6080-cae0-11eb-9881-4a8735643b8f.png)

##[Here's the documentation for v0.1](breadWare/v0.1-alpha/)
