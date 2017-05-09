import numpy as np
import csv
#import pandas as pd
from ROOT import TLorentzVector
from sklearn.ensemble import GradientBoostingClassifier as GBC
from sklearn.model_selection import train_test_split
from matplotlib import pyplot as plt


#Now loading in training data
training_data=np.loadtxt('training.csv',delimiter=",")
labelled_data=training_data[:,0:7] 

#Splitting the data
x = labelled_data
y = training_data[:,7]
X_train, X_test, y_train, y_test = train_test_split(x, y, test_size=0.5,random_state=9)

#Checking actual number of signal/background events in testing
number_sig=[]
number_back=[]
for i in range(len(y_test)):
    if (y_test[i]==1):
        number_sig.append(1)
    if(y_test[i]==2):
        number_back.append(1)
        
print len(number_sig)
print len(number_back)
        
#Training the GBC
classifier=GBC(n_estimators=100,max_depth=5,min_samples_leaf=100)
classifier.fit(X_train,y_train)

test_index=range(len(y_test))

# get the background probability only
pred_training=classifier.predict_proba(X_train)[:,1]# subset of training_data
pred_testing=classifier.predict_proba(X_test)[:,1] # testing data, ie without the labels

#this tells us what cut on probability we need to make to keep 50% of the background
pcut50 = np.percentile(pred_testing,50)


#list of events with probability great than pcut50
test_labels=list(pred_testing>pcut50)

# just turn background probability into a list
pred_test=list(pred_testing)
results=[]

for i in range(len(test_index)):
    # append(identifier, background probabilty, print either s or b)
    #print "test_labels[i]: ",test_labels[i]
    results.append([int(test_index[i]), pred_test[i], 's'*int((test_labels[i]==1.0))+'b'*int((test_labels[i]==0.0))]) 

#Sort list by probability prediction

results = sorted(results, key=lambda a_entry: a_entry[1])

csv=open("Predictions.csv","w")
csv.write('Number index,Probability,Classification,\n')
for line in results:
    theline=str(line[0])+","+str(line[1])+","+line[2]+"\n"
    csv.write(theline)
csv.close()

#When all variables used background estimate 27250 - reality 25106



#Plot
#print "testing[training<1.5]: ",X_train[y_train<1.5]
Classifier_training_S = classifier.predict_proba(X_train[y_train<1.5])[:,1] # signal
#print "testing[training>1.5]: ",X_train[y_train>1.5]
Classifier_training_B = classifier.predict_proba(X_train[y_train>1.5])[:,1] # background

Classifier_testing_A = classifier.predict_proba(X_test)[:,1]


c_max = max([Classifier_training_S.max(),Classifier_training_B.max(),Classifier_testing_A.max()])
c_min = min([Classifier_training_S.min(),Classifier_training_B.min(),Classifier_testing_A.min()])
  
# Get histograms of the classifiers
Hist_training_S = np.histogram(Classifier_training_S,bins=50,range=(c_min,c_max))
Hist_training_B = np.histogram(Classifier_training_B,bins=50,range=(c_min,c_max))
Hist_testing_A = np.histogram(Classifier_testing_A,bins=50,range=(c_min,c_max))
  
# Lets get the min/max of the Histograms
AllHists= [Hist_training_S,Hist_training_B]
h_max = max([hist[0].max() for hist in AllHists])*1.2
# h_min = max([histo[0].min() for hist in AllHists])
h_min = 1.0
  
# Get the histogram properties (binning, widths, centers)
bin_edges = Hist_training_S[1]
bin_centers = ( bin_edges[:-1] + bin_edges[1:]  ) /2.
bin_widths = (bin_edges[1:] - bin_edges[:-1])
  
# To make error bar plots for the data, take the Poisson uncertainty sqrt(N)
ErrorBar_testing_A = np.sqrt(Hist_testing_A[0])
# ErrorBar_testing_B = np.sqrt(Hist_testing_B[0])
  
# Draw objects
ax1 = plt.subplot(111)
  
# Draw solid histograms for the training data
ax1.bar(bin_centers-bin_widths/2.,Hist_training_B[0],facecolor='red',linewidth=0,width=bin_widths,label='B (Train)',alpha=0.5)
ax1.bar(bin_centers-bin_widths/2.,Hist_training_S[0],bottom=Hist_training_B[0],facecolor='blue',linewidth=0,width=bin_widths,label='S (Train)',alpha=0.5)
 
ff = (1.0*(sum(Hist_training_S[0])+sum(Hist_training_B[0])))/(1.0*sum(Hist_testing_A[0]))
 
# # Draw error-bar histograms for the testing data
ax1.errorbar(bin_centers, ff*Hist_testing_A[0], yerr=ff*ErrorBar_testing_A, xerr=None, ecolor='black',c='black',fmt='.',label='Test data')
# ax1.errorbar(bin_centers, Hist_testing_B[0], yerr=ErrorBar_testing_B, xerr=None, ecolor='red',c='red',fmt='o',label='B (Test)')
  
  
# Adjust the axis boundaries (just cosmetic)
ax1.axis([c_min, c_max, h_min, h_max])
  
# Make labels and title
plt.title("Higgs Signal-Background Classifier")
plt.xlabel("Probability Output")
plt.ylabel("Counts/Bin")
 
# Make legend with small font
legend = ax1.legend(loc='upper center', shadow=True,ncol=2)
for alabel in legend.get_texts():
            alabel.set_fontsize('small')
  
# Save the result to png
plt.savefig("classifier_gbc.png")

