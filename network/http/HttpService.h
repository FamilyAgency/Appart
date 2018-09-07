#pragma once

#include "ofMain.h"
#include "ofxHttpTypes.h"
#include "ofxHttpUtils.h"

namespace bbrother
{
	class HttpService
	{
	public:
		HttpService();
		virtual ~HttpService();
		
		struct RequestParam
		{
			RequestParam(const string& key, const string& value)
			{
				this->key = key;
				this->value = value;
			}
			string key;
			string value;
		};

		typedef RequestParam HeaderParam;

		void update();

		void makeRequest(const string& Url,
			HTTPRequestMethod requestMethod,
			const vector<RequestParam>& requestParams,
			const vector<HeaderParam>& headerParams);

		void makeRequest(const string& Url,
			HTTPRequestMethod requestMethod,
			const vector<RequestParam>& requestParams,
			const vector<HeaderParam>& headerParams,
			const RequestParam& fileParam);

		ofEvent<const string&> ServerResponseEvent;


	private:
		ofxHttpUtils httpUtils;
		void onServerResponse(ofxHttpResponse& response);
		vector<string> responses;
	};
}