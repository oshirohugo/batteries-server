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
#include "player.hpp"
#include "battery.hpp"

// Forward declaration
class websocket_session;

// Represents the shared server state
class shared_state
{
    // This mutex synchronizes all access to sessions_
    std::mutex mutex_;

    // Keep a list of all the connected clients
    std::unordered_set<websocket_session *> sessions_;

    std::vector<player> connected_players;
    std::vector<battery> batteries;

    Json::Value get_game_objs_msg();

    void generate_batteries();

  public:
    void join(websocket_session *session);
    void leave(websocket_session *session);
    void send(std::string message);
    void broadcast_state();
    void broadcast_player(int id, bool is_join);
    void broadcast_last_player();
    player get_new_player();
    void send_game_set_msg(int player_id, websocket_session *session);
    void process(std::string message);
};

#endif
