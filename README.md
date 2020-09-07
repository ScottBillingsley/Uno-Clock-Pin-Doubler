# Uno-Clock-Pin-Doubler
Doubles the freq of the Timer pin on arduino Uno and Nano

This is the Clock and Not Clock generator used in another project..It uses the internal comparator along with a 
15n capacitor to generate a delayed Not Clock..The Clock and Not Clock are input into one gate of a Quad XOR, CD4070,
to produce a pulsed wave output at twice the input frequency...
