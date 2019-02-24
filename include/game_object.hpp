#ifndef BATTERIES_SERVER_GAME_OBJ_HPP
#define BATTERIES_SERVER_GAME_OBJ_HPP

#include <string>

class game_object
{
public:
  double x;
  double y;
  int id;
  game_object(double x, double y, int id);
  game_object(const game_object& other);
};

#endif
