# Trabalho - Twitter Analyser
### Computação em Nuvem e Arquitetura Orientadas a Serviços (SSC-0158)
Neste repositório se encontra o trabalho de Computação em Nuvem e Arquitetura Orientadas a Serviços.

## Guia de configuração
* Instale o Docker e o 'docker-compose' em todas as máquinas que vai utilizar para a aplicação:  
``` sudo apt-get install docker.io docker-compose ```  
* A partir de qualquer máquina que tenha acesso às outras, execute o script abaixo e siga as instruções na tela:  
``` ./cluster-install.sh ```  

## Guia de execução
Ao instalar utilizando o script "./cluster-install.sh", a aplicação já se inicia automaticamente. Caso você deseje encerrar a aplicação:
* Entre em cada uma das máquinas em que a aplicação foi instalada e execute o comando:  
``` docker ps ```  
* Encerre todos os contâiners docker em funcionamento.  
  
Para iniciar a aplicação novamente:
* Vá para a máquina em que o Kafka está instalado e execute o comando:  
``` docker ps -a ```  
* Inicie o contâiner do Zookeeper e, quando funcionando, inicie o contâiner do Kafka:  
``` docker start TA_zookeeper && sleep 10 && docker start TA_kafka && sleep 10```  
* Agora vá para a máquina em que o banco de dados MySQL está instalado e execute o comando:  
``` docker ps -a ```  
* Inicie o contâiner do MySQL:  
``` docker start TA_mysql && sleep 10```  
* Por fim, vá para a máquina em que o servidor web (a aplicação em si) está instalado e execute o comando:  
``` docker ps -a ```  
* Inicie o contâiner do servidor web:  
``` docker start TA_app && sleep 10```  

## Organização do Repositório
O repositório foi organizado da seguinte maneira:
* [Code/](Code): Todo o código de funcionamento da aplicação se encontra aqui.
* [Especificacao.pdf](Especificacao.pdf): Especificação do projeto fornecida pelo docente.
* [Relatorio-Parte1.pdf](Relatorio-Parte1.pdf): Relatório parcial do trabalho (parte escrita).
* [Relatorio-Parte2.pdf](Relatorio-Parte2.pdf): Relatório final do trabalho (parte escrita).


