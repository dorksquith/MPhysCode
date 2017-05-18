#All variables

#Gradient boosted decision tree
import numpy as np
import scipy
from sklearn.ensemble import GradientBoostingClassifier
import csv
import pandas
import matplotlib.pyplot as plt

#Converting Data from CSV to a (100000, 13) array (50,000 b and bbar) 
inputH = np.genfromtxt(open("50000HVariable.csv","rb"),delimiter=",", skip_header=1)
#Deletes first column of b/bbar and mother mass
a = np.delete(inputH, [0], 1)

#extra column to hold label 
A=np.append(a,np.zeros([len(a),1]),1)
for i in range(0, len(A)):
	A[i][13]=1		

inputZ = np.genfromtxt(open("50000ZVariable.csv","rb"),delimiter=",", skip_header=1)
#Deletes first column of b/bbar
b = np.delete(inputZ, [0], 1)

#extra column to hold label 
B = np.append(b,np.zeros([len(b),1]),1)
for j in range(0, len(B)):
	B[j][13]=0

Data=np.vstack((A,B))

#Separating data into teaching and learning
# X array = training samples, size [n_samples, n_features]
# Y array = class labels, size [n_samples]

np.random.seed(1)
r=np.random.rand(Data.shape[0])
# cutoff is 0.9 so it will use 90% of th data for training, 10% for validation.
cutoff=0.9

Y_train = Data[:,13][r<cutoff]
X_train = Data[:,0:12][r<cutoff]

Y_test = Data[:,13][r>=cutoff]
X_test = Data[:,0:12][r>=cutoff]


#clf = GradientBoostingClassifier(n_estimators=100, max_depth=1, random_state=0).fit(X_train, Y_train)
clf = GradientBoostingClassifier(n_estimators=50 , max_depth = 5, min_samples_leaf=200,max_features=10,verbose=1)
#n_estimator=The number of boosting stages to perform, robust to over-fitting, large number better performance.
#max_depth= limits the number of nodes in the tree, depends on the interaction of input variables.
#min_samples_leaf= minimum number of samples required to be at a leaf node.
#max_features= number of features to consider when looking for the best split
#verbose=Enable verbose output, if 1 prints progress & performance once in a while.


clf.fit(X_train, Y_train)
Prediction=clf.predict(X_test)
PredictProb=clf.predict_proba(X_test)
Comparison= np.vstack((Y_test,Prediction))
print(PredictProb)
print(Comparison)

HZ2=np.zeros((len(PredictProb), 2))
#Prob of higgs only
for a in range(0, len(PredictProb)):
	if PredictProb[a][0] > PredictProb[a][1]:
	 	HZ2[a][0]=1
	 	HZ2[a][1]= PredictProb[a][0]
	if PredictProb[a][0] <= PredictProb[a][1]:
	 	HZ2[a][0]=0
	 	HZ2[a][1]= PredictProb[a][0]

correct=0
correctprob=0
HiggsWrong=0
HiggsN=0
ZWrong=0
ZN=0

#shape=Comparison.shape
#print (shape)
#print (Comparison)

for b in range(0, 20000):
	if Comparison[0][b]==0:
		ZN+=1
	if Comparison[1][b]==1:	
		HiggsN+=1

for c in range(0, 20000):
	if Comparison[0][c]==0 and Comparison[1][c]==1 :
		ZWrong+=1
	if Comparison[0][c]==1 and Comparison[1][c]==0 :
		HiggsWrong+=1
	if Comparison[0][c]==Comparison[1][c]:
		correct+=1
		
correctprob=correct/20000
correctZ=1-(ZWrong/ZN)
correctH=1-(HiggsWrong/HiggsN)

print("ZWrong")
print(ZWrong)
print("HiggsWrong")
print(HiggsWrong)
print ("correct")
print (correct)
print ("len(Comparison)")
print (len(Comparison))
print ("correctprob")
print (correctprob)
print ("correctZ")
print (correctZ)
print ("correctH")
print (correctH)



csv=open("ProbSet1.csv","w")
csv.write('ProbH, ProbZ \n')
for line in PredictProb:
	theline=str(line[0])+","+str(line[1])+"\n"
	csv.write(theline)
csv.close()

'''
csv=open("SortedProbSet1.csv","w")
csv.write('Class, ProbH \n')
for line in HZ2:
	theline=str(line[0])+","+str(line[1])+"\n"
	csv.write(theline)
csv.close()
'''


 






	