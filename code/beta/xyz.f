\\ this is an example position 

\\: example ( -- )

\\    -90 move-x \ move x-motor to -90 degrees 

\\     45 move-y \ move y-motor to 45 degrees 

\\     30 move-z \ move z-motor to 30 degrees



: move-x ( n -- )
     x-motor @ ON
     x-motor-steps n 
     x-motor @ OFF ;
 

: move-y ( n -- )
     y-motor @ ON
     y-motor-steps n 
     y-motor @ OFF ;
 

: move-z ( n -- )
     z-motor @ ON
     z-motor-steps n 
     z-motor @ OFF ;
 
