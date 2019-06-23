import pathlib
import matplotlib.pyplot as plt
import numpy as np
import sys


filename = sys.argv[1]

dataPath = pathlib.Path(filename)

parsed = {
    "frame": [],
    "render": [],
    "update": [],
    "input": []
}

def parse(openedfile):
    for raw in openedfile:
        key, _, value = raw.strip().partition(' ')
        yield key, float(value)

def getData(filepath: pathlib.Path):
    with filepath.open(mode='r') as dataFile:
        for key, number in parse(dataFile):
            if key == "F":
                parsed["frame"].append(number)
            if key == "U":
                parsed["update"].append(number)
            if key == "R":
                parsed["render"].append(number)
            if key == "I":
                parsed["input"].append(number)

def plotit(data, title):
    pl, = plt.plot(data, label=title)
    plt.grid(True)
    return pl

if __name__ == "__main__":
    getData(dataPath)

    pl1 = plotit(parsed["frame"], "Frame time elapsed")
    pl2 = plotit(parsed["render"], "Render time elapsed")
    pl3 = plotit(parsed["update"], "Update time elapsed")
    pl4 = plotit(parsed["input"], "Input time elapsed")

    plt.legend([pl1, pl2, pl3, pl4], loc=1)
    plt.show()