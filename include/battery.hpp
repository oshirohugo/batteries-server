#ifndef BATTERIES_SERVER_BATTERY_HPP
#define BATTERIES_SERVER_BATTERY_HPP

#include <string>
#include <jsoncpp/json/json.h>

#include "game_object.hpp"

class battery : public game_object
{
public:
  battery();
  battery(double x, double y, int id);
  battery(const battery &other);

  Json::Value to_json();
};

#endif
