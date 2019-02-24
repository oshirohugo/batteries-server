#include "message.hpp"

message::message() : type(-1) {}

message::message(int type_, Json::Value payload_) : type(type_), payload(payload_) {}

message::message(const message &other) : type(other.type), payload(other.payload) {}

Json::Value message::to_json() {
  Json::Value root;
  
  root["type"] = this->type;
  root["payload"] = this->payload;

  return root;
}