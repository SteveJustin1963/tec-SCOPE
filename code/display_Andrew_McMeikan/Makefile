all: ztest

crt0: crt0.s
	sdasz80 -g -l -o -s crt0.s

ztest: ztest.c crt0
	sdcc -c -mz80 --no-std-crt0 --code-loc 0x0804  ztest.c 
	sdld -m -i  -u -b "_GSINIT = 0x09D0" -b "_CODE = 0x0818"  ztest crt0.rel ztest.rel 





