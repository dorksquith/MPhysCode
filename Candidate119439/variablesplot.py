import numpy as np
import csv
import pandas as pd
import matplotlib.pyplot as plt
import pylab
import ROOT
from ROOT import TLorentzVector
from math import pi,fabs



plt.close('all')
#Signal events
number_signal=0
number_background=0
number_bsig=0
number_bbarsig=0


combined = []
bmass=[]
blist=[]
bbarlist=[]
sigmass=[]
number_h=0
with open('Signal_50k.csv','rU') as f:
    reader=csv.reader(f)
    #This for loop only stores 5 or -5 and ignores the 5 particle row after each <event>
    for row in reader:
            if row[1]=='5' or row[1]=='-5':
                # this is a b
                px = float(row[10])
                py = float(row[11])
                pz = float(row[12])
                e  = float(row[13])
                if(px==0): #don't want px,py etc to be 0, so avoids this
                    continue
                #print "row: ",row
                b = TLorentzVector(px,py,pz,e)
                bbar=TLorentzVector(px,py,pz,e)
                combined.append(b)
                #Starts from 1 to avoid the space at beginning of row and skips last unknown thing plus spin which all zero
                number_signal+=1
            if row[1]=='5':
                blist.append(b)
                number_bsig+=1
                
            if row[1]=='-5':
                bbarlist.append(bbar) 
                number_bbarsig+=1   
                
            if row[1]=='25':
                px = float(row[10])
                py = float(row[11])
                pz = float(row[12])
                e  = float(row[13])
                h=TLorentzVector(px,py,pz,e)
                sigmass.append(h)
                number_h+=1
            
      
#Same process for background events
combined2 = []
b2list=[]
bbar2list=[]
number_bback=0
number_bbarback=0


with open('Background_25k.csv','rU') as f:
    reader=csv.reader(f)
    #This for loop only stores 5 or -5 and ignores the 5 particle row after each <event>
    for row in reader:
            if row[1]=='5' or row[1]=='-5':
                px2 = float(row[10])
                py2 = float(row[11])
                pz2 = float(row[12])
                e2 = float(row[13])
                if(px==0):
                    continue
                b2=TLorentzVector(px2,py2,pz2,e2)
                bbar2=TLorentzVector(px,py,pz,e)
                #Starts from 1 to avoid the space at beginning of row and skips last unknown thing plus spin which all zero
                combined2.append(b2)
                number_background+=1
           
            if row[1]=='5':
                b2list.append(b2)
                number_bback+=1
                
            if row[1]=='-5':
                bbar2list.append(bbar2)     
                number_bbarback+=1
                

             
                 
px=[]
py=[]
pz=[]
e=[]
pt=[]
eta=[]
phi=[]
pT=[]
etab=[]
etabbar=[]
phib=[]
phibbar=[]
hmass=[]

for i in range(number_signal):
    px.append(combined[i].Px())
    py.append(combined[i].Py())
    pz.append(combined[i].Pz())
    e.append(combined[i].E())
    eta.append(combined[i].Eta())
    phi.append(combined[i].Phi())
    pT.append(combined[i].Pt())

for i in range(number_h):
    hmass.append(sigmass[i].M()) 

deta=[]
dphi=[]
dR=[]

for i in range(number_bbarsig):  
    etab.append(blist[i].Eta())
    etabbar.append(bbarlist[i].Eta())
    phib.append(blist[i].Phi())
    phibbar.append(bbarlist[i].Phi())
    
    
    deta.append(fabs(etab[i]-etabbar[i]))
    dphi.append(fabs(phib[i]-phibbar[i]))
    if (dphi[i]>pi):
        dphi[i]=fabs(dphi[i]-2*pi)
        
    dR.append(((deta[i]*deta[i])+(dphi[i]*dphi[i]))**0.5)  
    

px2=[]
py2=[]
pz2=[]
e2=[]
pt2=[]
eta2=[]
phi2=[]
pT2=[]
etab2=[]
etabbar2=[]
phib2=[]
phibbar2=[]
deta2=[]
dphi2=[]
dR2=[]
mbb=[]

for i in range(number_background):
    px2.append(combined2[i].Px())
    py2.append(combined2[i].Py())
    pz2.append(combined2[i].Pz())
    e2.append(combined2[i].E())
    eta2.append(combined2[i].Eta())
    phi2.append(combined2[i].Phi())
    pT2.append(combined2[i].Pt())  
   

for i in range(number_bbarback):  
    etab2.append(b2list[i].Eta())
    etabbar2.append(bbar2list[i].Eta())
    phib2.append(b2list[i].Phi())
    phibbar2.append(bbar2list[i].Phi())
    deta2.append(fabs(etab2[i]-etabbar2[i]))
    dphi2.append(fabs(phib2[i]-phibbar2[i]))
    if (dphi2[i]>pi):
        dphi2[i]=fabs(dphi2[i]-2*pi)
    dR2.append(((deta2[i]*deta2[i])+(dphi2[i]*dphi2[i]))**0.5)
    mbb.append((b2list[i]+bbar2list[i]).M()) 


#Creating lists for combined csv file to be used in machine learning
biglist=[[px[i],py[i],pz[i],e[i],pT[i],eta[i],phi[i],1] for i in range(len(px))]
biglist2=[[px2[i],py2[i],pz2[i],e2[i],pT2[i],eta2[i],phi2[i],2] for i in range(len(px2))]

testlist=[[px[i],py[i],pz[i],e[i],pT[i],eta[i],phi[i]] for i in range(len(px))]
testlist2=[[px2[i],py2[i],pz2[i],e2[i],pT2[i],eta2[i],phi2[i]] for i in range(len(px2))]

