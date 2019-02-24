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
#include "websocket_session.hpp"

void shared_state::
    join(websocket_session *session)
{
    std::lock_guard<std::mutex> lock(mutex_);
    sessions_.insert(session);

    int n_of_players = this->connected_players.size();

    player new_player = player(300 + 60 * n_of_players, 300 + 60 * n_of_players, "#9C27B0", LIVE,
                               this->connected_players.size());

    Json::Value msg;
    msg["type"] = 0;
    msg["payload"] = new_player.to_json();

    Json::FastWriter fastWriter;
    std::string msg_data = fastWriter.write(msg);

    auto const msg_0 = boost::make_shared<std::string const>(
        std::move(msg_data));

    std::cout << "Connection received\n";

    session->send(msg_0);

    this->connected_players.push_back(new_player);
    this->generate_batteries();
}

void shared_state::
    leave(websocket_session *session)
{
    std::cout << "Disconnect\n";
    std::lock_guard<std::mutex> lock(mutex_);
    sessions_.erase(session);
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
    broadcast_state()
{
    Json::Value msg;
    Json::FastWriter fastWriter;

    msg["type"] = 1;
    msg["payload"] = this->get_game_objs_msg();

    this->send(fastWriter.write(msg));
}

void shared_state::
    send_last_player()
{
}

void shared_state::
    process(std::string message)
{
    Json::Reader reader;
    Json::Value message_json;

    bool success = reader.parse(message.c_str(), message_json);

    if (!success)
        std::cerr << "Parse failed";

    int type = message_json["type"].asInt();

    switch (type)
    {
    case 0:
    {
        int player_id = (message_json["payload"]["id"]).asInt();
        Json::Value player_data = message_json["payload"];
        this->connected_players[player_id].update(player_data);
        this->broadcast_state();
        break;
    }

    default:
        break;
    }

    std::cout << message_json["payload"] << "\n";
}

Json::Value shared_state::get_game_objs_msg()
{
    Json::Value players_json;
    for (size_t i = 0; i < this->connected_players.size(); i++)
    {
        players_json[static_cast<int>(i)] = this->connected_players[i].to_json();
    }

    Json::Value batteries_json;
    for (size_t i = 0; i < this->batteries.size(); i++)
    {
        batteries_json[static_cast<int>(i)] = this->batteries[i].to_json();
    }

    Json::Value root;
    root["players"] = players_json;
    root["batteries"] = batteries_json;

    return root;
}

void shared_state::generate_batteries()
{
    int N_OF_BATTERIES = 10;
    double X_RANGE = 800;
    double Y_RANGE = 800;

    srand(time(NULL));

    double last_player_pos[] = {
        this->connected_players[this->connected_players.size() - 1].x,
        this->connected_players[this->connected_players.size() - 1].y};

    double x_limits[] = {last_player_pos[0] - X_RANGE, last_player_pos[0] + X_RANGE};
    double y_limits[] = {last_player_pos[1] - Y_RANGE, last_player_pos[1] + Y_RANGE};

    for (int i = 0; i < N_OF_BATTERIES; i++)
    {
        double x_diff = 0, y_diff = 0;
        double batteries_pos[2];
        do
        {
            batteries_pos[0] = rand() % static_cast<int>(x_limits[1]) + x_limits[0];
            batteries_pos[1] = rand() % static_cast<int>(y_limits[1]) + y_limits[0];

            x_diff = batteries_pos[0] - last_player_pos[0];
            y_diff = batteries_pos[1] - last_player_pos[1];
        } while (x_diff < 20 || y_diff < 20);

        int id = this->batteries.size();

        this->batteries.push_back(battery(batteries_pos[0], batteries_pos[1], id));
    }
}
