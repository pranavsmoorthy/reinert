#pragma once

#ifndef INCLUDE_RETRIEVER_H_
#define INCLUDE_RETRIEVER_H_

#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include <iostream>
#include <string>

/**
 * @struct Retriever
 * @brief A utility structure for executing HTTP requests to external financial APIs.
 * 
 * This structure provides static methods to retrieve and parse JSON data from 
 * Yahoo Finance and the SEC EDGAR database using libcurl and nlohmann::json.
 */
struct Retriever {
    
    /**
     * @brief Callback function used by libcurl to write received HTTP response data into a string buffer.
     * 
     * @param contents Pointer to the delivered data chunk.
     * @param size Size of one data element.
     * @param nmemb Number of data elements in the chunk.
     * @param userp Pointer to the user-defined std::string buffer to append data to.
     * @return std::size_t The total size of the data written.
     */
    static std::size_t WriteCallback(void* contents, std::size_t size, std::size_t nmemb, void* userp) {
        std::size_t totalSize = size * nmemb;
        static_cast<std::string*>(userp)->append(static_cast<char*>(contents), totalSize);
        return totalSize;
    }

    /**
     * @brief Retrieves historical market data for a given ticker from Yahoo Finance.
     * 
     * Fetches a 6-month daily interval chart for the specified ticker and parses 
     * the HTTP response payload into a JSON object.
     * 
     * @param ticker The stock ticker symbol to query (e.g., "AAPL").
     * @return nlohmann::json The parsed JSON response containing the market data.
     */
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

    /**
     * @brief Retrieves company facts and financial filings from the SEC EDGAR API.
     * 
     * Queries the SEC EDGAR database using the company's padded Central Index Key (CIK) 
     * to retrieve fundamental financial metrics and historical filing data.
     * 
     * @param cik The 10-digit padded CIK of the target company.
     * @return nlohmann::json The parsed JSON response containing the SEC company facts.
     * @throws std::runtime_error If the HTTP request fails or returns an empty payload.
     */
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