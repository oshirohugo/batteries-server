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
  GAME_SET,
  GAME_UPDATE,
};

class message
{
public:
  msg_type type;
  Json::Value payload;
  message(std::string msg);
  message(msg_type type_, Json::Value payload_);
  message(const message &other);

  Json::Value to_json();

  std::string to_string();
};

#endif
