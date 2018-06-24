

#
#
#	BITWISE TRIE
#	
#	Organização de Computadores Digitais I	(SSC-0112 2018.1)
#	Professor Paulo Sérgio Lopes de Souza
#	ICMC - Universidade de São Paulo
#
#
#	OBS: O projeto foi desenvolvido no Linux Mint. Caso a acentuação do programa
# 	ou dos comentários não esteja correta, o problema provavelmente deve-se à
#	codificação do Windows. 
#
#	 _______ _______ _______
#	|   |   |               \
#	|   |   |      \    |___|
#	|   |    \      |   |
#	|_______ _______|___|
#
#




			.data
			.align 0
			
			
str_menu:		.asciiz "\n== Menu principal de opções ==\n"
str_menu_options:	.asciiz	"1 - Inserção\n2 - Remoção\n3 - Busca\n4 - Visualização\n5 - Fim\n"	
str_menu_choose:	.asciiz	"\nEscolha uma opção (1 a 5, sem necessidade do [enter]): "
str_menu_invalid:	.asciiz "Opção inválida. Tente algo entre 1 e 5.\n"
str_menu_exit:		.asciiz "\n\n== Terminando a execução do programa ==\n"

str_insertion:		.asciiz "\tDigite o binário para inserção: "
str_insertion_success:	.asciiz "\tChave inserida com sucesso.\n"
str_repeated_insertion:	.asciiz "\tChave repetida. Inserção não permitida.\n"

str_remove:		.asciiz "\tDigite o binário para remoção: "
str_remove_success:	.asciiz "\tChave removida com sucesso.\n"

str_find:		.asciiz "\tDigite o binário para busca: "
str_PrintLevel:		.asciiz "\tN"
str_PrintLevelNode:	.asciiz " ("
str_PrintLevelRoot:	.asciiz "\tImprimindo árvore... os endereços estão em base decimal.\n\tN0 (raiz, NT, "
str_PrintLevelNodeT:	.asciiz ", T, "
str_PrintLevelNodeNT:	.asciiz ", NT, "
str_PrintLevelNodeNull:	.asciiz "null"
str_PrintLevelNodeComma:.asciiz ", "
str_PrintLevelNodeEnd:	.asciiz ")"

str_invalid_key:	.asciiz "\tChave inválida. Insira somente números binários (ou -1 retorna ao menu).\n"
str_KeyFound:		.asciiz "\tChave encontrada na árvore: "
str_KeyNotFound:	.asciiz "\tChave não encontrada na árvore: -1"

str_Path:		.asciiz "\n\tCaminho percorrido: raiz"
str_PathLeft:		.asciiz ", esq"
str_PathRight:		.asciiz ", dir"

str_ReturningMenu:	.asciiz "\tRetornando ao menu...\n"
str_new_line: 		.asciiz "\n"

mem_ScanStr:		.space 16 # Espaço de memória para a String dos dígitos binários que será lida





			.text

main:			
			#   Como a raiz da Bitwise Trie deve sempre existir, aloca-se esta raiz
			# e salva seu endereço em $s0, de modo que ele sempre estará lá (por convenção).
			jal alloc_node 
			move $s0, $v0 





menu:		
			# Imprimir-se-ão 'strings', portanto $v0 é configurado para isso.
			li $v0, 4
			
			# Imprime-se o cabeçalho do Menu.
			la $a0, str_menu
			syscall
			
			# Imprime-se as opções do Menu.
			la $a0, str_menu_options
			syscall
			
			# Imprime-se o campo de escolha de opção do Menu.
			la $a0, str_menu_choose
			syscall
			
			# Recebe do usuário a opção do Menu desejada.
			li $v0, 12
			syscall
			
			# Transforma o valor de 'char' para 'int' e move esse valor
			# para o registrador temporário $t0.
			addi $v0, $v0, -48
			move $t0, $v0
			
			# Pula uma linha para as 'strings' conseguintes.
			li $v0, 4
			la $a0, str_new_line
			syscall
			
			# Avalia a partir da opção escolhida qual "função" executar.
			beq $t0, 1, insert_menu
			beq $t0, 2, remove_menu
			beq $t0, 3, find_menu 
			beq $t0, 4, print_menu
			beq $t0, 5, exit_program
			
			# Caso seja uma operação inválida, informa o usuário e reinicia o processo.
			li $v0, 4 
			la $a0, str_menu_invalid
			syscall
			
			j menu





