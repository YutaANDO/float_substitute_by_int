sample:sample.o float2int.o
	gcc -O0 -o sample sample.o float2int.o -lm

eval_time:eval_time.o float2int.o
	gcc -O0 -o eval_time eval_time.o float2int.o -lm

sample.o:sample.c
	gcc -O0 -c sample.c

float2int.o:float2int.c
	gcc -O0 -c float2int.c

eval_time.o:eval_time.c
	gcc -O0 -c eval_time.c