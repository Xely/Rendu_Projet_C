cmake_minimum_required(VERSION 3.5)
project(projet_RPG_a_modif)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    dungeon.c
    dungeon.h
    Equipement.c
    Equipement.h
    game.c
    game.h
    GameUtil.c
    main.c
    Mob.c
    Mob.h
    MobRace.c
    MobRace.h
    Player.c
    Player.h
    StuffItem.c
    StuffItem.h
    tavern.c
    tavern.h
    UsableItem.c
    Usableitem.h)

add_executable(projet_RPG_a_modif ${SOURCE_FILES})