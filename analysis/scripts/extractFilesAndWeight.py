#!/usr/bin/python

import sys, getopt, re, os

def main(argv):
   inputfile = ''
   outputfile = ''
   outputdir = ''
   try:
      opts, args = getopt.getopt(argv,"hi:o:d:",["ifile=","ofile=","odir"])
   except getopt.GetoptError:
      print 'extractJSONS.py -i <inputfile> -o <outputfile> -d <outputdir>'
      sys.exit(2)
   for opt, arg in opts:
      if opt == '-h':
         print 'extractJSONS.py -i <inputfile> -o <outputfile> -d <outputdir>'
         sys.exit()
      elif opt in ("-i", "--ifile"):
         inputfile = arg
      elif opt in ("-o", "--ofile"):
         outputfile = arg
      elif opt in ("-d", "--odir"):
         outputdir = arg
   print 'Input file is ', inputfile
   hand = open(inputfile) # open catalogue from Flashgg

 
   outputname = outputdir+'/'+outputfile+'.list'
   outputname2 = outputdir+'/'+outputfile+'.weight'
   #check that output directory exists and if not create it
   dir = os.path.dirname(outputname)
   if not os.path.exists(dir):
      os.makedirs(dir)   

   target = open(outputname,'w') #output file with the EOS file of dataset to be analyzed
   target2 = open(outputname2, 'w')
          
   lines = hand.readlines()
   hand.close()

   for i in range(0, len(lines)):
      line = lines[i]
      if 'store' in line :
        line = re.sub('                "name": "', '', line)
        line = re.sub('",', '', line)
        target.write(line)
      if 'weights' in line:
        line = re.sub('                "weights": ', '', line)
        target2.write(line)
        #target2.write('\n')

   target.close()        
   target2.close()
   print 'Output file is ', outputname , 'and', outputname2

if __name__ == "__main__":
   main(sys.argv[1:])
