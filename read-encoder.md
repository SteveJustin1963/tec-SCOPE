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
