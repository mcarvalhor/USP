<?php

/* Estes são os dados do servidor MySQL */
$dbServer = "HOSTNAME_BD";
$dbUsername = "USUARIO_BD";
$dbPassword = "SENHA_BD";
$dbName = "NOME_BD";

/* Senha de acesso ao painel de administrador que mostra os dados capturados pela extensão */
$accessPassword = "admin";



$sleep = false;

if(file_get_contents("sleep.txt") == "true") {
	$sleep = true;
}

