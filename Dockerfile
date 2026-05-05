FROM ubuntu:latest

RUN apt-get update && apt-get install -y \
    build-essential \
    libomp-dev \
    vim \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

CMD ["/bin/bash"]

