<Cabbage>
form size(600, 520), caption("evolutionary synthesizer"), pluginID("plu1")

image bounds(0, 8, 600, 50), file("sine.png")

groupbox text("test population"), bounds(10, 65, 580, 95), plant("test") {
hslider bounds(.02, .2, .62, .45), text("synth #"), channel("ins"), tracker(128, 128, 255), colour(128, 128, 255), range(1, 5, 1, 1, 1), textBox(1)
hslider bounds(.02, .55, .62, .45), text("gain"), channel("gain"), tracker(128, 128, 255), colour(128, 128, 255), range(0, 1, 0.2), textBox(1)
infobutton bounds(.69, .33, .30, .6), text("networks"), file("networks.txt")
}

groupbox text("selection"), bounds(10, 170, 380, 100), plant("selections") {
checkbox bounds(.02, .25, .17, .7), colour(29, 172, 29), channel("one")
checkbox bounds(.22, .25, .17, .7), colour(29, 172, 29), channel("two")
checkbox bounds(.42, .25, .17, .7), colour(29, 172, 29), channel("three")
checkbox bounds(.62, .25, .17, .7), colour(29, 172, 29), channel("four")
checkbox bounds(.82, .25, .17, .7), colour(29, 172, 29), channel("five")
}

groupbox text("generation control"), bounds(10, 280, 380, 75), plant("buttons") {
button bounds(.02, .35, .48, .59), channel("prev"), value(0), text("previous", "previous")
button bounds(.52, .35, .48, .59), channel("next"), value(0), text("next", "next")
}

groupbox text("evolution parameters"), bounds(10, 365, 380, 125), plant("params") {
hslider bounds(.02, .19, .96, .13), tracker(29, 172, 29), colour(29, 172, 29), text("mutate"), channel("mutationRate"), range(0, 1, 1.0), textBox(1)
hslider bounds(.02, .32, .96, .13), tracker(29, 172, 29), colour(29, 172, 29), text("simp/comp"), channel("simplifyMutationRate"), range(0, 1, 0.0), textBox(1)
hslider bounds(.02, .45, .96, .13), tracker(29, 172, 29), colour(29, 172, 29), text("spcl/nrml"), channel("specialMutationRate"), range(0, 1, 0.5), textBox(1)
hslider bounds(.02, .58, .96, .13), tracker(29, 172, 29), colour(29, 172, 29), text("hrmn/mdltn"), channel("harmonyNodeRate"), range(0, 1, 0.5), textBox(1)
hslider bounds(.02, .71, .96, .13), tracker(29, 172, 29), colour(29, 172, 29), text("fnc/val"), channel("functionMutationRate"), range(0, 1, 0.5), textBox(1)
hslider bounds(.02, .84, .96, .13), tracker(29, 172, 29), colour(29, 172, 29), text("cross"), channel("crossoverRate"), range(0, 1, 0.2), textBox(1)
}

groupbox text("function likelihood weights"), bounds(400, 170, 190, 320), plant("weights") {
vslider bounds(.03, .08, .11, .89), tracker(29, 172, 29), colour(29, 172, 29), text("+"), channel("addWeight"), range(0, 1, 0.5), textBox(1)
vslider bounds(.15, .08, .11, .89), tracker(29, 172, 29), colour(29, 172, 29), text("-"), channel("subWeight"), range(0, 1, 0.5), textBox(1)
vslider bounds(.27, .08, .11, .89), tracker(29, 172, 29), colour(29, 172, 29), text("*"), channel("multWeight"), range(0, 1, 0.5), textBox(1)
vslider bounds(.39, .08, .11, .89), tracker(29, 172, 29), colour(29, 172, 29), text("/"), channel("divWeight"), range(0, 1, 0.0), textBox(1)
vslider bounds(.51, .08, .11, .89), tracker(29, 172, 29), colour(29, 172, 29), text("^"), channel("powWeight"), range(0, 1, 0.0), textBox(1)
vslider bounds(.63, .08, .11, .89), tracker(29, 172, 29), colour(29, 172, 29), text("%"), channel("modWeight"), range(0, 1, 0.0), textBox(1)
vslider bounds(.75, .08, .11, .89), tracker(29, 172, 29), colour(29, 172, 29), text("sin"), channel("sinWeight"), range(0, 1, 0.5), textBox(1)
vslider bounds(.87, .08, .11, .89), tracker(29, 172, 29), colour(29, 172, 29), text("cos"), channel("cosWeight"), range(0, 1, 0.0), textBox(1)
}

;keyboard bounds(10, 500, 580, 75)

