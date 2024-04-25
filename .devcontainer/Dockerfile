FROM mcr.microsoft.com/devcontainers/base:jammy

# Avoid warnings by switching to noninteractive
ENV DEBIAN_FRONTEND=noninteractive

# Configure apt and install packages
RUN apt-get update

RUN apt-get -y install \
    build-essential \
    cmake \
    git \
    jq \
    libssl-dev \
    clang-15

RUN apt-get -y install libcurl4-openssl-dev

# Copy the environment variable ALCHEMY_API_KEY from host
ARG ALCHEMY_API_KEY
ENV ALCHEMY_API_KEY=$ALCHEMY_API_KEY

# Append the ALCHEMY_API_KEY variable to the user profile
RUN echo "export ALCHEMY_API_KEY=$ALCHEMY_API_KEY" >> /root/.bashrc
RUN echo "export ALCHEMY_API_KEY=$ALCHEMY_API_KEY" >> /home/vscode/.bashrc
