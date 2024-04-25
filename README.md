# CryptoChecker - A Tool for Verifying Token Fiability

[![Build Status](https://github.com/ignacionr/crypto-check/actions/workflows/cmake-single-platform.yml/badge.svg)](https://github.com/ignacionr/crypto-check/actions/workflows/cmake-single-platform.yml)

**CryptoChecker** aims to verify the reliability of blockchain tokens by employing a two-stage process involving data collection and analysis.

## Components

...
These processes are responsible for gathering data points about a given Token/Symbol:
- **Token Gatherer**: Saves the token that the user wants to investigate.
- **Token Contract Resolver**: Resolves a given token to its corresponding on-chain contract.
- **Contract Content**: Downloads the bytecode from an on-chain contract by making queries to a blockchain node, possibly using Alchemy.

### Sensory Analysts
These processes analyze the information collected by Farmhands and may augment the data with derived information or annotate it:
- **JEB Decompiler**: [Ethereum Smart Contract Decompiler by PNF Software](https://www.pnfsoftware.com/blog/ethereum-smart-contract-decompiler/)
- **Mythril - Security Analysis**: [GitHub - Consensys/mythril](https://github.com/Consensys/mythril)
- **Panoramix Decompiler**: [GitHub - palkeo/panoramix](https://github.com/palkeo/panoramix)
  
**Suggestions**:
- **Foundry toolkit**
- **Anvil (private fork)** - akin to integration testing
- **Forge**

### Development Status

Development has commenced and is publicly visible at [CryptoChecker GitHub](https://github.com/ignacionr/crypto-check). The initial implementation includes:
- Running Anvil for testing ERC20 tokens.

### Testing ERC20 Tokens
To test an ERC20 token, one might need to simulate transactions:
1. Purchase tokens via a DEX, such as using [1Inch](https://1inch.io).
2. Identify and use Wrapped Ether (WETH) for transactions where ETH itself cannot be used directly:
    ```shell
    curl "https://api.1inch.io/v4.0/1/quote?fromTokenAddress=0xC02aaa39b223fe8d0a0e5c4f27ead9083c756cc2&toTokenAddress=0xTokenAddressOfERC20&amount=AmountOfWETHInWei"
    ```
    Example for LINK:
    ```shell
    curl "https://api.1inch.io/v4.0/1/quote?fromTokenAddress=0xC02aaa39b223fe8d0a0e5c4f27ead9083c756cc2&toTokenAddress=0x514910771af9ca656af840dff83e8264ecf986ca&amount=AmountOfWETHInWei"
    ```
3. Solidity script can be created to interact with the smart contract using the determined liquidity pool.

### Goal

The ultimate goal of **CryptoChecker** is to establish a system where the fiability of tokens can be assessed with traceable and reliable data sources, ensuring high confidence in the results produced. This will also include persistent and safe storage of user requests to maintain data integrity and traceability.

### Contributing

We welcome contributions to **CryptoChecker**. Please feel free to fork the repository, make your changes, and submit a pull request.

## License

Distributed under the MIT License. See `LICENSE` for more information.
