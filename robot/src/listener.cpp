#include <VirtualWire.h>
#include "listener.h"
#define HEADER_BYTE 0xFF
#define FOOTER_BYTE 0x00
#define MAX_PACKET_LENGTH 4

static void (*listenerRegistar)(RemotePacketType*, char* data, byte len);

void remote_set_listener_registar(void (*registerer)(RemotePacketType*, char* data, byte len)){
  listenerRegistar = registerer;
}

void remote_listening(uint8_t port){
  vw_set_rx_pin(port);
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(2000);	 // Bits per sec
  vw_rx_start();       // Start the receiver PLL running
}

void _remote_parse_packet(byte* data, byte len) {
  RemotePacketType type = UNKOWN;
  switch (data[0]) {
    case 0:
      type = MOVEMENT_INFORMATION;
    break;
    case 1:
      type = SOUND_INFORMATION;
    break;
  }
  listenerRegistar(&type, (char *)data, len);
}

void remote_tick() {
  static bool listening = false;
  static byte packetByteIndex = 0;
  static byte packetData[MAX_PACKET_LENGTH];

  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  if (vw_get_message(buf, &buflen)) { // Non-blocking
    // Message with a good checksum received, dump it.
    for (int i = 0; i < buflen; i++) {
      if (listening) {
        listening = packetByteIndex < MAX_PACKET_LENGTH && buf[i] == FOOTER_BYTE;
        packetData[packetByteIndex] = buf[i];
        packetByteIndex++;
        if (!listening) {
          _remote_parse_packet(packetData, packetByteIndex);
        }
      } else {
        listening = !listening && buf[i] == HEADER_BYTE;
        packetByteIndex = 1;
      }
    }
  }
}
