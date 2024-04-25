#include <iostream>
#include <fstream>

#include "tokengatherer.hpp"
#include "contractcontent.hpp"
#include "symboltocoinid.hpp"

int main() {
    std::string api_key = std::getenv("ALCHEMY_API_KEY");
    if (api_key.empty()) {
        std::cerr << "Error: Alchemy API key not found in environment" << std::endl;
        return -1;
    }
    TokenGatherer gatherer;
    ContractContent content(api_key);
    SymbolToCoinId symbolToCoinId;

    try {
        auto token = gatherer.getTokenFromUser();
        std::cout << "Looking for: " << token << std::endl;
        auto coinId = symbolToCoinId.queryCoinGeckoAPI(token);
        if (coinId.empty()) {
            throw std::runtime_error("Coin ID not found");
        }
        std::cout << "Coin ID: " << coinId << std::endl;
        auto ethereum_contract = symbolToCoinId.getEthPlatformForCoin(coinId);
        std::cout << "Ethereum Contract: " << ethereum_contract << std::endl;
        if (ethereum_contract.empty()) {
            throw std::runtime_error("Ethereum contract not found");
        }
        auto bytecode = content.getContractContent(ethereum_contract);
        // save it to codefile.txt
        {
            std::ofstream codefile("codefile.txt");
            codefile << bytecode;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
