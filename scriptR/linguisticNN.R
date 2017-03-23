require(abc)
library(abcrf)

wd = getwd()

rl = read.table("outSumStatsRealL.txt", sep="\t", header = F)
rl = rl[,1:(dim(rl)[2]-1)]
rl = rl[,1:(dim(rl)[2]-1)]

wdA = paste(wdA, "/scenarioA/", sep = "")
wd = paste(wdB, "/scenarioB/", sep = "")
wd = paste(wdC, "/scenarioC/", sep = "")
wd = paste(wdD, "/scenarioD/", sep = "")
wd = paste(wdE, "/scenarioE/", sep = "")

wdX = c(wdA, wdB, wdC, wdD, wdE)

load("choixModel")

##########################################
### LECTURE DES DOCUMENTS
##########################################

setwd(wdX[choixModel])

dlNN = read.table("outSumStatsL.txt", sep="\t", header = F)
dlNN = dlNN[1:100000,1:(dim(dlNN)[2]-1)]
dlNN = dlNN[1:100000,1:(dim(dlNN)[2]-1)]

pl = read.table("outParamL.txt", sep = "\t", header = T)
pl = pl[1:100000,]

##########################################
### SELECTION DE MODELE ET ESTIMATION 
##########################################

setwd(wd)

i = seq(1:dim(dlNN)[1])

### CONSTRUCTION DU VECTEUR DE PARAMETRES A ESTIMER

if ((choixModel == 1)||(choixModel == 3)) {
	plb = cbind(pl$MUTATION_1, pl$EVENT_0, pl$EVENT_1)
	colnames(plb) = c("mu", "t0", "t1")
	plc = cbind(pl$MUTATION_1*pl$EVENT_0, pl$MUTATION_1*pl$EVENT_1, pl$EVENT_1/pl$EVENT_0)
	colnames(plc) = c("t0*mu", "t1*mu", "t1/t0")
	plt = cbind(plb,plc)
		dimension = 3
}	

if ((choixModel == 2)||(choixModel == 4)) {
	plb = cbind(pl$MUTATION_1, pl$EVENT_0, pl$EVENT_1, pl$EMPRUNT)
	colnames(plb) = c("mu", "t0", "t1", "emprunt")
	plc = cbind(pl$MUTATION_1*pl$EVENT_0, pl$MUTATION_1*pl$EVENT_1, pl$EVENT_1/pl$EVENT_0, pl$EVENT_1*pl$EMPRUNT)
	colnames(plc) = c("t0*mu", "t1*mu", "t1/t0", "t1*emprunt")
	plt = cbind(plb,plc)
	dimension = 4
}
	
if (choixModel == 5) {
	plb = cbind(pl$MUTATION_1, pl$EVENT_0, pl$EVENT_1, pl$ADMIXTURE)
	colnames(plb) = c("mu", "t0", "t1", "admx")
	plc = cbind(pl$MUTATION_1*pl$EVENT_0, pl$MUTATION_1*pl$EVENT_1, pl$EVENT_1/pl$EVENT_0, pl$EVENT_1*pl$ADMIXTURE)
	colnames(plc) = c("t0*mu", "t1*mu", "t1/t0", "t1*admx")
	plt = cbind(plb,plc)
	dimension = 4
}

### CONSTRUCTION DES VARIABLES NECESSAIRES A L'ABC

min = c() ; max = c() ; logit = c()
for (i in 1:dim(plt)[2])
{
	min = c(min, min(plt[,i]))
	max = c(max, max(plt[,i]))
	logit = c(logit, "logit")
}
bound = matrix(c(min,max), nrow = dim(plt)[2])

#cv4abc_g = cv4abc(param = pgb, sumstat = dg, nval = 3, tols = c(0.1, 0.01), method = "neuralnet", transf = logit, logit.bound = bound, sizenet = dimension)

abc_l = abc(target = rl, param = plt, sumstat = dlNN, tol = 0.01, method = "neuralnet", transf = logit, logit.bound = bound, sizenet = dimension)

save(abc_l, plt, file="save_nn")
