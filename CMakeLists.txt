cmake_minimum_required(VERSION 3.23)
project(Pacman)

set(CMAKE_CXX_STANDARD 14)

include_directories(Pacman)
include_directories(Pacman/S2D)
include_directories(Pacman/S2D/AL)
include_directories(Pacman/S2D/GL)
include_directories(Pacman/S2D/SOIL)

add_executable(Pacman
        Pacman/S2D/AL/al.h
        Pacman/S2D/AL/alc.h
        Pacman/S2D/AL/alure.h
        Pacman/S2D/GL/freeglut.h
        Pacman/S2D/GL/freeglut_ext.h
        Pacman/S2D/GL/freeglut_std.h
        Pacman/S2D/GL/glut.h
        Pacman/S2D/SOIL/SOIL.h
        Pacman/S2D/Audio.h
        Pacman/S2D/Game.h
        Pacman/S2D/Graphics.h
        Pacman/S2D/Input.h
        Pacman/S2D/MathHelper.h
        Pacman/S2D/S2D.h
        Pacman/S2D/SoundEffect.h
        Pacman/S2D/SpriteBatch.h
        Pacman/S2D/Texture2D.h
        Pacman/main.cpp
        Pacman/Pacman.cpp
        Pacman/Pacman.h)
