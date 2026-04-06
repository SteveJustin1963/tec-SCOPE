# basic
```
' Constants for encoder setup
Const SERIAL_PORT = 1      ' Serial port for output
' Altitude encoder ports and bits
Const ALT_PORT_A = 2       ' Alt encoder channel A port
Const ALT_PORT_B = 3       ' Alt encoder channel B port
Const ALT_BIT_A = 0        ' Alt encoder channel A bit
Const ALT_BIT_B = 1        ' Alt encoder channel B bit
' Azimuth encoder ports and bits
Const AZ_PORT_A = 4        ' Az encoder channel A port
Const AZ_PORT_B = 5        ' Az encoder channel B port
Const AZ_BIT_A = 0         ' Az encoder channel A bit
Const AZ_BIT_B = 1         ' Az encoder channel B bit

' Encoder specifications
Const PULSES_PER_REV = 600     ' Base encoder resolution
Const GEAR_RATIO = 10          ' Gear reduction ratio

' Conversion factors - THESE NEED TO BE CALIBRATED!
' How many geared encoder counts for full range
Const ALT_COUNTS_90DEG = 15000  ' Counts for 90 degrees in altitude
Const AZ_COUNTS_180DEG = 30000  ' Counts for 180 degrees in azimuth

' Variables for both encoders
Dim AltA As Bit            ' Current state of Alt channel A
Dim AltB As Bit           ' Current state of Alt channel B
Dim AzA As Bit            ' Current state of Az channel A
Dim AzB As Bit            ' Current state of Az channel B
Dim AltCount As Long      ' Raw Alt encoder count
Dim AzCount As Long       ' Raw Az encoder count
Dim PrevAltA As Bit       ' Previous state of Alt channel A
Dim PrevAltB As Bit       ' Previous state of Alt channel B
Dim PrevAzA As Bit        ' Previous state of Az channel A
Dim PrevAzB As Bit        ' Previous state of Az channel B
Dim Temp As UShort        ' Temporary variable for port reading

' Variables for angle calculations
Dim AltAngle As Single    ' Calculated altitude angle
Dim AzAngle As Single     ' Calculated azimuth angle
Dim Degrees As Integer
Dim Minutes As Integer
Dim Seconds As Single
Dim TempAngle As Single
Dim NumStr As String      ' For number conversion

' Initialize all variables
AltCount = 0
AzCount = 0
PrevAltA = 0
PrevAltB = 0
PrevAzA = 0
PrevAzB = 0

' Main loop
mainloop:
    ' Read Altitude Encoder
    Temp = Get(ALT_PORT_A)
    AltA = TestBit(ALT_BIT_A, Temp)
    Temp = Get(ALT_PORT_B)
    AltB = TestBit(ALT_BIT_B, Temp)

    ' Read Azimuth Encoder
    Temp = Get(AZ_PORT_A)
    AzA = TestBit(AZ_BIT_A, Temp)
    Temp = Get(AZ_PORT_B)
    AzB = TestBit(AZ_BIT_B, Temp)

    ' Process Altitude changes
    If AltA <> PrevAltA Or AltB <> PrevAltB Then
        ' Determine direction
        If AltA = 1 And AltB = 0 Then
            AltCount = AltCount + 1
        Endif
        If AltA = 0 And AltB = 1 Then
            AltCount = AltCount - 1
        Endif

        ' Convert count to degrees (90 degree range)
        AltAngle = (AltCount * 90.0) / ALT_COUNTS_90DEG
        ' Limit altitude range 0-90
        If AltAngle > 90 Then
            AltAngle = 90
            AltCount = ALT_COUNTS_90DEG
        Endif
        If AltAngle < 0 Then
            AltAngle = 0
            AltCount = 0
        Endif

        ' Convert to deg/min/sec
        TempAngle = AltAngle
        Degrees = TempAngle
        TempAngle = (TempAngle - Degrees) * 60
        Minutes = TempAngle
        Seconds = (TempAngle - Minutes) * 60

        ' Output Altitude
        Put SERIAL_PORT, Asc("A")
        Put SERIAL_PORT, Asc("l")
        Put SERIAL_PORT, Asc("t")
        Put SERIAL_PORT, Asc(":")
        Put SERIAL_PORT, Asc(" ")

        ' Degrees
        NumStr = #Degrees
        Dim i As UShort
        For i = 0 To Len(NumStr) - 1
            Put SERIAL_PORT, NumStr(i)
        Next i
        Put SERIAL_PORT, Asc("°")

        ' Minutes
        Put SERIAL_PORT, Asc(" ")
        NumStr = #Minutes
        For i = 0 To Len(NumStr) - 1
            Put SERIAL_PORT, NumStr(i)
        Next i
        Put SERIAL_PORT, Asc("'")

        ' Seconds
        Put SERIAL_PORT, Asc(" ")
        NumStr = #Seconds
        For i = 0 To Len(NumStr) - 1
            Put SERIAL_PORT, NumStr(i)
        Next i
        Put SERIAL_PORT, Asc("""")

        Put SERIAL_PORT, 13  ' CR
        Put SERIAL_PORT, 10  ' LF

        PrevAltA = AltA
        PrevAltB = AltB
    Endif

    ' Process Azimuth changes
    If AzA <> PrevAzA Or AzB <> PrevAzB Then
        ' Determine direction
        If AzA = 1 And AzB = 0 Then
            AzCount = AzCount + 1
        Endif
        If AzA = 0 And AzB = 1 Then
            AzCount = AzCount - 1
        Endif

        ' Convert count to degrees (180 degree range)
        AzAngle = (AzCount * 180.0) / AZ_COUNTS_180DEG
        ' Limit azimuth range 0-180
        If AzAngle > 180 Then
            AzAngle = 180
            AzCount = AZ_COUNTS_180DEG
        Endif
        If AzAngle < 0 Then
            AzAngle = 0
            AzCount = 0
        Endif

        ' Convert to deg/min/sec
        TempAngle = AzAngle
        Degrees = TempAngle
        TempAngle = (TempAngle - Degrees) * 60
        Minutes = TempAngle
        Seconds = (TempAngle - Minutes) * 60

        ' Output Azimuth
        Put SERIAL_PORT, Asc("A")
        Put SERIAL_PORT, Asc("z")
        Put SERIAL_PORT, Asc(":")
        Put SERIAL_PORT, Asc(" ")

        ' Degrees
        NumStr = #Degrees
        For i = 0 To Len(NumStr) - 1
            Put SERIAL_PORT, NumStr(i)
        Next i
        Put SERIAL_PORT, Asc("°")

        ' Minutes
        Put SERIAL_PORT, Asc(" ")
        NumStr = #Minutes
        For i = 0 To Len(NumStr) - 1
            Put SERIAL_PORT, NumStr(i)
        Next i
        Put SERIAL_PORT, Asc("'")

        ' Seconds
        Put SERIAL_PORT, Asc(" ")
        NumStr = #Seconds
        For i = 0 To Len(NumStr) - 1
            Put SERIAL_PORT, NumStr(i)
        Next i
        Put SERIAL_PORT, Asc("""")

        Put SERIAL_PORT, 13  ' CR
        Put SERIAL_PORT, 10  ' LF

        PrevAzA = AzA
        PrevAzB = AzB
    Endif

    ' Small delay
    For i = 0 To 100
        ' Empty loop for delay
    Next i

    Goto mainloop
End

```


