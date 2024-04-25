#pragma once

#include <cstring>  // For size_t and std::memcpy
#include <string>
#include <stdexcept>
#include <curl/curl.h>
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

class ContractContent {
    std::string api_key;

    // The WriteCallback function
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
        size_t newLength = size * nmemb;
        size_t oldLength = s->size();
        try {
            s->resize(oldLength + newLength);  // Resize the string to accommodate the new data
            std::memcpy(&(s->operator[](oldLength)), contents, newLength); // Copy new data to the end of the string
        } catch (std::bad_alloc& e) {
            // Handle memory problems, if any
            return 0; // Returning 0 will signal to libcurl to stop the transfer
        }
        return newLength; // Return the number of bytes taken care of
    }

public:
    ContractContent(const std::string& key) : api_key(key) {}

    std::string getContractContent(const std::string& contractAddress) {
        CURL *curl;
        CURLcode res;
        std::string readBuffer;
        std::string url = "https://eth-mainnet.alchemyapi.io/v2/" + api_key;

        // Prepare the JSON data for the POST request
        rapidjson::Document doc;
        doc.SetObject();
        rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
        doc.AddMember("jsonrpc", rapidjson::Value("2.0", allocator), allocator);
        doc.AddMember("method", rapidjson::Value("eth_getCode", allocator), allocator);
        rapidjson::Value params(rapidjson::kArrayType);
        params.PushBack(rapidjson::Value(contractAddress.c_str(), allocator), allocator);
        params.PushBack(rapidjson::Value("latest", allocator), allocator);
        doc.AddMember("params", params, allocator);
        doc.AddMember("id", 1, allocator);

        rapidjson::StringBuffer buffer;
        rapidjson::Writer <rapidjson::StringBuffer> writer(buffer);
        doc.Accept(writer);

        // Convert the JSON document to a string
        std::string jsonStr = buffer.GetString();

        // Initialize CURL
        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();
        if(curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonStr.c_str());
            res = curl_easy_perform(curl);
            if(res != CURLE_OK) {
                std::cerr << "cURL error: " << curl_easy_strerror(res) << std::endl;
            }
            curl_easy_cleanup(curl);
        }
        curl_global_cleanup();

        // use rapidjson to parse the response
        rapidjson::Document d;
        d.Parse(readBuffer.data(), readBuffer.size());
        if (d.HasMember("result") && d["result"].IsString()) {
            return d["result"].GetString();
        } else {
            return std::string("Error: Contract bytecode not found");
        }
        return readBuffer;
    }
};