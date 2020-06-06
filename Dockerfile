FROM gcc:10
RUN apt-get update && apt-get install telnet -y
ARG cachebust=1
RUN git clone https://github.com/jasiekbialas/SIK2
RUN cd SIK2 && make
