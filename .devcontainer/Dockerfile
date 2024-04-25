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

