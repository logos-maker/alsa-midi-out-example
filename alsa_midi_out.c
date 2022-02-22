/* 
 * Daniel Viksporre 2022-02-22
 * Demo example of a ALSA MIDI out c-code.
 * Licence type: MIT No Attribution
 *
 * dependencies for compilation (please report missing dependencies to list).
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
 */

#include <alsa/asoundlib.h>
static snd_seq_t *seq;
static snd_seq_event_t ev_send;
static int port;


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

	for(int note=20;note<100;note+=12){	// go one octave higher for each time
		// A event to trigg a note
		printf("Press [Enter] key to play note %d\n",note);
		fflush(stdin); // clean stdin
		getchar(); // wait for ENTER

		// send MIDI message
		snd_seq_ev_clear(&ev_send);
		snd_seq_ev_set_source(&ev_send, port);
		snd_seq_ev_set_subs(&ev_send);
		snd_seq_ev_set_direct(&ev_send);
			// set event type, data, so on..
			ev_send.type = SND_SEQ_EVENT_NOTEON;
			snd_seq_ev_set_fixed(&ev_send);
			ev_send.data.note.velocity= 127;
			ev_send.data.note.note= note; // note value increased by loop
		snd_seq_event_output(seq, &ev_send);
		snd_seq_drain_output(seq);
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


