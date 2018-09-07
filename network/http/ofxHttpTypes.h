/*
    ofxHttpUtils v0.3
    Chris O'Shea, Arturo, Jesus, CJ

    Modified: 16th March 2009
    openFrameworks 0.06

*/
#pragma once
#include "ofUtils.h"

enum class HTTPRequestMethod
{
	GET,
	POST,
	PUT,
	_DELETE
};

struct ofxHttpForm
{
	HTTPRequestMethod method;
    std::string action;
	std::string name;

    ofxHttpForm():method(HTTPRequestMethod::GET), expectBinaryResponse(false)
	{
    	
    }

    ~ofxHttpForm()
	{
        clearFormFields();
    }

	void addHeaderField(const std::string& id, const std::string& value)
	{
		headerIds.push_back(id);
		headerValues.push_back(value);
	}

	// ----------------------------------------------------------------------
	void addFormField(const std::string& id, const std::string& value)
	{
        formIds.push_back(id);
        formValues.push_back(value);
	}
	// ----------------------------------------------------------------------
	void clearFormFields()
	{
	    formIds.clear();
        formValues.clear();
        formFiles.clear();
	}
	// ----------------------------------------------------------------------
    void addFile(const std::string& fieldName, const std::string& path)
	{
		formFiles[fieldName] = ofToDataPath(path);
	}

	std::string getFieldValue(std::string id)
	{
		for(unsigned int i=0; i < formIds.size(); i++)
		{
			if (formIds[i] == id)
			{
				return formValues[i];
			}
		}
		return "";
	}

	std::vector <std::string> formIds;
	std::vector <std::string> formValues;
	std::vector <std::string> headerIds;
	std::vector <std::string> headerValues;
	std::map<std::string,std::string> formFiles;
	bool expectBinaryResponse;
};
