require(abc)
library(abcrf)

wd = getwd()

rg = read.table("outSumStatsRealG.txt", sep="\t", header = T)
rg = rg[,1:(dim(rg)[2]-1)]

wdA1 = paste(wd, "/scenarioA/", sep = "")
wdA2 = paste(wd, "/scenarioB/", sep = "")
wdB1 = paste(wd, "/scenarioC/", sep = "")
wdB2 = paste(wd, "/scenarioD/", sep = "")
wdC = paste(wd, "/scenarioE/", sep = "")

wdX = c(wdA, wdB, wdC, wdD, wdE)

##########################################
### LECTURE DES DOCUMENTS
##########################################

setwd(wdA)

dgRF1 = read.table("outSumStatsG.txt", sep="\t", header = T)
dgRF1 = dgRF1[1:10000,1:(dim(dgRF1)[2]-1)]

setwd(wdB)

dgRF2 = read.table("outSumStatsG.txt", sep="\t", header = T)
dgRF2 = dgRF2[1:10000,1:(dim(dgRF2)[2]-1)]

setwd(wdC)

dgRF3 = read.table("outSumStatsG.txt", sep="\t", header = T)
dgRF3 = dgRF3[1:10000,1:(dim(dgRF3)[2]-1)]

setwd(wdD)

dgRF4 = read.table("outSumStatsG.txt", sep="\t", header = T)
dgRF4 = dgRF4[1:10000,1:(dim(dgRF4)[2]-1)]

setwd(wdE)

dgRF5 = read.table("outSumStatsG.txt", sep="\t", header = T)
dgRF5 = dgRF5[1:10000,1:(dim(dgRF5)[2]-1)]

setwd(wd)

dgRF = rbind(dgRF1, dgRF2, dgRF3, dgRF4, dgRF5)

##########################################
### SELECTION DE SCENARIO PAR RANDOM FOREST
##########################################

setwd(wd)

i = seq(1:dim(dgRF1)[1])
i2 = seq(1:dim(dgRF2)[1])
i3 = seq(1:dim(dgRF3)[1])
i4 = seq(1:dim(dgRF4)[1])
i5 = seq(1:dim(dgRF5)[1])
ind = i/i
ind2 = 2*i2/i2
ind3 = 3*i3/i3
ind4 = 4*i4/i4
ind5 = 5*i5/i5
i = as.factor(as.vector(c(ind,ind2,ind3,ind4,ind5)))

### VALIDATION CROISEE POUR LA SELECTION DE SCENARIO

abcrf_g = abcrf(modindex = i, sumsta = dgRF, lda = TRUE, paral = FALSE)
#err_abcrf_g = err.abcrf(abcrf_g) # Très long par rapport au reste, et uniquement erreur a pripori

### CHOIX DE SCENARIO

pred_g = predict(abcrf_g, rg, paral = FALSE) # Prend tout les processeurs si paral = TRUE

### FORMATAGE ET ECRITURE DES DONNEES DE SORTIE

M = abcrf_g$model.rf$confusion[,1:5]

#for (i in 1:5)
#{
#	M[i,] = M[i,]/sum(M[i,])
#}

choixModel = which(summary(pred_g) == 1)[[1]]

sink("choixModel.txt")
print(choixModel)
sink()

save(abcrf_g, pred_g, choixModel, file="save_rf")
save(choixModel, file="choixModel")
