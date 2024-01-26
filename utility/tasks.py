from os import system
import time

# MAINPATH = "../"
MAINPATH = ""
COMP_DIST = f"{MAINPATH}bin/"
BUILD_DIST = f"{MAINPATH}build/"
MERGE_DIST = f"{MAINPATH}merge/"

MAIN_FILE_NAME = "main"
MAIN_FILE = f"{MAINPATH}{MAIN_FILE_NAME}"
IMPORTS_FILE = f"{MAINPATH}libs/project/imports.h"

PACKAGES = [
    "sdl2",
    "SDL2_ttf",
    "SDL2_mixer",
    # "SDL2_image"
]


def _genArgs(lst):
    args = ""

    for i in lst:
        args += f"{i} "

    return args


def compileS():
    start = time.time()

    system(
        f"gcc {MAIN_FILE}.c -lm $(pkg-config --cflags --libs {_genArgs(PACKAGES)}) -o {COMP_DIST}{MAIN_FILE_NAME}"
    )
    end = time.time()
    print(f"compiled in {round(end - start, 3)}s")


def merge():
    with open(IMPORTS_FILE, "r") as f:
        importsList = f.readlines()
        f.close()

    includes = []
    resultLines = []

    for i in importsList:
        if i != "\n":
            if not (i.__contains__("<") or i.__contains__(">")):
                # get the fil path
                currentPath = i.split("#include ")[1]
                currentPath = currentPath.replace('"', "")
                currentPath = currentPath.replace("\n", "")

                currentPath = f"{MAINPATH}libs/project/{currentPath}"
                print(currentPath)

                # copy the content
                with open(currentPath, "r") as f:
                    for i in f.readlines():
                        if not includes.__contains__(i):
                            if i.__contains__("#include"):
                                includes.append(i)

                            resultLines.append(i)
                            if i.__contains__("#define"):
                                resultLines.append("\n")

                    f.close()
            resultLines.append("\n")

    # add the main.c
    resultLines.append("\n")
    with open(f"{MAIN_FILE}.c", "r") as f:
        for i in f.readlines():
            if not i.__contains__(IMPORTS_FILE):
                resultLines.append(i)

        f.close()

    with open(f"{MERGE_DIST}merged.c", "w") as f:
        f.writelines(resultLines)
        f.close()


def build():
    # compile
    compileS()
    system(f"cp {COMP_DIST}{MAIN_FILE_NAME} {BUILD_DIST}")
    print("bin built")
    # copy to build
    buildList = {"assets", "data"}
    for i in buildList:
        system(f"cp {MAINPATH}{i} {BUILD_DIST} -r")
        print(f"{i} built")
