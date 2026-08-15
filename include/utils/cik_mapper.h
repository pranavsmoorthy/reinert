#pragma once

#include <unordered_map>
#include <string>

/**
 * @class CIKMapper
 * @brief A utility class responsible for mapping stock ticker symbols to their Central Index Key (CIK).
 * 
 * The CIKMapper loads and stores a dictionary of ticker-to-CIK relationships. 
 * This is essential for querying SEC EDGAR databases, which require a properly 
 * padded CIK rather than standard stock ticker symbols to retrieve corporate filings.
 */
class CIKMapper {
private:
    /**
     * @brief Internal hash map storing the relationship between stock tickers and their raw CIKs.
     */
    std::unordered_map<std::string, std::string> ticker_to_cik_;

    /**
     * @brief Pads a raw CIK string with leading zeros.
     * 
     * SEC endpoints strictly require CIKs to be exactly 10 digits long. This helper 
     * function ensures any parsed CIK string is formatted to meet that specification.
     * 
     * @param raw_cik The unpadded CIK string retrieved from the map.
     * @return std::string The properly zero-padded 10-digit CIK.
     */
    std::string PadCIK(const std::string& raw_cik) const;

public:
    /**
     * @brief Loads the ticker-to-CIK mapping data from a file on disk.
     * 
     * Reads the provided file (typically a JSON configuration) and populates 
     * the internal unordered map with the ticker and CIK pairs.
     * 
     * @param filepath The absolute or relative path to the mapping file.
     * @return true If the file was successfully opened and parsed.
     * @return false If the file could not be read or the parsing failed.
     */
    bool Load(const std::string& filepath);

    /**
     * @brief Retrieves the padded CIK associated with a specific stock ticker.
     * 
     * Looks up the requested ticker symbol in the internal map and returns its 
     * padded CIK. If the ticker is not found, it typically returns an empty string 
     * or handles the missing key appropriately.
     * 
     * @param ticker The stock ticker symbol to look up (e.g., "AAPL").
     * @return std::string The padded 10-digit CIK, or an empty string if not found.
     */
    std::string GetCIK(const std::string& ticker) const;
};