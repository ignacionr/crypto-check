#pragma once

#include <cstring> // For size_t and std::memcpy


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
        std::string url = "https://eth-mainnet.alchemyapi.io/v2/" + api_key + "/getBytecode?address=" + contractAddress;

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
            d.Parse(readBuffer.c_str());
            if (d.HasMember("bytecode") && d["bytecode"].IsString()) {
                return d["bytecode"].GetString();
            } else {
                return std::string("Error: Bytecode not found");
            }
        } else {
            throw std::runtime_error("Failed to initialize curl");
        }
    }
};
