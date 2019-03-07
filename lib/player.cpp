#include "player.hpp"


player::player(double _x, double _y, std::string _color, Status _status, int _id) : game_object(_x, _y, _id),
                                                                                    status(_status),
                                                                                    color(_color),
                                                                                    dx(0),
                                                                                    dy(0)
{
}

player::player() : game_object(0, 0, 0),
                   status(LIVE),
                   color("#FFFFFF"),
                   dx(0),
                   dy(0)
{
}

player::player(const player& other) : game_object(other.x, other.y, other.id),
                   status(other.status),
                   color(other.color),
                   dx(other.dx),
                   dy(other.dy)
{
}

void player::update(Json::Value player_data) {
  dx = player_data["dx"].asDouble();
  dy = player_data["dy"].asDouble();
  x = player_data["x"].asDouble();
  y = player_data["y"].asDouble();
  status = static_cast<Status>(player_data["status"].asInt());
}

Json::Value player::to_json() {
  Json::Value root;
  root["x"] = x;
  root["y"] = y;
  root["id"] = id;
  root["color"] = color;
  root["dx"] = dx;
  root["dy"] = dy;
  root["status"] = status;

  return root;
}