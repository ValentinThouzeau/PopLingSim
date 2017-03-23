require(abc)
library(abcrf)

wd = getwd()

rg = read.table("outSumStatsRealG.txt", sep="\t", header = T)
rg = rg[,1:(dim(rg)[2]-1)]

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

dgNN = read.table("outSumStatsG.txt", sep="\t", header = T)
dgNN = dgNN[1:100000,1:(dim(dgNN)[2]-1)]

pg = read.table("outParamG.txt", sep = "\t", header= T)
pg = pg[1:100000,]

##########################################
### ESTIMATION DE PARAMETRE PAR NEURAL NETWORK
##########################################

setwd(wd)

i = seq(1:dim(dgNN)[1])

### CALCUL DU TAUX DE MUTATION MOYEN

vecteurMutation = grep("MUTATION_[0-9][0-9]" , labels(pg)[[2]], value = T)
mutationAuto = c(0)
for (i in 1:length(vecteurMutation)) 
{
	mutationAuto = mutationAuto + (pg[labels(pg)[[2]] == vecteurMutation[i]])/length(vecteurMutation)
}

### CONSTRUCTION DU VECTEUR DE PARAMETRES A ESTIMER

if ((choixModel == 1)||(choixModel == 3)) {
	pgb = cbind(pg$NE_0, pg$NE_1, pg$NE_2, pg$NE_3, pg$NE_4, mutationAuto, pg$EVENT_0, pg$EVENT_1, pg$NE_0*mutationAuto*4, pg$NE_1*mutationAuto*4, pg$NE_2*mutationAuto*4, pg$NE_3*mutationAuto*4, pg$NE_4*mutationAuto*4, pg$EVENT_0*mutationAuto, pg$EVENT_1*mutationAuto, pg$EVENT_1/pg$EVENT_0)
	colnames(pgb) = c("NE_0", "NE_1", "NE_2", "NE_3", "NE_4", "muAuto", "t0", "t1", "4*NE_0*mu", "4*NE_1*mu", "4*NE_2*mu", "4*NE_3*mu", "4*NE_4*mu", "t0*mu", "t1*mu", "t1/t0")
	dimension = 6
}

if ((choixModel == 2)||(choixModel == 4)) {
	pgb = cbind(pg$NE_0, pg$NE_1, pg$NE_2, pg$NE_3, pg$NE_4, mutationAuto, pg$EVENT_0, pg$EVENT_1, pg$MIGRATION, pg$NE_0*mutationAuto*4, pg$NE_1*mutationAuto*4, pg$NE_2*mutationAuto*4, pg$NE_3*mutationAuto*4, pg$NE_4*mutationAuto*4, pg$EVENT_0*mutationAuto, pg$EVENT_1*mutationAuto, pg$EVENT_1/pg$EVENT_0, pg$EVENT_1*pg$MIGRATION) #, pg$MIGRATION/(pg$EVENT_1*mutationAuto))
	colnames(pgb) = c("NE_0", "NE_1", "NE_2", "NE_3", "NE_4", "muAuto", "t0", "t1", "mig", "4*NE_0*mu", "4*NE_1*mu", "4*NE_2*mu", "4*NE_3*mu", "4*NE_4*mu", "t0*mu", "t1*mu", "t1/t0", "t1*mig") #, "mig/(t1*mu)")
	dimension = 7
}

if (choixModel == 5) {
	pgb = cbind(pg$NE_0, pg$NE_1, pg$NE_2, pg$NE_3, pg$NE_4, mutationAuto, pg$EVENT_0, pg$EVENT_1, pg$ADMIXTURE_1, pg$NE_0*mutationAuto*4, pg$NE_1*mutationAuto*4, pg$NE_2*mutationAuto*4, pg$NE_3*mutationAuto*4, pg$NE_4*mutationAuto*4, pg$EVENT_0*mutationAuto, pg$EVENT_1*mutationAuto, pg$EVENT_1/pg$EVENT_0, pg$EVENT_1*pg$ADMIXTURE_1) #, pg$ADMIXTURE_1/(pg$EVENT_1*mutationAuto))
	colnames(pgb) = c("NE_0", "NE_1", "NE_2", "NE_3", "NE_4", "muAuto", "t0", "t1", "admx", "4*NE_0*mu", "4*NE_1*mu", "4*NE_2*mu", "4*NE_3*mu", "4*NE_4*mu", "t0*mu", "t1*mu", "t1/t0", "t1*admx")#, "admx/(t1*mu)")
	dimension = 7
}

### CONSTRUCTION DES VARIABLES NECESSAIRES A L'ABC

min = c() ; max = c() ; logit = c()
for (i in 1:dim(pgb)[2])
{
	min = c(min, min(pgb[,i]))
	max = c(max, max(pgb[,i]))
	logit = c(logit, "logit")
}
bound = matrix(c(min,max), nrow = dim(pgb)[2])

#cv4abc_g = cv4abc(param = pgb, sumstat = dg, nval = 3, tols = c(0.1, 0.01), method = "neuralnet", transf = logit, logit.bound = bound, sizenet = dimension)

abc_g = abc(target = rg, param = pgb, sumstat = dgNN, tol = 0.01, method = "neuralnet", transf = logit, logit.bound = bound, sizenet = dimension)

save(abc_g, pgb, file="save_nn")
