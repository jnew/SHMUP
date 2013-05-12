SHMUP
=====

Title: Ace "SPACE" Pilot

Info: This is basically a tech demo of a shoot-em-up engine we wrote from the ground up in C++ with libsfml. There is not much "gameplay" but we have waves of enemies that will spawn to demo player tracking, bullet patterns, enemy movement, enemy health/health bars, our scrolling backgrounds, and scorekeeping. You can play until you lose, or you clear the waves and nothing spawns anymore!

Required libraries: libglew, libsfml (version 2.0, NOT 1.6), freeglut3, libopenal, libopenal-data

These libraries are packaged in some repositories. If you are using Ubuntu or Debian, the packages that are not in your repository can be found in a PPA. If you are running Ubuntu, here are the installation instructions:

sudo add-apt-repository ppa:sonkun/sfml-development

sudo apt-get update

sudo apt-get install freeglut3 libopenal1 libopenal-data libglew-dev libsfml libsfml-dev

If you are using another distro, you may have to compile the libraries or find them elsewhere. (Arch users have all required libraries in their repos, and all are the right versions. Ubuntu's versions are outdated, hence the included PPA address.)

Once the required libraries are installed, run "make" in the root game directory.

The compiled game will be an executable named "Game" also in the root.

Make sure the game is marked as executable if you are having problems running it.
