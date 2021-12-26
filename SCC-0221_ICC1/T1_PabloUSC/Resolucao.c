#include <stdio.h>
#include <stdlib.h>

/*
Matheus
mcarvalhor@usp.br
*/

int main(){
	int k,n;
	unsigned char byte_str[8],boys=0,girls=0;
	scanf("%d",&k); // Ler entrada (k)
	scanf("%d",&n); // Ler entrada (n)
	if( !((k>=0) && (k<=3) && (n>=0) && (n<=1000000000)) ){ // Verificar entradas
		printf("Invalid input\n");
		return EXIT_FAILURE;
	}
	n=n>>8*k; // Shiftar o inteiro para o byte desejado (k)
	// *** Agora vamos verificar o primeiro bit
	if(n%2){ // Eh uma menina, porque esse bit eh '1'
		byte_str[0]='1';
		girls++;
	
	}else{ // Eh um menino, porque esse bit eh '0'
		byte_str[0]='0';
		boys++;
	}
	n=n>>1; // Shiftar para o próximo bit
	// *** Agora vamos verificar o segundo bit
	if(n%2){
		byte_str[1]='1';
		girls++;
	
	}else{
		byte_str[1]='0';
		boys++;
	}
	n=n>>1;
	// *** Agora vamos verificar o terceiro bit
	if(n%2){
		byte_str[2]='1';
		girls++;
	
	}else{
		byte_str[2]='0';
		boys++;
	}
	n=n>>1;
	// *** Agora vamos verificar o quarto bit
	if(n%2){
		byte_str[3]='1';
		girls++;
	
	}else{
		byte_str[3]='0';
		boys++;
	}
	n=n>>1;
	// *** Agora vamos verificar o quinto bit
	if(n%2){
		byte_str[4]='1';
		girls++;
	
	}else{
		byte_str[4]='0';
		boys++;
	}
	n=n>>1;
	// *** Agora vamos verificar o sexto bit
	if(n%2){
		byte_str[5]='1';
		girls++;
	
	}else{
		byte_str[5]='0';
		boys++;
	}
	n=n>>1;
	// *** Agora vamos verificar o setimo bit
	if(n%2){
		byte_str[6]='1';
		girls++;
	
	}else{
		byte_str[6]='0';
		boys++;
	}
	n=n>>1;
	// *** Agora vamos verificar o oitavo bit
	if(n%2){
		byte_str[7]='1';
		girls++;
	
	}else{
		byte_str[7]='0';
		boys++;
	}
	n=n>>1;
	printf("%s\n",byte_str); // Imprimir a sequencia binaria reversa
	printf("%d\n",boys); // Imprimir o numero de meninos (0s) na sequencia binaria
	printf("%d\n",girls); // Imprimir o numero de meninas (1s) na sequencia binaria
	return EXIT_SUCCESS;
}
