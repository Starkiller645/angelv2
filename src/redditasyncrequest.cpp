#include "redditasync.h"

#include <QObject>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

redasync::AsyncRequest::AsyncRequest(redasync::request_type requestType, cpr::Url url_endpoint, cpr::Header headers, cpr::Parameters body) {
  this->requestType = requestType;
  this->endpointUri = url_endpoint;
  this->requestHeaders = headers;
  switch requestType {
    case redasync::request_type::PostRequest:
      this->postPayload = body;
      break;
    case redasync::request_type::GetRequest:
      this->getParams = body;
      break;
    case redasync::request_type::PatchRequest:
      this->patchJson = body;
      break;
  };
};

void redasync::AsyncRequest::run() {
  switch this->requestType {
    case redasync::request_type::PostRequest:
      this->postRequest = cpr::Post(this->endpointUri, this->requestHeaders, this->postPayload);
      this->postRequest.
  }
}
