#include "pch.h"
#include <iostream>	
#include <unordered_map>
#include "oscpkt.hh"
#include "udp.hh"
#define OSCPKT_OSTREAM_OUTPUT
using namespace std;

//OSC constants
const int PORT_IN = 9109;			//Port of Target
const int PORT_OUT = 9110;			//Port of keyboard 
const string OSC_TARGET_ADDRESS = "localhost";	//Target ip address

//Overloaded functions for different OSC bundle content types
using namespace oscpkt;
UdpSocket* sock = nullptr;
unsigned throttle_rate = 10;
void send_osc(string address, double f) {
	static std::unordered_map<string, unsigned> throttle_counters;
	if (throttle_counters[address]++ % throttle_rate != 0) { return; }
	Message msg;
	PacketWriter pkt;
	pkt.startBundle().addMessage(msg.init(address).pushFloat(f)).endBundle();
	sock->sendPacket(pkt.packetData(), pkt.packetSize());
};
void send_osc(string address, bool b) {
	static std::unordered_map<string, unsigned> throttle_counters;
	if (throttle_counters[address]++ % throttle_rate != 0) { return; }
	Message msg;
	PacketWriter pkt;
	pkt.startBundle().addMessage(msg.init(address).pushBool(b)).endBundle();
	sock->sendPacket(pkt.packetData(), pkt.packetSize());
};
void send_osc(string address, string s) {
	static std::unordered_map<string, unsigned> throttle_counters;
	if (throttle_counters[address]++ % throttle_rate != 0) { return; }
	Message msg;
	PacketWriter pkt;
	pkt.startBundle().addMessage(msg.init(address).pushStr(s)).endBundle();
	sock->sendPacket(pkt.packetData(), pkt.packetSize());
};

