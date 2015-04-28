receiver for the rfm12
dependency: python, gnuradio, some SDR.

use fsk-gui.py to dump a signal, then use:

 $ ./decode <capture

decoder dependencies:
 $ pip install bitarray

example capture:
capture.xz

so you can test the decoder like this:

 $ xz -dc capture.xz | ./decode.py | less

if you want you can change the gnuradio capture program by modifying the 
fsk.grc file.

bands.py converts frequencies into the RFM frequency parameter.