insert_menu:		
			# Pede ao usuário o valor a ser inserido.
			li $v0, 4
			la $a0, str_insertion
			syscall
			
			# Lê a string de entrada e faz sua validação.
			jal read_string
			
			# Caso o usuário queira voltar ao Menu (read_string retornou -1 em $v0).
			bltz $v0, back_to_menu
			
			# Caso o usuário tenha inserido caracteres não binários (read_string retornou 0 em $v0).
			beqz $v0, insert_menu_invalid
			
			# Caso a entrada esteja de acordo com o previsto, move-se o endereço da 'string', para ela 
			# ser tratada na inserção. Da mesma forma, retoma a raiz de $s0.
			move $a0, $v1
			move $a1, $s0
			
			# Realiza a inserção.
			jal insert
			
			# Caso o valor retornado em $v0 pela função de inserção seja diferente de zero,
			# notificamos o usuário de que a string já havia sido inserida.
			bnez $v0, insert_menu_repeated
			
			# Por outro lado, caso a inserção tenha sido um sucesso, notifica o usuário.
			li $v0, 4
			la $a0, str_insertion_success
			syscall
			
			# Retorna ao menu de inserção de strings para inserir outra 'string'.
			j insert_menu
			
			
insert_menu_repeated:	
			# Notifica o usuário de que trata-se de uma 'string' já inserida.
			li $v0, 4 
			la $a0, str_repeated_insertion
			syscall
			
			# Retorna ao menu de inserção de strings para tentar inserir outra 'string'.
			j insert_menu
			
			
insert_menu_invalid:	
			# Notifica o usuário de que existem caracteres inválidos presentes na 'string' inserida.
			li $v0, 4
			la $a0, str_invalid_key
			syscall
			
			# Retorna ao menu de inserção de strings para tentar inserir outra 'string'.
			j insert_menu





find_menu:		
			# Pede ao usuário o valor a ser inserido.
			li $v0, 4
			la $a0, str_find
			syscall
			
			# Lê a string de entrada e faz sua validação.
			jal read_string
			
			# Caso o usuário queira voltar ao Menu (read_string retornou -1 em $v0).
			bltz $v0, back_to_menu
			
			# Caso o usuário tenha inserido caracteres não binários (read_string retornou 0 em $v0).
			beqz $v0, find_menu_invalid
			
			# Caso a entrada esteja de acordo com o previsto, move-se o endereço da 'string', para ela 
			# ser tratada na inserção. Da mesma forma, retoma a raiz de $s0.
			move $a0, $v1
			move $a1, $s0
			
			# Realiza a busca.
			jal find # Buscar String na Trie
			
			# Caso a string não tenha sido encontrada:
			beqz $v0, find_menu_notfound
			
			# Caso a string tenha sido encontrada, notifica o usuário e a imprime.
			li $v0, 4
			la $a0, str_KeyFound
			syscall
			
			li $v0, 4
			la $a0, mem_ScanStr
			syscall
			
			# Inicia a impressão do caminho.
			li $v0, 4
			la $a0, str_Path
			syscall
			
			# Atualiza os parâmetros necessários ('string' e raiz da árvore).
			la $a0, mem_ScanStr
			move $a1, $s0
			
			# Imprime o caminho de fato.
			jal print_path
			
			# Pula uma linha ('\n').
			li $v0, 4
			la $a0, str_new_line
			syscall
			
			# Retorna ao menu de busca de strings para buscar outra 'string'.
			j find_menu
			
			
find_menu_notfound:	
			# Notifica o usuário de que a 'string' não foi encontrada.
			li $v0, 4
			la $a0, str_KeyNotFound
			syscall
			
			# Inicia a impressão do caminho:
			li $v0, 4
			la $a0, str_Path
			syscall
			
			# Atualiza os parâmetros necessários ('string' e raiz da árvore).
			la $a0, mem_ScanStr
			move $a1, $s0
			
			# Imprime o caminho de fato.
			jal print_path
			
			# Pula uma linha ('\n').
			li $v0, 4
			la $a0, str_new_line
			syscall
			
			# Retorna ao menu de busca de strings para buscar outra 'string'.
			j find_menu
			
			
