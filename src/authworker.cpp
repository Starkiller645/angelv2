#include "authworker.h"
#include "filejson.h"
#include <QtWebSockets/QWebSocketServer>
#include <QtWebSockets/QWebSocket>
#include <iomanip>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QStringLiteral>
#include <QUrl>
#include <QRunnable>
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cpr/cpr.h>
#include <cstdlib>
#include <sstream>

using namespace Qt;
using namespace authworker;

extern QTcpServer sock_server;
int sockfd, newsockfd, portno;
socklen_t clilen;
char buffer[256];
struct sockaddr_in serv_addr, cli_addr;
int n;
std::string httpsend;


AuthorisationWorker::AuthorisationWorker(std::string sub, authworker::switch_type type, std::string before) {
  this->sub = sub;
  this->type = type;
  this->before = before;
  std::cout << "before: " << before << std::endl;
  return;
}

void AuthorisationWorker::run() {
  std::ostringstream httpsend_stream;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  bzero((char *) &serv_addr, sizeof(serv_addr));
  portno = 8800;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);
  if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    std::cout << "ERROR while binding socket\n";
  };
  listen(sockfd,5);
  clilen = sizeof(cli_addr);
  newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
  if(newsockfd < 0) {
    std::cout << "ERROR on connection accept\n";
  };
  bzero(buffer, 256);
  n = read(newsockfd, buffer, 255);
  if(n < 0) {
    std::cout << "ERROR while read from socket\n";
  };
  if(n < 0) {
    std::cout << "ERROR while write to socket\n";
  }
  close(newsockfd);
  close(sockfd);
  emit AuthorisationWorker::onResponseReceived(QString(buffer));
};

void AuthorisationWorker::checkCredentials() {
  std::string filename = std::string(std::getenv("HOME")) + std::string("/.config/angel.json");
  this->json = new filejson::JsonRead(filename);
  nlohmann::json jsondata = json->runSynced();
  cpr::Url frontpage_url{"https://oauth.reddit.com"};
  std::string authparam = jsondata["access_token"];
  authparam = "bearer " + authparam;
  cpr::Header headers;
  if(this->before != "") {
    headers = {{"Authorization", authparam.c_str()}, {"User-Agent", "angel/v1.0 (by /u/Starkiller645)"}};
    cpr::Parameters params{{"before", this->before.c_str()}};
  } else {
    headers = {{"Authorization", authparam.c_str()}, {"User-Agent", "angel/v1.0 (by /u/Starkiller645)"}};
    cpr::Parameters params{{"limit", "100"}};
  };
  cpr::Response response = cpr::Get(frontpage_url, headers);
  bool exceptionCaught = true;
  try {
    nlohmann::json json_about = nlohmann::json::parse(response.text);
    exceptionCaught = false;
  } catch(nlohmann::detail::parse_error) {
    cpr::Header headers = cpr::Header{{"User-Agent", "angel/v1.0 (by /u/Starkiller645)"}, {"Authorization", "Basic SnEwQml1VWVJcnNyM0E6"}};
    cpr::Payload refresh_payload{{"grant_type", "refresh_token"}, {"refresh_token", std::string(jsondata["refresh_token"]).c_str()}};
    cpr::Response refresh_response = cpr::Post(cpr::Url{"https://www.reddit.com/api/v1/access_token"}, headers, refresh_payload);
    if(refresh_response.status_code >= 400) {
      emit AuthorisationWorker::credCheckFailed();
    }
    nlohmann::json refresh_json;
    try {
      refresh_json = nlohmann::json::parse(refresh_response.text);
    } catch(nlohmann::detail::parse_error) {
      emit AuthorisationWorker::credCheckFailed();
      return;
    }
    if(refresh_response.status_code == 200) {
      std::string raw_json_data = "{\"access_token\":\"" + std::string(refresh_json["access_token"]) + "\",\"client_id\":\"Jq0BiuUeIrsr3A\",\"refresh_token\":\"" + std::string(jsondata["refresh_token"]) + "\",\"user_agent\":\"angel/v1.0 (by /u/Starkiller645)\"}";
      filejson::JsonWrite jsonwrite(nlohmann::json::parse(raw_json_data), filename);
      jsonwrite.run();
      emit AuthorisationWorker::credCheckSucceeded();
      return;
    } else {
      emit AuthorisationWorker::credCheckFailed();
      return;
    }
  } if(!exceptionCaught) {
    emit AuthorisationWorker::credCheckSucceeded();
    return;
  }
}

void AuthorisationWorker::switchSub () {
  std::string filename = std::string(std::getenv("HOME")) + std::string("/.config/angel.json");
  cpr::Url frontpage_url{"https://oauth.reddit.com"};
  cpr::Url url{"https://oauth.reddit.com/r/" + this->sub + "/hot.json"};
  cpr::Url about_url{"https://oauth.reddit.com/r/" + this->sub + "/about.json"};
  filejson::JsonRead *json = new filejson::JsonRead(filename);
  nlohmann::json jsondata = json->runSynced();
  std::string authparam = jsondata["access_token"];
  authparam = "bearer " + authparam;
  cpr::Header headers;
  cpr::Parameters params;
  cpr::Response response;
  if(this->before != "") {
    headers = {{"Authorization", authparam.c_str()}, {"User-Agent", "angel/v1.0 (by /u/Starkiller645)"}};
    params = {{"sort", "hot"}, {"show", "all"}, {"t", "all"}, {"after", this->before.c_str()}};
    response = cpr::Get(url, headers, params);
  } else {
    headers = {{"Authorization", authparam.c_str()}, {"User-Agent", "angel/v1.0 (by /u/Starkiller645)"}};
    params = {{"sort", "hot"}, {"show", "all"}, {"t", "all"}, {"after", this->before.c_str()}};
    response = cpr::Get(url, headers, params);
  };

  cpr::Response about_sub = cpr::Get(about_url, headers, params);
  cpr::Response frontpage_response = cpr::Get(frontpage_url, headers);
  nlohmann::json json_about = nlohmann::json::parse(about_sub.text);
  nlohmann::json json_response = nlohmann::json::parse(response.text);
  try {
    before = json_response["data"]["after"];
    std::cout << json_response["data"]["after"] << std::endl;
  } catch(nlohmann::detail::type_error) {
    try {
      std::cout << json_response["data"]["before"] << std::endl;
      before = "none";
    } catch(nlohmann::detail::type_error) {
      before = "none";
    }

  };

  nlohmann::json json_frontpage = nlohmann::json::parse(frontpage_response.text);
  emit AuthorisationWorker::onSwitchSubComplete(sub, json_response, json_about, json_frontpage, before);
}

void AuthorisationWorker::downloadImageFile(std::string url) {
  std::string fileExtension = url.substr(url.rfind("."));
  std::string filename = "/opt/angel-reddit/temp/.img." + fileExtension;
  CURL *curl;
  FILE *fp;
  CURLcode res;
  char buffer[CURL_ERROR_SIZE];
  char outfilename[FILENAME_MAX];
  strcpy(outfilename, filename.c_str());
  curl = curl_easy_init();
  if (curl) {
      fp = fopen(outfilename,"wb");
      curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
      curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, buffer);
      res = curl_easy_perform(curl);
      /* always cleanup */
      curl_easy_cleanup(curl);
      fclose(fp);
  }
  emit AuthorisationWorker::onDownloadImageFile(filename);
  return;
}

void AuthorisationWorker::ready_Read() {
  return;
}
