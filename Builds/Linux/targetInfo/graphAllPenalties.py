import matplotlib.pyplot as plt
from matplotlib.collections import LineCollection
from matplotlib.colors import Normalize
from scipy import interpolate
import numpy as np
import re
import sys
import os
import itertools as it
import glob

# if we need help
if ('-h' in sys.argv or len(sys.argv) < 8):
  print 'python graphAllPenalties.py "Title" "under/over" ymin ymax penaltymin penaltymax interpAmount'
  sys.exit(0)

# use command line args
title = sys.argv[1]
colorfiles = glob.glob('*' + sys.argv[2] + '*.txt')
colorfiles.sort()
macfiles = glob.glob('*moving*')
macfiles.sort()
yMin = float(sys.argv[3])
yMax = float(sys.argv[4])
penaltyMin = float(sys.argv[5])
penaltyMax = float(sys.argv[6])
interpAmount = int(sys.argv[7])

# float raw string
fp = r"([+-]? *(?:\d+(?:\.\d*)?|\.\d+)(?:[eE][+-]?\d+)?)"
threeDlinematch = fp + '\t' + fp + '\t' + fp
twoDlinematch = fp + '\t' + fp

# set up containers for graph options
xLabel = 'Frequency (Hz)'
yLabel = 'Magnitude (Amp' + u"\u00B2" + ')'
xMax = float('-inf')
xMin = float('inf')

# PARSE COLOR DATA FILE
for num, colordatafilepath in zip(range(len(colorfiles)), colorfiles):
  # set up axis containers
  xdata = []
  ydata = []
  penalties = []

  print 'PARSING: ' + colordatafilepath
  fileName, fileExtension = os.path.splitext(colordatafilepath)
  with open(colordatafilepath, 'r') as f:
    for line in f:
      match = re.search(threeDlinematch, line)

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
  
  # format data for graphs
  interpolatedx = []
  for i in range(len(xdata) - 1):
    xi = xdata[i]
    xf = xdata[i + 1]
    xd = (xf - xi) / float(interpAmount + 1)
    for j in range(interpAmount + 1):
      interpolatedx.append(xi + j*xd)
  interpolatedx.append(xdata[len(xdata) - 1])

  # interpolate
  yf = interpolate.interp1d(xdata, ydata, 'cubic')
  interpolatedy = yf(interpolatedx)
  pf = interpolate.interp1d(xdata, penalties, 'cubic')
  interpolatedp = pf(interpolatedx)
  interpolated = [interpolatedx, interpolatedy]
  
  # plot line
  points = np.array(interpolated).T.reshape(-1, 1, 2)
  segments = np.concatenate([points[:-1], points[1:]], axis=1)
  lc = LineCollection(segments, cmap=plt.get_cmap('cool'), norm=plt.Normalize(penaltyMin, penaltyMax))
  lc.set_array(interpolatedp)
#lc.set_array(np.array(penalties))
  lc.set_linewidth(2)
  
  # PLOT MOVING AVERAGE DATA FILE
  movingaveragefilepath = macfiles[num]
  print 'PARSING: ' + movingaveragefilepath
  xdata = []
  ydata = []
  with open(movingaveragefilepath, 'r') as f:
    for line in f:
      match = re.search(twoDlinematch, line)

      if match:
        x = float(match.group(1))
        y = float(match.group(2))
        xdata.append(x)
        ydata.append(y)
  f.close()
  plt.plot(np.array(xdata), np.array(ydata), color='red', linewidth=2.0)
    
  # plot figure
  plt.gca().add_collection(lc)
  plt.colorbar(lc)
  plt.title(title)
  plt.xlim(min([xMin, xMax]), max([xMin, xMax]))
  plt.ylim(min([yMin, yMax]), max([yMin, yMax]))
  plt.xlabel(xLabel)
  plt.ylabel(yLabel)
  #plt.gcf().set_size_inches(4, 3)
  plt.savefig(fileName + '.png')#, dpi=300)
  plt.clf()
