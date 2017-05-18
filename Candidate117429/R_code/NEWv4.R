Data<-read.csv("/Users/AliceWork/Documents/Dissertation/ProbSet4.csv", header=T)
SortedData1 <- read.csv("/Users/AliceWork/Documents/Dissertation/SortedProbSet4.csv", header=T)

hist(Data$ProbH, breaks=50, xlim=c(0,1), col="red", main="Probability of correct Classification (100,000)", xlab="Probability of correct Classification")
hist(Data$ProbZ, breaks=50, add=T, col=rgb(0, 0, 1, 0.5))
legend("top", c("H", "Z"), col=c("red", rgb(0, 0, 1, 0.5)), lwd=5)


#my.bin.width<-0.01
#breaks=seq(-35611,38708,by=my.bin.width)
#hist(SortedData1$ProbH, breaks=50, xlim=c(0,1), col="red", main="Probability of correct H Classification (100,000)", xlab="Probability of correct H Classification")
#hist(SortedData1$ProbZ, breaks=50, add=T, col=rgb(0, 0, 1, 0.5))
#legend("top", c("H", "Z"), col=c("red", rgb(0, 0, 1, 0.5)), lwd=5)