FROM ubuntu:20.04 as build

USER root

EXPOSE 4000

RUN apt-get -y update && apt-get install -y g++ cmake

COPY . /server

WORKDIR /server/build

RUN cmake .. && make

CMD [ "./cppserver" ]
