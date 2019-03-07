#include "battery.hpp"

battery::battery() : game_object(0, 0, 0)
{
}

battery::battery(double _x, double _y, int _id) : game_object(_x, _y, _id)
{
}

battery::battery(const battery &other) : game_object(other.x, other.y, other.id)
{
}

Json::Value battery::to_json() {
  Json::Value root;
  root["x"] = x;
  root["y"] = y;
  root["id"] = id;
  return root;
}