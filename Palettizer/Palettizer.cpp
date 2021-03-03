// Palettizer.cpp : Defines the entry point for the console application.
//

#include"stdafx.h"
#include<string.h>
#include<malloc.h>
#include<memory.h>

typedef struct tagBITMAPFILEHEADER
{
	unsigned short bfType; 
	unsigned long bfSize; 
	unsigned short bfReserved1; 
	unsigned short bfReserved2; 
	unsigned long bfOffBits; 
} BITMAPFILEHEADER; 
 
typedef struct tagBITMAPINFOHEADER
{
    unsigned long biSize; 
    long biWidth; 
    long biHeight; 
    unsigned short biPlanes; 
    unsigned short biBitCount;
    unsigned long biCompression; 
    unsigned long biSizeImage; 
	long biXPelsPerMeter; 
    long biYPelsPerMeter; 
    unsigned long biClrUsed; 
    unsigned long biClrImportant; 
} BITMAPINFOHEADER; 
 
typedef struct tagBITMAPLIST
{
	long width;
	long height;
	unsigned char* olddata;
	unsigned char* newdata;
	char name[20];
} BITMAPLIST,*LPBITMAPLIST;

void GetNumber(FILE* file,int* number);
void GetString(FILE* file,char* string);

int main(int argc, char* argv[])
{
	char* filename;
	FILE* file;
	char extension[20];
	int width,height,numbmps;
	int count;
	char bmpname[256];
	char tempname[256];
	FILE* bmpfile;
	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bih;
	int bmpwd,bmpht;
	LPBITMAPLIST bmplist;
	long bmpnum=0;
	long startx,starty,endx,endy;
	long x,y;
	unsigned char r,g,b;
	long numpx,newcl;
	int maxbmps;
	unsigned char* palette;
	long totalclr;
	long dithamount;
	int reserved;
	long palpos;
	char path[256];

	if(argc==1)
		filename="pspin.ptz";
	else
		filename=argv[1];
	printf("opening file %s\n",filename);
	file=fopen(filename,"rb");
	if(!file)
	{
		printf("-- file not found\n");
		goto error1;
	}
	GetString(file,path);
	GetString(file,extension);
	GetNumber(file,&maxbmps);
	printf("  allocating memory for %d bitmaps\n\n",maxbmps);
	GetNumber(file,&reserved);
	bmplist=(LPBITMAPLIST)malloc(sizeof(BITMAPLIST)*maxbmps);
	GetNumber(file,&width);
morebmps:;
	GetNumber(file,&height);
	GetNumber(file,&numbmps);
	printf("there are %d bitmaps in this set\n",numbmps);
	printf("bitmaps in this set will be cropped to %dx%d\n",width,height);
	totalclr=0;
	for(count=0;count<numbmps;count++)
	{
		GetString(file,bmpname);
		strcpy(tempname,bmpname);
		strcpy(bmpname,path);
		strcat(bmpname,tempname);
		strcat(bmpname,".bmp");
		printf("  opening file %s (%d)\n",bmpname,count+1);
		bmpfile=fopen(bmpname,"rb");
		if(!bmpfile)
		{
			printf("  -- file not found\n");
			goto error4;
		}
		if(fread(&bfh.bfType,2,1,bmpfile)!=1)
		{
			printf("  -- error reading header\n");
			goto error3;
		}
		if(fread(&bfh.bfSize,4,1,bmpfile)!=1)
		{
			printf("  -- error reading header\n");
			goto error3;
		}
		if(fread(&bfh.bfReserved1,2,1,bmpfile)!=1)
		{
			printf("  -- error reading header\n");
			goto error3;
		}
		if(fread(&bfh.bfReserved2,2,1,bmpfile)!=1)
		{
			printf("  -- error reading header\n");
			goto error3;
		}
		if(fread(&bfh.bfOffBits,4,1,bmpfile)!=1)
		{
			printf("  -- error reading header\n");
			goto error3;
		}
		if(bfh.bfType!=0x4D42)
		{
			printf("  -- not a bitmap file\n");
			goto error3;
		}
		if(fread(&bih,sizeof(bih),1,bmpfile)!=1)
		{
			printf("  -- error reading data\n");
			goto error3;
		}
		if(bih.biSize!=sizeof(bih))
		{
			printf("  -- invalid structure size\n");
			goto error3;
		}
		if(bih.biCompression!=0)
		{
			printf("  -- invalid compression setting\n");
			goto error3;
		}
		if(bih.biBitCount!=24&&bih.biBitCount!=8)
		{
			printf("  -- invalid bit count\n");
			goto error3;
		}
		switch(bih.biBitCount)
		{
		case(24):
			bmpwd=bih.biWidth;
			bmpht=bih.biHeight;
			printf("  cropping image from %dx%d to %dx%d\n",bmpwd,bmpht,width,height);
			bmplist[bmpnum].width=width;
			bmplist[bmpnum].height=height;
			bmplist[bmpnum].newdata=(unsigned char*)malloc(width*height);
			bmplist[bmpnum].olddata=(unsigned char*)malloc(width*height*3);
			strcpy(bmplist[bmpnum].name,tempname);
			if(!bmplist[bmpnum].olddata)
				{
					printf("  -- out of memory\n");
					goto error3;
				}
			if(!bmplist[bmpnum].newdata)
				{
					printf("  -- out of memory\n");
					goto error3;
				}
			startx=(bmpwd>>1)-(width>>1);
			starty=(bmpht>>1)-(height>>1);
			endx=startx+width;
			endy=starty+height;
			numpx=0;
			newcl=0;
			for(y=0;y<bmpht;y++)
				for(x=0;x<bmpwd;x++)
				{
					b=getc(bmpfile);
					g=getc(bmpfile);
					r=getc(bmpfile);
					if(x>=startx&&
					   x<endx&&
					   y>=starty&&
					   y<endy)
					{
						bmplist[bmpnum].olddata[((x-startx)+((y-starty)*width))*3]=r;
						bmplist[bmpnum].olddata[(((x-startx)+((y-starty)*width))*3)+1]=g;
						bmplist[bmpnum].olddata[(((x-startx)+((y-starty)*width))*3)+2]=b;
					}
				}
			break;
		case(8):
			long c;
			unsigned char* palette;
			palette=(unsigned char*)malloc(256*3);
			for(x=0;x<256;x++)
			{
				palette[x*3]=getc(bmpfile);
				palette[(x*3)+1]=getc(bmpfile);
				palette[(x*3)+2]=getc(bmpfile);
			}
			bmpwd=bih.biWidth;
			bmpht=bih.biHeight;
			printf("  cropping image from %dx%d to %dx%d\n",bmpwd,bmpht,width,height);
			bmplist[bmpnum].width=width;
			bmplist[bmpnum].height=height;
			bmplist[bmpnum].newdata=(unsigned char*)malloc(width*height);
			bmplist[bmpnum].olddata=(unsigned char*)malloc(width*height*3);
			strcpy(bmplist[bmpnum].name,tempname);
			if(!bmplist[bmpnum].olddata)
				{
					printf("  -- out of memory\n");
					goto error3;
				}
			if(!bmplist[bmpnum].newdata)
				{
					printf("  -- out of memory\n");
					goto error3;
				}
			startx=(bmpwd>>1)-(width>>1);
			starty=(bmpht>>1)-(height>>1);
			endx=startx+width;
			endy=starty+height;
			numpx=0;
			newcl=0;
			for(y=0;y<bmpht;y++)
				for(x=0;x<bmpwd;x++)
				{
					c=(long)getc(bmpfile);
					b=palette[c*3];
					g=palette[(c*3)+1];
					r=palette[(c*3)+2];
					if(x>=startx&&
					   x<endx&&
					   y>=starty&&
					   y<endy)
					{
						bmplist[bmpnum].olddata[((x-startx)+((y-starty)*width))*3]=r;
						bmplist[bmpnum].olddata[(((x-startx)+((y-starty)*width))*3)+1]=g;
						bmplist[bmpnum].olddata[(((x-startx)+((y-starty)*width))*3)+2]=b;
					}
				}
			free(palette);
			break;
		}
		bmpnum++;
error3:;
		printf("  closing file %s\n\n",bmpname);
		fclose(bmpfile);
error4:;
	}
	printf("set complete\n\n");
	GetNumber(file,&width);
	if(width)
		goto morebmps;
	printf("closing file %s\n\n",filename);
	fclose(file);
	printf("%d colors are reserved, leaving a maxium of %d colors available\n\n",reserved,255-reserved);
	dithamount=0;
	palette=(unsigned char*)malloc(256*3);
repeat:;
	dithamount++;
	printf("dithering by %d\n",dithamount);
	memset(palette,0,256*3);
	palpos=reserved+1;
	for(count=0;count<bmpnum;count++)
		for(y=0;y<bmplist[count].height;y++)
			for(x=0;x<bmplist[count].width;x++)
			{
				r=bmplist[count].olddata[(x+(y*bmplist[count].width))*3];
				g=bmplist[count].olddata[((x+(y*bmplist[count].width))*3)+1];
				b=bmplist[count].olddata[((x+(y*bmplist[count].width))*3)+2];
				long rr=r;
				long gg=g;
				long bb=b;
				rr/=dithamount;
				gg/=dithamount;
				bb/=dithamount;
				rr*=dithamount;
				gg*=dithamount;
				bb*=dithamount;
				long palcnt;
				bool found=0;
				for(palcnt=palpos;palcnt>=0;palcnt--)
					if(rr==palette[palcnt*3])
						if(gg==palette[(palcnt*3)+1])
							if(bb==palette[(palcnt*3)+2])
							{
								bmplist[count].newdata[x+(y*bmplist[count].width)]=(unsigned char)palcnt;
								found=1;
							}
				if(!found)
				{
					palette[palpos*3]=(unsigned char)rr;
					palette[(palpos*3)+1]=(unsigned char)gg;
					palette[(palpos*3)+2]=(unsigned char)bb;
					bmplist[count].newdata[x+(y*bmplist[count].width)]=(unsigned char)palpos;
					palpos++;
					if(palpos>255)
					{
						printf("  maximum colors exceeded\n");
						goto repeat;
					}
				}
			}
	printf("\ndithering was successful\noutputting bitmaps\n\n");
	for(count=0;count<bmpnum;count++)
	{
		struct
		{
			BITMAPFILEHEADER bfh;
			BITMAPINFOHEADER bih;
			unsigned char pdata[1024];
			unsigned char* data;
		} bmp;
		unsigned char* dataptr;
		bmp.bfh.bfType=0x4D42;
		bmp.bfh.bfSize=14+40+(bmplist[count].width*bmplist[count].height)+1024;
		bmp.bfh.bfReserved1=0;
		bmp.bfh.bfReserved2=0;
		bmp.bfh.bfOffBits=14+40+1024;
		bmp.bih.biSize=sizeof(BITMAPINFOHEADER);
		bmp.bih.biWidth=bmplist[count].width;
		bmp.bih.biHeight=bmplist[count].height;
		bmp.bih.biPlanes=1;
		bmp.bih.biBitCount=8;
		bmp.bih.biCompression=0;
		bmp.bih.biSizeImage=0;
		bmp.bih.biXPelsPerMeter=11811;
		bmp.bih.biYPelsPerMeter=11811;
		bmp.bih.biClrUsed=0;
		bmp.bih.biClrImportant=0;
		bmp.data=(unsigned char*)malloc(bmplist[count].width*bmplist[count].height);
		for(y=0;y<bmplist[count].height;y++)
			for(x=0;x<bmplist[count].width;x++)
				bmp.data[x+(y*bmplist[count].width)]=bmplist[count].newdata[x+(y*bmplist[count].width)];
		for(x=0;x<256;x++)
		{
			bmp.pdata[x*4]=palette[(x*3)+2];
			bmp.pdata[(x*4)+1]=palette[(x*3)+1];
			bmp.pdata[(x*4)+2]=palette[x*3];
			bmp.pdata[(x*4)+3]=0;
		}
		strcpy(bmpname,path);
		strcat(bmpname,bmplist[count].name);
		strcat(bmpname,".");
		strcat(bmpname,extension);
		printf("  %s\n",bmpname);
		bmpfile=fopen(bmpname,"wb");
		dataptr=(unsigned char*)&bmp.bfh.bfType;
		for(x=0;x<sizeof(short);x++)
			putc(dataptr[x],bmpfile);
		dataptr=(unsigned char*)&bmp.bfh.bfSize;
		for(x=0;x<sizeof(long);x++)
			putc(dataptr[x],bmpfile);
		dataptr=(unsigned char*)&bmp.bfh.bfReserved1;
		for(x=0;x<sizeof(short);x++)
			putc(dataptr[x],bmpfile);
		dataptr=(unsigned char*)&bmp.bfh.bfReserved2;
		for(x=0;x<sizeof(short);x++)
			putc(dataptr[x],bmpfile);
		dataptr=(unsigned char*)&bmp.bfh.bfOffBits;
		for(x=0;x<sizeof(long);x++)
			putc(dataptr[x],bmpfile);
		dataptr=(unsigned char*)&bmp.bih.biSize;
		for(x=0;x<sizeof(BITMAPINFOHEADER);x++)
			putc(dataptr[x],bmpfile);
		dataptr=&bmp.pdata[0];
		for(x=0;x<1024;x++)
			putc(dataptr[x],bmpfile);
		dataptr=&bmp.data[0];
		for(x=0;x<bmplist[count].width*bmplist[count].height;x++)
			putc(dataptr[x],bmpfile);
		free(bmp.data);
		fclose(bmpfile);
	}
	printf("freeing resources\n");
	for(count=0;count<bmpnum;count++)
	{
		if(bmplist[count].olddata)
			free(bmplist[count].olddata);
		if(bmplist[count].newdata)
			free(bmplist[count].newdata);
	}
	free(bmplist);
	free(palette);
error1:;
	printf("program complete\n");
	return 0;
}

void GetNumber(FILE* file,int* number)
{
	unsigned char data;
	int retval;

	data=getc(file);
	if(data=='*')
	{
		while(data!=10)
			data=getc(file);
		GetNumber(file,number);
		return;
	}
	retval=0;
	while(data!=13)
	{
		retval*=10;
		retval+=(int)(data-'0');
		data=getc(file);
	}
	getc(file);
	*number=retval;
	return;
}

void GetString(FILE* file,char* string)
{
	unsigned char data;
	int strpos;

	data=getc(file);
	if(data=='*')
	{
		while(data!=10)
			data=getc(file);
		GetString(file,string);
		return;
	}
	strpos=0;
	while(data!=13)
	{
		string[strpos++]=data;
		data=getc(file);
	}
	getc(file);
	string[strpos]=0;
	return;
}

// END //