int main()
{
	//Load DLL
	typedef int(__cdecl *proc_wooting_read_full_buffer)(uint8_t data[], unsigned int length);
	typedef bool(__cdecl *proc_wooting_rgb_reset)(void);
	typedef bool(__cdecl *proc_wooting_rgb_direct_set_key)(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
	typedef bool(__cdecl *proc_wooting_rgb_direct_reset_key)(uint8_t, uint8_t);

	HINSTANCE wooting_analog_sdk = LoadLibraryA("wooting-analog-sdk64.dll");
	HINSTANCE wooting_rgb_sdk = LoadLibraryA("wooting-rgb-sdk64.dll");

	proc_wooting_read_full_buffer wooting_read_full_buffer;
	wooting_read_full_buffer = (proc_wooting_read_full_buffer)GetProcAddress(wooting_analog_sdk, "wooting_read_full_buffer");

	proc_wooting_rgb_reset wooting_rgb_reset;
	proc_wooting_rgb_direct_set_key wooting_rgb_direct_set_key;
	proc_wooting_rgb_direct_reset_key wooting_rgb_direct_reset_key;

	wooting_rgb_reset = (proc_wooting_rgb_reset)GetProcAddress(wooting_rgb_sdk, "wooting_rgb_reset");
	wooting_rgb_direct_set_key = (proc_wooting_rgb_direct_set_key)GetProcAddress(wooting_rgb_sdk, "wooting_rgb_direct_set_key");
	wooting_rgb_direct_reset_key = (proc_wooting_rgb_direct_reset_key)GetProcAddress(wooting_rgb_sdk, "wooting_rgb_direct_reset_key");
	wooting_rgb_reset();

	//OSC initialisation
	UdpSocket sock;
	::sock = &sock;
	sock.connectTo(OSC_TARGET_ADDRESS, PORT_IN);
	sock.bindTo(PORT_OUT);
	PacketReader pr;

	//main loop
	for (;;) {
		unsigned char buf[256];
		memset(buf, 0x00, sizeof(buf));
		buf[0] = 0x01;
		buf[1] = 0x81;

		int res = wooting_read_full_buffer(buf, sizeof(buf));

		for (int i = 0; i <= 30; i += 2) {
			int key = buf[i];
			float val = buf[i + 1];

			//OSC bundle component types
			string adress;
			bool b;
			float f;
			string s;

			if (val != 0) {
				switch (key) {
				case 0:										//Escape
					break;
				case 1:										//F1

					break;
				case 2:										//F2

					break;
				case 3:										//F3

					break;
				case 4:										//F4

					break;
				case 5:										//F5

					break;
				case 6:										//F6

					break;
				case 7:										//F7
					val--;

					break;
				case 8:										//F8

					break;
				case 9:										//F9

					break;
				case 10:									//F10

					break;
				case 11:									//F11

					break;
				case 12:									//F12

					break;
				case 13:									//Prinstscreen

					break;
				case 14:									//Pause

					break;
				case 15:									//None (Mode)
					wooting_rgb_reset();
					break;
				case 16:									//Tilde

					break;
				case 17:									//Number1

					break;
				case 18:									//Number2

					break;
				case 19:									//Number3

					break;
				case 20:									//Number4

					break;
				case 21:									//Number5

					break;
				case 22:									//Number6

					break;
				case 23:									//Number7

					break;
				case 24:									//Number8

					break;
				case 25:									//Number9

					break;
				case 26:									//Number0

					break;
				case 27:									//Underscore

					break;
				case 28:									//Plus

					break;
				case 29:									//Backspace

					break;
				case 30:									//Insert

					break;
				case 31:									//Home
					s = "/scroll/x";
					throttle_rate = 1000 / val;
					send_osc(s, -val / 255);
					break;
				case 32:									//Tab

					break;
				case 33:									//Q

					break;
				case 34:									//W

					break;
				case 35:									//E

					break;
				case 36:									//R

					break;
				case 37:									//T

					break;
				case 38:									//Y

					break;
				case 39:									//U

					break;
				case 40:									//I

					break;
				case 41:									//O

					break;
				case 42:									//P

					break;
				case 43:									//OpenBracket

					break;
				case 44:									//CloseBracket

					break;
				case 45:									//Backslash

					break;
				case 46:									//Delete

					break;
				case 47:									//End
					val--;
					s = "/scroll/x";
					throttle_rate = 1000 / val;
					send_osc(s, val / 255);
					break;
				case 48:									//CapsLock
					val--;

					break;
				case 49:									//A

					break;
				case 50:									//S

					break;
				case 51:									//D

					break;
				case 52:									//F

					break;
				case 53:									//G

					break;
				case 54:									//H

					break;
				case 55:									//J

					break;
				case 56:									//K

					break;
				case 57:									//L

					break;
				case 58:									//Colon

					break;
				case 59:									//Quote

					break;
				case 60:									//Enter

					break;
				case 61:									//PageUp
					s = "/scroll/y";
					throttle_rate = 1000 / val;
					send_osc(s, -val / 255);
					break;
				case 62:									//PageDown
					s = "/scroll/y";
					throttle_rate = 1000 / val;
					send_osc(s, val / 255);
					break;
				case 63:									//Up

					break;
				case 64:									//ModiferLeftShift

					break;
				case 65:									//Z

					break;
				case 66:									//X

					break;
				case 67:									//C

					break;
				case 68:									//V

					break;
				case 69:									//B

					break;
				case 70:									//N

					break;
				case 71:									// M

					break;
				case 72:									//Comma

					break;
				case 73:									//Dot

					break;
				case 74:									//Slash

					break;
				case 75:									//ModifierRightShift
					val--;

					break;
				case 76:									//Left

					break;
				case 77:									//Down
					val--;

					break;
				case 78:									//Right
					val--;

					break;
				case 79:									//ModifierRightCtrl
					val--;

					break;
				case 80:									//ModifierLeftCtrl

					break;
				case 81:									//ModifierLeftUI

					break;
				case 82:									//ModifierLeftAlt

					break;
				case 83:									//Spacebar
					if (val > 0xe0) {
						s = "/play";
						send_osc(s, 1.0);
						break;
					}
					else { break; }
				case 84:									//ModifierRightAlt

					break;
				case 85:									//ModifierRightUi

					break;
				case 86:									//ModifierNone

					break;
				case 87:									//ModifierNone

					break;
				}
			}
			else
				break;
		};
		//Sleep the loop to drop cpu usage (miliseconds)
		Sleep(1);
	}
	FreeLibrary(wooting_analog_sdk);
	FreeLibrary(wooting_rgb_sdk);
}
