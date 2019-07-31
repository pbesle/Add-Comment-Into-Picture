#! /usr/bin/env python3


import sys
import os
import csv
from array import *

Frame=[]
FileType=".suf"
TabFileType=[]
NbPictures=0
noexist=False

def AddFrame(Picture):
	flag_find=0
	i=0
	print("Picture = " + Picture)
	for i in range(int(NbPictures)):
		if Picture.find(Frame[i][0]) == 0:
			flag_find=1
			break
	if (i == int(NbPictures) - 1 and flag_find == 0) or noexist == True:
		Frame.append([Picture,0])
		print("*** Add : " + str(Frame[-1][0]))


def printdir(directory):
	for dossier, sous_dossiers, files in os.walk(directory):
		for file in files:
			Picture=os.path.join(dossier, file)
			FileType=str.lower(Picture[-4:])
			for Type in range(NbFileType):
				if TabFileType[Type] == FileType:
					AddFrame(Picture)
					break


if len(sys.argv) < 4:
        print("Usage : CreateFrame <Frame> <Root> <type1> [type2 ... typen]\n")
        sys.exit(0)

NbFileType=len(sys.argv)-3
for Type in range(NbFileType):
	TabFileType.append(sys.argv[3+Type])

os.chdir(sys.argv[1])

try:
	with open("frame.csv",'r') as fp_frame:
		ligne=csv.reader(fp_frame,delimiter=';')
		for Pictures in ligne:
			Frame.append([Pictures[0],Pictures[1]])
			chaine=Frame[-1][0]
			print("**" + str(chaine))
	NbPictures=str(len(Frame))

except IOError:
	noexist=True
	print ("INEXISTANT")

printdir(os.sys.argv[2])

with open("frame.csv",'w') as fp_frame:
	writer=csv.writer(fp_frame,delimiter=";",lineterminator='\n')
	writer.writerows(Frame)