# asm 
```
; Compiled with: Z80 Simulator IDE v14.67
; Microprocessor model: Z80
; Clock frequency: 4.0MHz
;
;       The value of 'SERIAL_PORT' (global) is 1
;       The value of 'ALT_PORT_A' (global) is 2
;       The value of 'ALT_PORT_B' (global) is 3
;       The value of 'ALT_BIT_A' (global) is 0
;       The value of 'ALT_BIT_B' (global) is 1
;       The value of 'AZ_PORT_A' (global) is 4
;       The value of 'AZ_PORT_B' (global) is 5
;       The value of 'AZ_BIT_A' (global) is 0
;       The value of 'AZ_BIT_B' (global) is 1
;       The value of 'PULSES_PER_REV' (global) is 600
;       The value of 'GEAR_RATIO' (global) is 10
;       The value of 'ALT_COUNTS_90DEG' (global) is 15000
;       The value of 'AZ_COUNTS_180DEG' (global) is 30000
;       The address of 'AltA' (boolean) (global) is 0xFEED (IX-19)
;       The address of 'AltB' (boolean) (global) is 0xFEEC (IX-20)
;       The address of 'AzA' (boolean) (global) is 0xFEEB (IX-21)
;       The address of 'AzB' (boolean) (global) is 0xFEEA (IX-22)
;       The address of 'AltCount' (long) (global) is 0xFEFB (IX-5)
;       The address of 'AzCount' (long) (global) is 0xFEF7 (IX-9)
;       The address of 'PrevAltA' (boolean) (global) is 0xFEE9 (IX-23)
;       The address of 'PrevAltB' (boolean) (global) is 0xFEE8 (IX-24)
;       The address of 'PrevAzA' (boolean) (global) is 0xFEE7 (IX-25)
;       The address of 'PrevAzB' (boolean) (global) is 0xFEE6 (IX-26)
;       The address of 'Temp' (ushort) (global) is 0xFEF2 (IX-14)
;       The address of 'AltAngle' (single) (global) is 0xFEE2 (IX-30)
;       The address of 'AzAngle' (single) (global) is 0xFEDE (IX-34)
;       The address of 'Degrees' (integer) (global) is 0xFEF0 (IX-16)
;       The address of 'Minutes' (integer) (global) is 0xFEEE (IX-18)
;       The address of 'Seconds' (single) (global) is 0xFEDA (IX-38)
;       The address of 'TempAngle' (single) (global) is 0xFEF3 (IX-13)
;       The address of 'NumStr' (array 17) (string) (global) is 0xFEC9 (IX-55)
;       The address of 'i' (ushort) (global) is 0xFEFF (IX-1)
	oshonsoft_temp_string_1[1] .EQU 0xFEC7
	PRINT_STR .EQU 0xFEBB
	PRINT_IND .EQU 0xFEBA
	REG_R3 .EQU 0xFEB9
	REG_R2 .EQU 0xFEB8
	REG_R1 .EQU 0xFEB7
	REG_R0 .EQU 0xFEB6
	.ORG 0000H
	LD IX,0FF00H
	LD SP,0FEB1H
; User code start
; 1: ' Constants for encoder setup
; 2: Const SERIAL_PORT = 1      ' Serial port for output
; 3: ' Altitude encoder ports and bits
; 4: Const ALT_PORT_A = 2       ' Alt encoder channel A port
; 5: Const ALT_PORT_B = 3       ' Alt encoder channel B port
; 6: Const ALT_BIT_A = 0        ' Alt encoder channel A bit
; 7: Const ALT_BIT_B = 1        ' Alt encoder channel B bit
; 8: ' Azimuth encoder ports and bits
; 9: Const AZ_PORT_A = 4        ' Az encoder channel A port
; 10: Const AZ_PORT_B = 5        ' Az encoder channel B port
; 11: Const AZ_BIT_A = 0         ' Az encoder channel A bit
; 12: Const AZ_BIT_B = 1         ' Az encoder channel B bit
; 13:
; 14: ' Encoder specifications
; 15: Const PULSES_PER_REV = 600     ' Base encoder resolution
; 16: Const GEAR_RATIO = 10          ' Gear reduction ratio
; 17:
; 18: ' Conversion factors - THESE NEED TO BE CALIBRATED!
; 19: ' How many geared encoder counts for full range
; 20: Const ALT_COUNTS_90DEG = 15000  ' Counts for 90 degrees in altitude
; 21: Const AZ_COUNTS_180DEG = 30000  ' Counts for 180 degrees in azimuth
; 22:
; 23: ' Variables for both encoders
; 24: Dim AltA As Bit            ' Current state of Alt channel A
; 25: Dim AltB As Bit           ' Current state of Alt channel B
; 26: Dim AzA As Bit            ' Current state of Az channel A
; 27: Dim AzB As Bit            ' Current state of Az channel B
; 28: Dim AltCount As Long      ' Raw Alt encoder count
; 29: Dim AzCount As Long       ' Raw Az encoder count
; 30: Dim PrevAltA As Bit       ' Previous state of Alt channel A
; 31: Dim PrevAltB As Bit       ' Previous state of Alt channel B
; 32: Dim PrevAzA As Bit        ' Previous state of Az channel A
; 33: Dim PrevAzB As Bit        ' Previous state of Az channel B
; 34: Dim Temp As UShort        ' Temporary variable for port reading
; 35:
; 36: ' Variables for angle calculations
; 37: Dim AltAngle As Single    ' Calculated altitude angle
; 38: Dim AzAngle As Single     ' Calculated azimuth angle
; 39: Dim Degrees As Integer
; 40: Dim Minutes As Integer
; 41: Dim Seconds As Single
; 42: Dim TempAngle As Single
; 43: Dim NumStr As String      ' For number conversion
	XOR A
	LD (IX-37H),A
; 44:
; 45: ' Initialize all variables
; 46: AltCount = 0
	LD BC,0000H
	LD DE,0000H
	LD (0FEFBH),DE
	LD (0FEFDH),BC
; 47: AzCount = 0
	LD BC,0000H
	LD DE,0000H
	LD (0FEF7H),DE
	LD (0FEF9H),BC
; 48: PrevAltA = 0
	XOR A
	LD (0FEE9H),A
; 49: PrevAltB = 0
	XOR A
	LD (0FEE8H),A
; 50: PrevAzA = 0
	XOR A
	LD (0FEE7H),A
; 51: PrevAzB = 0
	XOR A
	LD (0FEE6H),A
; 52:
; 53: ' Main loop
; 54: mainloop:
L0001:
; 55: ' Read Altitude Encoder
; 56: Temp = Get(ALT_PORT_A)
	IN A,(02H)
	LD (0FEF2H),A
; 57: AltA = TestBit(ALT_BIT_A, Temp)
	LD A,(0FEF2H)
	AND 01H
	JR NZ,$+2+3
	XOR A
	JR Z,$+2+2
	LD A,0FFH
	LD (0FEEDH),A
; 58: Temp = Get(ALT_PORT_B)
	IN A,(03H)
	LD (0FEF2H),A
; 59: AltB = TestBit(ALT_BIT_B, Temp)
	LD A,(0FEF2H)
	AND 02H
	JR NZ,$+2+3
	XOR A
	JR Z,$+2+2
	LD A,0FFH
	LD (0FEECH),A
; 60:
; 61: ' Read Azimuth Encoder
; 62: Temp = Get(AZ_PORT_A)
	IN A,(04H)
	LD (0FEF2H),A
; 63: AzA = TestBit(AZ_BIT_A, Temp)
	LD A,(0FEF2H)
	AND 01H
	JR NZ,$+2+3
	XOR A
	JR Z,$+2+2
	LD A,0FFH
	LD (0FEEBH),A
; 64: Temp = Get(AZ_PORT_B)
	IN A,(05H)
	LD (0FEF2H),A
; 65: AzB = TestBit(AZ_BIT_B, Temp)
	LD A,(0FEF2H)
	AND 02H
	JR NZ,$+2+3
	XOR A
	JR Z,$+2+2
	LD A,0FFH
	LD (0FEEAH),A
; 66:
; 67: ' Process Altitude changes
; 68: If AltA <> PrevAltA Or AltB <> PrevAltB Then
	LD A,(0FEEDH)
	LD L,(IX-17H)
	SUB L
	JP NZ,L0003
	LD A,(0FEECH)
	LD L,(IX-18H)
	SUB L
	JP NZ,L0003
	JP L0002
L0003:
; 69: ' Determine direction
; 70: If AltA = 1 And AltB = 0 Then
	LD A,(0FEEDH)
	LD L,0FFH
	SUB L
	JP NZ,L0004
	LD A,(0FEECH)
	LD L,00H
	SUB L
	JP NZ,L0004
; 71: AltCount = AltCount + 1
	LD DE,(0FEFBH)
	LD BC,(0FEFDH)
	PUSH BC
	PUSH DE
	LD BC,0000H
	LD DE,0001H
	CALL A101
	LD (0FEFBH),DE
	LD (0FEFDH),BC
; 72: Endif
L0004:
; 73: If AltA = 0 And AltB = 1 Then
	LD A,(0FEEDH)
	LD L,00H
	SUB L
	JP NZ,L0005
	LD A,(0FEECH)
	LD L,0FFH
	SUB L
	JP NZ,L0005
; 74: AltCount = AltCount - 1
	LD DE,(0FEFBH)
	LD BC,(0FEFDH)
	PUSH BC
	PUSH DE
	LD BC,0000H
	LD DE,0001H
	OR A
	CALL S101
	LD (0FEFBH),DE
	LD (0FEFDH),BC
; 75: Endif
L0005:
; 76:
; 77: ' Convert count to degrees (90 degree range)
; 78: AltAngle = (AltCount * 90.0) / ALT_COUNTS_90DEG
	oshonsoft_temp_single_2 .EQU 0xFEB2
	LD DE,(0FEFBH)
	LD BC,(0FEFDH)
	CALL X101
	PUSH BC
	PUSH DE
	LD BC,42B4H
	LD DE,0000H
	CALL M201
	LD (0FEB2H),DE
	LD (0FEB4H),BC
	LD DE,(0FEB2H)
	LD BC,(0FEB4H)
	PUSH BC
	PUSH DE
	LD BC,466AH
	LD DE,6000H
	CALL D201
	LD (0FEE2H),DE
	LD (0FEE4H),BC
; 79: ' Limit altitude range 0-90
; 80: If AltAngle > 90 Then
	LD DE,(0FEE2H)
	LD BC,(0FEE4H)
	PUSH BC
	PUSH DE
	LD BC,42B4H
	LD DE,0000H
	CALL C202
	JP M,L0006
	JP Z,L0006
; 81: AltAngle = 90
	LD BC,42B4H
	LD DE,0000H
	LD (0FEE2H),DE
	LD (0FEE4H),BC
; 82: AltCount = ALT_COUNTS_90DEG
	LD BC,0000H
	LD DE,3A98H
	LD (0FEFBH),DE
	LD (0FEFDH),BC
; 83: Endif
L0006:
; 84: If AltAngle < 0 Then
	LD DE,(0FEE2H)
	LD BC,(0FEE4H)
	PUSH BC
	PUSH DE
	LD BC,0000H
	LD DE,0000H
	CALL C202
	JP P,L0007
; 85: AltAngle = 0
	LD BC,0000H
	LD DE,0000H
	LD (0FEE2H),DE
	LD (0FEE4H),BC
; 86: AltCount = 0
	LD BC,0000H
	LD DE,0000H
	LD (0FEFBH),DE
	LD (0FEFDH),BC
; 87: Endif
L0007:
; 88:
; 89: ' Convert to deg/min/sec
; 90: TempAngle = AltAngle
	LD DE,(0FEE2H)
	LD BC,(0FEE4H)
	LD (0FEF3H),DE
	LD (0FEF5H),BC
; 91: Degrees = TempAngle
	LD DE,(0FEF3H)
	LD BC,(0FEF5H)
	CALL X001
	LD (0FEF0H),DE
; 92: TempAngle = (TempAngle - Degrees) * 60
	LD DE,(0FEF3H)
	LD BC,(0FEF5H)
	PUSH BC
	PUSH DE
	LD DE,(0FEF0H)
	LD A,D
	RLCA
	SBC A,A
	LD C,A
	LD B,A
	CALL X101
	CALL S201
	LD (0FEB2H),DE
	LD (0FEB4H),BC
	LD DE,(0FEB2H)
	LD BC,(0FEB4H)
	PUSH BC
	PUSH DE
	LD BC,4270H
	LD DE,0000H
	CALL M201
	LD (0FEF3H),DE
	LD (0FEF5H),BC
; 93: Minutes = TempAngle
	LD DE,(0FEF3H)
	LD BC,(0FEF5H)
	CALL X001
	LD (0FEEEH),DE
; 94: Seconds = (TempAngle - Minutes) * 60
	LD DE,(0FEF3H)
	LD BC,(0FEF5H)
	PUSH BC
	PUSH DE
	LD DE,(0FEEEH)
	LD A,D
	RLCA
	SBC A,A
	LD C,A
	LD B,A
	CALL X101
	CALL S201
	LD (0FEB2H),DE
	LD (0FEB4H),BC
	LD DE,(0FEB2H)
	LD BC,(0FEB4H)
	PUSH BC
	PUSH DE
	LD BC,4270H
	LD DE,0000H
	CALL M201
	LD (0FEDAH),DE
	LD (0FEDCH),BC
; 95:
; 96: ' Output Altitude
; 97: Put SERIAL_PORT, Asc("A")
	LD B,01H
	LD HL,0FEC7H
	LD A,41H
	CALL SG01
	oshonsoft_temp_ushort_3 .EQU 0xFEB1
	LD HL,0FEC7H
	CALL SG03
	LD (IX-4FH),B
	LD A,(0FEB1H)
	OUT (01H),A
; 98: Put SERIAL_PORT, Asc("l")
	LD B,01H
	LD HL,0FEC7H
	LD A,6CH
	CALL SG01
	LD HL,0FEC7H
	CALL SG03
	LD (IX-4FH),B
	LD A,(0FEB1H)
	OUT (01H),A
; 99: Put SERIAL_PORT, Asc("t")
	LD B,01H
	LD HL,0FEC7H
	LD A,74H
	CALL SG01
	LD HL,0FEC7H
	CALL SG03
	LD (IX-4FH),B
	LD A,(0FEB1H)
	OUT (01H),A
; 100: Put SERIAL_PORT, Asc(":")
	LD B,01H
	LD HL,0FEC7H
	LD A,3AH
	CALL SG01
	LD HL,0FEC7H
	CALL SG03
	LD (IX-4FH),B
	LD A,(0FEB1H)
	OUT (01H),A
; 101: Put SERIAL_PORT, Asc(" ")
	LD B,01H
	LD HL,0FEC7H
	LD A,20H
	CALL SG01
	LD HL,0FEC7H
	CALL SG03
	LD (IX-4FH),B
	LD A,(0FEB1H)
	OUT (01H),A
; 102:
; 103: ' Degrees
; 104: NumStr = #Degrees
	LD DE,(0FEF0H)
	CALL P101
	LD B,10H
	LD HL,0FEC9H
	LD DE,PRINT_STR
	CALL SG10
; 105: Dim i As UShort
; 106: For i = 0 To Len(NumStr) - 1
	XOR A
	LD (0FEFFH),A
L0008:
	LD B,10H
	LD DE,0FEC9H
	CALL SG05
	LD (IX-4FH),C
	LD A,(0FEB1H)
	LD L,01H
	SUB L
	LD (0FEB1H),A
	LD A,(0FEFFH)
	LD L,(IX-4FH)
	SCF
	SBC A,L
	JP NC,L0009
; 107: Put SERIAL_PORT, NumStr(i)
	LD E,(IX-01H)
	LD D,00H
	LD HL,0FEC9H
	ADD HL,DE
	LD A,(HL)
	OUT (01H),A
; 108: Next i
	LD A,(0FEFFH)
	LD L,01H
	ADD A,L
	LD (0FEFFH),A
	JR C,L0009
	JP L0008
L0009:
; 109: Put SERIAL_PORT, Asc("°")
	LD B,01H
	LD HL,0FEC7H
	LD A,0B0H
	CALL SG01
	LD HL,0FEC7H
	CALL SG03
	LD (IX-4FH),B
	LD A,(0FEB1H)
	OUT (01H),A
; 110:
; 111: ' Minutes
; 112: Put SERIAL_PORT, Asc(" ")
	LD B,01H
	LD HL,0FEC7H
	LD A,20H
	CALL SG01
	LD HL,0FEC7H
	CALL SG03
	LD (IX-4FH),B
	LD A,(0FEB1H)
	OUT (01H),A
; 113: NumStr = #Minutes
	LD DE,(0FEEEH)
	CALL P101
	LD B,10H
	LD HL,0FEC9H
	LD DE,PRINT_STR
	CALL SG10
; 114: For i = 0 To Len(NumStr) - 1
	XOR A
	LD (0FEFFH),A
L0010:
	LD B,10H
	LD DE,0FEC9H
	CALL SG05
	LD (IX-4FH),C
	LD A,(0FEB1H)
	LD L,01H
	SUB L
	LD (0FEB1H),A
	LD A,(0FEFFH)
	LD L,(IX-4FH)
	SCF
	SBC A,L
	JP NC,L0011
; 115: Put SERIAL_PORT, NumStr(i)
	LD E,(IX-01H)
	LD D,00H
	LD HL,0FEC9H
	ADD HL,DE
	LD A,(HL)
	OUT (01H),A
; 116: Next i
	LD A,(0FEFFH)
	LD L,01H
	ADD A,L
	LD (0FEFFH),A
	JR C,L0011
	JP L0010
L0011:
; 117: Put SERIAL_PORT, Asc("'")
	LD B,01H
	LD HL,0FEC7H
	LD A,27H
	CALL SG01
	LD HL,0FEC7H
	CALL SG03
	LD (IX-4FH),B
	LD A,(0FEB1H)
	OUT (01H),A
; 118:
; 119: ' Seconds
; 120: Put SERIAL_PORT, Asc(" ")
	LD B,01H
	LD HL,0FEC7H
	LD A,20H
	CALL SG01
	LD HL,0FEC7H
	CALL SG03
	LD (IX-4FH),B
	LD A,(0FEB1H)
	OUT (01H),A
; 121: NumStr = #Seconds
	LD DE,(0FEDAH)
	LD BC,(0FEDCH)
	CALL P501
	LD B,10H
	LD HL,0FEC9H
	LD DE,PRINT_STR
	CALL SG10
; 122: For i = 0 To Len(NumStr) - 1
	XOR A
	LD (0FEFFH),A
L0012:
	LD B,10H
	LD DE,0FEC9H
	CALL SG05
	LD (IX-4FH),C
	LD A,(0FEB1H)
	LD L,01H
	SUB L
	LD (0FEB1H),A
	LD A,(0FEFFH)
	LD L,(IX-4FH)
	SCF
	SBC A,L
	JP NC,L0013
; 123: Put SERIAL_PORT, NumStr(i)
	LD E,(IX-01H)
	LD D,00H
	LD HL,0FEC9H
	ADD HL,DE
	LD A,(HL)
	OUT (01H),A
; 124: Next i
	LD A,(0FEFFH)
	LD L,01H
	ADD A,L
	LD (0FEFFH),A
	JR C,L0013
	JP L0012
L0013:
; 125: Put SERIAL_PORT, Asc("""")
	LD B,01H
	LD HL,0FEC7H
	LD A,22H
	CALL SG01
	LD HL,0FEC7H
	CALL SG03
	LD (IX-4FH),B
	LD A,(0FEB1H)
	OUT (01H),A
; 126:
; 127: Put SERIAL_PORT, 13  ' CR
	LD A,0DH
	OUT (01H),A
; 128: Put SERIAL_PORT, 10  ' LF
	LD A,0AH
	OUT (01H),A
; 129:
; 130: PrevAltA = AltA
	LD A,(0FEEDH)
	LD (0FEE9H),A
; 131: PrevAltB = AltB
	LD A,(0FEECH)
	LD (0FEE8H),A
; 132: Endif
L0002:
; 133:
; 134: ' Process Azimuth changes
; 135: If AzA <> PrevAzA Or AzB <> PrevAzB Then
	LD A,(0FEEBH)
	LD L,(IX-19H)
	SUB L
	JP NZ,L0015
	LD A,(0FEEAH)
	LD L,(IX-1AH)
	SUB L
	JP NZ,L0015
	JP L0014
L0015:
; 136: ' Determine direction
; 137: If AzA = 1 And AzB = 0 Then
	LD A,(0FEEBH)
	LD L,0FFH
	SUB L
	JP NZ,L0016
	LD A,(0FEEAH)
	LD L,00H
	SUB L
	JP NZ,L0016
; 138: AzCount = AzCount + 1
	LD DE,(0FEF7H)
	LD BC,(0FEF9H)
	PUSH BC
	PUSH DE
	LD BC,0000H
	LD DE,0001H
	CALL A101
	LD (0FEF7H),DE
	LD (0FEF9H),BC
; 139: Endif
L0016:
; 140: If AzA = 0 And AzB = 1 Then
	LD A,(0FEEBH)
	LD L,00H
	SUB L
	JP NZ,L0017
	LD A,(0FEEAH)
	LD L,0FFH
	SUB L
	JP NZ,L0017
; 141: AzCount = AzCount - 1
	LD DE,(0FEF7H)
	LD BC,(0FEF9H)
	PUSH BC
	PUSH DE
	LD BC,0000H
	LD DE,0001H
	OR A
	CALL S101
	LD (0FEF7H),DE
	LD (0FEF9H),BC
; 142: Endif
L0017:
; 143:
; 144: ' Convert count to degrees (180 degree range)
; 145: AzAngle = (AzCount * 180.0) / AZ_COUNTS_180DEG
	LD DE,(0FEF7H)
	LD BC,(0FEF9H)
	CALL X101
	PUSH BC
	PUSH DE
	LD BC,4334H
	LD DE,0000H
	CALL M201
	LD (0FEB2H),DE
	LD (0FEB4H),BC
	LD DE,(0FEB2H)
	LD BC,(0FEB4H)
	PUSH BC
	PUSH DE
	LD BC,46EAH
	LD DE,6000H
	CALL D201
	LD (0FEDEH),DE
	LD (0FEE0H),BC
; 146: ' Limit azimuth range 0-180
; 147: If AzAngle > 180 Then
	LD DE,(0FEDEH)
	LD BC,(0FEE0H)
	PUSH BC
	PUSH DE
	LD BC,4334H
	LD DE,0000H
	CALL C202
	JP M,L0018
	JP Z,L0018
; 148: AzAngle = 180
	LD BC,4334H
	LD DE,0000H
	LD (0FEDEH),DE
	LD (0FEE0H),BC
; 149: AzCount = AZ_COUNTS_180DEG
	LD BC,0000H
	LD DE,7530H
	LD (0FEF7H),DE
	LD (0FEF9H),BC
; 150: Endif
L0018:
; 151: If AzAngle < 0 Then
	LD DE,(0FEDEH)
	LD BC,(0FEE0H)
	PUSH BC
	PUSH DE
	LD BC,0000H
	LD DE,0000H
	CALL C202
	JP P,L0019
; 152: AzAngle = 0
	LD BC,0000H
	LD DE,0000H
	LD (0FEDEH),DE
	LD (0FEE0H),BC
; 153: AzCount = 0
	LD BC,0000H
	LD DE,0000H
	LD (0FEF7H),DE
	LD (0FEF9H),BC
; 154: Endif
L0019:
; 155:
; 156: ' Convert to deg/min/sec
; 157: TempAngle = AzAngle
	LD DE,(0FEDEH)
	LD BC,(0FEE0H)
	LD (0FEF3H),DE
	LD (0FEF5H),BC
; 158: Degrees = TempAngle
	LD DE,(0FEF3H)
	LD BC,(0FEF5H)
	CALL X001
	LD (0FEF0H),DE
; 159: TempAngle = (TempAngle - Degrees) * 60
	LD DE,(0FEF3H)
	LD BC,(0FEF5H)
	PUSH BC
	PUSH DE
	LD DE,(0FEF0H)
	LD A,D
	RLCA
	SBC A,A
	LD C,A
	LD B,A
	CALL X101
	CALL S201
	LD (0FEB2H),DE
	LD (0FEB4H),BC
	LD DE,(0FEB2H)
	LD BC,(0FEB4H)
	PUSH BC
	PUSH DE
	LD BC,4270H
	LD DE,0000H
	CALL M201
	LD (0FEF3H),DE
	LD (0FEF5H),BC
; 160: Minutes = TempAngle
	LD DE,(0FEF3H)
	LD BC,(0FEF5H)
	CALL X001
	LD (0FEEEH),DE
; 161: Seconds = (TempAngle - Minutes) * 60
	LD DE,(0FEF3H)
	LD BC,(0FEF5H)
	PUSH BC
	PUSH DE
	LD DE,(0FEEEH)
	LD A,D
	RLCA
	SBC A,A
	LD C,A
	LD B,A
	CALL X101
	CALL S201
	LD (0FEB2H),DE
	LD (0FEB4H),BC
	LD DE,(0FEB2H)
	LD BC,(0FEB4H)
	PUSH BC
	PUSH DE
	LD BC,4270H
	LD DE,0000H
	CALL M201
	LD (0FEDAH),DE
	LD (0FEDCH),BC
; 162:
; 163: ' Output Azimuth
; 164: Put SERIAL_PORT, Asc("A")
	LD B,01H
	LD HL,0FEC7H
	LD A,41H
	CALL SG01
	LD HL,0FEC7H
	CALL SG03
	LD (IX-4FH),B
	LD A,(0FEB1H)
	OUT (01H),A
; 165: Put SERIAL_PORT, Asc("z")
	LD B,01H
	LD HL,0FEC7H
	LD A,7AH
	CALL SG01
	LD HL,0FEC7H
	CALL SG03
	LD (IX-4FH),B
	LD A,(0FEB1H)
	OUT (01H),A
; 166: Put SERIAL_PORT, Asc(":")
	LD B,01H
	LD HL,0FEC7H
	LD A,3AH
	CALL SG01
	LD HL,0FEC7H
	CALL SG03
	LD (IX-4FH),B
	LD A,(0FEB1H)
	OUT (01H),A
; 167: Put SERIAL_PORT, Asc(" ")
	LD B,01H
	LD HL,0FEC7H
	LD A,20H
	CALL SG01
	LD HL,0FEC7H
	CALL SG03
	LD (IX-4FH),B
	LD A,(0FEB1H)
	OUT (01H),A
; 168:
; 169: ' Degrees
; 170: NumStr = #Degrees
	LD DE,(0FEF0H)
	CALL P101
	LD B,10H
	LD HL,0FEC9H
	LD DE,PRINT_STR
	CALL SG10
; 171: For i = 0 To Len(NumStr) - 1
	XOR A
	LD (0FEFFH),A
L0020:
	LD B,10H
	LD DE,0FEC9H
	CALL SG05
	LD (IX-4FH),C
	LD A,(0FEB1H)
	LD L,01H
	SUB L
	LD (0FEB1H),A
	LD A,(0FEFFH)
	LD L,(IX-4FH)
	SCF
	SBC A,L
	JP NC,L0021
; 172: Put SERIAL_PORT, NumStr(i)
	LD E,(IX-01H)
	LD D,00H
	LD HL,0FEC9H
	ADD HL,DE
	LD A,(HL)
	OUT (01H),A
; 173: Next i
	LD A,(0FEFFH)
	LD L,01H
	ADD A,L
	LD (0FEFFH),A
	JR C,L0021
	JP L0020
L0021:
; 174: Put SERIAL_PORT, Asc("°")
	LD B,01H
	LD HL,0FEC7H
	LD A,0B0H
	CALL SG01
	LD HL,0FEC7H
	CALL SG03
	LD (IX-4FH),B
	LD A,(0FEB1H)
	OUT (01H),A
; 175:
; 176: ' Minutes
; 177: Put SERIAL_PORT, Asc(" ")
	LD B,01H
	LD HL,0FEC7H
	LD A,20H
	CALL SG01
	LD HL,0FEC7H
	CALL SG03
	LD (IX-4FH),B
	LD A,(0FEB1H)
	OUT (01H),A
; 178: NumStr = #Minutes
	LD DE,(0FEEEH)
	CALL P101
	LD B,10H
	LD HL,0FEC9H
	LD DE,PRINT_STR
	CALL SG10
; 179: For i = 0 To Len(NumStr) - 1
	XOR A
	LD (0FEFFH),A
L0022:
	LD B,10H
	LD DE,0FEC9H
	CALL SG05
	LD (IX-4FH),C
	LD A,(0FEB1H)
	LD L,01H
	SUB L
	LD (0FEB1H),A
	LD A,(0FEFFH)
	LD L,(IX-4FH)
	SCF
	SBC A,L
	JP NC,L0023
; 180: Put SERIAL_PORT, NumStr(i)
	LD E,(IX-01H)
	LD D,00H
	LD HL,0FEC9H
	ADD HL,DE
	LD A,(HL)
	OUT (01H),A
; 181: Next i
	LD A,(0FEFFH)
	LD L,01H
	ADD A,L
	LD (0FEFFH),A
	JR C,L0023
	JP L0022
L0023:
; 182: Put SERIAL_PORT, Asc("'")
	LD B,01H
	LD HL,0FEC7H
	LD A,27H
	CALL SG01
	LD HL,0FEC7H
	CALL SG03
	LD (IX-4FH),B
	LD A,(0FEB1H)
	OUT (01H),A
; 183:
; 184: ' Seconds
; 185: Put SERIAL_PORT, Asc(" ")
	LD B,01H
	LD HL,0FEC7H
	LD A,20H
	CALL SG01
	LD HL,0FEC7H
	CALL SG03
	LD (IX-4FH),B
	LD A,(0FEB1H)
	OUT (01H),A
; 186: NumStr = #Seconds
	LD DE,(0FEDAH)
	LD BC,(0FEDCH)
	CALL P501
	LD B,10H
	LD HL,0FEC9H
	LD DE,PRINT_STR
	CALL SG10
; 187: For i = 0 To Len(NumStr) - 1
	XOR A
	LD (0FEFFH),A
L0024:
	LD B,10H
	LD DE,0FEC9H
	CALL SG05
	LD (IX-4FH),C
	LD A,(0FEB1H)
	LD L,01H
	SUB L
	LD (0FEB1H),A
	LD A,(0FEFFH)
	LD L,(IX-4FH)
	SCF
	SBC A,L
	JP NC,L0025
; 188: Put SERIAL_PORT, NumStr(i)
	LD E,(IX-01H)
	LD D,00H
	LD HL,0FEC9H
	ADD HL,DE
	LD A,(HL)
	OUT (01H),A
; 189: Next i
	LD A,(0FEFFH)
	LD L,01H
	ADD A,L
	LD (0FEFFH),A
	JR C,L0025
	JP L0024
L0025:
; 190: Put SERIAL_PORT, Asc("""")
	LD B,01H
	LD HL,0FEC7H
	LD A,22H
	CALL SG01
	LD HL,0FEC7H
	CALL SG03
	LD (IX-4FH),B
	LD A,(0FEB1H)
	OUT (01H),A
; 191:
; 192: Put SERIAL_PORT, 13  ' CR
	LD A,0DH
	OUT (01H),A
; 193: Put SERIAL_PORT, 10  ' LF
	LD A,0AH
	OUT (01H),A
; 194:
; 195: PrevAzA = AzA
	LD A,(0FEEBH)
	LD (0FEE7H),A
; 196: PrevAzB = AzB
	LD A,(0FEEAH)
	LD (0FEE6H),A
; 197: Endif
L0014:
; 198:
; 199: ' Small delay
; 200: For i = 0 To 100
	XOR A
	LD (0FEFFH),A
L0026:
	LD A,(0FEFFH)
	LD L,64H
	SCF
	SBC A,L
	JP NC,L0027
; 201: ' Empty loop for delay
; 202: Next i
	LD A,(0FEFFH)
	LD L,01H
	ADD A,L
	LD (0FEFFH),A
	JR C,L0027
	JP L0026
L0027:
; 203:
; 204: Goto mainloop
	JP L0001
; 205: End
	HALT
; End of user code
	HALT
; Integer Division Routine
D001:
	LD A,D
	XOR H
	PUSH AF
	BIT 7,H
	JR Z,D002
	XOR A
	SUB L
	LD L,A
	LD A,00H
	SBC A,H
	LD H,A
D002:
	BIT 7,D
	JR Z,D003
	XOR A
	SUB E
	LD E,A
	LD A,00H
	SBC A,D
	LD D,A
D003:
	CALL D011
	POP AF
	RET P
	XOR A
	SUB L
	LD L,A
	LD A,00H
	SBC A,H
	LD H,A
	RET
D011:
	LD B,D
	LD C,E
	EX DE,HL
	LD HL,0000H
	LD A,10H
D012:
	ADD HL,HL
	SLA E
	RL D
	JR NC,D013
	INC L
	OR A
D013:
	SBC HL,BC
	JR NC,D014
	ADD HL,BC
	JR D015
D014:
	INC E
D015:
	DEC A
	JR NZ,D012
	EX DE,HL
	RET
; Long Addition Routine
A101:	EXX
	POP HL
	POP DE
	POP BC
	PUSH HL
	LD A,E
	EXX
	ADD A,E
	LD E,A
	EXX
	LD A,D
	EXX
	ADC A,D
	LD D,A
	EXX
	LD A,C
	EXX
	ADC A,C
	LD C,A
	EXX
	LD A,B
	EXX
	ADC A,B
	LD B,A
	RET
; Long Subtraction Routine
S101:	EXX
	POP HL
	POP DE
	POP BC
	PUSH HL
S102:	LD A,E
	EXX
	SBC A,E
	LD E,A
	EXX
	LD A,D
	EXX
	SBC A,D
	LD D,A
	EXX
	LD A,C
	EXX
	SBC A,C
	LD C,A
	EXX
	LD A,B
	EXX
	SBC A,B
	LD B,A
	JR C,S103
	OR C
	OR D
	OR E
	RET
S103:	RET NZ
	INC A
	RET
; Long Division Routine
D104:
	XOR A
	SUB E
	LD E,A
	LD A,00H
	SBC A,D
	LD D,A
	LD A,00H
	SBC A,C
	LD C,A
	LD A,00H
	SBC A,B
	LD B,A
	RET
D101:
	EXX
	POP HL
	POP DE
	POP BC
	PUSH HL
	CALL D103
	RET
D103:
	LD A,B
	EXX
	XOR B
	PUSH AF
	BIT 7,B
	CALL NZ,D104
	EXX
	BIT 7,B
	CALL NZ,D104
	CALL D111
	POP AF
	RET P
	CALL D104
	RET
D121:
	EXX
	POP HL
	POP DE
	POP BC
	PUSH HL
	CALL D111
	RET
D111:
	PUSH BC
	LD B,D
	LD C,E
	EXX
	EX DE,HL
	LD D,B
	LD E,C
	POP BC
	PUSH HL
	LD HL,0000H
	EXX
	POP DE
	LD HL,0000H
	LD A,20H
	OR A
D112:
	RL C
	RL B
	EXX
	RL C
	RL B
	EXX
	ADC HL,HL
	EXX
	ADC HL,HL
	EXX
	SBC HL,DE
	EXX
	SBC HL,DE
	EXX
	JR NC,D113
	ADD HL,DE
	EXX
	ADC HL,DE
	EXX
D113:
	CCF
	DEC A
	JR NZ,D112
	RL C
	RL B
	EXX
	RL C
	RL B
	EXX
	LD D,B
	LD E,C
	PUSH HL
	EXX
	POP DE
	PUSH BC
	LD B,H
	LD C,L
	EXX
	POP BC
	RET
; Single Precision Subtraction Routine
S201:	LD A,B
	XOR 80H
	LD B,A
	EXX
	POP HL
	POP DE
	POP BC
	PUSH HL
	EXX
	PUSH HL
	CALL S202
	CALL S203
	POP HL
	RET
S202:	EXX
	LD L,00H
	LD H,E
	LD E,D
	LD D,C
	SET 7,D
	RL C
	LD A,B
	ADC A,A
	LD C,A
	EXX
	LD L,00H
	LD H,E
	LD E,D
	LD D,C
	SET 7,D
	RL C
	LD A,B
	ADC A,A
	LD C,A
	LD A,C
	EXX
	SUB C
	JR Z,S204
	JR NC,S205
	NEG
	EXX
S205:	SRL D
	RR E
	RR H
	RR L
	INC C
	DEC A
	JR NZ,S205
S204:	LD A,B
	EXX
	XOR B
	EXX
	JP M,S206
	LD A,L
	EXX
	ADD A,L
	EXX
	LD L,A
	LD A,H
	EXX
	ADC A,H
	EXX
	LD H,A
	LD A,E
	EXX
	ADC A,E
	EXX
	LD E,A
	LD A,D
	EXX
	ADC A,D
	EXX
	LD D,A
	JR NC,S207
S210:	LD A,D
	RR D
	RR E
	RR H
	RR L
	OR C
	OR E
	OR H
	OR L
	JR Z,S207
	INC C
	JR S207
S206:	LD A,D
	EXX
	CP D
	JR C,S208
	JR NZ,S209
	LD A,E
	EXX
	CP E
	JR C,S208
	JR NZ,S209
	LD A,H
	EXX
	CP H
	JR C,S208
	JR NZ,S209
	LD A,L
	EXX
	CP L
	JR C,S208
S209:	EXX
S208:	LD A,L
	EXX
	SUB L
	EXX
	LD L,A
	LD A,H
	EXX
	SBC A,H
	EXX
	LD H,A
	LD A,E
	EXX
	SBC A,E
	EXX
	LD E,A
	LD A,D
	EXX
	SBC A,D
	EXX
	LD D,A
	JR C,S210
S207:	RET
S203:	LD A,C
	OR A
	JR NZ,S211
S213:	LD B,A
	LD E,A
	LD D,A
	RET
S211:	BIT 7,D
	JR NZ,S212
	LD A,C
	OR A
	JR Z,S213
	DEC C
	SLA L
	RL H
	RL E
	RL D
	JP S211
S212:	LD A,L
	ADD A,80H
	LD A,H
	ADC A,00H
	LD L,A
	LD A,E
	ADC A,00H
	LD H,A
	LD A,D
	ADC A,00H
	JR NC,S214
	INC C
S214:	ADD A,A
	SRL C
	RRA
	LD E,A
	LD A,B
	AND 80H
	OR C
	LD D,A
	LD B,D
	LD C,E
	EX DE,HL
	RET
; Single Precision Multiplication Routine
M201:	EXX
	POP HL
	POP DE
	POP BC
	PUSH HL
	EXX
	PUSH HL
	EXX
	LD L,00H
	LD H,E
	LD E,D
	LD D,C
	SET 7,D
	RL C
	LD A,B
	ADC A,A
	LD C,A
	JR Z,M202
	EXX
	LD L,00H
	LD H,E
	LD E,D
	LD D,C
	SET 7,D
	RL C
	LD A,B
	ADC A,A
	LD C,A
	JR Z,M202
	CALL M203
M202:	CALL M204
	POP HL
	RET
M203:	LD A,B
	EXX
	XOR B
	LD B,A
	LD A,C
	EXX
	SUB 7FH
	LD B,A
	LD A,C
	SUB 7FH
	ADD A,B
	JP PO,M205
	EXX
	LD HL,0002H
	LD (0FF00H),HL
	LD C,0FFH
	JR NC,M206
	INC C
M206:	LD D,C
	LD E,C
	LD H,C
	LD L,00H
	RET
M205:	ADD A,80H
	EXX
	LD C,A
	PUSH BC
	PUSH DE
	LD B,H
	LD C,L
	EXX
	POP BC
	PUSH HL
	EXX
	POP DE
	LD A,20H
M208:	DEC C
	INC C
	JR NZ,M207
	CP 09H
	JR C,M207
	SUB 08H
	PUSH AF
	EXX
	LD A,C
	LD C,B
	LD B,00H
	EXX
	LD C,B
	LD B,A
	EXX
	LD A,L
	LD L,H
	LD H,00H
	EXX
	LD L,H
	LD H,A
	POP AF
	JR M208
M207:	EXX
	SRL B
	RR C
	EXX
	RR B
	RR C
	JR NC,M209
	ADD HL,DE
	EXX
	ADC HL,DE
	EXX
M209:	EXX
	RR H
	RR L
	EXX
	RR H
	RR L
	DEC A
	JR NZ,M208
	EXX
	PUSH HL
	EXX
	POP DE
	POP BC
	RET
M204:	LD A,C
	OR A
	JR NZ,M210
M212:	LD B,A
	LD E,A
	LD D,A
	RET
M210:	BIT 7,D
	JR NZ,M211
	LD A,C
	OR A
	JR Z,M212
	DEC C
	SLA L
	RL H
	RL E
	RL D
	JP M210
M211:	LD A,L
	ADD A,80H
	LD A,H
	ADC A,00H
	LD L,A
	LD A,E
	ADC A,00H
	LD H,A
	LD A,D
	ADC A,00H
	JR NC,M213
	INC C
M213:	ADD A,A
	SRL C
	RRA
	LD E,A
	LD A,B
	AND 80H
	OR C
	LD D,A
	LD B,D
	LD C,E
	EX DE,HL
	RET
; Single Precision Division Routine
D201:	EXX
	POP HL
	POP DE
	POP BC
	PUSH HL
	EXX
	PUSH HL
	EXX
	LD L,00H
	LD H,E
	LD E,D
	LD D,C
	SET 7,D
	RL C
	LD A,B
	ADC A,A
	LD C,A
	JR Z,D202
	EXX
	LD L,00H
	LD H,E
	LD E,D
	LD D,C
	SET 7,D
	RL C
	LD A,B
	ADC A,A
	LD C,A
	JR NZ,D203
	LD HL,0001H
	LD (0FF00H),HL
	LD C,00H
	JR D204
D203:	CALL D205
D202:	CALL D206
D204:	POP HL
	RET
D205:	LD A,C
	SUB 7FH
	LD C,A
	LD A,B
	EXX
	XOR B
	LD B,A
	LD A,C
	PUSH DE
	EXX
	SUB 7FH
	SUB C
	JP PO,D207
	POP DE
	LD HL,0002H
	LD (0FF00H),HL
	LD C,00H
	RET
D207:	ADD A,7FH
	LD B,H
	LD C,L
	POP HL
	PUSH BC
	EXX
	LD C,A
	POP DE
	PUSH BC
	LD A,20H
D211:	OR A
	SBC HL,DE
	EXX
	SBC HL,DE
	EXX
	CCF
	JR C,D208
	ADD HL,DE
	EXX
	ADC HL,DE
	EXX
	OR A
D208:	RL C
	RL B
	EXX
	RL C
	RL B
	BIT 7,H
	EXX
	JR NZ,D209
	ADD HL,HL
	EXX
	ADC HL,HL
	EXX
	JR D210
D209:	EXX
	SRL D
	RR E
	EXX
	RR D
	RR E
D210:	DEC A
	JR NZ,D211
	EXX
	PUSH BC
	EXX
	LD H,B
	LD L,C
	POP DE
	POP BC
	RET
D206:	LD A,C
	OR A
	JR NZ,D212
D214:	LD B,A
	LD E,A
	LD D,A
	RET
D212:	BIT 7,D
	JR NZ,D213
	LD A,C
	OR A
	JR Z,D214
	DEC C
	SLA L
	RL H
	RL E
	RL D
	JP D212
D213:	LD A,L
	ADD A,80H
	LD A,H
	ADC A,00H
	LD L,A
	LD A,E
	ADC A,00H
	LD H,A
	LD A,D
	ADC A,00H
	JR NC,D215
	INC C
D215:	ADD A,A
	SRL C
	RRA
	LD E,A
	LD A,B
	AND 80H
	OR C
	LD D,A
	LD B,D
	LD C,E
	EX DE,HL
	RET
; Single Precision Comparison Routine
C202:	LD A,B
	XOR 80H
	LD B,A
	EXX
	POP HL
	POP DE
	POP BC
	PUSH HL
	EXX
	PUSH HL
	CALL C203
	CALL C204
	POP HL
	RET
C203:	EXX
	LD L,00H
	LD H,E
	LD E,D
	LD D,C
	SET 7,D
	RL C
	LD A,B
	ADC A,A
	LD C,A
	EXX
	LD L,00H
	LD H,E
	LD E,D
	LD D,C
	SET 7,D
	RL C
	LD A,B
	ADC A,A
	LD C,A
	LD A,C
	EXX
	SUB C
	JR Z,C205
	JR NC,C206
	NEG
	EXX
C206:	SRL D
	RR E
	RR H
	RR L
	INC C
	DEC A
	JR NZ,C206
C205:	LD A,B
	EXX
	XOR B
	EXX
	JP M,C207
	LD A,L
	EXX
	ADD A,L
	EXX
	LD L,A
	LD A,H
	EXX
	ADC A,H
	EXX
	LD H,A
	LD A,E
	EXX
	ADC A,E
	EXX
	LD E,A
	LD A,D
	EXX
	ADC A,D
	EXX
	LD D,A
	JR NC,C208
C211:	LD A,D
	RR D
	RR E
	RR H
	RR L
	OR C
	OR E
	OR H
	OR L
	JR Z,C208
	INC C
	JR C208
C207:	LD A,D
	EXX
	CP D
	JR C,C209
	JR NZ,C210
	LD A,E
	EXX
	CP E
	JR C,C209
	JR NZ,C210
	LD A,H
	EXX
	CP H
	JR C,C209
	JR NZ,C210
	LD A,L
	EXX
	CP L
	JR C,C209
C210:	EXX
C209:	LD A,L
	EXX
	SUB L
	EXX
	LD L,A
	LD A,H
	EXX
	SBC A,H
	EXX
	LD H,A
	LD A,E
	EXX
	SBC A,E
	EXX
	LD E,A
	LD A,D
	EXX
	SBC A,D
	EXX
	LD D,A
	JR C,C211
C208:	RET
C201:	LD A,80H
	SUB B
	JR NZ,C212
	LD B,A
C212:	EXX
	POP HL
	POP DE
	POP BC
	PUSH HL
	LD A,80H
	SUB B
	JR NZ,C213
	LD B,A
C213:	LD A,B
	EXX
	CP B
	RET NZ
	EXX
	LD A,C
	EXX
	CP C
	RET NZ
	EXX
	LD A,D
	EXX
	CP D
	RET NZ
	EXX
	LD A,E
	EXX
	CP E
	RET
C204:	LD A,C
	OR A
	JR NZ,C214
C216:	LD B,A
	LD E,A
	LD D,A
	RET
C214:	BIT 7,D
	JR NZ,C215
	LD A,C
	OR A
	JR Z,C216
	DEC C
	SLA L
	RL H
	RL E
	RL D
	JP C214
C215:	LD A,L
	ADD A,80H
	LD A,H
	ADC A,00H
	LD L,A
	LD A,E
	ADC A,00H
	LD H,A
	LD A,D
	ADC A,00H
	JR NC,C217
	INC C
C217:	ADD A,A
	SRL C
	RRA
	LD E,A
	LD A,B
	AND 80H
	OR C
	LD D,A
	LD B,D
	LD C,E
	EX DE,HL
	RET
; Single To Long Conversion Routine
X001:	PUSH HL
	LD L,00H
	LD H,E
	LD E,D
	LD D,C
	SET 7,D
	RL C
	LD A,B
	ADC A,A
	LD C,A
	LD A,C
	SUB 7FH
	JR NC,X002
X005:	LD BC,0000H
	LD D,C
	LD E,C
	JR X003
X002:	NEG
	ADD A,1FH
	JR Z,X004
	CP 20H
	JR NC,X005
X006:	SRL D
	RR E
	RR H
	RR L
	DEC A
	JR NZ,X006
X004:	BIT 7,B
	LD B,D
	LD C,E
	EX DE,HL
	JR Z,X003
	CALL X007
X003:	POP HL
	RET
X007:	XOR A
	SUB E
	LD E,A
	LD A,00H
	SBC A,D
	LD D,A
	LD A,00H
	SBC A,C
	LD C,A
	LD A,00H
	SBC A,B
	LD B,A
	RET
; Long To Single Conversion Routine
X111:	PUSH HL
	LD A,00H
	JR X102
X101:	PUSH HL
	LD A,B
	BIT 7,B
	JR Z,X102
	CALL X103
	LD A,80H
X102:	EX DE,HL
	LD E,C
	LD D,B
	LD B,A
	LD C,9EH
	CALL X104
	POP HL
	RET
X104:	LD A,C
	OR A
	JR NZ,X105
X107:	LD B,A
	LD E,A
	LD D,A
	RET
X105:	BIT 7,D
	JR NZ,X106
	LD A,C
	OR A
	JR Z,X107
	DEC C
	SLA L
	RL H
	RL E
	RL D
	JP X105
X106:	LD A,L
	ADD A,80H
	LD A,H
	ADC A,00H
	LD L,A
	LD A,E
	ADC A,00H
	LD H,A
	LD A,D
	ADC A,00H
	JR NC,X108
	INC C
X108:	ADD A,A
	SRL C
	RRA
	LD E,A
	LD A,B
	AND 80H
	OR C
	LD D,A
	LD B,D
	LD C,E
	EX DE,HL
	RET
X103:	XOR A
	SUB E
	LD E,A
	LD A,00H
	SBC A,D
	LD D,A
	LD A,00H
	SBC A,C
	LD C,A
	LD A,00H
	SBC A,B
	LD B,A
	RET
; Print Routine
P001:
	LD HL,PRINT_STR
P002:
	LD A,(HL)
	CP 00H
	RET Z
	OUT (C),A
	INC HL
	JR P002
P011:
	PUSH DE
	LD HL,PRINT_IND
	INC (HL)
	LD E,(HL)
	LD D,00H
	ADD HL,DE
	LD (HL),A
	POP DE
	RET
P021:
	XOR A
	LD (PRINT_IND),A
	RET
P031:
	XOR A
	CALL P011
	RET
P041:
	LD HL,PRINT_IND
	DEC (HL)
	RET
; Integer Print Routine
P111:
	CALL P021
	JR P102
P101:
	CALL P021
	BIT 7,D
	JR Z,P102
	XOR A
	SUB E
	LD E,A
	LD A,00H
	SBC A,D
	LD D,A
	LD A,2DH
	CALL P011
P102:
	LD B,00H
	EX DE,HL
	LD DE,10000
	CALL P103
	LD DE,1000
	CALL P103
	LD DE,100
	CALL P103
	LD DE,10
	CALL P103
	LD A,L
	ADD A,30H
	CALL P011
	CALL P031
	RET
P103:
	PUSH BC
	CALL D011
	POP BC
	LD A,L
	CP 00H
	JR Z,P104
	LD B,30H
P104:
	ADD A,B
	JR Z,P105
	CALL P011
P105:
	EX DE,HL
	RET
; Long Print Routine
P411:
	CALL P021
	JR P402
P401:
	CALL P021
	BIT 7,B
	JR Z,P402
	CALL D104
	LD A,2DH
	CALL P011
P402:
	XOR A
	LD (REG_R0),A
	LD A,1
	LD (REG_R1),A
	PUSH BC
	PUSH DE
	LD BC,highw 1000000000
	LD DE,loww 1000000000
	CALL D121
	CALL P403
	PUSH BC
	PUSH DE
	LD BC,highw 100000000
	LD DE,loww 100000000
	CALL D121
	CALL P403
	PUSH BC
	PUSH DE
	LD BC,highw 10000000
	LD DE,loww 10000000
	CALL D121
	CALL P403
P401_7:	PUSH BC
	PUSH DE
	LD BC,highw 1000000
	LD DE,loww 1000000
	CALL D121
	CALL P403
P401_6:	PUSH BC
	PUSH DE
	LD BC,highw 100000
	LD DE,loww 100000
	CALL D121
	CALL P403
P401_5:	PUSH BC
	PUSH DE
	LD BC,highw 10000
	LD DE,loww 10000
	CALL D121
	CALL P403
P401_4:	PUSH BC
	PUSH DE
	LD BC,highw 1000
	LD DE,loww 1000
	CALL D121
	CALL P403
P401_3:	PUSH BC
	PUSH DE
	LD BC,highw 100
	LD DE,loww 100
	CALL D121
	CALL P403
P401_2:	PUSH BC
	PUSH DE
	LD BC,highw 10
	LD DE,loww 10
	CALL D121
	CALL P403
P401_1:	LD A,E
	ADD A,30H
	CALL P011
	CALL P031
	RET
P403:
	LD A,E
	CP 00H
	JR Z,P404
	LD A,30H
	LD (REG_R0),A
P404:
	LD A,(REG_R0)
	ADD A,E
	JR Z,P405
	CALL P011
	LD A,(REG_R1)
	INC A
	LD (REG_R1),A
P405:
	EXX
	RET
; Single Print Routine
P501:
	CALL P021
	LD A,C
	RLA
	LD A,B
	RLA
	CP 6BH
	JR C,P511
	CP 96H
	JR C,P512
P511:
	LD A,45H
	CALL P011
	CALL P031
	RET
P512:
	BIT 7,B
	JR Z,P502
	RES 7,B
	LD A,2DH
	CALL P011
P502:
	PUSH BC
	PUSH DE
	CALL X001
	XOR A
	LD (REG_R0),A
	LD A,1
	LD (REG_R1),A
	CALL P401_7
	LD A,(REG_R1)
	CP 6
	JR C,P513
	POP DE
	POP BC
	RET
P513:
	CALL P041
	LD A,2EH
	CALL P011
	POP DE
	POP BC
	PUSH BC
	PUSH DE
	CALL X001
	CALL X101
	CALL S201
	PUSH BC
	PUSH DE
	LD BC,447AH
	LD DE,0000H
	CALL M201
	CALL X001
	LD A,30H
	LD (REG_R0),A
	CALL P401_3
	RET
; String Routine
SG01:
	LD (HL),A
	INC HL
SG01A:
	XOR A
	LD (HL),A
	RET
SG03:
	LD B,(HL)
	RET
SG05:
	LD C,00H
SG06:
	LD A,(DE)
	OR A
	RET Z
	INC DE
	INC C
	DJNZ SG06
	XOR A
	LD (DE),A
	RET
SG10:
SG11:
	LD A,(DE)
	LD (HL),A
	INC DE
	INC HL
	OR A
	RET Z
	DJNZ SG11
	XOR A
	LD (HL),A
	RET
; End of listing
	.END

```
# mint

  MINT code limitations 

