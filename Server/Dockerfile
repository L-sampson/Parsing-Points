FROM ubuntu:latest

ARG SPORTS_ODDS_API_KEY

RUN apt-get update && apt-get install -y --no-install-recommends gcc g++ cmake curl libcurl4-openssl-dev ca-certificates make git libssl-dev libasio-dev

WORKDIR /usr/src/parsing-points

COPY . .


RUN mkdir build && cd build && cmake -G "Unix Makefiles" ..

RUN cd build && make

EXPOSE 8080

ENV SPORTS_ODDS_API_KEY=$SPORTS_ODDS_API_KEY

CMD [ "./build/API" ]
