enum RemotePacketType{
  MOVEMENT_INFORMATION = 0,
  SOUND_INFORMATION = 1,
  UNKOWN = -1
};
void remote_tick();
void remote_listening(uint8_t port);
void remote_set_listener_registar(void (*registerer)(RemotePacketType*, char* data, byte len));
