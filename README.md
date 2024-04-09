### Para compilar o código usando Lamport:

gcc -o <nome_arquivo> <nome_arquivo.c> -L. -llamport

### Para compilar Lamport:

gcc -c lamport.c -o lamport.o

ar rcs liblamport.a lamport.o