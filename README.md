# PewPewManVS
Dockerize your pewpew, work in progress

[[_TOC_]]

# Prérequis
With Docker :
- Docker
- python
- make
Locally :
- gcc and dev sdl libs (see Dockerfile)
- make


# Run client with Docker (Linux with Docker)
In the root folder, enter the command `make` to build the docker image, launch a web browser at [http://127.0.0.1:6080//#/?video](http://127.0.0.1:6080//#/?video) then run the server on console.

Server may take few seconds to be fully ready.

Exit with `Ctrl + C` in the console to stop and destroy the container.


# Run game on Linux

## Docker guest
Open a new terminal in the displayed desktop on your browser, go to folder `/usr/games` and run `./BombermanVS` to play the game.

Exit the game with `Esc` key at any point or click on the `Close Window` button.

## Host
You need to compile the game in order to run it.

In the `./sdl` folder, run `make` to compile.

If you make changes in the code afterward, use `make re` to remove old binaries and compile the new code.

Run the game with the command `./BombermanVS`.

# Run game on Windows(Docker only)
Remark : You can run the game localy but you hame to use Cygwin and stuff to compile using Windows-compatible libraries.
## Docker build

```ps1
docker.exe build -t test_container .
```

## Docker run 

```ps1
docker.exe run -it --rm -p 6080:80 --device /dev/snd test_container
```
Go to [http://127.0.0.1:6080//#/?video](http://127.0.0.1:6080//#/?video) in your web browser.

## Docker guest
Open a new terminal in the displayed desktop on your browser, go to folder `/usr/games` and run `./BombermanVS` to play the game.

Exit the game with `Esc` key at any point or click on the `Close Window` button.



# How to play
Escape = Exit;
## Menu 
Arrow keys = move;
Space, Return = Accept;
## Game
Arrow keys = move;
Space = bomb;