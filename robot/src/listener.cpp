#include <VirtualWire.h>
#include "listener.h"
#define HEADER_BYTE 0xFF
#define FOOTER_BYTE 0xFE
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
static byte packetByteIndex = 0;
static byte packetData[MAX_PACKET_LENGTH];

void remote_tick() {
  // static bool listening = false;

  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  if (vw_get_message(buf, &buflen)) { // Non-blocking
    // Message with a good checksum received, dump it.
    for (char i = 0; i < buflen; i++) {
      Serial.println(buf[i]);
      switch (buf[i]) {
        case FOOTER_BYTE:
          _remote_parse_packet(packetData, packetByteIndex);
        break;
        case HEADER_BYTE:
          for (int y = 0; y < MAX_PACKET_LENGTH; y++) {
            packetData[y] = 0;
          }
          packetByteIndex = 0;
        break;
        default:
          packetData[packetByteIndex] = buf[i];
          packetByteIndex++ ;
        break;
      }
      // packetData[packetByteIndex] = buf[i];
      // if (listening) {
        // listening = packetByteIndex < MAX_PACKET_LENGTH && buf[i] == FOOTER_BYTE;
        // if (!listening) {
          // _remote_parse_packet(packetData, packetByteIndex);
          // packetByteIndex = 0;
        // }
      // } else {
        // listening = !listening && buf[i] == HEADER_BYTE;
      // }
      // packetByteIndex++;
    }
  }
}
