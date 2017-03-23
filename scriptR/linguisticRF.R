require(abc)
library(abcrf)

wd = getwd()

rl = read.table("outSumStatsRealL.txt", sep="\t", header = F)
rl = rl[,1:(dim(rl)[2]-1)]
rl = rl[,1:(dim(rl)[2]-1)]

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

dl = read.table("outSumStatsL.txt", sep="\t", header = F)
dl = dl[,1:(dim(dl)[2]-1)]
dl = dl[1:10000,1:(dim(dl)[2]-1)]

dlRF1 = dl

setwd(wdB)

dl = read.table("outSumStatsL.txt", sep="\t", header = F)
dl = dl[1:10000,1:(dim(dl)[2]-1)]
dl = dl[,1:(dim(dl)[2]-1)]

dlRF2 = dl

setwd(wdC)

dl = read.table("outSumStatsL.txt", sep="\t", header = F)
dl = dl[1:10000,1:(dim(dl)[2]-1)]
dl = dl[,1:(dim(dl)[2]-1)]

dlRF3 = dl

setwd(wdD)

dl = read.table("outSumStatsL.txt", sep="\t", header = F)
dl = dl[1:10000,1:(dim(dl)[2]-1)]
dl = dl[,1:(dim(dl)[2]-1)]

dlRF4 = dl

setwd(wdE)

dl = read.table("outSumStatsL.txt", sep="\t", header = F)
dl = dl[1:10000,1:(dim(dl)[2]-1)]
dl = dl[,1:(dim(dl)[2]-1)]

dlRF5 = dl


dlRF = rbind(dlRF1, dlRF2, dlRF3, dlRF4, dlRF5)

##########################################
### SELECTION DE MODELE ET ESTIMATION 
##########################################

setwd(wd)

i = seq(1:dim(dlRF1)[1])
i2 = seq(1:dim(dlRF2)[1])
i3 = seq(1:dim(dlRF3)[1])
i4 = seq(1:dim(dlRF4)[1])
i5 = seq(1:dim(dlRF5)[1])
ind = i/i
ind2 = 2*i2/i2
ind3 = 3*i3/i3
ind4 = 4*i4/i4
ind5 = 5*i5/i5
i = as.factor(as.vector(c(ind,ind2,ind3,ind4,ind5)))


### VALIDATION CROISEE POUR LA SELECTION DE MODELE

abcrf_l = abcrf(modindex = i, sumsta = dlRF, lda = TRUE, paral = FALSE)
#err_abcrf_g = err.abcrf(abcrf_g) # Très long par rapport au reste, et uniquement erreur a pripori

### CHOIX DE SCENARIO

pred_l = predict(abcrf_l, rl, paral = FALSE) # Prend tout les processeurs si paral = TRUE

### FORMATAGE ET ECRITURE DES DONNEES DE SORTIE

M = abcrf_l$model.rf$confusion[,1:5]

#for (i in 1:5)
#{
#	M[i,] = M[i,]/sum(M[i,])
#}

choixModel = which(summary(pred_l) == 1)[[1]]

sink("choixModel.txt")
print(choixModel)
sink()

save(abcrf_l, pred_l, choixModel, file="save_rf")
save(choixModel, file="choixModel")
