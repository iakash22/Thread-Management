# Use Ubuntu as base image
FROM ubuntu:latest

# Install required dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    gdb \
    vim \
    man \
    gcc \
    g++ \
    clang \
    cmake \
    make \
    libncurses5-dev \
    libc-dev \
    libffi-dev \
    pkg-config \
    && rm -rf /var/lib/apt/lists/*

# Set working directory inside the container
WORKDIR /workspace

# Default command to keep container running
CMD [ "sleep", "infinity" ]
