//
// Copyright (c) 2018 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/vinniefalco/CppCon2018
//

#include <time.h>
#include <jsoncpp/json/json.h>

#include "shared_state.hpp"
#include "message.hpp"

player shared_state::get_new_player()
{
    int n_of_players = connected_players.size();
    int id = get_time_miliseconds();
    std::string color = "#9C27B0";
    int position[] = {300 + 60 * n_of_players, 300 + 60 * n_of_players};

    return player(position[0], position[1], color, LIVE, id);
}

void shared_state::
    join(websocket_session *session)
{
    std::cout << "Connection received\n";

    std::lock_guard<std::mutex> lock(mutex_);
    sessions_.insert(session);

    connected_players[session] = get_new_player();

    generate_batteries(session);
    send_game_set_msg(session);
}

void shared_state::
    leave(websocket_session *session)
{
    std::cout << "Disconnect\n";
    std::lock_guard<std::mutex> lock(mutex_);
    sessions_.erase(session);
    connected_players.erase(session);
}

// Broadcast a message to all websocket client sessions
void shared_state::
    send(std::string message)
{
    // Put the message in a shared pointer so we can re-use it for each client
    auto const ss = boost::make_shared<std::string const>(std::move(message));

    // Make a local list of all the weak pointers representing
    // the sessions, so we can do the actual sending without
    // holding the mutex:
    std::vector<boost::weak_ptr<websocket_session>> v;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        v.reserve(sessions_.size());
        for (auto p : sessions_)
            v.emplace_back(p->weak_from_this());
    }

    // For each session in our local list, try to acquire a strong
    // pointer. If successful, then send the message on that session.
    for (auto const &wp : v)
        if (auto sp = wp.lock())
            sp->send(ss);
}

void shared_state::
    send_game_set_msg(websocket_session *session)
{
    Json::Value root = get_game_objs_msg();
    root["player"] = connected_players[session].to_json();

    message msg = message(msg_type::GAME_SET, root);

    auto const msg_0 = boost::make_shared<std::string const>(
        std::move(msg.to_string()));

    session->send(msg_0);
}

void shared_state::
    broadcast_state()
{
    message game_state_msg = message(msg_type::GAME_UPDATE, get_game_objs_msg());

    send(game_state_msg.to_string());
}

void shared_state::
    broadcast_player(websocket_session *session, bool is_join)
{

    msg_type type = is_join ? msg_type::PLAYER_JOIN : msg_type::PLAYER_UPDATE;

    message player_msg = message(type, connected_players[session].to_json());

    send(player_msg.to_string());
}

void shared_state::
    broadcast_charge(websocket_session *session, int battery_id)
{
    Json::Value json_message;
    json_message["player"] = connected_players[session].to_json();
    json_message["battery"]["id"] = battery_id;

    message charge_msg = message(msg_type::CHARGE, json_message);

    send(charge_msg.to_string());
}

void shared_state::
    process(websocket_session *session, std::string msg_string)
{
    message msg = message(msg_string.c_str());

    switch (msg.type)
    {
    case msg_type::PLAYER_UPDATE:
    {
        Json::Value player_data = msg.payload;
        connected_players[session].update(player_data);
        broadcast_player(session, false);
        break;
    }

    case msg_type::CHARGE:
    {
        Json::Value player_data = msg.payload["player"];
        connected_players[session].update(player_data);

        int battery_id = msg.payload["battery"]["id"].asInt();

        broadcast_charge(session, battery_id);
        break;
    }

    default:
        break;
    }

    std::cout << msg.payload << "\n";
}

Json::Value shared_state::get_game_objs_msg()
{
    Json::Value players_json;
    int player_index = 0;
    for (auto &player_entry : connected_players)
    {
        players_json[static_cast<int>(player_index++)] = player_entry.second.to_json();
    }

    Json::Value batteries_json;
    int battery_index = 0;
    for (auto &battery_entry : batteries)
    {
        batteries_json[static_cast<int>(battery_index++)] = battery_entry.second.to_json();
    }

    Json::Value root;
    root["players"] = players_json;
    root["batteries"] = batteries_json;

    return root;
}

void shared_state::generate_batteries(websocket_session *session)
{
    int N_OF_BATTERIES = 10;
    double X_RANGE = 1E3;
    double Y_RANGE = 1E3;

    srand(time(NULL));

    double last_player_pos[] = {
        connected_players[session].x,
        connected_players[session].y};

    double x_limits[] = {last_player_pos[0] - X_RANGE, last_player_pos[0] + X_RANGE};
    double y_limits[] = {last_player_pos[1] - Y_RANGE, last_player_pos[1] + Y_RANGE};

    for (int i = 0; i < N_OF_BATTERIES; i++)
    {
        double x_diff = 0, y_diff = 0;
        double batteries_pos[2];
        do
        {
            batteries_pos[0] = rand() % static_cast<int>((x_limits[1] + 1 - x_limits[0])) + x_limits[0];
            batteries_pos[1] = rand() % static_cast<int>((y_limits[1] + 1 - y_limits[0])) + y_limits[0];

            x_diff = abs(batteries_pos[0] - last_player_pos[0]);
            y_diff = abs(batteries_pos[1] - last_player_pos[1]);
        } while (x_diff < 20 || y_diff < 20);

        int id = get_time_miliseconds() + i;

        batteries[id] = battery(batteries_pos[0], batteries_pos[1], id);
    }
}

int shared_state::get_time_miliseconds()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::system_clock::now().time_since_epoch())
        .count();
}