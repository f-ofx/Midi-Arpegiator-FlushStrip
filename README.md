# Midi-Arpegiator-FlushStrip

Pretty simple Arpegiator, main thing of interest here is the flush/strip idea taken from Max/msp.

From the Max manual: "When working with MIDI synthesizers and samplers, one of the most distracting situations is when a note-on message is sent to a device, but no note-off message is sent. This results in “stuck notes”, where the note is sounded but never turned off. This is never a good result, and may be seen as a “bug” by anyone that uses your patch."

The idea in this code is to do away with really listening to the note off information. The note ons are collected in a big group, and if a note already exists at the same note pitch as an incoming note it's turned off and removed from the set. 

There's an emergency flush all, and flush single. Example is made with a simple arp using the host's timing information. 
