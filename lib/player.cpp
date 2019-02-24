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
  this->dx = player_data["dx"].asDouble();
  this->dy = player_data["dy"].asDouble();
  this->x = player_data["x"].asDouble();
  this->y = player_data["y"].asDouble();
  this->status = static_cast<Status>(player_data["status"].asInt());
}

Json::Value player::to_json() {
  Json::Value root;
  root["x"] = this->x;
  root["y"] = this->y;
  root["id"] = this->id;
  root["color"] = this->color;
  root["dx"] = this->dx;
  root["dy"] = this->dy;

  return root;
}