#!/bin/bash

source /cvmfs/cms.cern.ch/cmsset_default.sh
cd /cvmfs/cms.cern.ch/slc6_amd64_gcc530/cms/cmssw/CMSSW_9_0_2/src/
eval `scramv1 runtime -sh`
cd -

xrdcp -r root://cmseos.fnal.gov//store/group/cmstestbeam/2020_02_CMSTiming/condor/add_branches .
ls
python add_branches.py $1
rm *.root
rm *.csv
rm *.py

echo "##### HOST DETAILS #####"
echo "I ran add_branches on:"
hostname
date