find_menu_invalid:	
			# Notifica o usuário de que existem caracteres inválidos presentes na 'string' inserida.
			li $v0, 4
			la $a0, str_invalid_key
			syscall
			
			# Retorna ao menu de busca de strings para buscar outra 'string'.
			j find_menu





remove_menu:		
			# Pede ao usuário o valor a ser inserido.
			li $v0, 4
			la $a0, str_remove
			syscall
			
			# Lê a string de entrada e faz sua validação.
			jal read_string
			
			# Caso o usuário queira voltar ao Menu (read_string retornou -1 em $v0).
			bltz $v0, back_to_menu
			
			# Caso o usuário tenha inserido caracteres não binários (read_string retornou 0 em $v0).
			beqz $v0, remove_menu_invalid
			
			# Caso a entrada esteja de acordo com o previsto, move-se o endereço da 'string', para ela 
			# ser tratada na inserção. Da mesma forma, retoma a raiz de $s0.
			move $a0, $v1
			move $a1, $s0
			
			# Busca-se a string inserida na Trie, antes da remoção.
			jal find
			
			# Caso a string não tenha sido encontrada:
			beqz $v0, remove_menu_notfound
			
			# Caso a string tenha sido encontrada, notifica o usuário e a imprime.
			li $v0, 4
			la $a0, str_KeyFound
			syscall
			
			li $v0, 4
			la $a0, mem_ScanStr
			syscall
			
			# Inicia a impressão do caminho.
			li $v0, 4
			la $a0, str_Path
			syscall
			
			# Atualiza os parâmetros necessários ('string' e raiz da árvore).
			la $a0, mem_ScanStr
			move $a1, $s0
			
			# Imprime o caminho de fato.
			jal print_path
			
			# Pula uma linha ('\n').
			li $v0, 4
			la $a0, str_new_line
			syscall
			
			# Atualiza os parâmetros necessários ('string' e raiz da árvore).
			la $a0, mem_ScanStr
			move $a1, $s0
			
			# Realiza a remoção efetiva da 'string' da Trie
			jal remove
			
			# Notifica ao usuário o sucesso da operação.
			li $v0, 4
			la $a0, str_remove_success
			syscall
			
			# Retorna ao menu de remoção para remover outra 'string'.
			j remove_menu
			
			
remove_menu_notfound:	
			# Notifica o usuário de que a 'string' não foi encontrada.
			li $v0, 4
			la $a0, str_KeyNotFound
			syscall
			
			# Inicia a impressão do caminho:
			li $v0, 4
			la $a0, str_Path
			syscall
			
			# Atualiza os parâmetros necessários ('string' e raiz da árvore).
			la $a0, mem_ScanStr
			move $a1, $s0
			
			# Imprime o caminho de fato.
			jal print_path
			
			# Pula uma linha ('\n').
			li $v0, 4
			la $a0, str_new_line
			syscall
			
			# Retorna ao menu de remoção de strings para remover outra 'string'.
			j remove_menu
			
			
remove_menu_invalid:	
			# Notifica o usuário de que existem caracteres inválidos presentes na 'string' inserida.
			li $v0, 4
			la $a0, str_invalid_key
			syscall
			
			# Retorna ao menu de remoção de strings para remover outra 'string'.
			j remove_menu





print_menu:
			# Posiciona a raíz e realiza a impressão de toda a árvore por nível.
			move $a0, $s0
			jal print_trie
			
			# Volta ao Menu Principal
			j menu





back_to_menu:		
			# Notifica o usuário de que ele está voltando ao Menu Principal.
			li $v0, 4
			la $a0, str_ReturningMenu
			syscall
			
			# Volta ao Menu Principal
			j menu





exit_program:		
			# Notifica o usuário do fim da execução do programa.
			li $v0, 4
			la $a0, str_menu_exit
			syscall
			
			# Termina a execução do programa.
			li $v0, 10
			syscall





