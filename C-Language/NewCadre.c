#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <libexif/exif-data.h>

char XPTitle[1024];

int NbPhotos;
int Photos;
int NbNewPhotos;

struct photo
	{
	char *nom;
	int used;
	};

struct photo *cadre;

char chaine[1025];
char LABEL[1024];
char NewPhoto[1024];
char libelle[1024];
char *dossier;

ExifData *ed;
ExifEntry *entry;

/* Remove spaces on the right of the string */
static void trim_spaces(char *buf)
{
    char *s = buf-1;
    for (; *buf; ++buf) {
        if (*buf != ' ')
            s = buf;
    }
    *++s = 0; /* nul terminate the string on the first of the final spaces */
}

main (argc,argv)
int argc;
char **argv;
{
int i;
int ind_libelle;
FILE *fp_cadre;
int flag;
int ret;
char *toto;
int MaxPhoto;
int NumPhoto;
int NumAleatoire;
int fd_read,fd_write;
int taille;

flag=0;

if (argc != 3)
	{
	printf("Usage : CreateCadre <Cadre> <Nombre Photos>\n");
	exit(0);
	}
if (chdir(argv[1]) !=0)
	{
	printf("<CreateCadre> Cadre %s inexistant\n",argv[1]);
	exit(0);
	}
sscanf(argv[2],"%d",&MaxPhoto);
NbPhotos=0;
cadre = malloc(0 * sizeof(struct photo));
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
for (NumPhoto = 0; NumPhoto < NbPhotos; NumPhoto++)
	printf("*** 0 *** cadre[%d].nom= %s cadre[%d].used= %d\n",NumPhoto,cadre[NumPhoto].nom,NumPhoto,cadre[NumPhoto].used);


srand(time(NULL));
for (Photos=0; Photos < MaxPhoto; Photos++)
	{
	NumAleatoire=1+rand()%NbPhotos;
	for (NumPhoto = NumAleatoire; NumPhoto < NbPhotos && cadre[NumPhoto].used != 0; NumPhoto++);
	if (NumPhoto == NbPhotos)
		{
		for (NumPhoto= 0; NumPhoto < NumAleatoire && cadre[NumPhoto].used != 0; NumPhoto++);
		if (NumPhoto == NumAleatoire)
			{
			printf ("Plus de Photos Disponible\n");
			exit(0);
			}
		}
	cadre[NumPhoto].used++;
	if ((fd_read = open(cadre[NumPhoto].nom,O_RDONLY))==-1)
		{
		printf ("Probleme Open %s\n",cadre[NumPhoto].nom);
		continue;
		}
	sprintf(NewPhoto,"%3.3d.jpg",Photos);
	if ((fd_write = open("PhotoTmp",O_CREAT|O_WRONLY,0666)) == -1)
		{
		printf ("Probleme Open %s\n",chaine);
		exit(0);
		}
	while ((taille=read(fd_read,chaine,1024)) == 1024 && write(fd_write,chaine,1024));
	write(fd_write,chaine,taille);
		
	close(fd_read);
	close(fd_write);

	strcpy(chaine,cadre[NumPhoto].nom);
	printf("chaine = %s CADRE *** cadre[%d].nom= *%s*\n",chaine,NumPhoto,cadre[NumPhoto].nom);
	for (i=strlen(chaine);i>0 & chaine[i]!='/';i--);
	chaine[i]='\0';
	printf("dirname = %s\n",chaine);
	for (i=strlen(chaine);i>0 & chaine[i]!='/';i--);
	printf("basename = %s\n",&chaine[i+1]);
	strcpy(libelle,&chaine[i+1]);
	for (ind_libelle=0; ind_libelle < strlen(libelle); ind_libelle++)
		libelle[ind_libelle]=toupper(libelle[ind_libelle]);
        XPTitle[0]='\0';
printf("*** 3***\n");
        ed = exif_data_new_from_file(cadre[NumPhoto].nom);
        if (ed) 
	    {
    	    entry = exif_content_get_entry(ed->ifd[EXIF_IFD_0],EXIF_TAG_XP_TITLE);
    	    if (entry) 
		{
        	exif_entry_get_value(entry, XPTitle, sizeof(XPTitle));
        	trim_spaces(XPTitle);
        	if (*XPTitle) 
			{
            		printf("%s\n",XPTitle);
			strcat(libelle," - ");
			}
       	 	}
	    }
    	exif_data_unref(ed);
	strcat(libelle,XPTitle);
	printf ("*** libelle = %s ***\n",libelle);
	sprintf(LABEL,"Height=`/usr/bin/identify -format %%h PhotoTmp`; Hauteur=`expr $Height / 20`; /usr/bin/convert PhotoTmp -fill white -box '#0008' -gravity South -pointsize ${Hauteur} -annotate +0+0 \"%s\" \"%s\"",libelle,NewPhoto);
	printf("*** LABEL = %s ***\n",LABEL);
	system(LABEL);
	}

if ((fp_cadre=(FILE *)fopen("frame.csv","w"))!=NULL)
	{
	printf("Cadre[%d]=%s;%d\n",NumPhoto,cadre[NumPhoto].nom,cadre[NumPhoto].used); for (i=0;i<NbPhotos;i++)
		fprintf(fp_cadre,"%s;%d\n",cadre[i].nom,cadre[i].used);
	fclose(fp_cadre);
	}
exit(0);
}

