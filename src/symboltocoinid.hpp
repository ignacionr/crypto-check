#pragma once
#include <string>
#include <iostream>
#include <string>
#include <iostream>
#include <curl/curl.h>
#include <rapidjson/document.h>
#include <regex>

class SymbolToCoinId {
public:
    SymbolToCoinId() {
        curl_global_init(CURL_GLOBAL_DEFAULT);
    }

    ~SymbolToCoinId() {
        curl_global_cleanup();
    }

    std::string queryCoinGeckoAPI(const std::string& symbol) {
        std::string url = "https://api.coingecko.com/api/v3/search?query=" + symbol;

        CURL* curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

            CURLcode res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                std::cerr << "Failed to query CoinGecko API: " << curl_easy_strerror(res) << std::endl;
            }

            curl_easy_cleanup(curl);
        }

        return parseResponse(response, symbol);
    }

    std::string parseResponse(const std::string& response, const std::string &target_symbol) {
        rapidjson::Document document;
        document.Parse(response.c_str());

        if (document.HasParseError()) {
            std::cerr << "Failed to parse response: " << document.GetParseError() << std::endl;
            return "";
        }

        // find the array of "coins"
        if (document.HasMember("coins") && document["coins"].IsArray()) {
            const rapidjson::Value& coins = document["coins"];
            for (rapidjson::SizeType i = 0; i < coins.Size(); i++) {
                const rapidjson::Value& coin = coins[i];
                if (coin.HasMember("symbol") && coin["symbol"].IsString()) {
                    std::string symbol = coin["symbol"].GetString();
                    if (symbol == target_symbol) {
                        if (coin.HasMember("id") && coin["id"].IsString()) {
                            return coin["id"].GetString();
                        }
                    }
                }
            }
        }

        return "";
    }

    std::string queryCoinGeckoCoinAPI(const std::string& coinId) {
        std::string url = "https://api.coingecko.com/api/v3/coins/" + coinId;
        CURL* curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
            CURLcode res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                std::cerr << "Failed to query CoinGecko API: " << curl_easy_strerror(res) << std::endl;
            }

            curl_easy_cleanup(curl);
        }

        return response;
    }

    std::string getEthPlatformForCoin(const std::string& coinId) {
        std::string response = queryCoinGeckoCoinAPI(coinId);
        // use a regex to find the ethereum platform
        std::regex regex("\"platforms\":\\s*\\{\\s*\"ethereum\":\\s*\"([^\"]+)\"");
        std::smatch match;
        if (std::regex_search(response, match, regex)) {
            return match[1].str();
        }

        return "";
    }

private:
    std::string response;

    static size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* response) {
        size_t totalSize = size * nmemb;
        response->append(static_cast<char*>(contents), totalSize);
        return totalSize;
    }
};