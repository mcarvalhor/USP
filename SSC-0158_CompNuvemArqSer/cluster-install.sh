#!/usr/bin/env bash

HTTP_EXPOSED_PORT=8080

echo "Este script vai instalar a aplicação em um cluster distribuído."
echo "Antes de começar, por favor note que precisamos do acesso 'root' de todas as máquinas em que os serviços serão instalados!"
echo "Configure o acesso root via SSH. Se você desejar, também pode utilizar uma chave SSH - evitando ter que digitar a senha toda vez que formos nos conectar à máquina."
echo "Instale o Docker e o docker-compose em todas as máquinas e ative seu serviço."
echo "Vamos precisar também das suas chaves de API do Twitter. Se você ainda não tem, crie uma agora."
echo
echo "Vamos começar..."
echo



read -r -p "Onde será instalada a aplicação (entre com o hostname)? " app_host
read -r -p "Onde será instalado o Kafka (entre com o hostname)? " kafka_host
read -r -p "Onde será instalado o banco de dados MySQL (entre com o hostname)? " mysql_host

read -r -p "Qual é sua chave pública da API do Twitter (API Key)? " api_key
read -r -p "Qual é sua chave secreta da API do Twitter (API Secret)? " api_secret



echo
echo "===== Dados da Instalação ====="
echo "A aplicação será instalada em:             root@${app_host}"
echo "O Kafka será instalado em:                 root@${kafka_host}"
echo "O banco de dados MySQL será instalado em:  root@${mysql_host}"
echo "A chave pública do Twitter é:              ${api_key}"
echo "A chave secreta do Twitter é:              ${api_secret}"
echo "==============================="
echo
read -r -p "(se estiver tudo certo, pressione [ENTER] para continuar com a instalação) "



echo
echo "Gerando arquivo de configuração..."

cp config.sample.json config.json
sed -i "s/YOUR_DATABASE_HOSTNAME/${mysql_host}/g" config.json
sed -i "s/YOUR_DATABASE_USERNAME/root/g" config.json
sed -i "s/YOUR_DATABASE_PASSWORD/root/g" config.json
sed -i "s/YOUR_DATABASE_NAME/TwitterAnalyser/g" config.json
sed -i "s/YOUR_TWITTER_API_KEY/${api_key}/g" config.json
sed -i "s/YOUR_TWITTER_API_SECRET/${api_secret}/g" config.json
sed -i "s/YOUR_KAFKA_CONNECTION_STRING/${kafka_host}:29092/g" config.json



echo
echo "Gerando arquivo de implantação do Kafka..."

cp kafka-docker-compose.sample.yaml docker-compose.yaml
sed -i "s/YOUR_KAFKA_CONNECTION_HOST/${kafka_host}/g" docker-compose.yaml



echo
echo "Verificando máquinas..."
ssh root@${kafka_host} "which docker >/dev/null"
if [[ $? -ne 0 ]]
then
	echo
	echo "Por favor, instale o Docker na máquina ${kafka_host}!"
	exit 1
fi
ssh root@${kafka_host} "which docker-compose >/dev/null"
if [[ $? -ne 0 ]]
then
	echo
	echo "Por favor, instale o 'docker-compose' na máquina ${kafka_host}!"
	exit 1
fi
ssh root@${mysql_host} "which docker >/dev/null"
if [[ $? -ne 0 ]]
then
	echo
	echo "Por favor, instale o Docker na máquina ${mysql_host}!"
	exit 1
fi
ssh root@${app_host} "which docker >/dev/null"
if [[ $? -ne 0 ]]
then
	echo
	echo "Por favor, instale o Docker na máquina ${app_host}!"
	exit 1
fi



echo
echo "Implantando Kafka..."

ssh root@${kafka_host} "rm -Rf ~/TwitterAnalyser && mkdir ~/TwitterAnalyser"
scp docker-compose.yaml root@${kafka_host}:~/TwitterAnalyser/.
ssh root@${kafka_host} "cd ~/TwitterAnalyser; docker-compose up -d"



echo
echo "Implantando MySQL..."

ssh root@${mysql_host} "docker run --name TA_mysql -e MYSQL_ROOT_PASSWORD=root -p 3306:3306 -d mysql"
read -r -t 240 -p "Aguardando o MySQL iniciar... (não se preocupe, essa etapa do MySQL realmente vai demorar muito)"
scp database-deploy.sql root@${mysql_host}:/tmp/TA_db-deploy.sql
ssh root@${mysql_host} "docker exec -i TA_mysql mysql -uroot -proot < /tmp/TA_db-deploy.sql"



echo
echo "Implantando aplicação..."

ssh root@${app_host} "rm -Rf ~/TwitterAnalyser && mkdir ~/TwitterAnalyser"
scp -r Code/* root@${app_host}:~/TwitterAnalyser/.
scp config.json root@${app_host}:~/TwitterAnalyser/.
ssh root@${app_host} "cd ~/TwitterAnalyser/ && docker build -t ta_app-image ."
ssh root@${app_host} "docker run --name TA_app -p ${HTTP_EXPOSED_PORT}:8080 -d ta_app-image"



echo
echo "Limpando arquivos temporários..."

rm config.json docker-compose.yaml



echo
echo "Pronto! Sua aplicação está rodando em http://${app_host}:${HTTP_EXPOSED_PORT}/."


