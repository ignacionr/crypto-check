#include <iostream>

#include "tokengatherer.hpp"
#include "tokencontractresolver.hpp"
#include "contractcontent.hpp"

int main() {
    std::string api_key = std::getenv("ALCHEMY_API_KEY");
    if (api_key.empty()) {
        std::cerr << "Error: Alchemy API key not found in environment" << std::endl;
        return -1;
    }
    TokenGatherer gatherer;
    TokenContractResolver resolver(api_key);
    ContractContent content(api_key);

    try {
        auto token = gatherer.getTokenFromUser();
        auto contractAddress = resolver.resolveTokenContract(token);
        auto bytecode = content.getContractContent(contractAddress);

        std::cout << "Contract Bytecode: " << bytecode << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
