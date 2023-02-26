\\mint


\\ R read-io-port 
:R 1 \<;

\\count-pulses, variable a
  0 a!  \\store 0 in a
  3 0 do
    R 0= if
      a @ 1+ a !
    then
    2500()  \\1000 ms
  loop
  a @
;

