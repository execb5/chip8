FROM ubuntu:latest

WORKDIR /root

RUN apt update
RUN apt install -y curl xz-utils cmake libncurses5-dev libncursesw5-dev
RUN curl https://cmocka.org/files/1.1/cmocka-1.1.5.tar.xz --output cmocka-1.1.5.tar.xz --silent
RUN xz -d cmocka-1.1.5.tar.xz && tar vxf cmocka-1.1.5.tar
RUN cd cmocka-1.1.5 && mkdir build && cd build && cmake -DCMAKE_BUILD_TYPE=Release .. && make && make install
