from random import randint
import os
import json
from typing import List, Dict

height = 10
width = 24

characters: List[str] = ["", "", "", ""]
art: List[List[str]] = []
localsaves: Dict[str, List[List[str]]]

if os.path.exists("artsave.txt"):
    localsaves = json.load(open("artsave.txt", "rt"))
else:
    createFile = open("artsave.txt", "w")
    createFile.write("{")
    localsaves = {}


def generateArt(gen: List[List[str]]):
    for i in range(height):
        gen.append([])
        for j in range(width):
            randnum = randint(1, 4)

            match randnum:
                case 1:
                    gen[i].append(characters[0])

                case 2:
                    gen[i].append(characters[1])

                case 3:
                    gen[i].append(characters[2])

                case 4:
                    gen[i].append(characters[3])


def printArt(gen: List[List[str]]):
    for i in range(height):
        for j in range(width):
            if j == width - 1:
                print(gen[i][j])

            else:
                print(gen[i][j], end="")


def clearScreen():
    print("\x1b[2J", end="")
    print("\x1b[H", end="")


clearScreen()
generateArt(art)
printArt(art)

while True:
    command = input("Enter a command (eg new, exit, save, load, names)\n")

    match command:
        case "new":
            art = []
            clearScreen()
            generateArt(art)
            printArt(art)

        case "exit":
            clearScreen()
            json.dump(localsaves, open("artsave.txt", "wt"))
            exit()

        case "save":
            clearScreen()
            name = input("Please name the pattern\n")

            if localsaves.get(name):
                clearScreen()
                print("Error: key already exists")
            else:
                clearScreen()
                localsaves[name] = art
                art = []
                generateArt(art)
                printArt(art)

        case "load":
            clearScreen()
            name = input("Enter the name of the peice of art.\n")

            clearScreen()
            if localsaves.get(name):
                printArt(localsaves[name])
                print(name)
            else:
                print("Error: key does not exist")

        case "names":
            names: List[str] = []

            for i in localsaves:
                names.append(i)

            clearScreen()
            print(names)

        case "editicons":
            clearScreen()
            icons: str = input("add the icons seperated by commas. (4 max)")

            characters = icons.split(" ")

            clearScreen()
            art = []
            generateArt(art)
            printArt(art)
