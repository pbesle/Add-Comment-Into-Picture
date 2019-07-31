#!/usr/bin/env python3


import sys
import os
import csv
import array
import random
from shutil import copyfile
import exifread

Frame=[]
FileType=".suf"
TabFileType=[]
NbPictures=0
noexist=False

if len(sys.argv) != 3:
	print("Usage : CreateFrame <Frame> <Number Pictures>")
	sys.exit(0)

os.chdir(sys.argv[1])
MaxPicture=int(sys.argv[2])

try:
	with open("frame.csv",'r') as fp_cadre:
		ligne=csv.reader(fp_cadre,delimiter=';')
		for Pictures in ligne:
			Frame.append([Pictures[0],Pictures[1]])
			chaine=Frame[-1][0]
			print("**" + str(chaine))
	NbPictures=str(len(Frame))

except IOError:
	print ("File frame.csv does not exit")
	exit

for Pictures in range(MaxPicture):
	NumRandom=random.randrange(0,int(NbPictures))
	print("** NumRandom =" + str(NumRandom))
	for NumPicture in range(int(NumRandom),int(NbPictures)):
		if (Frame[NumPicture][1] == "1"):
			continue
		print("** Frame[" + str(NumPicture) + "] = " + Frame[NumPicture][0] + " ** used = " +  Frame[NumPicture][1])
		Frame[NumPicture][1]=1
		NewPicture='{:04d}'.format(Pictures) + ".jpg"
		print("** NewPicture = " + NewPicture)
		copyfile(Frame[NumPicture][0],"PictureTmp")
		label=os.path.basename(os.path.dirname(Frame[NumPicture][0]))
		print("*** Libelle = " + str(label))
		f = open(Frame[NumPicture][0], 'rb')
		tags = exifread.process_file(f)
		XPTitle = str(tags.get('Image ImageDescription'))
		print("*** XPTitle = " + XPTitle + "**")
		if XPTitle != "None" and XPTitle[0] !=  ' ':
			label = label + " - " + str(XPTitle)
		LABEL="Height=`/usr/bin/identify -format %h PictureTmp`;Hauteur=`expr $Height / 20`; /usr/bin/convert PictureTmp -fill white -box '#0008' -gravity South -pointsize ${Hauteur} -annotate +0+0 \"" + label + "\" \"" + NewPicture + "\""
		print("** LABEL = " + LABEL)
		os.system(LABEL)
		break


with open("frame.csv",'w') as fp_cadre:
	writer=csv.writer(fp_cadre,delimiter=";",lineterminator='\n')
	writer.writerows(Frame)
