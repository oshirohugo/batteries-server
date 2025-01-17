#include <iostream>
#include "message.hpp"

message::message(std::string msg)
{

  Json::Reader reader;
  Json::Value message_json;

  bool success = reader.parse(msg.c_str(), message_json);

  if (!success)
    std::cerr << "Parse failed";

  type = static_cast<msg_type>(message_json["type"].asInt());
  payload = message_json["payload"];
}

message::message(msg_type type_, Json::Value payload_) : type(type_), payload(payload_) {}

message::message(const message &other) : type(other.type), payload(other.payload) {}

Json::Value message::to_json()
{
  Json::Value root;

  root["type"] = static_cast<int>(type);
  root["payload"] = payload;

  return root;
}

std::string message::to_string()
{
  Json::FastWriter fastWriter;
  return (fastWriter.write(to_json()));
}