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
if ('-h' in sys.argv or len(sys.argv) < 3):
  print 'python makeSpectrogram.py "Title" interpAmount'
  sys.exit(0)

# use command line args
title = sys.argv[1]
magnitudefiles = glob.glob('*magnitude.txt')
magnitudefiles.sort()
interpAmount = int(sys.argv[2])

# float raw string
fp = r"([+-]? *(?:\d+(?:\.\d*)?|\.\d+)(?:[eE][+-]?\d+)?)"
threeDlinematch = fp + '\t' + fp + '\t' + fp
twoDlinematch = fp + '\t' + fp

# set up containers for graph options
xLabel = 'Time (s)'
yLabel = 'Frequency (Hz)'
magLabel = 'Magnitude (Amp' + u"\u00B2" + ')'
xMax = float('-inf')
xMin = float('inf')
yMax = float('-inf')
yMin = float('inf')
magMax = float('-inf')
magMin = float('inf')

# PARSE COLOR DATA FILE
for num, magnitudefilepath in zip(range(len(magnitudefiles)), magnitudefiles):
  # set up axis containers
  xdata = []
  ydata = []
  magnitudes = []

  print 'PARSING: ' + magnitudefilepath
  fileName, fileExtension = os.path.splitext(magnitudefilepath)
  with open(magnitudefilepath, 'r') as f:
    for line in f:
      match = re.search(threeDlinematch, line)

      if match:
        x = float(match.group(1))
        y = float(match.group(2))
        xdata.append(x)
        ydata.append(y)
        magnitude = float(match.group(3))
        magnitudes.append(magnitude)
    f.close()

  xMax = max(xMax, max(xdata))
  xMin = min(xMin, min(xdata))
  yMax = max(yMax, max(ydata))
  yMin = min(yMin, min(ydata))
  magMax = max(magMax, max(magnitudes))
  magMin = min(magMin, min(magnitudes))
  
  # format data for graphs
  
  #interpolatedx = []
  #for i in range(len(xdata) - 1):
  #  xi = xdata[i]
  #  xf = xdata[i + 1]
  #  xd = (xf - xi) / float(interpAmount + 1)
  #  for j in range(interpAmount + 1):
  #    interpolatedx.append(xi + j*xd)
  #interpolatedx.append(xdata[len(xdata) - 1])

  # interpolate
  #yf = interpolate.interp1d(xdata, ydata, 'cubic')
  #interpolatedy = yf(interpolatedx)
  #pf = interpolate.interp1d(xdata, magnitudes, 'cubic')
  #interpolatedp = pf(interpolatedx)
  #interpolated = [interpolatedx, interpolatedy]
  interpolated = [np.array(xdata), np.array(ydata)]
  
  # plot line
  points = np.array(interpolated).T.reshape(-1, 1, 2)
  segments = np.concatenate([points[:-1], points[1:]], axis=1)
  lc = LineCollection(segments, cmap=plt.get_cmap('jet'), norm=plt.Normalize(magMin, magMax))
  #lc.set_array(interpolatedp)
  lc.set_array(np.array(magnitudes))
  lc.set_linewidth(interpAmount)
    
  # plot figure
  plt.gca().add_collection(lc)
  plt.title(title)
  plt.xlim(min([xMin, xMax]), max([xMin, xMax]))
  plt.ylim(min([yMin, yMax]), max([yMin, yMax]))
  plt.xlabel(xLabel)
  plt.ylabel(yLabel)

cb1 = plt.colorbar(lc)
cb1.set_label(magLabel)
#plt.gcf().set_size_inches(4, 3)
plt.savefig('spectrum.png')#, dpi=300)
#plt.show()
plt.clf()
