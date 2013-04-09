from __future__ import print_function
import matplotlib.pyplot as plt
import numpy as np
import glob
import os
import re

# make analysis folder
folder = 'summary/'
if not os.path.exists(folder):
  os.makedirs(folder)

# files
info = open(folder + 'trialinfo.txt', 'w')
# graph of the average of each generation average across all runs
trialaveragesgraphpath = folder + 'averages.png'
# graph of best of all each generation best across all run
trialbestsgraphpath = folder + 'bests.png'
# graph of all the averages across all runs overlayed
overlayedaveragesgraphpath = folder + 'overlayed.averages.png'
# graph of all the bests across all runs overlayed
overlayedbestsgraphpath = folder + 'overlayed.bests.png'

# find files
files = glob.glob("*.err")

# float raw string
fp = r"([+-]? *(?:\d+(?:\.\d*)?|\.\d+)(?:[eE][+-]?\d+)?)"
linematch = 'Generation (\d+) had average fitness ' + fp + ' and best fitness ' + fp + ' attained by algorithm (\d+) with structure (.+)'

# parse data files
runs = []
numberOfRuns = 0
for path in files:
  run = []
  
  f = open(path)
  for line in f:
    match = re.search(linematch, line)
    if match:
      gen = int(match.group(1))
      avg = float(match.group(2))
      best = float(match.group(3))
      bestalg = match.group(5)
      run.append((gen, avg, best, bestalg))
  f.close()
  
  runs.append(run)
  numberOfRuns += 1
 
# run data structure:
#  [[(0, gen0avg, gen0best), (1, gen1avg, gen1best), ...], [(...)]]

# ANALYZE DATA
# length of each run
lengthsOfEachRun = [len(runs[i]) for i in range(len(runs))]
# sum of all generation averages across all runs indexed by generation number
sumOfAveragesByGeneration = [0.0 for i in range(max(lengthsOfEachRun))]
# number of runs that got to a particular generation indexed by generation number
numRunsWithThisGeneration = [0 for i in range(max(lengthsOfEachRun))]
# best individual for a generation across all runs indexed by generation numbre
generationBestsAcrossAllRuns = [float('inf') for i in range(max(lengthsOfEachRun))]
# best algorithm across all runs across all generations (run, gen, alg, fit)
bestAlgorithm = [0, 0, 0, float('inf')]
# for each run
for i in range(numberOfRuns):
  # for each completed generation
  for j in range(len(runs[i])):
    # sum each average
    sumOfAveragesByGeneration[j] += runs[i][j][1]
    generationBestsAcrossAllRuns[j] = min(generationBestsAcrossAllRuns[j], runs[i][j][2])
    if runs[i][j][2] < bestAlgorithm[3]:
      bestAlgorithm[0] = i
      bestAlgorithm[1] = j
      bestAlgorithm[2] = runs[i][j][3]
      bestAlgorithm[3] = runs[i][j][2]
    numRunsWithThisGeneration[j] += 1

# find averages
generationAveragesAcrossAllRuns = [sumOfAveragesByGeneration[i]/numRunsWithThisGeneration[i] for i in range(max(lengthsOfEachRun))]

# PRINT INFO
print('NUMBER OF GENERATIONS BY RUN', file=info)
print(lengthsOfEachRun, file=info)
print('NUMBER OF RUNS BY GENERATION', file=info)
print(numRunsWithThisGeneration, file=info)
print('AVERAGE OF ALL GENERATION AVERAGES ACROSS ALL RUNS BY GENERATION', file=info)
print(generationAveragesAcrossAllRuns, file=info)
print('BESTS ACROSS ALL RUNS BY GENERATION', file=info)
print(generationBestsAcrossAllRuns, file=info)
print('BEST ALGORITHM ACROSS ALL RUNS AND ALL GENERATIONS', file=info)
beststr = 'From run %d and generation %d with fitness %f:' % (bestAlgorithm[0], bestAlgorithm[1], bestAlgorithm[3])
print(beststr, file=info)
print(bestAlgorithm[2], file=info)

# MAKE GRAPH OF AVERAGES ACROSS ALL RUNS
generations = np.array(range(max(lengthsOfEachRun)))
averages = np.array(generationAveragesAcrossAllRuns)
plt.plot(generations, averages)
plt.xlabel('Generation #')
plt.ylabel('Average of Averages Across All Runs')
plt.xlim(0, max(lengthsOfEachRun))
plt.ylim(min(averages), max(averages))
plt.show()
plt.savefig(trialaveragesgraphpath)
plt.clf()

# MAKE GRAPH OF BESTS ACROSS ALL RUNS
bests = np.array(generationBestsAcrossAllRuns)
plt.plot(generations, bests)
plt.xlabel('Generation #')
plt.ylabel('Best of Generation Across All Runs')
plt.xlim(0, max(lengthsOfEachRun))
plt.ylim(0, max(bests))
plt.show()
plt.savefig(trialbestsgraphpath)
plt.clf()

info.close()
