#pragma once

#ifndef INCLUDE_RETRIEVER_H_
#define INCLUDE_RETRIEVER_H_

#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include <iostream>
#include <string>

struct Retriever {
    static std::size_t WriteCallback(void* contents, std::size_t size, std::size_t nmemb, void* userp) {
        std::size_t totalSize = size * nmemb;
        static_cast<std::string*>(userp)->append(static_cast<char*>(contents), totalSize);
        return totalSize;
    }

    static nlohmann::json FetchYFinanceJson(const std::string& ticker) {
        std::string url = "https://query1.finance.yahoo.com/v8/finance/chart/" + ticker + "?interval=1d&range=6mo";

        CURL* curl = curl_easy_init();
        std::string read_buffer;

        if (curl) {
            curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36");

            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);
            
            CURLcode res = curl_easy_perform(curl);
            
            if (res != CURLE_OK) {
                std::cerr << "libcurl Error: " << curl_easy_strerror(res) << std::endl;
            }

            curl_easy_cleanup(curl);
        }

        return nlohmann::json::parse(read_buffer);
    }

    static nlohmann::json FetchSECEdgarJson(const std::string& cik) {
        std::string url = "https://data.sec.gov/api/xbrl/companyfacts/CIK" + cik + ".json";

        CURL* curl = curl_easy_init();
        std::string read_buffer;
        
        if (curl) {
            struct curl_slist* headers = NULL;

            headers = curl_slist_append(headers, "User-Agent: VectorForge/1.0 pranavsmoorthy@gmail.com");
            headers = curl_slist_append(headers, "Accept: application/json");
            
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            
            curl_easy_setopt(curl, CURLOPT_ENCODING, ""); 
            
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);

            CURLcode res = curl_easy_perform(curl);
            
            if (res != CURLE_OK) {
                std::cerr << "libcurl Error: " << curl_easy_strerror(res) << std::endl;
            }

            curl_slist_free_all(headers);
            curl_easy_cleanup(curl);
        }

        if (read_buffer.empty()) {
            throw std::runtime_error("SEC EDGAR request failed or returned empty data.");
        }

        return nlohmann::json::parse(read_buffer);
    }

    
};

#endif