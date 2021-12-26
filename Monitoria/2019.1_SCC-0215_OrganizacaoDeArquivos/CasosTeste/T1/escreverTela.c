
#include <stdlib.h>
#include <stdio.h>



/*
* Abaixo seguem funções que fazem a escrita do binário em "stdout" (tela) pra poder ser comparado no run.codes.
*
* Funciona assim: você faz tudo o que tiver que fazer na funcionalidade no arquivo em disco, assim como ensinado nas aulas da disciplina.
* Ao fim da funcionalidade, você escolhe uma das funções abaixo (a que achar mais fácil de usar de acordo com sua implementação) e a função já cuida de tudo para você. É só chamar a função.
*
* "Qual função escolher?"
* As duas fazem A MESMA coisa: escrever na tela o arquivo binário para funcionar com o sistema do run.codes.
*     - Você escolhe a binarioNaTela1 se você ainda não fez o fclose no arquivo binário. Você passa o ponteiro 'FILE *' para ela e ela vai ler tudo e escrever na tela.
*     - Você escolhe a binarioNaTela2 se você já fez o fclose no arquivo binário. Você passa o nome do arquivo binário ("arquivoTrabX.bin") pra ela e ela vai ler tudo e escrever na tela.
*
* Você pode colocar isso num módulo .h separado, ou incluir as funções no próprio código .c: como preferir.
* VOCÊ NÃO PRECISA ENTENDER ESSAS FUNÇÕES. É só usar elas da forma certa depois de acabar a funcionalidade.
*
* Tá tudo testado e funcionando, mas qualquer dúvida acerca dessas funções, falar com o monitor Matheus (mcarvalhor@usp.br).
*/


	// Se você for incluir no .h separado, tá abaixo:

	#ifndef H_ESCREVERNATELA_
	#define H_ESCREVERNATELA_
	#include <stdio.h>

	void binarioNaTela1(FILE *ponteiroArquivoBinario);
	void binarioNaTela2(char *nomeArquivoBinario);

	#endif

	// Acabou o código que vai no .h



// Abaixo vai em algum .c


void binarioNaTela1(FILE *ponteiroArquivoBinario) {

	/* Escolha essa função se você ainda tem o ponteiro de arquivo 'FILE *' aberto.
	*  Lembrando que você tem que ter aberto ele no fopen para leitura também pra funcionar (exemplo: rb, rb+, wb+, ...) */

	size_t fl;
	char *mb;
	fseek(ponteiroArquivoBinario, 0, SEEK_END);
	fl = ftell(ponteiroArquivoBinario);
	fseek(ponteiroArquivoBinario, 0, SEEK_SET);
	mb = (char *) malloc(fl);
	fread(mb, 1, fl, ponteiroArquivoBinario);
	fwrite(mb, 1, fl, stdout);
	free(mb);
}

void binarioNaTela2(char *nomeArquivoBinario) {

	/* Escolha essa função se você já fechou o ponteiro de arquivo 'FILE *'.
	*  Ela vai abrir de novo para leitura e depois fechar. */

	size_t fl;
	FILE *fs;
	char *mb;
	if(nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
		fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela2): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar? Se você não fechou ele, pode usar a outra função, binarioNaTela1, ou pode fechar ele antes de chamar essa função!\n");
		return;
	}
	fseek(fs, 0, SEEK_END);
	fl = ftell(fs);
	fseek(fs, 0, SEEK_SET);
	mb = (char *) malloc(fl);
	fread(mb, 1, fl, fs);
	fwrite(mb, 1, fl, stdout);
	free(mb);
	fclose(fs);
}



