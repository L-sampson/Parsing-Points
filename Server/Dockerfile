FROM ubuntu:latest

ARG SPORTS_ODDS_API_KEY
ARG POSTGRES_DB

RUN apt-get update && apt-get install -y --no-install-recommends gcc g++ libpqxx-dev curl libcurl4-openssl-dev ca-certificates

WORKDIR /usr/include
RUN mkdir nlohmann
RUN curl -L https://github.com/nlohmann/json/releases/download/v3.11.3/json.hpp -o nlohmann/json.hpp

COPY . /usr/src/parsing-points

WORKDIR /usr/src/parsing-points

ENV SPORTS_ODDS_API_KEY=$SPORTS_ODDS_API_KEY
ENV POSTGRES_DB=$POSTGRES_DB


RUN g++ main.cpp -o main ./data/*.cpp ./logic/*.cpp -I./logic -I./data -lcurl -lpqxx

CMD [ "./main" ]
