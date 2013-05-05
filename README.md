SHMUP
=====

Title: Ace "SPACE" Pilot

Required libraries: libglew, libsfml (version 2.0, NOT 1.6), freeglut3, libopenal, libopenal-data

These libraries are packaged in some repositories. If you are using Ubuntu or Debian, the packages that are not in your repository have been included in the "libpackages". Please install, in this order:

freeglut3, which is in the repos, as well as openal (sudo apt-get install freeglut3 libopenal1 libopenal-data)

libglew, which is in the libpackages folder.

sfml, which is in the libpackages folder.

sfml-dev, also in the libpackages folder. (installing libsfml-dev may prompt you to remove libsfml, this is fine, do it)

If you are using another distro, you may have to compile the libraries or find them elsewhere. (Arch users have all required libraries in their repos, and all are the right versions. Ubuntu's versions are outdated, hence the included .deb packages.)

Once the required libraries are installed, run "make".

The compiled game will be an executable named "Game" in the root of this directory.

Make sure the game is marked as executable if you are having problems running it.
