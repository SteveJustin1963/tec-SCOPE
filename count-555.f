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

===========
reduces to

0 a! \\store 0 in a
3( 1\< 0>(a@ 1+ a! 2500())  \\ do 3 times, read port1 compare if > than o then save in a, add 1 and save in a again, delay
a@. \\ recall a and print to terminal


