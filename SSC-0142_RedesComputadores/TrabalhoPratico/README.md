# Trabalho Prático de Redes de Computadores - 2019.1
Trabalho prático de Redes de Computadores.

# Sala Inteligente

## Membros do Grupo
- Ana Carolina Fainelo de Oliveira (Nº  10284542)
- Carolina Arenas Okawa (Nº 10258876)
- Matheus Carvalho Raimundo (Nº 10369014)
- Matheus dos Santos Luccas (Nº 9005961)

## Como executar
Para executar o trabalho, você vai precisar executar vários programas Java.
- Em alguns sistemas operacionais, basta clicar duas vezes no arquivo _X.jar_.
- Em outros sistemas operacionais, é possível executar no terminal (bash) através do comando `java -jar X.jar`.

Siga os passos abaixo para que a execução ocorra com sucesso:
- Inicie o [gerenciador](Gerenciador.jar). Ele será apenas um servidor, então ele vai solicitar uma porta para servir. Lembre-se de definir uma porta que não esteja já em uso em seu computador.
- Inicie todos os alimentadores: [iluminação](SistemaIluminacao.jar), [ar-condicionado](AlimentadorAr.jar) e [projetor](AlimentadorProjetor.jar). Cada um desses funciona como servidor e cliente para o gerenciador. Isso significa que eles vão solicitar uma porta para servir, e essa porta também deve ser única para cada um desses. Eles também vão solicitar o endereço de host e porta do gerenciador (iniciado anteriormente) no formato adequado. Você pode usar "localhost:PORTA" no caso de estar executando tudo em uma mesma máquina.
- Inicie todos os sensores: [presença](SensorPresenca.jar), [leitor de cartão](LeitorCartao.jar) e [chave do projetor](ChaveProjetor.jar). Estes funcionam apenas como cliente para o gerenciador. Sendo assim, vão solicitar o endereço de host e porta do gerenciador (iniciado no primeiro passo). Você pode usar "localhost:PORTA" no caso de estar executando tudo em uma mesma máquina.
- Inicie a [interface gráfica do professor](UI.jar). Ela funciona também apenas como cliente, e vai solicitar o endereço de host e porta do gerenciador, do mesmo modo que antes.

## Código-fonte
No diretório [Codigo-fonte](Codigo-fonte) encontram-se todos os projetos do trabalho prático e sua implementação em Java.