# Função para ler uma string de entrada e determinar se é um valor binário válido (removendo também o '\n').
# Retorna em $v0 = 1 se a string for válida, 0 se for inválida e -1 se for para regressar ao Menu.
read_string:		
			# Inicia a leitura de uma string (com tamanho máximo de 16 caracteres).			
			li $v0, 8
			la $a0, mem_ScanStr
			li $a1, 16
			syscall
			
			# Atualiza os parâmetros para a análise.
			la $t0, mem_ScanStr
			move $v1, $t0
			lb $t1, 0($t0)
			
			# Carrega os caracteres para futura comparação.
			li $t2, 48 # '0'
			li $t3, 49 # '1'
			li $t4, 10 # '\n'
			li $t5, 45 # '-'
			
			# Verifica se o usuário deseja retornar ao Menu.
			beq $t1, $t5, read_string_should_return
			
			# Verifica se trata-se de uma string composta somente por um '\n' (uma string inválida).
			beq $t1, $t4, read_string_invalid
			
			# Verifica se a string está totalmente vazia (uma string inválida).
			beqz $t1, read_string_invalid
			
			
# Percorrerá a string caractere a caractere.			
read_string_loop:	
			# Carrega tem $t1 o primeiro caractere.
			lb $t1, 0($t0)
			
			# Verifica se é '\0' (indicando que a leitura acabou).
			beqz $t1, read_string_success
			
			# Incrementa '1' para dessa forma percorrer toda a string de entrada.
			addi $t0, $t0, 1
			
			# Continua a análise caso trate-se de 0 ou 1.
			beq $t1, $t2, read_string_loop
			beq $t1, $t3, read_string_loop
			
			# Finaliza a análise e retorna a string em $v1 caso ela esteja de acordo com as especificações.
			beqz $t1, read_string_success
			
			# Verifica que se configura um caractere inválido, terminando a leitura com erro.
			bne $t1, $t4, read_string_invalid
			
			# Guarda '0' no endereço de $t0 decrementado de 1.
			sb $zero, -1($t0)
			
			
read_string_success:		
			# Retorna 1 para indicar que a leitura foi um sucesso e volta à "função" que chamou read_string.
			li $v0, 1
			jr $ra
			
			
read_string_should_return:	
			# Recupera em $t1 o conteúdo presente em $t0 incrementado de 1.
			lb $t1, 1($t0)
			
			# Verifica se depois do '-' realmente vem o caractere '1'.
			bne $t1, $t3, read_string_invalid
			lb $t1, 2($t0)
			
			# Caso venha, retorna ao Menu na função seguinte.
			beqz $t1, read_string_return_menu
			
			# Caso contrário, configura-se um erro (string inválida). 
			bne $t1, $t4, read_string_invalid
			sb $zero, 2($t0) 
			
			
read_string_return_menu:		
			# Retorna -1 para indicar que o usuário deseja retornar ao menu e volta à "função" que chamou read_string.
			li $v0, -1
			jr $ra
			
			
read_string_invalid:	
			# Retorna 0 para indicar erro na string de entrada e volta à "função" que chamou read_string.
			li $v0, 0
			jr $ra





# Função para alocar um "nó" dinamicamente.
alloc_node:		
			# Aloca 12 bytes dinamicamente.
			li $v0, 9
			li $a0, 12
			syscall
			
			sw $zero, 0($v0) # Posiciona a 'flag' - indicando se é nó terminal ou não (inicializando-a como zero). 
			sw $zero, 4($v0) # Posiciona o endereço para o nó da esquerda.
			sw $zero, 8($v0) # Posifiona o endereço para o nó da direita.
			
			# Retorna à função que chamou alloc_node.
			jr $ra





# Função para inserir um elemento na árvore.
insert:			
			# Decrementa o ponteiro da stack de 4 posições (abrindo espaço para armazenar 4 bytes).
			addi $sp, $sp -4
			
			# Salva na stack o endereço da "função" a qual chamou 'insert'.
			sw $ra, 0($sp)
			
			# Move para $t0 e $t2 a string lida e o endereço da raiz, respectivamente.
			move $t0, $a0
			move $t2, $a1
			
			# Carrega valores para futura comparação.
			li $t4, 48 # '0'
			li $t5, 49 # '1'
			li $t6, 1
			
			
