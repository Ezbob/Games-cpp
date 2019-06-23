import pathlib
import matplotlib.pyplot as plt
import numpy as np
import sys

data = {
    "F": ([], "Frame time"),
    "R": ([], "Render time"),
    "U": ([], "Update time"),
    "I": ([], "Input time"),
}

def parse(openedfile):
    for raw in openedfile:
        key, delimiter, value = raw.strip().partition(' ')
        if delimiter == ' ' and len(key) == 1 and len(value) > 0:
            yield key.upper(), float(value)

def fillData(filepath: pathlib.Path):
    with filepath.open(mode='r') as dataFile:
        for key, number in parse(dataFile):
            if data.get(key) is not None:
                data[key][0].append(number)

def plotit(dataKey):
    pl, = plt.plot(data[dataKey][0], label=data[dataKey][1])
    return pl

def main():
    if len(sys.argv) < 2:
        print("Missing data file path")
        exit(1)
    filename = sys.argv[1]
    dataPath = pathlib.Path(filename)

    print("Parsing data...")
    fillData(dataPath)
    print("Plotting data...")
    plotlines = tuple( plotit(key) for key in data.keys())
    print("Rendering graphs...")
    plt.grid(True, which='both')
    plt.title("Game loop performance")
    plt.ylabel("Time in milliseconds")
    plt.xlabel("Frame sample count")
    plt.legend(plotlines, loc=1)
    plt.show()
    print("Done.")

if __name__ == "__main__":
    main()