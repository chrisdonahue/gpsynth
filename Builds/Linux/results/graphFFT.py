import matplotlib.pyplot as plt
import numpy as np
import glob
import os
import re
import math
import sys

# find files
files = [sys.argv[1]]

# float raw string
fp = r"([+-]? *(?:\d+(?:\.\d*)?|\.\d+)(?:[eE][+-]?\d+)?)"
linematch = fp + '\t\t' + fp + '\t' + fp + '\t' + fp + '\t' + fp

#http://lists.apple.com/archives/perfoptimization-dev/2005/Aug/msg00033.html
# -54 found empirically from sine wave

# parse data files
freqs = []
mags = []
phases = []
for path in files:
  f = open(path)

  for line in f:
    match = re.search(linematch, line)
    if match:
      freq = match.group(1)
      real = match.group(2)
      imag = match.group(3)
      freqs.append(float(freq))
      mag = match.group(4)
      dBmag = 10 * math.log(float(real)**2 + float(imag)**2, 10) - 54
      print dBmag
      mags.append(float(dBmag))
      phase = match.group(5)
      phases.append(float(phase))
  f.close()

# format data for graphs
binfreqs = np.array(freqs)
magdata = np.array(mags)
phasedata = np.array(phases)

# plot graphs
plt.plot(binfreqs, magdata)
plt.xlabel('Frequency (Hz)')
plt.ylabel('Magnitude')
plt.xlim(0, 44100/2)
plt.ylim(min(mags), 0)
plt.show()
#plt.savefig(scoregraph)
#plt.clf()

#plt.plot(t, stps)
#plt.xlabel('Episode #')
#plt.ylabel('Average Steps')
#plt.xlim(0, maxEpisodeReached)
#plt.ylim(minsteps, maxsteps)
#plt.savefig(stepsgraph)