insert_check_zero:	
			# Move o endereço armazenado em $t2 para $t1, para guardar o nó atual.
			move $t1, $t2
			
			# Carrega o primeiro byte de $t0 para $t3.
			lb $t3, 0($t0)
			
			# Verifica se o caractere lido é 1.
			bne $t3, $t4, insert_check_one 
			
			# Se não, desloca o ponteiro presente em $t0.
			addi $t0, $t0, 1
			
			# Carrega em $t2 o endereço do nó a esquerda de $t1.
			lw $t2, 4($t1)
			
			# Checa pra ver se não é nulo.
			bnez $t2, insert_check_zero
			
			# Caso seja zero, aloca um nó.
			jal alloc_node
			
			# Passa o endereço do novo nó para $t2.
			move $t2, $v0
			
			# Coloca o novo nó alocado no ponteiro de $t1.
			sw $t2, 4($t1)
		
			# Reinicia a checagem do próximo caractere.
			j insert_check_zero
			
			
insert_check_one:	
			# Como já realizada a verificação de se trata de um zero, se não for 1 a string acabou.
			bne $t3, $t5, insert_end
			
			# Incrementa de 1 $t0.
			addi $t0, $t0, 1 
			
			# Carrega em $t2 o endereço do nó a direita de $t1.
			lw $t2, 8($t1)
			
			# Checa para ver se não é nulo.
			bnez $t2, insert_check_zero
			
			# Caso seja zero, aloca um nó.
			jal alloc_node
			
			# Passa o endereço do novo nó para $t2.
			move $t2, $v0
			
			# Coloca o novo nó alocado no ponteiro de $t1.
			sw $t2, 8($t1)
			
			# Reinicia a checagem do próximo caractere.
			j insert_check_zero
			
			
insert_end:		
			# A inserção chegou ao fim. Retoma o endereço da "função" que chamou 'insert'.
			lw $v0, 0($t1)
			sw $t6, 0($t1)
			lw $ra, 0($sp)

			# Incrementa o ponteiro da stack de 4 posições.
			addi $sp, $sp, 4
			
			# Retorna à função que chamou 'insert'.
			jr $ra





# Função para imprimir o caminho percorrido.
print_path:		
			# Move para $t0 e $t2 a string lida e o endereço da raiz, respectivamente.
			move $t0, $a0
			move $t1, $a1
			
			# Carrega os caracteres para futura comparação.
			li $t3, 48 # '0'
			li $t4, 49 # '1'
			
			
print_path_is_zero:	
			# Carrega o byte para o qual aponta $t0.
			lb $t2, 0($t0)
			
			# Caso não seja zero, desvia para a função print_path_is_one.
			bne $t2, $t3, print_path_is_one
			
			# Move o ponteiro de $t0.
			addi $t0, $t0, 1
			
			# Carrega em $t1 o endereço do caminho esquerdo.
			lw $t1, 4($t1)
			
			# Se o endereço carregado em $t1 for nulo, finaliza a impressão do caminho percorrido.
			beqz $t1, print_path_end
			
			# Imprime o caminho esquerdo.
			li $v0, 4
			la $a0, str_PathLeft
			syscall
			
			# Reinicia a checagem para o proximo nó.
			j print_path_is_zero
			
			
print_path_is_one:	
			# Como o zero já foi checado, se não for 1 acabou a 'string'.
			bne $t2, $t4, print_path_end
			
			# Move o ponteiro $t0.
			addi $t0, $t0, 1
			
			# Carrega em $t1 o endereço do caminho direito.
			lw $t1, 8($t1)
			
			# Se o endereço carregado em $t1 for nulo, finaliza a impressão do caminho percorrido.
			beqz $t1, print_path_end 
			
			# Se não, imprime o caminho direito.
			li $v0, 4
			la $a0, str_PathRight
			syscall
			
			# Reinicia a checagem para o próximo nó.
			j print_path_is_zero
			
			
print_path_end:		
			# Caso tenha chego ao fim, retorna à função que chamou print_path.
			jr $ra
		




# Função para encontrar um dado elemento na árvore.
find:			
			# Atualiza em $t0 e em $t1 a string e a raíz, respectivamente.
			li $v0, 0
			move $t0, $a0
			move $t1, $a1
			
			# Carrega valores para futuras comparações.
			li $t3, 48 # '0'
			li $t4, 49 # '1'
			
			