1. Integer Size Limitations:
- Only 16-bit integers (-32,768 to +32,767)
- At 600 pulses/rev × 4 (quadrature) × 10 (gear ratio) = 24,000 pulses/rev
- This means we'll overflow after:
  * Alt (90°): 24,000/4 = 6,000 pulses
  * Az (180°): 24,000/2 = 12,000 pulses

2. Math Limitations:
- No floating point capability
- No direct degree/minute/second calculations
- Cannot handle precision needed for arc seconds
- No automatic carry handling for overflows

3. Memory Limitations:
- Single-character variables (a-z only)
- Limited to 26 functions (A-Z)
- 256 byte line length limit

4. No Error Handling:
- No stack underflow protection
- No error checking for invalid encoder states
- No detection of missed pulses

5. Performance Issues:
- Every loop reads ports separately
- No debouncing for encoder inputs
- Simple delay with empty loop (100())

6. Display Limitations:
- Can only output raw counts
- No formatted degree display
- No decimal point display
- No degree symbol support

To improve this, we would need to:
1. Add overflow detection using /c and /r variables
2. Scale numbers differently to avoid overflow
3. Add error checking for quadrature states
4. Add proper debouncing
5. Implement degree conversion math
6. Add proper display formatting



```
// Variables used:
// a - alt encoder A channel state
// b - alt encoder B channel state
// c - az encoder A channel state 
// d - az encoder B channel state
// e - alt count
// f - az count
// p - port temp storage
// t - temp calculations

:A              // Init variables
0 e! 0 f!      // zero counters
;

:B              // Read alt encoder
2 /I p!        // read port 2
0 p & a!       // get bit 0
3 /I p!        // read port 3
0 p & b!       // get bit 0
;

:C              // Read az encoder
4 /I p!        // read port 4
0 p & c!       // get bit 0
5 /I p!        // read port 5
0 p & d!       // get bit 0
;

:D              // Process alt change
a 1 = b 0 = & (e 1 + e!) // if a=1 and b=0, increment
a 0 = b 1 = & (e 1 - e!) // if a=0 and b=1, decrement
e 15000 > (15000 e!)     // limit to 90 degrees worth
e 0 < (0 e!)             // prevent negative
;

:E              // Process az change  
c 1 = d 0 = & (f 1 + f!) // if c=1 and d=0, increment
c 0 = d 1 = & (f 1 - f!) // if c=0 and d=1, decrement
f 30000 > (30000 f!)     // limit to 180 degrees worth
f 0 < (0 f!)             // prevent negative
;

:F              // Output values
`Alt:` e . /N   // print alt count
`Az:` f . /N    // print az count
;

:M              // Main loop
A               // init
/U(             // infinite loop
  B C           // read encoders
  D E           // process changes
  F             // output values
  100()         // delay
)
;
```
