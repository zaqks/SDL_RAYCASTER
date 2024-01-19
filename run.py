from utility.tasks import *


class Vars:
    run = True


def toggleRun():
    Vars.run = not Vars.run


funcs = [toggleRun, compileS, build, merge]
choices = [f"toggle run: {Vars.run}", "compile", "build", "merge"]
choice = -1

while choice > funcs.__len__() - 1 or choice < 1:
    # display menu
    print("welcome to C project utility tools")
    print("please select an operation")

    for i in range(choices.__len__()):
        print(f"{i}- {choices[i]}")
    print("\n")

    # user input
    try:
        choice = int(input())
    except:
        # auto compile
        choice = 1

    # for run t0ggle
    if choice == 0:
        funcs[choice]()
        choices[0] = f"toggle run: {Vars.run}"


funcs[choice]()
if Vars.run:
    print(f"running ./{COMP_DIST}{MAIN_FILE_NAME}")
    system(f"./{COMP_DIST}{MAIN_FILE_NAME}")
