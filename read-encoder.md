```
' Declare constants for ports
Const PORT1 = 1    ' A channel on port 1
Const PORT2 = 2    ' B channel on port 2

' Declare variables
Dim A As Bit           ' Current state of channel A
Dim B As Bit           ' Current state of channel B
Dim Counter As Integer ' Position counter
Dim PrevA As Bit      ' Previous state of channel A
Dim PrevB As Bit      ' Previous state of channel B
Dim Temp As UShort    ' Temporary variable for port reading

' Initialize variables
Counter = 0
PrevA = 0
PrevB = 0

' Main loop
mainloop:
    ' Read the A and B signals from the ports
    Temp = Get(PORT1)     ' Read A channel
    A = TestBit(0, Temp)  ' Convert to bit
    
    Temp = Get(PORT2)     ' Read B channel
    B = TestBit(0, Temp)  ' Convert to bit
    
    ' Check if there has been a change in state
    If A <> PrevA Or B <> PrevB Then
        ' Determine direction of rotation
        If A = 1 And B = 0 Then
            Counter = Counter + 1  ' Clockwise
        Endif
        If A = 0 And B = 1 Then
            Counter = Counter - 1  ' Counter-clockwise
        Endif
        
        ' Output the current counter value
        Print PORT1, "Counter: ", Counter, CrLf
        
        ' Store the current state as previous
        PrevA = A
        PrevB = B
    Endif
    
    ' Small delay implemented as a counting loop
    Dim i As UShort
    For i = 0 To 100
        ' Empty loop for delay
    Next i
    
    Goto mainloop
End

```

```

; Compiled with: Z80 Simulator IDE v14.76
; Microprocessor model: Z80
; Clock frequency: 4.0MHz
;
;       The value of 'PORT1' (global) is 1
;       The value of 'PORT2' (global) is 2
;       The address of 'A' (boolean) (global) is 0xFEFC (IX-4)
;       The address of 'B' (boolean) (global) is 0xFEFB (IX-5)
;       The address of 'Counter' (integer) (global) is 0xFEFE (IX-2)
;       The address of 'PrevA' (boolean) (global) is 0xFEFA (IX-6)
;       The address of 'PrevB' (boolean) (global) is 0xFEF9 (IX-7)
;       The address of 'Temp' (ushort) (global) is 0xFEF8 (IX-8)
;       The address of 'i' (ushort) (global) is 0xFEFD (IX-3)
	PRINT_STR .EQU 0xFEEC
	PRINT_IND .EQU 0xFEEB
	REG_R3 .EQU 0xFEEA
	REG_R2 .EQU 0xFEE9
	REG_R1 .EQU 0xFEE8
	REG_R0 .EQU 0xFEE7
	.ORG 0000H
	LD IX,0FF00H
	LD SP,0FEE7H
; User code start
; 1: ' Declare constants for ports
; 2: Const PORT1 = 1    ' A channel on port 1
; 3: Const PORT2 = 2    ' B channel on port 2
; 4:
; 5: ' Declare variables
; 6: Dim A As Bit           ' Current state of channel A
; 7: Dim B As Bit           ' Current state of channel B
; 8: Dim Counter As Integer ' Position counter
; 9: Dim PrevA As Bit      ' Previous state of channel A
; 10: Dim PrevB As Bit      ' Previous state of channel B
; 11: Dim Temp As UShort    ' Temporary variable for port reading
; 12:
; 13: ' Initialize variables
; 14: Counter = 0
	LD BC,0000H
	LD (0FEFEH),BC
; 15: PrevA = 0
	XOR A
	LD (0FEFAH),A
; 16: PrevB = 0
	XOR A
	LD (0FEF9H),A
; 17:
; 18: ' Main loop
; 19: mainloop:
L0001:
; 20: ' Read the A and B signals from the ports
; 21: Temp = Get(PORT1)     ' Read A channel
	IN A,(01H)
	LD (0FEF8H),A
; 22: A = TestBit(0, Temp)  ' Convert to bit
	LD A,(0FEF8H)
	AND 01H
	JR NZ,$+2+3
	XOR A
	JR Z,$+2+2
	LD A,0FFH
	LD (0FEFCH),A
; 23:
; 24: Temp = Get(PORT2)     ' Read B channel
	IN A,(02H)
	LD (0FEF8H),A
; 25: B = TestBit(0, Temp)  ' Convert to bit
	LD A,(0FEF8H)
	AND 01H
	JR NZ,$+2+3
	XOR A
	JR Z,$+2+2
	LD A,0FFH
	LD (0FEFBH),A
; 26:
; 27: ' Check if there has been a change in state
; 28: If A <> PrevA Or B <> PrevB Then
	LD A,(0FEFCH)
	LD L,(IX-06H)
	SUB L
	JP NZ,L0003
	LD A,(0FEFBH)
	LD L,(IX-07H)
	SUB L
	JP NZ,L0003
	JP L0002
L0003:
; 29: ' Determine direction of rotation
; 30: If A = 1 And B = 0 Then
	LD A,(0FEFCH)
	LD L,0FFH
	SUB L
	JP NZ,L0004
	LD A,(0FEFBH)
	LD L,00H
	SUB L
	JP NZ,L0004
; 31: Counter = Counter + 1  ' Clockwise
	LD DE,0001H
	LD HL,(0FEFEH)
	OR A
	ADC HL,DE
	LD (0FEFEH),HL
; 32: Endif
L0004:
; 33: If A = 0 And B = 1 Then
	LD A,(0FEFCH)
	LD L,00H
	SUB L
	JP NZ,L0005
	LD A,(0FEFBH)
	LD L,0FFH
	SUB L
	JP NZ,L0005
; 34: Counter = Counter - 1  ' Counter-clockwise
	LD DE,0001H
	LD HL,(0FEFEH)
	OR A
	SBC HL,DE
	LD (0FEFEH),HL
; 35: Endif
L0005:
; 36:
; 37: ' Output the current counter value
; 38: Print PORT1, "Counter: ", Counter, CrLf
	LD A,43H
	OUT (01H),A
	LD A,6FH
	OUT (01H),A
	LD A,75H
	OUT (01H),A
	LD A,6EH
	OUT (01H),A
	LD A,74H
	OUT (01H),A
	LD A,65H
	OUT (01H),A
	LD A,72H
	OUT (01H),A
	LD A,3AH
	OUT (01H),A
	LD A,20H
	OUT (01H),A
	LD DE,(0FEFEH)
	CALL P101
	LD C,01H
	CALL P001
	LD A,0DH
	OUT (01H),A
	LD A,0AH
	OUT (01H),A
; 39:
; 40: ' Store the current state as previous
; 41: PrevA = A
	LD A,(0FEFCH)
	LD (0FEFAH),A
; 42: PrevB = B
	LD A,(0FEFBH)
	LD (0FEF9H),A
; 43: Endif
L0002:
; 44:
; 45: ' Small delay implemented as a counting loop
; 46: Dim i As UShort
; 47: For i = 0 To 100
	XOR A
	LD (0FEFDH),A
L0006:
	LD A,(0FEFDH)
	LD L,64H
	SCF
	SBC A,L
	JP NC,L0007
; 48: ' Empty loop for delay
; 49: Next i
	LD A,(0FEFDH)
	LD L,01H
	ADD A,L
	LD (0FEFDH),A
	JR C,L0007
	JP L0006
L0007:
; 50:
; 51: Goto mainloop
	JP L0001
; 52: End
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
; End of listing
	.END


```



