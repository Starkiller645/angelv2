#include "filejson.h"

#include <fstream>
#include <string>
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

filejson::JsonWrite::JsonWrite(json jsonobj, std::string filename) {
  this->json_dump = jsonobj.dump(4);
  this->filename = filename;
}

void filejson::JsonWrite::run() {
  std::ofstream filestream;
  filestream.open(this->filename);
  filestream << this->json_dump << std::endl;
  filestream.close();
}

filejson::JsonRead::JsonRead(std::string filename) {
  this->filename = filename;
}

void filejson::JsonRead::run() {
  std::ifstream filestream;
  filestream.open(this->filename);
  try {
    filestream >> this->jobj;
  } catch (...) {
    std::string filename = std::string(std::getenv("OS") + std::string("/.config/angel.json"));
    std::ofstream fstream;
    fstream.open(filename);
    fstream << "{\"access_token\":\"331010684722-OCHcddqsN4BeDs7VOSw-QVsnyncThg\",\"client_id\":\"Jq0BiuUeIrsr3A\",\"refresh_token\":\"331010684722-Q1bR0hRkpdxw3pNqpVHpzBMhZJaOPA\",\"user_agent\":\"angel/v1.0 (by /u/Starkiller645)\"}" << std::endl;
    fstream.close();
    filestream >> this->jobj;
  }
  filestream.close();
  emit this->onJsonRead(this->jobj);
}

nlohmann::json filejson::JsonRead::runSynced() {
  std::ifstream filestream;
  filestream.open(this->filename);
  try {
    std::cout << "[DBG] Trying to get JSON from ~/.config/angel.json..." << std::endl;
    filestream >> this->jobj;
    std::cout << "[DBG] Done and caught no errors" << std::endl;
  } catch (...) {
    std::cout << "[DBG] Done and found an error. Writing blank config to ~/.config/angel.json..." << std::endl;
    std::string filename = std::string(std::getenv("HOME")) + std::string("/.config/angel.json");
    std::ofstream fstream;
    fstream.open(filename);
    fstream << "{\"access_token\":\"NONE\",\"client_id\":\"Jq0BiuUeIrsr3A\",\"refresh_token\":\"NONE\",\"user_agent\":\"angel/v1.0 (by /u/Starkiller645)\"}" << std::endl;
    fstream.close();
    filestream >> this->jobj;
  }
  filestream.close();
  return this->jobj;
}
