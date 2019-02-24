#define CATCH_CONFIG_MAIN

#include <catch/catch.hpp>

#include "player.hpp"

TEST_CASE( "create player" ) {
  std::string color = "#123456";
  Status status = LIVE;
  int id = 0;
  double x = 1.0, y = 2.0;
  player p1 = player(x, y, color, status, id);
    REQUIRE( p1.x == x );
    REQUIRE( p1.y == y );
    REQUIRE( p1.color == color );
    REQUIRE( p1.id == id );
}
