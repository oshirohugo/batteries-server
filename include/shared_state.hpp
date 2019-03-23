//
// Copyright (c) 2018 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/vinniefalco/CppCon2018
//

#ifndef BOOST_BEAST_EXAMPLE_WEBSOCKET_CHAT_MULTI_SHARED_STATE_HPP
#define BOOST_BEAST_EXAMPLE_WEBSOCKET_CHAT_MULTI_SHARED_STATE_HPP

#include <boost/smart_ptr.hpp>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_set>
#include "websocket_session.hpp"
#include "player.hpp"
#include "battery.hpp"
#include "message.hpp"

// Forward declaration
class websocket_session;

// Represents the shared server state
class shared_state
{
  // This mutex synchronizes all access to sessions_
  std::mutex mutex_;

  // Keep a list of all the connected clients
  std::unordered_set<websocket_session *> sessions_;

  std::map<int, battery> batteries;
  std::map<websocket_session *, player> connected_players;

  Json::Value get_game_objs_msg();

  void generate_batteries(websocket_session *session);
  int get_time_miliseconds();

public:
  void join(websocket_session *session);
  void leave(websocket_session *session);
  void send(std::string message);
  void broadcast_state();
  void broadcast_player(websocket_session *session,  msg_type type);
  void broadcast_charge(websocket_session *session, int battery_id);
  player get_new_player();
  void send_game_set_msg(websocket_session *session);
  void send_player_init(websocket_session *session);
  void process(websocket_session *session, std::string message);
};

#endif
