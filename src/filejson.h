#ifndef FILE_JSON_H
#define FILE_JSON_H

#include <nlohmann/json.hpp>
#include <fstream>
#include <QObject>
#include <QRunnable>

namespace filejson {
  using namespace Qt;
  class JsonWrite : public QObject, public QRunnable {
    Q_OBJECT
    std::string json_dump;
    std::ofstream file_write;
    std::string filename;
  public:
    JsonWrite(nlohmann::json, std::string);
    void run() override;
  };

  class JsonRead : public QObject, public QRunnable {
    Q_OBJECT
  signals:
    nlohmann::json onJsonRead();
  public:
    JsonRead(std::string);
    void run() override;
  private:
    std::string json_dump;
    std::ifstream file_read;
    std::string filename;
    nlohmann::json jobj;
  };
}

#endif
