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
scoregraph = folder + 'rewards.png'
stepsgraph = folder + 'steps.png'

# bounds
minreward = 0
maxreward = 500
minsteps = 0
maxsteps = 500

# find files
files = glob.glob("*.err*")

# parse data files
trials = []
for path in files:
  scores = []
  steps = []
  errors = []
  trials.append((scores, steps, errors))
  f = open(path)

  for line in f:
    match = re.search('(\-?\d+)\t(\d+)\t(\d+)', line)
    if match:
      score = match.group(1)
      step = match.group(2)
      error = match.group(3)
      scores.append(score)
      steps.append(step)
      errors.append(error)
  f.close()

# find number of episodes that each trial lasted
numEpisodesPerTrial = []
for i in range(len(trials)):
  numEpisodesPerTrial.append(len(trials[i][0]))
print ('NUMBER OF EPISODES PER TRIAL:', file=info)
print (numEpisodesPerTrial, file=info)

# analyze data
maxEpisodeReached = max(numEpisodesPerTrial)
totalrewards = []
totalsteps = []
totalerrors = []
errorcounts = []
for i in range(maxEpisodeReached):
  totalreward = 0
  totalstep = 0
  totalerror = 0
  numEpisodesThatReachedThisTrial = 0
  for j in range(len(trials)):
    trial = trials[j]
    if numEpisodesPerTrial[j] > i:
      if (int(trial[0][i]) > maxreward):
        maxreward = int(trial[0][i])
      totalreward = totalreward + int(trial[0][i])
      totalstep = totalstep + int(trial[1][i])
      totalerror = totalerror + int(trial[2][i])
      numEpisodesThatReachedThisTrial = numEpisodesThatReachedThisTrial + 1
  totalrewards.append((totalreward, numEpisodesThatReachedThisTrial))
  totalsteps.append((totalstep, numEpisodesThatReachedThisTrial))
  totalerrors.append((totalerror, numEpisodesThatReachedThisTrial))
  errorcounts.append(totalerror)

# total prints for debugging
#print ('PER EPISODE: (TOTAL REWARD, NUM TRIALS THAT REACHED THIS EPISODE)', file=info)
#print (totalrewards, file=info)
#print ('PER EPISODE: (TOTAL STEPS, NUM TRIALS THAT REACHED THIS EPISODE)', file=info)
#print (totalsteps, file=info)
#print ('PER EPISODE: (TOTAL ERRORS, NUM TRIALS THAT REACHED THIS EPISODE)', file=info)
#print (totalerrors, file=info)

# average rewards, steps and errors by episode
averagerewards = []
for (total, epCount) in totalrewards:
  averagerewards.append(float(total)/epCount)
print ('AVERAGE EPISODE REWARD ACROSS ALL TRIALS:', file=info)
print (averagerewards, file=info)

averagesteps = []
for (total, epCount) in totalsteps:
  averagesteps.append(float(total)/epCount)
print ('AVERAGE STEPS IN AN EPISODE ACROSS ALL TRIALS:', file=info)
print (averagesteps, file=info)

averageerrors = []
for (total, epCount) in totalerrors:
  averageerrors.append(float(total)/epCount)
#print ('AVERAGE ERRORS IN AN EPISODE ACROSS ALL TRIALS:', file=info)
#print (averageerrors, file=info)

print ('NUMBER OF IMAGE PARSING ERRORS BY EPISODE:', file=info)
print (errorcounts, file=info)

# format data for graphs
t = np.array(range(maxEpisodeReached))
avgs = np.array(averagerewards)
stps = np.array(averagesteps)
errs = np.array(averageerrors)

# plot graphs
plt.plot(t, avgs)
plt.xlabel('Episode #')
plt.ylabel('Average Reward')
plt.xlim(0, maxEpisodeReached)
plt.ylim(minreward, maxreward)
#plt.show()
plt.savefig(scoregraph)
plt.clf()

plt.plot(t, stps)
plt.xlabel('Episode #')
plt.ylabel('Average Steps')
plt.xlim(0, maxEpisodeReached)
plt.ylim(minsteps, maxsteps)
plt.savefig(stepsgraph)

info.close()
