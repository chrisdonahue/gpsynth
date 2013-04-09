import matplotlib.pyplot as plt
import numpy as np
import re
import sys

# use command line arg file
files = [sys.argv[1]]

# float raw string
fp = r"([+-]? *(?:\d+(?:\.\d*)?|\.\d+)(?:[eE][+-]?\d+)?)"
linematch = fp + '\t' + fp

# set up axis containers
x = []
y= []

# process graph options from file
f = open(files[0])
options = f.readline()
xIncreasesFromOrigin = True
yIncreasesFromOrigin = True
xAxisIsFloatData = True
yAxisIsFloatData = True
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
yLabel = labelArray[1][:-1]

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

# format data for graphs
xdata = np.array(x)
ydata = np.array(y)

# plot graphs
plt.plot(x, y)
plt.xlabel(xLabel)
plt.ylabel(yLabel)
if xIncreasesFromOrigin == True:
  plt.xlim(min(x), max(x))
else:
  plt.xlim(max(x), min(x))
if yIncreasesFromOrigin == True:
  plt.ylim(min(y), max(y))
else:
  plt.ylim(max(y), min(y))
plt.show()
#plt.savefig(scoregraph)
#plt.clf()