find_check_zero:		
			# Carrega o primeiro caratere em $t2.
			lb $t2, 0($t0)
			
			# Caso ele não seja o 0, chama find_check_one para buscar o 1.
			bne $t2, $t3, find_check_one 
			
			# Caso ele seja igual, desloca-se o ponteiro de um byte.
			addi $t0, $t0, 1
			
			# Carrega em $t1 o endereço do caminho esquerdo.
			lw $t1, 4($t1)
			
			# Se $t1 não for nulo continua a checagem para os próximos caracteres.
			bnez $t1, find_check_zero
			
			# Caso $t1 seja nulo, isso indica o fim da busca.
			j find_end
			
			
find_check_one:		
			# Como o zero já foi checado, se não for 1 acabou a 'string'.
			bne $t2, $t4, find_end_string
			
			# Move o ponteiro de uma posição.
			addi $t0, $t0, 1
			
			# Carrega em $t1 o enderço do caminho direito.
			lw $t1, 8($t1)
			
			# Se $t1 não for nulo, continua a checagem para os próximos caracteres.
			bnez $t1, find_check_zero
			
			# Termina a execução do programa.
			j find_end
			
			
find_end_string:		
			# Carrega o último item armazenado na pilha.
			lw $t2, 0($t1)
			
			# Se o endereço em $t2 for nulo, a busca chegou ao fim (retorna 0).
			beqz $t2, find_end
			
			# Configura o valor de retorno para 1.
			li $v0, 1
			
			
find_end:		
			# Retorna para a função que chamou 'find'.
			jr $ra





remove_node:		
			# Desloca o ponteiro da stack.
			addi $sp, $sp, -12
			
			# Empilha os dados presentes em $ra, $s0 e $s1.
			sw $ra, 0($sp)
			sw $s0, 4($sp)
			sw $s1, 8($sp)
			
			move $s0, $a0
			move $s1, $a1
			
			# Carrega o primeiro byte da string em #t0.
			lb $t0, 0($s0)
			
			# Carrega o nó da esquerda e da direita em $t1 e $t2, respectivamente.
			lw $t1, 4($s1)
			lw $t2, 8($s1) 
			
			# Configura o valor de retorno como 0.
			li $v0, 0
			
			# Se $t0 não for nulo.
			bnez $t0, remove_node_check_zero
			
			# Se $t1 não for nulo.
			bnez $t1, remove_node_set_removed
			
			# Se $t2 não for nulo.
			bnez $t2, remove_node_set_removed
			
			# Configura o valor de retorno como 1.
			li $v0, 1
			j remove_node_end
			
			
remove_node_check_zero:	
			# Caso o nó seja diferente de 0, desvia para remove_node_one
			bne $t0, $t3, remove_node_one
			
			# Incrementa de 1 o ponteiro da string.
			addi $a0, $s0, 1
			
			# Chama recursão para nó da esquerda ('0').
			move $a1, $t1
			jal remove_node
			
			# Não é possível remover o nó da esquerda pois ele sustenta outros valores.
			beqz $v0, remove_node_end
			
			# Caso o nó da esquerda seja removível, configura o ponteiro como NULL.
			sw $zero, 4($s1)
			
			# Verifica se o nó atual é removível. Isso é retornado para a função acima na pilha.
			li $v0, 0
			lw $t1, 0($s1)
			bnez $t1, remove_node_end
			lw $t1, 8($s1)
			bnez $t1, remove_node_end
			li $v0, 1
			
			# Finaliza a remoção.
			j remove_node_end
			
			
remove_node_one:		
			# Incrementa o ponteiro da String de 1 posição.
			addi $a0, $s0, 1
			
			# Recupera em $a1 o endereço do nó da direita.
			move $a1, $t2
			
			# Chama a recursão para o nó da direita ('1').
			jal remove_node
			
			# Não é possível remover o nó da esquerda pois ele sustenta outros valores.
			beqz $v0, remove_node_end
			
			# Caso o nó da direita seja removível, configura o ponteiro como NULL.
			sw $zero, 8($s1)
			
			# Verifica se o nó atual é removível. Isso é retornado para a função acima na pilha.
			li $v0, 0
			lw $t1, 0($s1)
			
			# Certifica-se de que não está removendo um nó cujo outros nós dependem.
			bnez $t1, remove_node_end
			lw $t1, 4($s1)
			bnez $t1, remove_node_end
			li $v0, 1
			
			# Finaliza a remoção
			j remove_node_end
			
			
