import matplotlib.pyplot as plt
import numpy as np
import re
import sys
import os
import itertools as it

# Two subplots, unpack the axes array immediately
f, (ax1, ax2) = plt.subplots(1, 2, sharey=True)
ax1.set_title('Undershooting Penalty')
ax2.set_title('Overshooting Penalty')
subplots = [ax1, ax2]

# if we need help
if ('-h' in sys.argv or len(sys.argv) < 3):
  print 'python graph2Axis.py undershooting.txt overshooting.txt'
  sys.exit(0)

# use command line arg file
files = sys.argv[1:]

# float raw string
fp = r"([+-]? *(?:\d+(?:\.\d*)?|\.\d+)(?:[eE][+-]?\d+)?)"
linematch = fp + '\t' + fp + '\t' + fp

# set up containers for graph options
xLabel = 'Bin Number'
yLabel = 'Magnitude'
xMax = float('-inf')
xMin = float('inf')
yMax = float('-inf')
yMin = float('inf')
for file, num in zip(files, range(len(files))):
  # set up axis containers
  x = []
  y = []
  penalty = []
 
  # open file
  fileName, fileExtension = os.path.splitext(file)
  f = open(file)

  # parse data file
  for line in f:
    match = re.search(linematch, line)
    
    if match:
      x.append(float(match.group(1)))
      y.append(float(match.group(2)))
      penalty.append(float(match.group(3)))
  f.close()

  xMax = max(xMax, max(x))
  xMin = min(xMin, min(x))
  yMax = max(yMax, max(y))
  yMin = min(yMin, min(y))

  # format data for graphs
  xdata = np.array(x)
  ydata = np.array(y)

  # plot line
  subplots[num].plot(xdata, ydata, label=fileName)

plt.xlim(min([xMin, xMax]), max([xMin, xMax]))
plt.ylim(min([yMin, yMax]), max([yMin, yMax]))
plt.xlabel(xLabel)
plt.ylabel(yLabel)

plt.savefig('./overshootingUndershootingFitness.png')
plt.show()
plt.clf()
