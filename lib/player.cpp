#include "player.hpp"

player::player(
    double _x, double _y, std::string _color, Status _status, int _id) : game_object(_x, _y, _id),
                                                                         status(_status),
                                                                         color(_color),
                                                                         dx(0),
                                                                         dy(0),
                                                                         remaining_time(player::MAX_TIME),
                                                                         updated_at(0),
                                                                         username(""),
                                                                         points(0)
{
}

player::player() : game_object(0, 0, 0),
                   status(LIVE),
                   color("#FFFFFF"),
                   dx(0),
                   dy(0),
                   remaining_time(player::MAX_TIME),
                   updated_at(0),
                   username(""),
                   points(0)
{
}

player::player(const player &other) : game_object(other.x, other.y, other.id),
                                      status(other.status),
                                      color(other.color),
                                      dx(other.dx),
                                      dy(other.dy),
                                      remaining_time(other.remaining_time),
                                      updated_at(other.updated_at),
                                      username(""),
                                      points(0)
{
}

void player::update(Json::Value player_data)
{
  dx = player_data["dx"].asDouble();
  dy = player_data["dy"].asDouble();
  x = player_data["x"].asDouble();
  y = player_data["y"].asDouble();
  status = static_cast<Status>(player_data["status"].asInt());
  remaining_time = player_data["remainingTime"].asDouble();
  updated_at = player_data["updateAt"].asDouble();
  points = player_data["points"].asInt();
  username = player_data["username"].asString();
}

Json::Value player::to_json()
{
  Json::Value root;
  root["x"] = x;
  root["y"] = y;
  root["id"] = id;
  root["color"] = color;
  root["dx"] = dx;
  root["dy"] = dy;
  root["status"] = status;
  root["remainingTime"] = remaining_time;
  root["updatedAt"] = updated_at;
  root["username"] = username;
  root["points"] = points;

  return root;
}