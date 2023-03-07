
#include "tec1.h"

                                                             
								
//volatile __data __at (0xDF00) unsigned char USBFIFO;								



void main(void)
{
	int i;
	for ( i = 0; i < 10000; i++)
	{
		scrollDisplay("TEC-1 SCROLL 0123456789ABCDEF01234");
	}
	return;
}


void scanDisplay( char* buff )
{
	static unsigned char display[8] = {32,16,8,4,2,1,0,128};
	static unsigned char sound = 0;
	static int j = 0;
        unsigned char segs;

        segs=  digitLookup( buff[j]);
        portOut( SEGMENTDRIVE,0);
        portOut( DIGITDRIVE, display[ j ] );	
        portOut( SEGMENTDRIVE, segs );

        
        
        j++;
        if  ( 5 < j ) 
        {
                j = 0;
        }
      
        
}

unsigned char digitLookup( char c )
{
        if (( c >= 'A' ) && ( c <= 'Z' ))
        {
                if ( c < 'G' )
                {
                        return HexNumbers[ c - 'A' +10];
                }
                else 
                {
                        return Letters[ c - 'G'];
                }
        }
        else if (( c >= '0' ) && ( c <= '9' ))
        {
                return HexNumbers[ c- '0'];
        }
        else
        {
                switch (c)
                {
                        case '-': return 0x04;
                        case ' ': return 0;
                        
                        
                        default: return 0;
                }
        }
  
}

void scrollDisplay( char* buff )
{
        const int rate = 100;
        static int scancount = 0;
        
        static int offset = 0;
        
        scanDisplay( &buff[offset] );
        scancount++;
        if (scancount >= rate )
        {
                scancount = 0;
                if (buff[offset+6])
                {
                        offset++;
                }
                else
                {
                        offset = 0;
                }
        }
        
}
