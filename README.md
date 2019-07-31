# Add-Comment-Into-Picture
Add a comment into picture for running on numeric frame

These programs are written in python

They are running on windows 10 with CYGWIN

They browse a tree containing pictures (.jpg, .tiff, .bmp)  and add the directory name and the comment exif if it is present into the picture.

## Prerequisite
- Windows 10
- Cygwin 64
- python 3.6 (Intalled with cygwin Setup)
- ImageMagic (installed with cygwin Setup)
- pip (python-pip-whell installed with cygwin Setup)
- Exifreader (installed with PIP)

## Programs :

- CreateFrame.py

  This program browse a Picture Directory to add new pictures into a file named Cadre.csv located in the dorectory Frame_Name
  
  Usage : CreateFrame.py Frame_Name Picture_Directory_Name Picture_type1 (Picture_type2 ... Picture_typen)
  
  example : python3 ./CreateFrame.py "Frame Josefina" "/cygdrive/z/Mes Photos/Mes Photos" .jpg .bmp .png .tif
  
- NewCadre.py 

  This program ramdomly choose picture into the file Frame.csv and add the directory name and the exif comment at the bottom of the picture
  
  Usage : NewFrame.py Frame_Name Number_Of_Pictures
  
  Example : python3 ./NewFrame.py "Cadre Josefina" 100

![Rob Demo](pictures/ULM.jpg)