</Cabbage>
<CsoundSynthesizer>
<CsOptions>
-n -d -+rtmidi=NULL -M0 --midi-key-cps=5 --midi-velocity-amp=4
</CsOptions>
<CsInstruments>
	;AUTHOR: CHRIS DONAHUE

	; ######
	; SET UP
	; ######
	; set up Csound env
	sr	=	44100
	ksmps	=	5
	nchnls	=	2
	0dbfs	=	1
	
	; use playNetwork instrument for test
	massign	0, "playNetwork"
	
	; set up globals for daemon logic
	gkadvp	init	0
	gkprevp	init	0

	; initialize python and run main GP script
	pyinit
	#include "gpimport.py"
	pycalli "initSynthPopulation", 5

	; ######
	; DAEMON
	; ######
	; daemon instrument to monitor for button press
		instr	daemon
	kadv	chnget "next"
	kprev	chnget "prev"

	; check for clicked "next"
next:	if (kadv == gkadvp) goto prev
	; get evolution params
	kmut	chnget "mutationRate"
	ksimp	chnget "simplifyMutationRate"
	kspec	chnget "specialMutationRate"
	kharm	chnget "harmonyNodeRate"
	kfunc	chnget "functionMutationRate"
	kcross	chnget "crossoverRate"

	; get synth selections
	kone	chnget "one"
	ktwo	chnget "two"
	kthree	chnget "three"
	kfour	chnget "four"
	kfive	chnget "five"

	; get function weights
	kadd	chnget	"addWeight"
	ksub	chnget	"subWeight"
	kmult	chnget	"multWeight"
	kdiv	chnget	"divWeight"
	kpow	chnget	"powWeight"
	kmod	chnget	"modWeight"
	ksin	chnget	"sinWeight"
	kcos	chnget	"cosWeight"

	; update everything and increment generation
		event "i", "select", 0, 0.01, kone, ktwo, kthree, kfour, kfive
		event "i", "updateParams", 0.01, 0.01, kmut, ksimp, kspec, kharm, kfunc, kcross
		event "i", "updateWeights", 0.02, 0.01, kadd, ksub, kmult, kdiv, kpow, kmod, ksin, kcos
		event "i", "nextGen", 0.03, 0.3
	gkadvp	=	kadv

	; check for clicked "prev"
prev:	if (kprev == gkprevp) goto end
	; decerement generation
		event "i", "prevGen", 0, 0.1
	gkprevp	=	kprev
	
end:	; dummy label
		endin

	; ########
	; PLAYBACK
	; ########
	; instrument to playback produced networks
		instr playNetwork
	ivel	=	p4
	icps	=	p5

	; get synth # and gain, synth # 1-5 becomes 0-4
	knet	chnget "ins"
	knet	=	knet - 1
	igain	chnget "gain"

	kcount	init	0.0

	; set the time and frequency variables
	ktime	=	(kcount / kr)
	kcps	=	icps

	; call python
	ksound	pycall1	"produceSample", ktime, kcps, knet

	; gate using linenr
	kenv	linenr	igain, 0.01, 0.1, 0.01

	; interpolate the python samples to a-rate and apply env
	asig	interp	ksound
	asig	=	asig * kenv
	kcount	=	kcount + 1
		outs	asig, asig
		endin

	; ##########################
	; PYTHON CONTROL INSTRUMENTS
	; ##########################
	; selection instrument
		instr	select
	ione	=	p4
	itwo	=	p5
	ithree	=	p6
	ifour	=	p7
	ifive	=	p8
	isel	pycall1i "select", ione, itwo, ithree, ifour, ifive
		chnset 0, "one"
		chnset 0, "two"
		chnset 0, "three"
		chnset 0, "four"
		chnset 0, "five"
		print	isel
		endin

	; generation increment instrument
		instr	nextGen
	ires	pycall1i "nextGeneration", 0
		print	ires
		endin

	; generation decrement instrument
		instr 	prevGen
	ires	pycall1i "prevGeneration", 0
		print	ires
		endin

	; evolution parameter update instrument
		instr	updateParams
	ione	=	p4
	itwo	=	p5
	ithree	=	p6
	ifour	=	p7
	ifive	=	p8
	isix	=	p9
	ires	pycall1i "setParameters", ione, itwo, ithree, ifour, ifive, isix
		print	ires
		endin

	; function weight update instrument
		instr	updateWeights
	ione	=	p4
	itwo	=	p5
	ithree	=	p6
	ifour	=	p7
	ifive	=	p8
	isix	=	p9
	iseven	=	p10
	ieight	=	p11
	ires	pycall1i "setFunctionChances", ione, itwo, ithree, ifour, ifive, isix, iseven, ieight
		print 	ires
		endin

</CsInstruments>  
<CsScore>
i "daemon" 0 [60 * 60 * 24]
</CsScore>
</CsoundSynthesizer>