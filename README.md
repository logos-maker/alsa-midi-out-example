# alsa-midi-out-example
Linux example code in C for ALSA MIDI out. Could not find any working short example, so I made this.

### dependencies for compilation:
  sudo apt-get install libasound2-dev
  
### compile with: 
  gcc -o alsa_midi_out alsa_midi_out.c -lasound

### start program in a terminal with:
  ./alsa_midi_out

## how to use program:
 *  Start program in terminal it will give a MIDI out unit to connect to a soundsource with MIDI-in.
 *	Connect it together with a program like aconnectgui
 *	When ready press return for the first note.
 *	Continue to press return untill all 7 notes are played,
 *	then program exits automatically.
