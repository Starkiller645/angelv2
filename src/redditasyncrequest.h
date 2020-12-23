#ifndef REDDIT_ASYNC_REQUEST_H
#define REDDIT_ASYNC_REQUEST_H

#include <QRunnable>
#include <QObject>
#include <string>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>

namespace redasync {
  enum request_type {PostRequest, GetRequest, PatchRequest};
  class AsyncRequest : public QRunnable {
    Q_OBJECT
  public:
    AsyncRequest(redasync::request_type, cpr::Url, cpr::Header, cpr::Parameters);
    void run() override;
  signals:
    void request_received(std::string);
    void request_received_json(nlohmann::json);
  private:
    nlohmann::json jsonResponse;
    redasync::request_type requestType;
    std::string endpointUri;
    cpr::Header requestHeaders;
    cpr::Payload postPayload;
    cpr::Parameters getParams;
    cpr::Payload patchJson;
    cpr::Authentication
    cpr::Get getRequest;
    cpr::Post postRequest;
    cpr::Patch patchRequest;
  }
}

#endif
