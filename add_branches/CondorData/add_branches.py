#! /usr/bin/env python
import os, sys, shutil
import csv
from ROOT import *
from array import array
from collections import namedtuple
from math import sin, cos, pi

condorMode=True
globalConfFile='Configurations-Grid view.csv'
keySightConfFile='ConfigurationKeySightScope-Grid view.csv'
caenConfFile='ConfigurationCAENHV-Grid view.csv'
recoVersion = "v2"
scopeRecoDir = 'root://cmseos.fnal.gov//store/group/cmstestbeam/2021_CMSTiming_ETL/LecroyScope/RecoData/TimingDAQRECO/RecoWithTracks/%s/'%recoVersion
outDir = scopeRecoDir+'confInfo/'

### increment this number if more columns are added to global configuration
firstConfElementWithRunNumber = 11
nEmptyEntriesAtend=3

def getConfs(runNumber):
    globConf=-1
    scopeConf=-1
    caenConf=-1
    with open(globalConfFile) as csvfile:
        next(csvfile) # skip header
        for line in csvfile:
           # print line
            vals = line.strip().split(',')
            nvals = len(vals)
            # print "conf %s, columns %i" %(vals[0],nvals)
            if nvals < firstConfElementWithRunNumber+1 or vals[firstConfElementWithRunNumber]=="": continue ## no runs exist
            # print vals
            ##10 is the first entry with a run number.
            for x in vals[firstConfElementWithRunNumber:nvals-nEmptyEntriesAtend]: print x.replace('"','')
            runListThisConf = [int(x.replace('"','')) for x in vals[firstConfElementWithRunNumber:nvals-nEmptyEntriesAtend]]
            if runNumber in runListThisConf:
                globConf=int(vals[0])
                scopeConf = int(vals[2]) ##lecroy is 2, keysight is 3
                caenConf = int(vals[1])
                break
    csvfile.close()
    print "Run %i: globConf %i, scopeConf %i, caenConf %i" % (runNumber,globConf,scopeConf,caenConf)
    return(globConf,scopeConf,caenConf)

### only works for 4 channels.
nchan=8
def getChannelMap(scopeConf):
    sensors =[]
    pads = []
    with open(keySightConfFile) as csvfile:
        next(csvfile) # skip header
        for line in csv.reader(csvfile, quotechar='"', delimiter=',', quoting=csv.QUOTE_ALL, skipinitialspace=True):
            vals = line#.strip().split(',')
            nvals = len(vals)
           # print "conf %s, columns %i" %(vals[0],nvals)
            #print vals
            if int(vals[0]) == scopeConf:
               # print "found it"
                print vals
                if nchan==4:sensors=[vals[1].replace('"',''),vals[3].replace('"',''),vals[5].replace('"',''),vals[7].replace('"','')]
                elif nchan==8:sensors=[vals[1].replace('"',''),vals[3].replace('"',''),vals[5].replace('"',''),vals[7].replace('"',''),vals[9].replace('"',''),vals[11].replace('"',''),vals[13].replace('"',''),vals[15].replace('"','')]
                if (vals[2] != ''): pads.append(int(vals[2]))
                else: pads.append(1)
                if (vals[4] != ''): pads.append(int(vals[4]))
                else: pads.append(1)
                if (vals[6] != ''): pads.append(int(vals[6]))
                else: pads.append(1)
                if (vals[8] != ''): pads.append(int(vals[8]))
                else: pads.append(1)
                if nchan>4:
                    if (vals[10] != ''): pads.append(int(vals[10]))
                    else: pads.append(1)
                    if (vals[12] != ''): pads.append(int(vals[12]))
                    else: pads.append(1)
                    if (vals[14] != ''): pads.append(int(vals[14]))
                    else: pads.append(1)
                    if (vals[16] != ''): pads.append(int(vals[16]))

                break
    csvfile.close()
    print sensors, pads
    return(sensors,pads)

def getHVMap(caenConf):
    sensors =[]
    HVs = []
    with open(caenConfFile) as csvfile:
        next(csvfile) # skip header
        for line in csv.reader(csvfile, quotechar='"', delimiter=',', quoting=csv.QUOTE_ALL, skipinitialspace=True):
            vals = line#.strip().split(',')
            nvals = len(vals)
           # print "conf %s, columns %i" %(vals[0],nvals)
            #print vals
            if int(vals[0]) == caenConf:               
                print vals
                for i in range(7):                   
                    if vals[2*i+2] != '':
                        sensors.append(vals[2*i+2].replace('"',''))
                        if vals[2*i+3] != '':
                            HVs.append(int(vals[2*i+3]))
                        else: HVs.append(-1)
                break
    csvfile.close()
    print sensors, HVs
    return(sensors,HVs)


def processRun(runNumber,outFileName):
    globConf,scopeConf,caenConf = getConfs(runNumber)
    sensors,pads = getChannelMap(scopeConf)
    sensorsHV,HVs = getHVMap(caenConf)
    print "Processing run %i, global configuration %i, scopeConf %i, caenConf %i" % (runNumber,globConf,scopeConf,caenConf)

    rootfile = TFile(outFileName, "UPDATE")
    if (rootfile.IsZombie() or not rootfile.IsOpen()):
        return 'ERROR: Could not recover TTree, please check file:', outfile
    pulse = rootfile.Get('pulse')

    arr_run = array('i',[runNumber])
    arr_gconf = array('i',[globConf])
    v_sensors = vector('string')()
    for x in sensors: v_sensors.push_back(x)
    v_pads = vector('int')()
    for x in pads: v_pads.push_back(x)
    #arr_pads = array('i',pads)
    v_sensorsHV = vector('string')()
    for x in sensorsHV: v_sensorsHV.push_back(x)
    v_HVs = vector('int')()
    for x in HVs: v_HVs.push_back(x)
    #arr_HVs = array('i',HVs)

    b_run = pulse.Branch("run",arr_run,"run/I")
    b_gconf = pulse.Branch("gconf",arr_gconf,"gconf/I")
    b_sensors = pulse.Branch('sensors',v_sensors)
    b_pads = pulse.Branch("pads",v_pads)
    b_sensorsHV = pulse.Branch('sensorsHV',v_sensorsHV)
    b_HVs = pulse.Branch("HVs",v_HVs)
    #b_HVs = pulse.Branch("HVs",arr_HVs,'HVs[{0}]/I'.format(len(HVs)))

    for i in range(pulse.GetEntries()):
        pulse.GetEntry(i)
        b_run.Fill()
        b_gconf.Fill()
        b_sensors.Fill()
        b_pads.Fill()
        b_HVs.Fill()
        b_sensorsHV.Fill()

    pulse.Write()
    rootfile.Close()
    return '...done'


if __name__ == '__main__':
    
    runNumber = int(sys.argv[1])
    infileName = scopeRecoDir+"run_scope%i_converted.root" % runNumber
    
    outfileName = "run_scope%i_info.root" % runNumber
    if condorMode:
        cmd = "xrdcp %s %s" % (infileName,outfileName)
        print cmd
        os.system(cmd)
    else: 
        outfileName = outDir+"run_scope%i_info.root" % runNumber
        #cmd = "xrdcp %s %s" % (infileName,outfileName)
        #print cmd
        #os.system(cmd)

    print 'Processing file:', outfileName
    processRun(runNumber,outfileName)

    if condorMode:
        cmd = "xrdcp -f %s %s" % (outfileName,outDir)
        print cmd
        os.system(cmd)



  