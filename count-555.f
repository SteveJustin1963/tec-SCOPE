
: read-io-port ( port-address -- value )
  in port-address
;

: count-pulses ( port-address -- count )
  variable A
  0 A !
  3 0 do
    read-io-port port-address 0= if
      A @ 1+ A !
    then
    1000 ms
  loop
  A @
;

