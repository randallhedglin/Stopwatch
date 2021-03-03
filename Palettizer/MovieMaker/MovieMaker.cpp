// MovieMaker.cpp : Defines the entry point for the console application.
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
 
void GetNumber(FILE* file,long* number);
void GetString(FILE* file,char* string);
void FlipBitmap(unsigned char* bmpdata,long width,long height);
void EncodeBitmap(unsigned char* bmpdata,long width,long height,FILE* outputfile);
void DetermineDifferences(unsigned char* oldframe,unsigned char* newframe,long width,long height,unsigned char* newdata);
void EncodeDiffBitmap(unsigned char* newdata,long width,long height,FILE* outputfile);

int main(int argc, char* argv[])
{
	char* filename;
	FILE* file;
	unsigned char *frame1;
	unsigned char *frame2;
	unsigned char *newdata;
	char path[256];
	char outputname[20];
	char extension[20];
	long width,height,numbmps;
	FILE *outputfile;
	char bmpname[20];
	FILE *bmpfile;
	char tempstr[256];
	long count,count2;

	if(argc==1)
		filename="pspin.mmk";
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
	GetString(file,outputname);
	GetString(file,extension);
	GetNumber(file,&width);
	GetNumber(file,&height);
	GetNumber(file,&numbmps);
	printf("allocating memory\n");
	frame1=(unsigned char*)malloc(width*height);
	frame2=(unsigned char*)malloc(width*height);
	newdata=(unsigned char*)malloc(width*height*2);
	if(!frame1||!frame2||!newdata)
	{
		printf("-- out of memory\n");
		goto error2;
	}
	memset(frame1,0,width*height);
	memset(frame2,0,width*height);
	memset(newdata,0,width*height*2);
	strcpy(tempstr,path);
	strcat(tempstr,outputname);
	strcat(tempstr,".mve");
	printf("opening output file %s\n",tempstr);
	outputfile=fopen(tempstr,"wb");
	if(!outputfile)
	{
		printf("-- file not found\n");
		goto error2;
	}
	GetString(file,bmpname);
	strcpy(tempstr,path);
	strcat(tempstr,bmpname);
	strcat(tempstr,".");
	strcat(tempstr,extension);
	printf("initializing with file %s\n",tempstr);
	bmpfile=fopen(tempstr,"rb");
	if(!bmpfile)
	{
		printf("-- file not found\n");
		goto error3;
	}
	fwrite(&width,sizeof(long),1,outputfile);
	fwrite(&height,sizeof(long),1,outputfile);
	fwrite(&numbmps,sizeof(long),1,outputfile);
	for(count=0;count<54;count++)
		getc(bmpfile);
	for(count=0;count<256;count++)
	{
		unsigned char r=getc(bmpfile);
		unsigned char g=getc(bmpfile);
		unsigned char b=getc(bmpfile);
		getc(bmpfile);
		if(feof(bmpfile))
			printf(" -- eof error\n");
		//printf("%d:(%d,%d,%d)\n",count,r,g,b);
		putc(r,outputfile);
		putc(g,outputfile);
		putc(b,outputfile);
	}
	for(count=0;count<width*height;count++)
		frame1[count]=getc(bmpfile);
	FlipBitmap(frame1,width,height);
	EncodeBitmap(frame1,width,height,outputfile);
	printf("  complete\n");
	fclose(bmpfile);
	for(count2=1;count2<numbmps;count2++)
	{
		GetString(file,bmpname);
		strcpy(tempstr,path);
		strcat(tempstr,bmpname);
		strcat(tempstr,".");
		strcat(tempstr,extension);
		printf("encoding file %s\n",tempstr);
		bmpfile=fopen(tempstr,"rb");
		if(!bmpfile)
		{
			printf("-- file not found\n");
			goto error3;
		}
		for(count=0;count<1078;count++)
			getc(bmpfile);
		for(count=0;count<width*height;count++)
			frame2[count]=getc(bmpfile);
		FlipBitmap(frame2,width,height);
		DetermineDifferences(frame1,frame2,width,height,newdata);
		EncodeDiffBitmap(newdata,width,height,outputfile);
		memcpy(frame1,frame2,width*height);
		printf("  complete\n");
		fclose(bmpfile);
	}
error3:;
	printf("closing output file\n");
	fclose(outputfile);
error2:;
	printf("freeing resources\n");
	free(frame1);
	free(frame2);
	free(newdata);
	printf("closing file %s\n",filename);
	fclose(file);
error1:;
	printf("program complete\n");
	return 0;
}

void GetNumber(FILE* file,long* number)
{
	unsigned char data;
	long retval;

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
		retval+=(long)(data-'0');
		data=getc(file);
	}
	getc(file);
	*number=retval;
	return;
}