remove_node_set_removed:	
			# Este nó será removido, colocar Flag que indica se é terminal ou zerada (não-terminal).
			sw $zero, 0($s1)
			
			
remove_node_end:		
			# Carregar valores dos registradores vindos da pilha.
			lw $ra, 0($sp)
			lw $s0, 4($sp)
			lw $s1, 8($sp)
			
			# Redesloca o ponteiro da pilha.
			addi $sp, $sp, 12
			
			# Retorna para a função que chamou remove_node_end.
			jr $ra


remove:			
			# Carrega na pilha os registradores que serão usados posteriormente.
			addi $sp, $sp, 8
			sw $ra, 0($sp)
			sw $s1, 4($sp)
			
			# Carrega-se o endereço da Raíz.
			move $s1, $a1
			
			# Carrega o primeiro endereço da string (primeiro byte).
			lb $t0, 0($a0)
			
			# Carrega o nó ã esquerda do root.
			lw $t1, 4($s1)
			
			# Carrega o nó à direita do root.
			lw $t2, 8($s1)
			
			# Configura o valor ('0')em $t3 para futura comparação.
			li $t3, 48
			
			
remove_check_zero:	
			# Caso seja 0, continua a execução, se não desvia para a função que trata 0 1.
			bne $t0, $t3, remove_one
			
			# Desloca o ponteiro da string.
			addi $a0, $a0 1
			
			# Retoma o nó da esquerda ('0').
			move $a1, $t1
			
			# Chama a função para remover o dígito binário.
			jal remove_node
			
			# Caso não seja necessário zerar o nó da esquerda.
			beqz $v0, remove_end
			
			# Caso contrário:
			sw $zero, 4($s1)
			
			# Finaliza a remoção do nó.
			j remove_end
			
			
remove_one:		
			# Sabe-se que trata-se do '1'
			# Incrementa o ponteiro da string.
			addi $a0, $a0 1
			
			#Recupera o nó da direita ('1').
			move $a1, $t2
			
			# Chama a função para remover o dígito binário.
			jal remove_node
			
			# Verifica se é necessário zerar o nó da direita. Case não seja, desvia para remove_end.
			beqz $v0, remove_end
			
			# Caso seja necessário, zera o nó da direita.
			sw $zero, 8($s1)
			
			
remove_end:		
			# Recupera os valores dos registradores previamente empilhados.
			lw $ra, 0($sp)
			lw $s1, 4($sp)
			
			# Reposiciona o ponteiro da pilha.
			addi $sp, $sp, 8
			
			# Retorna para a função que chamou remove.
			jr $ra



# Função para imprimir o endereço presente em $a0.
print_node_address:	
			# Caso em $a0 o valor seja 0, imprime "Null".
			beqz $a0, print_node_address_null
			
			# Caso contrário, imprime o endereço em decimal (pois o SPIM não suporta imprimir em base hexadecimal, de código 34).
			li $v0, 1
			syscall
			
			# Retorna à função que chamou print_node_address.
			jr $ra
			
			
print_node_address_null:	
			# Caso seja nulo o endereço presente em $a0, notifica o usuário.
			li $v0, 4
			la $a0, str_PrintLevelNodeNull
			syscall
			
			# Retorna à função que chamou print_node_address.
			jr $ra



# Função para imprimir a árvore até um desejado nível, iniciando em $a1 (root) e indo até o nível desejado $a0.
# Se algum nó foi impresso, ela retorna 1 em $v0.
print_level:		
			# Salvar os registradores que serão futuramente usados na pilha.
			addi $sp, $sp, 20
			sw $ra, 0($sp)
			sw $s0, 4($sp)
			sw $s1, 8($sp)
			sw $s2, 12($sp)
			sw $s3, 16($sp)
			
			# Recupera o nível atual da recursão da árvore.
			move $s0, $a0
			
			# Recupera o nó atual.
			move $s1, $a1
			
			# Recupera o valor do nó atual (0 para esquerda ou 1 para direita).
			move $s2, $a2
			
			# Inicia retornando 0.
			li $v0, 0
			
			# Caso o nó seja nulo, finaliza a recursão.
			beqz $s1, print_level_end
			
			# Caso ainda não esteja no nível necessário da árvore, continua a recursão.
			bgtz $s0, print_level_recursive
			
			# 'Flag' para indicar se a impressão de um "N%d" é necessária.
			beqz $t1, print_level_print
			
			# Imprime uma a string 'str_PrintLevel'.
			li $t1, 0
			li $v0, 4
			la $a0, str_PrintLevel
			syscall
			
			# Imprime o inteiro presente em $t2.
			li $v0, 1
			move $a0, $t2
			syscall
			
