# imports
import math
import os
import random

# define networks
class Network:
	def __init__(self, head):
		global newNetworkID
		self.string = ''
		self.id = newNetworkID
		newNetworkID += 1
		self.head = head
	
	def evaluate(self):
		return self.head.evaluate()
		
	def toString(self):
		if self.string == '':
			self.string = self.head.toString()
		return self.string

	def mutate(self):
		global simplifyChance
		global specialChance
		global harmonyChance
		global functionChance
		global valueRange
		
		# first step, head if a special node (initialize relevent random fields)
		lfonode = random.choice(lfo)
		function = getRandomFunction()
		if self.head in special:
			newhead = FunctionNode(function, self.head, lfonode)
			self.head = newhead
			return

		# find all nodes
		functionNodes = []
		valueNodes = []
		specialNodes = []
		self.head.findNodes(functionNodes, valueNodes, specialNodes, None)
		
		# allow special nodes to be mutated if there is more than one
		if (len(specialNodes) > 1):
			functionNodes.extend(specialNodes)

		# initialize replacement value node for various mutations
		replacementNode = ValueNode([random.random() * valueRange])

		# select a random function and a random value for various types of mutations
		randomFunction = None
		if len(functionNodes) > 0:
			randomFunction = random.choice(functionNodes)
		else:
			newnode = FunctionNode(function, self.head, replacementNode)
			self.swap(self.head, newnode)
			return
		
		randomValue = None
		if len(valueNodes) > 0:
			randomValue = random.choice(valueNodes)
		else:
			newnode = FunctionNode(function, randomFunction, replacementNode)
			self.swap(randomFunction, newnode)
			return
		
		# mutation logic
		if (random.random() < simplifyChance):
			# simplify
			self.swap(randomFunction, replacementNode)
		else:
			# complexify
			if (random.random() < specialChance):
				# mutate special
				if (random.random() < harmonyChance):
					# add special node
					newpartial = random.choice(special)
					self.swap(randomValue, newpartial)
				else:
					# add time node
					self.swap(randomValue, lfonode)
			else:
				# mutate normal
				if (random.random() < functionChance):
					# mutate function
					randomFunction.setFunction(function, randomValue)
				else:
					# mutate value
					newnode = FunctionNode(function, randomFunction, replacementNode)
					self.swap(randomFunction, newnode)
		
	def inseminate(self, woman):
		sperm = self.head
		egg = woman.head
		return Network(FunctionNode((multiply, '*'), sperm, egg))
	
	def copy(self):
		return Network(self.head.copy())

	def swap(self, nodeone, nodetwo):
		if (nodeone == self.head):
			self.head = nodetwo
		elif nodeone.parent.left == nodeone:
			nodeone.parent.left = nodetwo
		elif nodeone.parent.right == nodeone:
			nodeone.parent.right = nodetwo
		else:
			debug('Bad swap.')
		
class FunctionNode:
	def __init__(self, (func, symbol), l, r):
		self.function = func
		self.symbol = symbol
		self.left = l
		self.right = r
		self.parent = None
		
	def setFunction(self, (func, symbol), rightSubstitute):
		if self.right == None:
			self.right = rightSubstitute
		self.function = func
		self.symbol = symbol

	def evaluate(self):
		return self.function(self.left, self.right)

	def toString(self):
		if self.right != None:
			return '(%s %s %s)' % (self.symbol, self.left.toString(), self.right.toString())
		else:
			return '(%s %s)' % (self.symbol, self.left.toString())
			
	def findNodes(self, functions, values, specials, parent):
		self.parent = parent
		if self in special:
			specials.append(self)
			return
			
		functions.append(self)
		if (self.left != None):
			self.left.findNodes(functions, values, specials, self)
		if (self.right != None):
			self.right.findNodes(functions, values, specials, self)
			
	def copy(self):
		if self in special:
			return self
		if self.right != None:
			return FunctionNode((self.function, self.symbol), self.left.copy(), self.right.copy())
		else:
			return FunctionNode((self.function, self.symbol), self.left.copy(), None)
		
class ValueNode:
	# values are arrays to allow for time, cps pointers
	def __init__(self, value):
		self.value = value
		self.symbol = 'value'
		self.parent = None

	def evaluate(self):
		return self.value[0]

	def toString(self):
		if (id(self.value) == id(time)):
			return 'time'
		elif (id(self.value) == id(cps)):
			return 'f'
		else:
			return '%f' % (self.value[0])
			
	def findNodes(self, functions, values, specials, parent):
		self.parent = parent
		values.append(self)
		return
		
	def copy(self):
		newcopy = ValueNode(self.value)
		return newcopy
		
# set up state
populationSize = 0
newNetworkID = 0
generationID = 0

# network storage
upForEvaluation = []
evaluated = set()
winners = set()
networks = []

# special node values
time = [0]
cps = [0]

# parameters
valueRange = 5
LFORange = 10
numPartials = 10

# set up parameters
mutationChance = 0.0
simplifyChance = 0.0
specialChance = 0.0
harmonyChance = 0.0
functionChance = 0.0
crossChance = 0.0
def setParameters(mutlik, complik, speciallik, harmlik, funclik, crosslik):
	global mutationChance
	global simplifyChance
	global specialChance
	global harmonyChance
	global functionChance
	global crossChance
	mutationChance = mutlik
	simplifyChance = complik
	specialChance = speciallik
	harmonyChance = harmlik
	functionChance = funclik
	crossChance = crosslik
	return 1.0
	
