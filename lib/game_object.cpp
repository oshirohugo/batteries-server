#include "game_object.hpp"

game_object::game_object(double _x, double _y, int _id) : x(_x), y(_y), id(_id)
{
}

game_object::game_object(const game_object &other) : x(other.x),
                                                     y(other.y),
                                                     id(other.id)
{
}