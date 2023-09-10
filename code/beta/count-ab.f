: A
    3 @ IF
        1 - counter
    ELSE
        1 + counter
    THEN
;

: B
    2 @ IF
        1 - counter
    ELSE
        1 + counter
    THEN
;

: setup
    2 INPUT !
    3 INPUT !
    2 A !
    3 B !
;

: loop
    counter .
    counter 0 !
;
\\\\\\\\\\\
