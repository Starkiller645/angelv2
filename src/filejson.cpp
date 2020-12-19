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
  filestream >> this->jobj;
  filestream.close();
}
