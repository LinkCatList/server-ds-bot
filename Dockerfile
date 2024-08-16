FROM ubuntu:latest

WORKDIR /app

RUN apt update && apt install -y \
    g++ \
    libssl-dev \
    cmake \
    libz-dev \
    git \
    libjsoncpp-dev \
    doxygen \
    graphviz \
    uuid-dev \
    postgresql-all \
    postgresql-contrib \
    postgresql-client

COPY . .

RUN chmod +x build.sh

RUN ./build.sh
CMD ["./build/krash-bot"]
