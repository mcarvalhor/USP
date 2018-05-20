#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
Matheus
mcarvalhor@usp.br
*/

int main(){
	float valor,poupanca,lci,cdb_a,cdb_b,cdb;
	int meses;
	scanf("%f %d",&valor,&meses);
	poupanca=valor*pow(1.0059,meses);
	if(meses<6){
		lci=valor;
	}else{
		lci=valor*pow(1.0086,meses);
	}
	cdb_a=valor*pow(1.01032,meses);
	if(meses<7){
		cdb_b=0.225;
	}else if(meses<13){
		cdb_b=0.2;
	}else if(meses<25){
		cdb_b=0.175;
	}else{
		cdb_b=0.15;
	}
	cdb=cdb_a-((cdb_a-valor)*cdb_b);
	printf("%.2f\n",poupanca);
	printf("%.2f\n",lci);
	printf("%.2f\n",cdb);
	if(poupanca>lci && poupanca>cdb){
		printf("P");
	}else if(lci>poupanca && lci>cdb){
		printf("L");
	}else{
		printf("C");
	}
	return EXIT_SUCCESS;
}