# set up function weights
weights = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
def setFunctionChances(addweight, subweight, multweight, divweight, powweight, modweight, sinweight, cosweight):
	weights[0] = addweight
	weights[1] = subweight
	weights[2] = multweight
	weights[3] = divweight
	weights[4] = powweight
	weights[5] = modweight
	weights[6] = sinweight
	weights[7] = cosweight
	return 1.0
	
# define available functions
def sanity(left, right):
	if left == None or right == None:
		debug('Invalid expression tree state.')
def add(left, right):
	return left.evaluate() + right.evaluate()
def subtract(left, right):
	return left.evaluate() - right.evaluate()
def multiply(left, right):
	return left.evaluate() * right.evaluate()
def modulo(left, right):
	mod = int(right.evaluate())
	if mod > 0:
		return left.evaluate() % int(right.evaluate())
	else:
		return left.evaluate()
def divide(left, right):
	denom = right.evaluate()
	if denom != 0:
		return float(left.evaluate()) / denom
	else:
		return 0.0
def pow(left, right):
	left = left.evaluate()
	right = right.evaluate()
	if left < 0:
		right = int(right)
	return math.pow(left, right)
def sine(left, right):
	return math.sin(left.evaluate())
def cosine(left, right):
	return math.cos(right.evaluate())
functions = [(add, '+'), (subtract, '-'), (multiply, '*'), (divide, '/'), (pow, '^'), (modulo, '%'), (sine, 'sin'), (cosine, 'cos')]

# define special nodes
special = []
lfo = []
for i in range(1, LFORange + 1):
	w = ValueNode([i * 2.0 * math.pi])
	rad = FunctionNode((multiply, '*'), w, ValueNode(time))
	node = FunctionNode((sine, 'sin'), rad, None)
	lfo.append(node)
for i in range(1, numPartials + 1):
	freq = ValueNode(cps)
	w = FunctionNode((multiply, '*'), freq, ValueNode([i * 2.0 * math.pi]))
	rad = FunctionNode((multiply, '*'), w, ValueNode(time))
	node = FunctionNode((sine, 'sin'), rad, None)
	special.append(node)

# get a weighted random function
def getRandomFunction():
	totals = []
	running_total = 0.0
	
	for i in range(len(weights)):
		running_total += weights[i]
		totals.append(running_total)

	if running_total == 0:
		return random.choice(functions)
	else:
		rnd = random.random() * running_total
		for i, total in enumerate(totals):
			if rnd < total:
				return functions[i]

# initializes a population of size n with x evaluated at a time
# n must be greater than and a multiple of x
def initSynthPopulation(x):
	global populationSize
	populationSize = int(x)
	clear('debug.txt', '')
	#clear('error.txt', '')
	clear('networks.txt', '')
	#import sys
	#sys.stderr = open('error.txt', 'w')
	for i in range(populationSize):
		network = Network(special[0])
		networks.append(network)
		upForEvaluation.append(network)
	updateNetworkFile()

# evaluates the network and returns one sample
def produceSample(t, hz, networknum):
	time[0] = t
	cps[0] = hz
	return upForEvaluation[int(networknum)].evaluate()

# mark a particular network ID as worthy of selection
def select(zero, one, two, three, four):
	selections = [zero, one, two, three, four]
	for i in range(len(selections)):
		if selections[i] == 1.0:
			winners.add(upForEvaluation[i])
	return float(len(winners))

# go back a time step
def prevGeneration(dummy):
	global generationID
	global populationSize
	if (generationID < 1):
		return float(generationID)
	del upForEvaluation[:]
	del networks[-populationSize:]
	for i in range(populationSize):
		j = len(networks) - populationSize + i
		upForEvaluation.append(networks[j])

	evaluated.clear()
	winners.clear()
	generationID = generationID - 1
	updateNetworkFile()
	return float(generationID)

# go forward a time step
def nextGeneration(dummy):
	global generationID
	global mutationChance
	global crossChance
	global populationSize

	for network in upForEvaluation:
		evaluated.add(network)
	del upForEvaluation[:]

	eligible = winners
	if (len(winners) == 0):
		eligible = evaluated

	for i in range(populationSize):
		dad = random.sample(eligible, 1)[0]
		networkone = dad.copy()
		offspring = networkone

		if (random.random() < mutationChance):
			networkone.mutate()
		
		if (random.random() < crossChance and len(eligible) > 1):
			mom = random.sample(eligible, 1)[0]
			while id(dad) == id(mom):
				mom = random.sample(eligible, 1)[0]
			networktwo = mom.copy()
			if (random.random() < mutationChance):
				networktwo.mutate()
			offspring = networkone.inseminate(networktwo)
		
		networks.append(offspring)
		upForEvaluation.append(offspring)
	
	evaluated.clear()
	winners.clear()
	generationID = generationID + 1
	updateNetworkFile()
	return float(generationID)
	
def updateNetworkFile():
	global populationSize
	global generationID
	clear('networks.txt', '')
	output('networks.txt', 'CURRENT GENERATION (%d):' % generationID)
	for i in range(len(upForEvaluation)):
		output('networks.txt', '%d: %s' % (i + 1, upForEvaluation[i].toString()))
	gen = 0
	for i in range(len(networks)):
		if (i % populationSize) == 0:
			output('networks.txt', '')
			output('networks.txt', 'GENERATION %d:' % gen)
			gen = gen + 1
		output('networks.txt', '%d: %s' % ((i % 5) + 1, networks[i].toString()))
		
def output(path, string):
	f = open(path, 'a')
	f.write(string)
	f.write(os.linesep)
	f.close()
	
def debug(string):
	output('debug.txt', string)
	
def clear(path, string):
	f = open(path, 'w')
	f.write(string)
	f.close()

