#include "authworker.h"
#include <QtWebSockets/QWebSocketServer>
#include <QtWebSockets/QWebSocket>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QStringLiteral>
#include <QUrl>
#include <QRunnable>
#include <QtCore/QDebug>
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
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


AuthorisationWorker::AuthorisationWorker() {
  return;
}

void AuthorisationWorker::run() {
  std::ostringstream httpsend_stream;

  /*if(QObject::connect(&sock_server, &QTcpServer::newConnection, this, &AuthorisationWorker::capture_signal)) {
    std::cout << "Connect to capture_signal succeeded\n";
  };
  if(sock_server.listen(QHostAddress::Any, 8080)) {
    std::cout << "Reddit Authorisation Worker listening on port 8080" << std::endl;*/
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

void AuthorisationWorker::capture_signal() {
  /*QObject::connect(connection, &QTcpSocket::textMessageReceived, this, &AuthorisationWorker::onEchoReceived);*/
}

void AuthorisationWorker::onEchoReceived(QString message) {
  std::cout << "Text message incoming!" << std::endl;
  qDebug() << "Message received:" << message;
}

void AuthorisationWorker::ready_Read() {
  return;
}