lists=[biglist,biglist2]
bigcombined=[item for sublist in lists for item in sublist]

lists2=[testlist,testlist2]
bigcombined2=[item for sublist in lists2 for item in sublist]

#writes training file of data for machine learning
csv=open("training.csv","w")
for line in bigcombined:
    theline=str(line[0])+","+str(line[1])+","+str(line[2])+","+str(line[3])+","+str(line[4])+","+str(line[5])+","+str(line[6])+","+str(line[7])+"\n"
    csv.write(theline)
csv.close()

#Histograms for the variables
plt.subplots(1)
plt.hist(px,bins=50, color='green', label='Signal', normed=True, range=[-200,200],alpha=0.5)#signal b
plt.hist(px2,bins=50, color='blue', label='Background', normed=True,range=[-200,200],alpha=0.5)#background
plt.ylabel('Number of events')
plt.xlabel('p_x GeV')
plt.title('p_x')
pylab.legend(loc='upper right')
plt.savefig('p_x.png')


plt.subplots(1)
plt.hist(py,bins=50, color='green', label='Signal',normed=True, range=[-200,200],alpha=0.5)#signal b
plt.hist(py2,bins=50, color='blue', label='Background',normed=True, range=[-200,200],alpha=0.5)#background
plt.ylabel('Number of events')
plt.xlabel('p_y GeV')
plt.title('p_y')
pylab.legend(loc='upper right')
plt.savefig('p_y.png')

plt.subplots(1)
plt.hist(pz,bins=50, color='green', label='Signal',normed=True, range=[-500,500],alpha=0.5)#signal b
plt.hist(pz2,bins=50, color='blue', label='Background',normed=True, range=[-500,500],alpha=0.5)#background
plt.ylabel('Number of events')
plt.xlabel('p_z GeV')
plt.title('p_z')
pylab.legend(loc='upper right')
plt.savefig('p_z.png')

plt.subplots(1)
plt.hist(e,bins=50, color='green', label='Signal',normed=True, range=[0,700],alpha=0.5)#signal b
plt.hist(e2,bins=50, color='blue', label='Background',normed=True,range=[0,700],alpha=0.5)#background
plt.ylabel('Number of events')
plt.xlabel('Energy GeV')
plt.title('Energy')
pylab.legend(loc='upper right')
plt.savefig('Energy.png')

plt.subplots(1)
plt.hist(eta,bins=50, color='green', label='Signal',normed=True,alpha=0.5)#signal b
plt.hist(eta2,bins=50, color='blue', label='Background',normed=True,alpha=0.5)#background
plt.ylabel('Number of events')
plt.xlabel('eta')
plt.title('Eta')
pylab.legend(loc='upper right')
plt.savefig('eta.png')

plt.subplots(1)
plt.hist(phi,bins=50, color='green', label='Signal',normed=True,alpha=0.5)#signal b
plt.hist(phi2,bins=50, color='blue', label='Background',normed=True,alpha=0.5)#background
plt.ylabel('Number of events')
plt.xlabel('phi')
plt.title('Phi')
pylab.legend(loc='upper right')
plt.savefig('phi.png')

plt.subplots(1)
plt.hist(pT,bins=50, color='green', label='Signal',normed=True, range=[0,200],alpha=0.5)#signal b
plt.hist(pT2,bins=50, color='blue', label='Background',normed=True,range=[0,200],alpha=0.5)#background
plt.ylabel('Number of events')
plt.xlabel('pT')
plt.title('pT')
pylab.legend(loc='upper right')
plt.savefig('pT.png')

plt.subplots(1)
plt.hist(deta,bins=50, color='green', label='Signal',normed=True,alpha=0.5)#signal b
plt.hist(deta2,bins=50, color='blue', label='Background',normed=True,alpha=0.5)#background
plt.ylabel('Number of events')
plt.xlabel('deta')
plt.title('deta')
pylab.legend(loc='upper right')
plt.savefig('deta.png')

plt.subplots(1)
plt.hist(dphi,bins=50, color='green', label='Signal',normed=True,alpha=0.5)#signal b
plt.hist(dphi2,bins=50, color='blue', label='Background',normed=True,alpha=0.5)#background
plt.ylabel('Number of events')
plt.xlabel('dphi')
plt.title('dphi')
pylab.legend(loc='upper right')
plt.savefig('dphi.png')

plt.subplots(1)
plt.hist(dR,bins=50, color='green', label='Signal',normed=True,alpha=0.5)#signal b
plt.hist(dR2,bins=50, color='blue', label='Background',normed=True,alpha=0.5)#background
plt.ylabel('Number of events')
plt.xlabel('dR')
plt.title('dR')
pylab.legend(loc='upper right')
plt.savefig('dR.png')

fig1=plt.figure()
plt.hist(hmass,bins=50, color='green', label='Signal')#signal b
plt.ylabel('Number of events')
plt.xlabel( 'mass')
plt.title('H Mass')
pylab.legend(loc='upper right')
plt.savefig('h mass.png')

fig2=plt.figure()
plt.hist(mbb,bins=5000, color='blue', label='Background',normed=True)#background
plt.ylabel('Number of events')
plt.xlabel( 'mass')
plt.title('g Mass')
plt.axis([50.0,200.0,0.0,0.1])
ax=plt.gca()
ax.set_autoscale_on(False)
pylab.legend(loc='upper right')
plt.savefig('g mass.png')


plt.show()

    


