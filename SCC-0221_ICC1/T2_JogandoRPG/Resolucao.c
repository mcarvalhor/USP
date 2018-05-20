#include <stdlib.h>
#include <stdio.h>

/*
Matheus Carvalho Raimundo
mcarvalhor@usp.br
*/

/*
Meu RPG tem como base dois pointeiros que indicam quem esta atacando (Attack) e quem esta recebendo o dano (Defense).
Ao mesmo tempo tem os vetores PD, que contem informacoes de Drizzt, e o vetor PA, que contem informacoes de Artemis.
*/

int main(){
	int N_F,n_f=0,N_DDA,R,i; // Respectivamente numero de lutas, numero da luta a qual estamos, DDA e duas variaveis usadas nos lacos abaixo (R eh o dano de cada luta e i eh o numero de vezes que o dado sera jogado)
	int PD[6]={0,0,0,10,1,9}; // Numeros de vitorias, Vida na luta, Vida geral, Classe de Armadura, Forca da Arma (N. de dados) e Forca da Arma (N. de faces) de Drizzt e de Artemis
	int PA[6]={0,0,0,7,2,7};
	int *Attack; // Aponta para quem esta atacando (Attack) e para quem esta sofrendo o dano ou tentando defender-se (Defense) na luta
	int *Defense;
	scanf("%d",&N_F); // Leia as entradas do usuario
	scanf("%d",&PD[2]);
	scanf("%d",&PA[2]);
	scanf("%d",&N_DDA);
	srand(N_DDA*N_DDA); // Gerar a semente do aleatorio
	while((PD[0]<((N_F>>1)+1)) && (PA[0]<((N_F>>1)+1))){ // Laco da batalha (enquanto ninguem venceu mais da metade das lutas)
		printf("Luta %d\n",++n_f);
		PA[1]=PA[2]; // Reseta a vida ("Vida na luta") dos personagens para o padrao ("Vida geral")
		PD[1]=PD[2];
		if(((rand()%N_DDA)+1)<((rand()%N_DDA)+1)){ // Decidir no DDA quem vai jogar primeiro na luta
			Attack=PA; // Artemis vai comecar atacando
			Defense=PD;
		}else{
			Attack=PD; // Drizzt vai comecar atacando
			Defense=PA;
		}
		while((PD[1]>0) && (PA[1]>0)){ // Laco interno de cada luta (enquanto a vida de ambos for maior que zero)
			if(((rand()%N_DDA)+1)>*(Defense+3)){ // Jogar o DDA para lidar com a autorizacao de ataque (saber se 'Attack' consegue quebrar a armadura de 'Defense')
				R=0;
				for(i=0;i<*(Attack+4);i++){ //Jogar o dado x vezes, onde x eh o "Forca da Arma (N. de faces)"
					R+=(rand()%(*(Attack+5)))+1; // Adicionar dano que saiu no dado
				}
				if(R==(*(Attack+4))*(*(Attack+5))){
					R=(int)R*1.5; // Lidar com o 'Extra Damage'
				}else if(R==*(Attack+4)){
					R=0; // Lidar com o 'Miss'
				}
				*(Defense+1)-=R; // Retirar 'R' da 'Vida na luta' de Defense
				if(R>0){
					if(Attack==PD){ // Imprimir dano retirado
						printf("Drizzt %d\n",R);
					}else{
						printf("Artemis %d\n",R);
					}
				}
			}
			if(Attack==PD){ // Agora vamos trocar quem vai jogar na proxima luta (inverter 'Attack' com 'Defense')
				Attack=PA;
				Defense=PD;
			}else{
				Attack=PD;
				Defense=PA;
			}
		}
		if(PD[1]>0){ // A luta acabou. Agora vamos imprimir quem ganhou ela
			PD[0]++;
			printf("Fim da luta. Vencedor: Drizzt\n");
		}else{
			PA[0]++;
			printf("Fim da luta. Vencedor: Artemis\n");
		}
	}
	if(PD[0]>=((N_F>>1)+1)){ // A batalha acabou. Agora vamos imprimir quem ganhou ela
		printf("Fim da batalha. Vencedor: DRIZZT\n");
	}else{
		printf("Fim da batalha. Vencedor: ARTEMIS\n");
	}
	return EXIT_SUCCESS;
}
