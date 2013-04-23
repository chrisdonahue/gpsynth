import matplotlib.pyplot as plt
from matplotlib.collections import LineCollection
from matplotlib.colors import Normalize
from scipy import interpolate
import numpy as np
import re
import sys
import os
import itertools as it

# if we need help
if ('-h' in sys.argv or len(sys.argv) < 7):
  print 'python graphSpectrumComparison.py "Title" min3rdAxis max3rdAxis movingAverage.txt colormapped3Axis.txt interpAmount'
  sys.exit(0)

# use command line arg file
title = sys.argv[1]
penaltyMin = float(sys.argv[2])
penaltyMax = float(sys.argv[3])
movingaveragefilepath = sys.argv[4]
colordatafilepath = sys.argv[5]
interpAmount = int(sys.argv[6])

# float raw string
fp = r"([+-]? *(?:\d+(?:\.\d*)?|\.\d+)(?:[eE][+-]?\d+)?)"
linematch = fp + '\t' + fp + '\t' + fp

# set up containers for graph options
xLabel = 'Frequency (Hz)'
yLabel = 'Magnitude (Amp' + u"\u00B2" + ')'
xMax = float('-inf')
xMin = float('inf')
yMax = float('-inf')
yMin = float('inf')

# set up axis containers
xdata = []
ydata = []
penalties = []

# PARSE COLOR DATA FILE
fileName, fileExtension = os.path.splitext(colordatafilepath)
with open(colordatafilepath, 'r') as f:
  for line in f:
    match = re.search(linematch, line)

    if match:
      x = float(match.group(1))
      y = float(match.group(2))
      xdata.append(x)
      ydata.append(y)
      penalty = float(match.group(3))
      penalties.append(penalty)

  f.close()

xMax = max(xMax, max(xdata))
xMin = min(xMin, min(xdata))
yMax = max(yMax, max(ydata))
yMin = min(yMin, min(ydata))

# format data for graphs
interpolatedx = []
for i in range(len(xdata) - 1):
  xi = xdata[i]
  xf = xdata[i + 1]
  xd = (xf - xi) / float(interpAmount + 1)
  for j in range(interpAmount + 1):
    interpolatedx.append(xi + j*xd)
interpolatedx.append(xdata[len(xdata) - 1])

yf = interpolate.interp1d(xdata, ydata, 'cubic')
interpolatedy = yf(interpolatedx)
pf = interpolate.interp1d(xdata, penalties, 'cubic')
interpolatedp = pf(interpolatedx)

#interpolatedcoordinates = [(interpolatedx[i], interpolatedy[i], interpolatedp[i]) for i in range(len(interpolatedx))]
#print interpolatedx
#print interpolatedy
#print interpolatedp
#print interpolatedcoordinates

# temp
#xarr = np.array(xdata)
#yarr = np.array(ydata)
interpolated = [interpolatedx, interpolatedy]
#interpolated = [xarr, yarr]

# plot line
points = np.array(interpolated).T.reshape(-1, 1, 2)
segments = np.concatenate([points[:-1], points[1:]], axis=1)
lc = LineCollection(segments, cmap=plt.get_cmap('cool'), norm=plt.Normalize(penaltyMin, penaltyMax))
lc.set_array(interpolatedp)
#lc.set_array(np.array(penalties))
lc.set_linewidth(2)

# PARSE MOVING AVERAGE DATA FILE
linematch = fp + '\t' + fp
fileName, fileExtension = os.path.splitext(movingaveragefilepath)
xdata = []
ydata = []
with open(movingaveragefilepath, 'r') as f:
  for line in f:
    match = re.search(linematch, line)

    if match:
      x = float(match.group(1))
      y = float(match.group(2))
      xdata.append(x)
      ydata.append(y)

  f.close()
  
  plt.plot(np.array(xdata), np.array(ydata), label=fileName, color='red', linewidth=2.0)

# plot figure
plt.gca().add_collection(lc)
plt.colorbar(lc)
plt.title(title)
plt.xlim(min([xMin, xMax]), max([xMin, xMax]))
plt.ylim(min([yMin, yMax]), max([yMin, yMax]))
plt.xlabel(xLabel)
plt.ylabel(yLabel)

plt.savefig(title + '.png')
plt.show()
plt.clf()
