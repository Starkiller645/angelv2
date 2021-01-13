#include "redditasyncrequest.h"

#include <QObject>
#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

using namespace redasync;

redasync::AsyncRequest::AsyncRequest(redasync::request_type requestType, cpr::Url url_endpoint, cpr::Header headers) {
  this->requestType = requestType;
  this->endpointUri = url_endpoint;
  this->requestHeaders = headers;
  /*switch(requestType) {
    case redasync::request_type::PostRequest:
      this->postPayload = body;
      break;
    case redasync::request_type::GetRequest:
      this->getParams = body;
      break;
    case redasync::request_type::PatchRequest:
      this->patchJson = body;
      break;
  };*/
};

void AsyncRequest::run() {
  switch(this->requestType) {
    case redasync::request_type::PostRequest:
      this->postRequest = cpr::Post(this->endpointUri, this->requestHeaders);
      this->jsonResponse = nlohmann::json::parse(this->postRequest.text);
      emit request_received_json(this->jsonResponse);
    case redasync::request_type::GetRequest:
      this->getRequest = cpr::Get(this->endpointUri, this->requestHeaders, this->getParams);
      this->jsonResponse = nlohmann::json::parse(this->getRequest.text);
      emit request_received_json(this->jsonResponse);
    case redasync::request_type::PatchRequest:
      std::cout << "Patch requests not yet implemented!" << std::endl;
      this->jsonResponse = nlohmann::json::parse("{\"response\" : \"ERR NOT IMPLTD\"}");
  }
}

cpr::Response AsyncRequest::runSynced() {
  switch(this->requestType) {
    case redasync::request_type::PostRequest:
      this->postRequest = cpr::Post(this->endpointUri, this->requestHeaders);
      //this->jsonResponse = nlohmann::json::parse(this->postRequest.text);
      return this->postRequest;
    case redasync::request_type::GetRequest:
      this->getRequest = cpr::Get(this->endpointUri, this->requestHeaders, this->getParams);
      //this->jsonResponse = nlohmann::json::parse(this->getRequest.text);
      return this->getRequest;
    case redasync::request_type::PatchRequest:
      std::cout << "Patch requests not yet implemented!" << std::endl;
      this->jsonResponse = nlohmann::json::parse("{\"response\" : \"ERR NOT IMPLTD\"}");
  }
}
