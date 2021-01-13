#ifndef REDDIT_ASYNC_REQUEST_H
#define REDDIT_ASYNC_REQUEST_H

#include <QRunnable>
#include <QObject>
#include <string>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>

namespace redasync {
  enum request_type {PostRequest, GetRequest, PatchRequest};
  class AsyncRequest : public QObject, public QRunnable {
    Q_OBJECT
  public:
    AsyncRequest(redasync::request_type, cpr::Url, cpr::Header);
    void run() override;
    cpr::Response runSynced();
  signals:
    void request_received(std::string);
    void request_received_json(nlohmann::json);
  private:
    nlohmann::json jsonResponse;
    redasync::request_type requestType;
    cpr::Url endpointUri;
    cpr::Header requestHeaders;
    cpr::Parameters getParams;
    cpr::Parameters patchJson;
    cpr::Response getRequest;
    cpr::Response postRequest;
    cpr::Response patchRequest;
  };
}

#endif