print_level_print:	
			# Efetivamente imprimir o nó.
			li $v0, 4
			la $a0, str_PrintLevelNode
			syscall
			
			li $v0, 1
			move $a0, $s2
			syscall
			
			# Verifica se é um nó terminal.
			lw $t0, 0($s1)
			bnez $t0, print_level_print_t
			
			# Para um nó não terminal.
			li $v0, 4
			la $a0, str_PrintLevelNodeNT
			syscall
			
			# Finaliza impressão do nó/nível.
			j print_level_print_end
			
			
print_level_print_t:	
			# Para um nó terminal
			li $v0, 4
			la $a0, str_PrintLevelNodeT
			syscall


print_level_print_end:	
			# Imprimir o endereço do nó da esquerda.
			lw $a0, 4($s1)
			jal print_node_address
			li $v0, 4
			la $a0, str_PrintLevelNodeComma
			syscall
			
			# Imprimir o endereço do nó da direita.
			lw $a0, 8($s1)
			jal print_node_address
			li $v0, 4
			la $a0, str_PrintLevelNodeEnd
			syscall
			
			# Configurar o endereço de retorno como 1.
			li $v0, 1
			
			# Finaliza a impressão.
			j print_level_end
			
			
print_level_recursive:	
			# Caso a execução chegou a esse ponto, é sinal de que o nível desejado ainda não foi atingido.
			addi $a0, $s0, -1
			lw $a1, 4($s1)
			li $a2, 0
			
			# Chama a recursão para a esquerda.
			jal print_level
			
			move $s3, $v0
			addi $a0, $s0, -1
			lw $a1, 8($s1)
			li $a2, 1
			
			# Chama a recursão para a direita.
			jal print_level
			
			# Verifica se chegou ao fim a impressão.
			bnez $v0, print_level_end
			
			# Configura o valor de retorno para 1.
			li $v0, 1
			
			# Verifica se chegou ao fim da impressão.
			bnez $s3, print_level_end
			
			# Configura o valor de retorno para 1.
			li $v0, 0
			
			# Finalizar a impressão.
			j print_level_end
			
			
print_level_end:		
			# Recuperar os valores armazenados na pilha.
			lw $ra, 0($sp)
			lw $s0, 4($sp)
			lw $s1, 8($sp)
			lw $s2, 12($sp)
			lw $s3, 16($sp)
			
			# Redesloca o ponteiro da pilha.
			addi $sp, $sp, -20
			
			# Retorna à função que chamou print_level.
			jr $ra


# Função que imprime a raiz (nível 0 da árvore) e chama a função que imprime todos os níveis, até n-1.
print_trie:		
			# Salva os registradores que serão usados na pilha (como o endereço de retorno para a "função" que chamou 'print_trie').
			addi $sp, $sp, 8
			sw $ra, 0($sp)
			sw $s0, 4($sp)
			
			move $s0, $a0
			
			# Imprime a raiz.
			li $v0, 4
			la $a0, str_PrintLevelRoot
			syscall
			lw $a0, 4($s0)
			jal print_node_address
			li $v0, 4
			la $a0, str_PrintLevelNodeComma
			syscall
			lw $a0, 8($s0)
			jal print_node_address
			li $v0, 4
			la $a0, str_PrintLevelNodeEnd
			syscall
			
			# Inicializa $t2 como zero.
			li $t2, 0
			

# Para cada nível de 0 a n-1, a função print_trie_level é chamada.
print_trie_loop:		
			# Imprime uma nova linha.
			li $v0, 4
			la $a0, str_new_line
			syscall
			
			# Incrementa $t2 (nível atual).
			addi $t2, $t2, 1
			
			li $t1, 1
			move $a0, $t2
			move $a1, $s0
			jal print_level
			bnez $v0, print_trie_loop
			lw $ra, 0($sp) # Recuperar registradores da pilha
			lw $s0, 4($sp)
			addi $sp, $sp, -8
			jr $ra



