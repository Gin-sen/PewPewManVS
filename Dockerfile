# How to run :
# sudo modprobe snd-aloop index=2
# docker build -t test_container .
# docker run -it --rm -p 6080:80 --device /dev/snd test_container
# connect to http://127.0.0.1:6080/#/?video in your host browser

# Stage 1 : Compile C code
FROM gcc:latest as build
COPY ./sdl /usr/src/sdl
RUN apt-get update && apt-get install apt-utils -y
RUN apt-get update && apt-get install libsdl2-dev -y
RUN apt-get install libsdl2-image-dev -y
RUN apt-get install libsdl2-ttf-dev -y
RUN apt-get install libsdl2-mixer-dev -y
#RUN apt-get install libsdl2-net-dev -y
#RUN apt-get install libsdl2-gfx-dev -y
WORKDIR /usr/src/sdl
RUN make re

# Stage 2 : Copy result of Stage 1 and run it (it will be "the last remaining container" as the program runs)
# Note : the "--rm" with docker run will delete the containers but the image is still cached
FROM dorowu/ubuntu-desktop-lxde-vnc:latest as runner
EXPOSE 80
ENV ASLADEV=hw:2,0
RUN apt-get update && apt-get install apt-utils -y
RUN apt-get update && apt-get install libsdl2-2.0-0 -y
RUN apt-get install libsdl2-image-2.0-0 -y
RUN apt-get install libsdl2-ttf-2.0-0 -y
RUN apt-get install libsdl2-mixer-2.0-0 -y
#RUN apt-get install libsdl2-net-2.0-0 -y
#RUN apt-get install libsdl2-gfx-2.0-0 -y
COPY --from=build /usr/src/sdl /usr/games/
