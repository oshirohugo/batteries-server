#ifndef BATTERIES_SERVER_PLAYER_HPP
#define BATTERIES_SERVER_PLAYER_HPP

#include <string>
#include <jsoncpp/json/json.h>

#include "game_object.hpp"

enum Status
{
  LIVE,
  CHARGING,
  DEAD
};

class player : public game_object
{
public:
  Status status;
  std::string color;
  double dx;
  double dy;

  player();
  player(double x, double y, std::string color, Status status, int id);
  player(const player& other);

  void update(Json::Value player_data);

  Json::Value to_json();
};

#endif
