# breadWare


Hey guys, here's the PCB layout for the switch matrix board. Those holes should line up with the columns on a breadboard so this whole thing can be soldered directly onto the bottom. I just ordered 10 of these boards (in red soldermask! I've never been patient enough to wait the extra 24-48 hours to do a color other than green) so we can put a few together (I can teach whoever's interested how to hot air solder these surface mount parts) and then get on making those header pins on the sides move the correct number of electrons to make this thing work.
![Screenshot](MTMatrixBack.png)
#
 
That white rectangle is roughly the size of a breadboard. We might be able to get the size down if we use [TQFP](https://en.wikipedia.org/wiki/Quad_flat_package) instead of these huge [PLCC](https://en.wikipedia.org/wiki/Chip_carrier#Plastic-leaded_chip_carrier) sockets. Also could someone who reads chinese look through [this datasheet](https://datasheet.lcsc.com/szlcsc/Jiangsu-Qin-Heng-CH446Q_C109471.pdf) for the [CH446Q](http://www.wch-ic.com/products/CH446.html) and tell me if there are any significant differences from [MT8816](http://images.100y.com.tw/pdf_file/41-ZARLINK-MT8816.pdf) that we're using? Because the [JLCPCB](https://jlcpcb.com/) (who is making these boards for us) has the CH446Q in stock and they'll make the board and then solder them on so we can just order fully assembled boards ridiculously cheap (and the chips themselves are ~$1, so like a quarter of the price.)
![Screenshot](MTMatrixFront.png)
#
 
Notice how some of the pads in the middle (Plated Through Holes or PTH is the correct term just so you know) are more square and others are rounded. The squarer ones means that that pin is connected to the headers on the sides, so it can be used for measurement or power or whatever we decide when we make the control board. Those pins are: 0,1,8,16,24,31,32,40,48,56,63 which is the Y0 pin on each chip plus the ones that end up on the edges of the breadboard (31,63) and 1 because I had an extra header pin available. 
Pin 0 is connected to the Negative rail on the top of the breadboard, 31 is the top Positive rail, 32 is the bottom Negaitve rail, and 63 is the bottom Positive rail. Everything else is where you'd expect it to be.
![Screenshot](3DMTBack.png)
#
 
Yes, these pin headers are on the wrong side but I'm not gonna let some stupid auto-generated 3D model tell me which side to solder them on. I might spend some time finding the 3D models for the PLCC sockets and a breadboard so this looks closer to the real thing.
Also, notice the silkscreened lines on the top and bottom are just a bit off from the holes, turns out the power rails on a breadboard arent spaced perfectly on the same grid as the rest of the pins. The line is marked where it would be if the universe made any sense.
![Screenshot](3dMTFront.png)
#
 
Those letters along the middle correspond to the row labels on a breadboard.
![Screenshot](3dMTAngled.png)
#
  
Here's a zoomed out view of the whole schematic, those blue lines are busses, so they're actually 4 wires bundled together to make it look less messy. This is following JJ's brilliant idea for how to connect everything except I looped the ends back around so they conceptually should be thought of as a ring, If someone wants to map that out on one of those [Ring Lattice Graphs](https://i.imgur.com/oiiyKlz.png), it might make all this easier to visualize.
![Screenshot](MTSchematic.png)
#

On the left we have the address lines, control lines and power input. Except for the last three with numbers, these are connected to all 8 chips together, so to make just the one we want to control do stuff, we use the Chip Select lines on the right side (labeled CS_A-H) which are wired to each chip individually. If the CS is logic 0 (~0 Volts, LOW, whatever), that chip will ignore everything that's put on the address and control lines because that signal isn't meant for it and these chips ain't no eavesdroppin' bitches. (just for everyone's learning, Chip Select lines seem to almost always be Active Low, meaning they'll ignore everything when that pin is held at logic 1 (5V or 3.3V depending on the chip) which is denoted with a line above, C̅S̅ (or /CS because overlining text is a pain in the ass on a computer. and it's typically spoken as "bar", so "CS Bar" in this case) this chip is unusual because it's Active High.)
![Screenshot](MTControlPinHeaders.png)
#
 
Here's a zoomed part so you can see how I named these interconnects, it goes Pin Name (X0, X1, etc.) then the letters of the two chips it's connecting (in alpabetical order). So the pin labeled X0CD connects pin X0 on chip C to pin X0 on chip D. I had to do this like 5 times to get the wires to always connect to the same pin on both chips, but it should make the software much easier to deal with. 
![Screenshot](MTSchematicZoomed.png)
#
  
Here's the table I used to figure out which chips are connected where, and this helped me put them all in alphabetical order (because I can't tell you if F is before or after H without singing the alpabet song in my head every time) so the labels are the same on both sides of the wire. Each chip is connected the to the two before and after it. 
![Screenshot](SwitchInterconnectTable.png)
#
One day we will have this piece of paper framed and signed by JJ.
![Screenshot](JJConceptInterconnects.jpeg)

![](https://komarev.com/ghpvc/?username=Architeuthis-Flux&color=grey)