void GetString(FILE* file,char* string)
{
	unsigned char data;
	long strpos;

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

void FlipBitmap(unsigned char* bmpdata,long width,long height)
{
	long x,y;
	unsigned char* tempbuffer;

	tempbuffer=(unsigned char*)malloc(width*height);
	if(!tempbuffer)
	{
		printf("-- out of memory\n");
		return;
	}
	memcpy(tempbuffer,bmpdata,width*height);
	for(y=0;y<height;y++)
		for(x=0;x<width;x++)
			bmpdata[x+(y*width)]=tempbuffer[x+(((height-1)-y)*width)];
	free(tempbuffer);
}

void EncodeBitmap(unsigned char* bmpdata,long width,long height,FILE* outputfile)
{
	long ReadPos=0;
	char ExitFlag=0;
	long StopPos=width*height;
	long count,c2;
	long ReadPos2;
		
	while(ReadPos<StopPos)
	{
		count=0;
		while(bmpdata[ReadPos]==0)
			if(ReadPos>=StopPos)
			{
				ExitFlag=1;
				break;
			}
			else
			{
				ReadPos++;
				count++;
			}
		if(ExitFlag)
			break;
		if(count>255)
			while(count>255)
			{
				putc(255,outputfile);
				putc(0,outputfile);
				count-=255;
			}
		putc(count,outputfile);
		if(ReadPos>=StopPos)
			break;
		ReadPos2=ReadPos;
		count=0;
		while(bmpdata[ReadPos]!=0)
			if(ReadPos>=StopPos)
			{
				ExitFlag=2;
				break;
			}
			else
			{
				ReadPos++;
				count++;
			}
		if(ExitFlag)
			break;
		if(count>255)
			while(count>255)
			{
				putc(255,outputfile);
				for(c2=0;c2<255;c2++)
					putc(bmpdata[ReadPos2++],outputfile);
				putc(0,outputfile);
				count-=255;
			}
		putc(count,outputfile);
		if(count)
			for(c2=0;c2<count;c2++)
				putc(bmpdata[ReadPos2++],outputfile);
	}
	if(ExitFlag==1)
	{
		if(count>255)
			while(count>255)
			{
				putc(255,outputfile);
				putc(0,outputfile);
				count-=255;
			}
		putc(count,outputfile);
	}
	else if(ExitFlag==2)
	{
		if(count>255)
			while(count>255)
			{
				putc(255,outputfile);
				for(c2=0;c2<255;c2++)
					putc(bmpdata[ReadPos2++],outputfile);
				putc(0,outputfile);
				count-=255;
			}
		putc(count,outputfile);
		if(count)
			for(c2=0;c2<count;c2++)
				putc(bmpdata[ReadPos2++],outputfile);
	}
}

void DetermineDifferences(unsigned char* oldframe,unsigned char* newframe,long width,long height,unsigned char* newdata)
{
	long x,y;

	for(y=0;y<height;y++)
		for(x=0;x<width;x++)
			if(oldframe[x+(y*width)]==newframe[x+(y*width)])
				newdata[(x+(y*width))*2]=0;
			else
			{
				newdata[(x+(y*width))*2]=1;
				newdata[((x+(y*width))*2)+1]=newframe[x+(y*width)];
			}
}

void EncodeDiffBitmap(unsigned char* newdata,long width,long height,FILE* outputfile)
{
	long ReadPos=0;
	char ExitFlag=0;
	long StopPos=width*height;
	long count,c2;
	long ReadPos2;
		
	while(ReadPos<StopPos)
	{
		count=0;
		while(newdata[ReadPos*2]==0)
			if(ReadPos>=StopPos)
			{
				ExitFlag=1;
				break;
			}
			else
			{
				ReadPos++;
				count++;
			}
		if(ExitFlag)
			break;
		if(count>255)
			while(count>255)
			{
				putc(255,outputfile);
				putc(0,outputfile);
				count-=255;
			}
		putc(count,outputfile);
		if(ReadPos>=StopPos)
			break;
		ReadPos2=ReadPos;
		count=0;
		while(newdata[ReadPos*2]!=0)
			if(ReadPos>=StopPos)
			{
				ExitFlag=2;
				break;
			}
			else
			{
				ReadPos++;
				count++;
			}
		if(ExitFlag)
			break;
		if(count>255)
			while(count>255)
			{
				putc(255,outputfile);
				for(c2=0;c2<255;c2++)
					putc(newdata[((ReadPos2++)*2)+1],outputfile);
				putc(0,outputfile);
				count-=255;
			}
		putc(count,outputfile);
		if(count)
			for(c2=0;c2<count;c2++)
				putc(newdata[((ReadPos2++)*2)+1],outputfile);
	}
	if(ExitFlag==1)
	{
		if(count>255)
			while(count>255)
			{
				putc(255,outputfile);
				putc(0,outputfile);
				count-=255;
			}
		putc(count,outputfile);
	}
	else if(ExitFlag==2)
	{
		if(count>255)
			while(count>255)
			{
				putc(255,outputfile);
				for(c2=0;c2<255;c2++)
					putc(newdata[((ReadPos2++)*2)+1],outputfile);
				putc(0,outputfile);
				count-=255;
			}
		putc(count,outputfile);
		if(count)
			for(c2=0;c2<count;c2++)
				putc(newdata[((ReadPos2++)*2)+1],outputfile);
	}
}

// END //
