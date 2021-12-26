# Projeto de Bases de Dados - Meu Brasil

Neste repositório se encontra o projeto de SCC0640 - Bases de Dados. É recomendável utilizar a aplicação em conjunto com o Docker.

## Integrantes
* Felipe Tetzner (Nº USP 9897870)
* João Pedro Almeida Santos Secundino (Nº USP 10692054)
* Juan Carlos Elias Obando Valdivia (Nº USP 7487156)
* Matheus Carvalho Raimundo (Nº USP 10369014)

## Guia de configuração (sem usar Docker)
* Instale o Python 3 e todas as bibliotecas necessárias para a aplicação:  
``` sudo apt update && sudo apt install python3 python3-pip && sudo pip3 install -r Code/requirements.txt```  
* Inicie um banco de dados PostgreSQL, crie uma nova base de dados para a aplicação e um respectivo usuário com o acesso a esta.  
* Execute o comando para criar um arquivo `config.json`:  
``` cp config.sample.json config.json```  
* Edite o arquivo `config.json`, inserindo as informações do banco de dados PostgreSQL.  

## Guia de execução (sem usar Docker)
* Inicie o comando do Python3:  
``` cd Code; python3 main.py ../config.json```  

## Guia de configuração (usando o Docker)
* Instale e inicie o Docker:  
``` sudo apt update && sudo apt install docker.io && sudo systemctl enable docker && sudo systemctl start docker```  
* Inicie um banco de dados PostgreSQL, crie uma nova base de dados para a aplicação e um respectivo usuário com o acesso a esta. Você pode também utilizar o Docker para criar este banco de dados.  
* Execute o comando para criar um arquivo `config.json`:  
``` cp config.sample.json config.json```  
* Edite o arquivo `config.json`, inserindo as informações do banco de dados PostgreSQL.  
* Construa a imagem da aplicação para o Docker:  
``` docker build -t bd2021_app-image .```  

## Guia de execução (usando o Docker)
* Inicie o container:  
``` docker run --name bd2021_app -p 8080:8080 -d bd2021_app-image```  

## Organização do Repositório
O repositório foi organizado da seguinte maneira:
* [BD/](BD): Diretório com arquivos SQL contendo o DDL e DML da base de dados.
* [BD/DDL/esquema.sql](BD/DDL/esquema.sql): arquivo com todo o esquema da base de dados.
* [BD/DML/dados.sql](BD/DML/dados.sql): arquivo com as tuplas a serem inseridas na base de dados.
* [BD/DML/consultas.sql](BD/DML/consultas.sql): Exemplos de consultas que foram implementadas.
* [Code/](Code): Todo o código de funcionamento da aplicação se encontra aqui.
* [Especificacao.pdf](Especificacao.pdf): Especificação do projeto fornecida pelo docente responsável pela disciplina.
* [Relatorio.pdf](Relatorio.pdf): Relatório final do trabalho (parte escrita).
* [Dockerfile](Dockerfile): Arquivo de configuração do Docker (caso tal seja de interesse).
* [config.sample.json](config.sample.json): Exemplo de arquivo de configuração necessário para funcionamento da aplicação.
