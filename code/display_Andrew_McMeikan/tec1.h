#ifndef _TEC1HEADER

#define _TEC1HEADER

#define DIGITDRIVE 0x01 //active high bits 0-5 digits, 6-NC, 7 speaker
#define SEGMENTDRIVE 0x02	//active high segments
#define SPEAKER 7


// see https://github.com/tec1group/TechNotes/blob/main/TEC-1%207%20Segment%20Codes.xlsx
// for 7 segment data generation

const unsigned char HexNumbers[]={	
                                        0xEB, 0X28, 0XCD, 0XAD,         //0,1,2,3
                                        0X2E, 0XA7, 0XE7, 0X29,         //4,5,6,7
                                        0XEF, 0X2F, 0X6F, 0XE6,         //8,9,A,b
                                        0XC3, 0XEC, 0XC7, 0X47          //C,d,E,F
                                        };
                                        
const unsigned char Letters[]={
                                        0xAF, 0x66, 0x40, 0xE8,         //g,h,i,j
                                        0x6E, 0xC2, 0x6B, 0x64,         //H,L,M,n
                                        0xE4, 0x4F, 0x3F, 0x44,         //o,p,q,r
                                        0xA7, 0xC6, 0xE0, 0xEA,          //S,t,u,V
                                        0xE2, 0x6E, 0x2E, 0xCD          //W,X,Y,Z
        };   



unsigned char digitLookup( char );
void scanDisplay( char* buff );
void scrollDisplay( char* buff );


//functions must always preserve IX

/*

See: https://gist.github.com/Konamiman/af5645b9998c802753023cf1be8a2970

The return value of a C function is passed to the caller as follows:

    Functions that return char: in the L register
    Functions that return int or a pointer: in the HL registers
    Functions that return long: in the DEHL registers


Parameters for the function are pushed to the stack before the function is called. They are pushed from right to left, so the leftmost parameter is the first one found when going up in the stack

__naked eliminates automatic ret from being generated

__asm and __endasm defines assembly block, __asm__ ("ei\nhalt"); inlines

The Z80 assembler supplied with SDCC uses a pretty much standard syntax for the assembler source code except for the following:

    Decimal numeric constants must be preceded with #
    Hexadecimal numeric constants must be preceded with #0x
    The syntax for offsets when using index registers is n(ix), where in other assemblers it's usually (ix+n)




*/


void portOut(unsigned char port, unsigned char value )	__naked
{
	(void) port;	//port will be accessed in asm
	(void) value;	//value will be accessed in asm
  __asm
   	ld iy,#2
    add iy,sp 	;Bypass the return address of the function 

    ld c,(iy)   ;port
    ld b,1(iy)  ;value
	out (c),b
	ret
  __endasm;

}





#endif
