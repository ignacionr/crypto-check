#pragma once
#include <curl/curl.h>
#include <string>
#include <stdexcept>
#include "rapidjson/document.h"

class TokenContractResolver {
    std::string api_key;

    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *s) {
        size_t newLength = size * nmemb;
        try {
            s->append((char*)contents, newLength);
        } catch (std::bad_alloc &e) {
            throw;
        }
        return newLength;
    }

public:
    TokenContractResolver(const std::string& key) : api_key(key) {}

    std::string resolveTokenContract(const std::string& token) {
        CURL *curl;
        CURLcode res;
        std::string readBuffer;
        std::string url = "https://eth-mainnet.alchemyapi.io/v2/" + api_key + "/getContract?token=" + token;

        curl = curl_easy_init();
        if(curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);

            if(res != CURLE_OK)
                throw std::runtime_error(curl_easy_strerror(res));

            rapidjson::Document d;

            std::cerr << __FILE__ << ":" << __LINE__ << "-->" << readBuffer << "<--" << std::endl;

            d.Parse(readBuffer.c_str());
            if (d.HasMember("contractAddress") && d["contractAddress"].IsString()) {
                return d["contractAddress"].GetString();
            } else {
                return std::string("Error: Contract address not found");
            }
        } else {
            throw std::runtime_error("Failed to initialize curl");
        }
    }
};
