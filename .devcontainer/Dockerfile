FROM ubuntu:23.04

# Avoid warnings by switching to noninteractive
ENV DEBIAN_FRONTEND=noninteractive

# Configure apt and install packages
RUN apt-get update

RUN apt-get -y install \
    build-essential \
    cmake \
    git \
    jq \
    libssl-dev

RUN apt-get -y install clang-16

RUN apt-get -y install libcurl4-openssl-dev

# Copy the environment variable ALCHEMY_API_KEY from host
ARG ALCHEMY_API_KEY
ENV ALCHEMY_API_KEY=$ALCHEMY_API_KEY

# Append the ALCHEMY_API_KEY variable to the user profile
RUN echo "export ALCHEMY_API_KEY=$ALCHEMY_API_KEY" >> /root/.bashrc

# Install solc
RUN apt -y install software-properties-common
RUN add-apt-repository ppa:ethereum/ethereum
RUN apt -y install solc

# Install libssl-dev, python3-dev, and python3-pip
RUN apt -y install libssl-dev python3-dev python3-pip

# Install mythril
RUN pip3 install mythril --break-system-packages

RUN apt -y install curl
RUN curl -L https://foundry.paradigm.xyz | bash
