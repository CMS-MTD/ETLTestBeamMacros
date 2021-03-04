#! /usr/bin/env python
import os, sys, shutil
import csv
from ROOT import *
from subprocess import Popen, PIPE

confStart = 295
confEnd = 310
checkOutputExisting = True
recoVersion = "v2"
globalConfFile="../CondorData/Configurations-Grid view.csv"
scopeRecoDir = 'root://cmseos.fnal.gov//store/group/cmstestbeam/2021_CMSTiming_ETL/LecroyScope/RecoData/TimingDAQRECO/RecoWithTracks/%s/' %recoVersion
outDir = '/store/group/cmstestbeam/2021_CMSTiming_ETL/LecroyScope/RecoData/TimingDAQRECO/RecoWithTracks/%s/confInfo/' %recoVersion
eosCodeLocation = "/store/group/cmstestbeam/2021_CMSTiming_ETL/condor"


#### No commas allowed in sensor names!

def getRuns():
    runlist=[]
    with open(globalConfFile) as csvfile:
        next(csvfile) # skip header
        for line in csv.reader(csvfile, quotechar='"', delimiter=',', quoting=csv.QUOTE_ALL, skipinitialspace=True):
            vals = line#.strip().split(',')
            nvals = len(vals)
           # print "conf %s, columns %i" %(vals[0],nvals)
            # print vals
            # print nvals

            if int(vals[0]) >=confStart and int(vals[0])<=confEnd:               
                print vals
                if vals[11] != '' and vals[2]!='': ###there are more than zero runs in this conf, and scope is included. 
                    runListThisConf = [int(x) for x in vals[11].split(",")]
                #print runListThisConf
                    for x in runListThisConf:
                        runlist.append(x)
    csvfile.close()
    #print sensors, pads
    #runlist = [27859]
    print runlist
    return runlist

def submitJob(runNumber):

    jdlfile = "jdl/condor_add_branches_"+str(run)+".jdl"
    f = open(jdlfile,"w+")
    f.write("universe = vanilla\n")
    f.write("Executable = add_branches.sh\n")
    f.write("should_transfer_files = YES\n")
    f.write("when_to_transfer_output = ON_EXIT\n")
    f.write("Output = logs/add_branches_%i.stdout\n"%runNumber)
    f.write("Error = logs/add_branches_%i.stderr\n"%runNumber)
    f.write("Log = logs/add_branches_%i.log\n"%runNumber)
    f.write("Arguments = %i\n"%runNumber)
    f.write("Queue 1\n")
    f.close()

    cmd = "condor_submit %s"%jdlfile
    print cmd
    os.system(cmd)

def updateCodeinEOS():
    cmd = "eos root://cmseos.fnal.gov rm -r %s/add_branches/" % eosCodeLocation
    os.system(cmd)
    cmd = "eos root://cmseos.fnal.gov mkdir %s/add_branches/" % eosCodeLocation
    os.system(cmd)
    cmd = "xrdcp -r ../CondorData root://cmseos.fnal.gov/%s/add_branches/" % eosCodeLocation
    os.system(cmd)
    cmd = "eos root://cmseos.fnal.gov mkdir %s" % outDir
    os.system(cmd)


def fileExistsEOS(file):
    cmd = ['eos','root://cmseos.fnal.gov','ls',file]
    p = Popen(cmd, stdout=PIPE, stderr=PIPE)
    output, error = p.communicate()
    return error==''

if __name__ == '__main__':
    updateCodeinEOS()
    runlist =  getRuns()
    print runlist
    print len(runlist)
    for run in runlist:
        # print infile
        infile = scopeRecoDir.replace("root://cmseos.fnal.gov/","") + "run_scope%i_converted.root"%run
        outfile = infile.replace("%s/"%recoVersion,"%s/confInfo/"%recoVersion).replace("_converted.root","_info.root")
        print infile
        if fileExistsEOS(infile):
            if not checkOutputExisting or (checkOutputExisting and not fileExistsEOS(outfile)):
               submitJob(run)

