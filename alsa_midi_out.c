/* 
 * Daniel Viksporre 2022-02-22 - (22 feb 2022)
 * ***************************************************************
 * Demo example of a ALSA MIDI out in C code.
 * The purpose is to get you started programming MIDI out code.
 * ***************************************************************
 * Licence type: MIT No Attribution
 *
 * dependencies for compilation (please report missing dependencies to list):
 *	sudo apt-get install libasound2-dev
 * 
 * compile with: 
 *	gcc -o alsa_midi_out alsa_midi_out.c -lasound
 *
 * start program in a terminal with:
 *	./alsa_midi_out
 *
 * how to use program:
 * 	Start program in terminal it will give a MIDI out unit to connect to a soundsource with MIDI-in.
 *	Connect it together with for example a program like aconnectgui
 *	When ready press return for the first note.
 *	Continue to press return untill all 7 notes are played,
 *	then program exits automatically.
 *
 * All the sequencer events are stored in a sequencer event record, snd_seq_event_t type.
 * The snd_seq_event_t struct declaration can be found and printed to the terminal with:
 * 	cat /usr/include/alsa/seq_event.h
 *
 * help wanted:
 * 	corrections and improvements of code comments, that improves understanding.
 *	more complete listing of dependencies	
 */


#include <alsa/asoundlib.h>
static snd_seq_t 	*seq;
static snd_seq_event_t	ev_send; // cat /usr/include/alsa/seq_event.h
static int 		port;


// wait function for pressing ENTER on computer keyboard
void wait_for_return_key(void){ // Function has nothing to do with ALSA
		fflush(stdin);
		getchar(); // wait for pressing ENTER on computer keyboard
}

int main(){
	if(snd_seq_open(&seq, "default", SND_SEQ_OPEN_OUTPUT, 0)<0){	// Create a alsa client.
		puts("Could not open sequencer");exit(1);		// error handling
	}
	if(snd_seq_set_client_name(seq, "my MIDI out")<0){		// Give you client a name (not expicity needed, will work without it).
		puts("Could not set client name");exit(1);		// error handling
	}
	if(port = snd_seq_create_simple_port(seq, "my port",		// Create a MIDI out port.
		SND_SEQ_PORT_CAP_READ|SND_SEQ_PORT_CAP_SUBS_READ,	// MIDI-out
                SND_SEQ_PORT_TYPE_APPLICATION)<0)
	{
		puts("Could not open port");exit(1);			// error handling
	}


	// Set up the MIDI event struct for sending normal MIDI messages
	snd_seq_ev_clear(&ev_send);		// resets the event struct
	snd_seq_ev_set_source(&ev_send, port);
	snd_seq_ev_set_subs(&ev_send);
	snd_seq_ev_set_direct(&ev_send);	// direct dispatch mode, send event to destination immediately without queing


	for(int note=20;note<100;note+=12){	// go one octave higher for each time

		// Wait for a incomming computer keyboard event. Then send MIDI message after that.
		printf("Press [Enter] key to play note %d\n",note);
		wait_for_return_key();
	
		
		//	snd_seq_ev_set_noteon(&ev_send,0,note,127);	// this row is similar to the rows below...
		//	ev_send.type = SND_SEQ_EVENT_NOTEON; 		// Make it an note on message
		//	snd_seq_ev_set_fixed(&ev_send);			// Make it a fixed size message, not SYSEX data
		//	ev_send.data.note.velocity= 127;		// Set the note on velocity to 127
		//	ev_send.data.note.note= note; 			// Set note value for the note on message.

		
		// Send MIDI note-on message
		snd_seq_ev_set_noteon(&ev_send,0,note,127);	// ev,channel,note,velocity.
		snd_seq_event_output(seq, &ev_send);		// pump it into queue
		snd_seq_drain_output(seq);			// send message


		// Wait for a incomming computer keyboard event. Then send MIDI message after that.
		printf("Press [Enter] key to end note %d\n",note);
		wait_for_return_key();

		// Send MIDI note-off message
		snd_seq_ev_set_noteoff(&ev_send,0,note,0); 	// ev,channel,note,velocity
        	snd_seq_event_output(seq, &ev_send);		// pump it into queue
        	snd_seq_drain_output(seq);			// send message

	}
}

/*
MIT No Attribution

Copyright <2022> <Daniel Viksporre>

Permission is hereby granted, free of charge, to any person obtaining a copy of this
software and associated documentation files (the "Software"), to deal in the Software
without restriction, including without limitation the rights to use, copy, modify,
merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


