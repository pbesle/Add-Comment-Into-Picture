#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <libexif/exif-data.h>

int NbPhotos;
int NbNewPhotos;

struct photo
	{
	char *nom;
	int used;
	};

struct photo *cadre;

char chaine[1024];

int NbFileType;
char **TabFileType;

void AddCadre(Picture)
char *Picture;
{
int i;
printf("Picture = %s\n",Picture);
for (i=0; i < NbPhotos; i++)
	if (strncmp(cadre[i].nom,Picture,strlen(cadre[i].nom)) == 0)
		break;
if (i == NbPhotos)
	{
	cadre=realloc(cadre, (NbNewPhotos + 1) * sizeof(struct photo));
	cadre[NbNewPhotos].nom=malloc(1024);
	strcpy(cadre[NbNewPhotos].nom,Picture);
	cadre[NbNewPhotos].used=0;
	printf("Ajout cadre[%d].nom= %s %d\n",NbNewPhotos,cadre[NbNewPhotos].nom,cadre[NbNewPhotos].used);
	NbNewPhotos=NbNewPhotos+1;
	}
}

void printdir(char *dir, int depth)
{
DIR *dp;
struct dirent *entry;
struct stat statbuf;
char repertoire[1024];
char FileType[5];
int i;

if((dp = opendir(dir)) == NULL) 
	{
      	printf("cannot open directory: %s\n", dir);
        return;
    	}
while((entry = readdir(dp)) != NULL) 
	{
        sprintf(repertoire,"%s/%s",dir,entry->d_name);
        lstat(repertoire,&statbuf);
        if(S_ISDIR(statbuf.st_mode)) 
		{
            	if(strcmp(".",entry->d_name) == 0 || strcmp("..",entry->d_name) == 0)
                	continue;
            	printdir(repertoire,depth+4);
        	}
        else 
		{
	    	strcpy(FileType,&repertoire[strlen(repertoire)-4]);
		FileType[0]=tolower(FileType[0]);
		FileType[1]=tolower(FileType[1]);
		FileType[2]=tolower(FileType[2]);
		FileType[3]=tolower(FileType[3]);
		for (i=0; i < NbFileType; i++)
	    	 	if (strcmp(FileType,TabFileType[i]) == 0)
				{
				AddCadre(repertoire);
				break;
				}
		}
    	}
    closedir(dp);
}

main (argc,argv)
int argc;
char **argv;
{
int i;
FILE *fp_cadre;
int flag;
int ret;
char *toto;


if (argc < 4)
	{
	printf("Usage : CreateCadre <Cadre> <Chemin Racine> <type1> [type2 ... typen]\n");
	exit(0);
	}

NbFileType=argc-3;

TabFileType=malloc(NbFileType * sizeof(*TabFileType));
for (i=0; i < NbFileType; i++)
    	{
	TabFileType[i]=malloc(5);
	strcpy(TabFileType[i],argv[3+i]);
	}
if (chdir(argv[1]) !=0)
	{
	printf("<CreateCadre> Cadre %s inexistant\n",argv[1]);
	exit(0);
	}
NbPhotos=0;
cadre=malloc(0 * sizeof(struct photo));
if ((fp_cadre=(FILE *)fopen("frame.csv","r"))!=NULL)
	{
	while ((ret = fscanf(fp_cadre,"%[^;];%d\n",chaine,&flag)) == 2)
		{
		cadre=realloc(cadre, (NbPhotos + 1) * sizeof(struct photo));
		cadre[NbPhotos].nom=malloc(1024);
		strcpy(cadre[NbPhotos].nom,chaine);
		cadre[NbPhotos].used=flag;
		printf("cadre[%d].nom= %s\n",NbPhotos,cadre[NbPhotos].nom);
		NbPhotos=NbPhotos+1;
		}
	fclose(fp_cadre);
	}
NbNewPhotos=NbPhotos;
printdir(argv[2],0);
if ((fp_cadre=(FILE *)fopen("frame.csv","w"))!=NULL)
	{
	printf("NewCadre[%d]=%s;%d\n",i,cadre[i].nom,cadre[i].used);
	for (i=0;i<NbNewPhotos;i++)
		fprintf(fp_cadre,"%s;%d\n",cadre[i].nom,cadre[i].used);
	fclose(fp_cadre);
	}
exit(0);
}


