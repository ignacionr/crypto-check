#!/bin/bash
rm -rf my-erc20-simulation
mkdir -p my-erc20-simulation
cd my-erc20-simulation
forge init
anvil --fork-url https://eth-mainnet.g.alchemy.com/v2/${ALCHEMY_API_KEY} 