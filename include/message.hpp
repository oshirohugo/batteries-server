#ifndef BATTERIES_SERVER_MESSAGE_HPP
#define BATTERIES_SERVER_MESSAGE_HPP

#include <string>
#include <jsoncpp/json/json.h>

enum class msg_type
{
  PLAYER_INIT,
  PLAYER_JOIN,
  PLAYER_UPDATE,
  PLAYER_LEAVE,
  BATTERY_CONSUMPTION,
};

class message
{
public:
  int type;
  Json::Value payload;
  message();
  message(int type_, Json::Value payload_);
  message(const message &other);

  Json::Value to_json();
};

#endif
