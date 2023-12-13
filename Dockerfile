FROM ubuntu:latest

RUN apt-get update && apt-get install -y \
    g++ \
    cmake \
    make

WORKDIR /usr/src/

COPY /cpp /usr/src/

RUN cmake . && make

CMD ["./osm_routing_test"]

