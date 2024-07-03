FROM ubuntu:latest

RUN apt-get update && apt-get install -y --no-install-recommends gcc g++ libpqxx-dev curl libcurl4-openssl-dev ca-certificates

WORKDIR /usr/include
RUN mkdir nlohmann
RUN curl -L https://github.com/nlohmann/json/releases/download/v3.11.3/json.hpp -o nlohmann/json.hpp

COPY . /usr/src/parsing-points
COPY apiKey.env /usr/src/parsing-points
COPY postgres_key.env /usr/src/parsing-points

WORKDIR /usr/src/parsing-points


RUN g++ main.cpp -o main ./data/*.cpp ./logic/*.cpp -I./logic -I./data -lcurl -lpqxx

CMD [ "./main" ]
