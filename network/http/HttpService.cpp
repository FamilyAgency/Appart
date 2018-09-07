#include "HttpService.h"

using namespace bbrother;

HttpService::HttpService()
{
	ofAddListener(httpUtils.newResponseEvent, this, &HttpService::onServerResponse);	
	httpUtils.start();	
}

void HttpService::makeRequest(const string& Url, 
	HTTPRequestMethod requestMethod, 
	const vector<RequestParam>& requestParams,
	const vector<HeaderParam>& headerParams)
{
	ofxHttpForm form;
	form.action = Url;
	form.method = requestMethod;

	for (auto param : requestParams)
	{
		form.addFormField(param.key, param.value);
	}

	for (auto param : headerParams)
	{
		form.addHeaderField(param.key, param.value);
	}

	httpUtils.addForm(form);
}

void HttpService::makeRequest(const string& Url,
	HTTPRequestMethod requestMethod,
	const vector<RequestParam>& requestParams,	
	const vector<HeaderParam>& headerParams,
	const RequestParam& fileParam)
{
	ofxHttpForm form;
	form.action = Url;
	form.method = requestMethod;

	for (auto param : requestParams)
	{
		form.addFormField(param.key, param.value);
	}	
	form.addFile(fileParam.key, fileParam.value);
	
	httpUtils.addForm(form);
}

void HttpService::update()
{
	ofMutex mutex;
	mutex.lock();
	if (responses.size() > 0)
	{
		for (size_t i = 0; i < responses.size(); i++)
		{
			ofNotifyEvent(ServerResponseEvent, responses[i], this);
		}
		responses.clear();
	}
	mutex.unlock();
}

void HttpService::onServerResponse(ofxHttpResponse& response)
{
	responses.push_back(response.responseBody);
}

HttpService::~HttpService()
{
	ofRemoveListener(httpUtils.newResponseEvent, this, &HttpService::onServerResponse);

}