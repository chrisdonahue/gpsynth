import matplotlib.pyplot as plt
import numpy as np
import re
import sys
import os
import itertools as it

# set up graph
fig = plt.figure(1)
ax = fig.add_subplot(1,1,1)

# use command line arg file
plt.title(sys.argv[1])
files = sys.argv[2:]

# float raw string
fp = r"([+-]? *(?:\d+(?:\.\d*)?|\.\d+)(?:[eE][+-]?\d+)?)"
linematch = fp + '\t' + fp

# set up containers for graph options
xIncreasesFromOrigin = True
yIncreasesFromOrigin = True
xAxisIsFloatData = True
yAxisIsFloatData = True
xLabel = ''
yLabel = ''
xMax = float('-inf')
xMin = float('inf')
yMax = float('-inf')
yMin = float('inf')
colors = ('b','g','m','r','c','y','k','w')
for file, color in zip(files, it.cycle(colors)):
  # set up axis containers
  x = []
  y = []
  
  # parse file options
  fileName, fileExtension = os.path.splitext(file)
  f = open(file)
  options = f.readline()
  for option in options.split():
    if option == 'x<':
      xIncreasesFromOrigin = False
    elif option == 'yv':
      yIncreasesFromOrigin = False
    elif option == 'xi':
      xAxisIsFloatData = False
    elif option == 'yi':
      yAxisIsFloatData = False

  labels = f.readline()
  labelArray = labels.split('\t')
  xLabel = labelArray[0]
  yLabel = labelArray[1][:-2]

  # parse data file
  for line in f:
    match = re.search(linematch, line)
    
    if match:
      if xAxisIsFloatData:
        x.append(float(match.group(1)))
      else:
        x.append(int(match.group(1)))
      if yAxisIsFloatData:
        y.append(float(match.group(2)))
      else:
        y.append(int(match.group(2)))
  f.close()

  xMax = max(xMax, max(x))
  xMin = min(xMin, min(x))
  yMax = max(yMax, max(y))
  yMin = min(yMin, min(y))

  # format data for graphs
  xdata = np.array(x)
  ydata = np.array(y)

  # plot line
  ax.plot(xdata, ydata, label=fileName, color=color)
ax.legend()

plt.xlabel(xLabel)
plt.ylabel(yLabel)
if xIncreasesFromOrigin == True:
  plt.xlim(min([xMin, xMax]), max([xMin, xMax]))
else:
  plt.xlim(max([xMin, xMax]), min([xMin, xMax]))
if yIncreasesFromOrigin == True:
  plt.ylim(min([yMin, yMax]), max([yMin, yMax]))
else:
  plt.ylim(max([yMin, yMax]), min([yMin, yMax]))

plt.savefig('./graph.png')
plt.show()
plt.clf()
