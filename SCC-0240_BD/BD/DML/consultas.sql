
-- 1. Calcule a quantidade de filmes, músicas e obras disponibilizadas em uma cinemateca, estúdio ou museu
SELECT espacopublico.id, bemcultural.titulo, 'MUSEU', (SELECT COUNT(*) FROM obra WHERE obra.museu = espacopublico.id) FROM bemcultural
	RIGHT JOIN espacopublico ON bemcultural.id = espacopublico.id
	UNION ALL SELECT espacopublico.id, bemcultural.titulo, 'CINEMATECA', (SELECT COUNT(*) FROM filme WHERE filme.cinemateca = espacopublico.id) FROM bemcultural
	RIGHT JOIN espacopublico ON bemcultural.id = espacopublico.id
	UNION ALL SELECT espacopublico.id, bemcultural.titulo, 'ESTUDIO', (SELECT COUNT(*) FROM musica WHERE musica.estudio = espacopublico.id) FROM bemcultural
	RIGHT JOIN espacopublico ON bemcultural.id = espacopublico.id
	ORDER BY 2 ASC, 3 ASC, 1 ASC, 4 DESC;

-- Otimização: criação de índice para 'obra.museu', 'filme.cinemateca', 'musica.estudio' e 'bemcultural.titulo'.



-- 2. Busca parametrizada do usuário (consulta mais complexa da aplicação).
SELECT id, titulo, cidade, estado, natureza, descricao FROM bemcultural WHERE LOWER(titulo) LIKE LOWER('%CRITERIO-DE-BUSCA%') -- Base da consulta.
	AND cidade = 'CIDADE' AND estado = 'ESTADO' -- Se o usuário quiser filtrar por cidade.
	AND natureza = 'NATUREZA' -- Se o usuário quiser filtrar pela natureza do bem-cultural.
	AND id IN (SELECT id FROM espacopublico WHERE `NOME_DA_COLUNA` = TRUE) -- Se o usuário quiser filtrar pelo tipo do espaço público.
	ORDER BY `CRITERIO` `ASC ou DESC` -- Ordenação escolhida pelo usuário.
	LIMIT 15 OFFSET `PRIMEIRA_TUPLA`; -- Paginação.

-- Como notável na consulta acima, é utilizado o operador LIKE em conjunto com %CRITERIO% para busca no título.
-- Isso significa que a complexidade é a mais alta possível: linear em todas as tuplas da tabela.
--
-- Infelizmente, não há o que possa ser feito nesse caso, pois esse é justamente o objetivo dessa consulta:
--  buscar pelo critério em qualquer posição da string (lembrando uma busca do Google).
--
-- Sem o uso de um sistema de busca mais especializado (como FULL TEXT SEARCH, por exemplo - que não faz parte do currículo da disciplina), não é possível implementar essa consulta de maneira eficiente.



-- 3. Busca pelas cidades que são responsáveis pelos bens culturais 25% mais populares.
SELECT bemcultural.cidade, bemcultural.estado, COUNT(*) FROM bemcultural
	WHERE bemcultural.popularidade >= (SELECT percentile_disc(1 - 0.25) WITHIN GROUP
									   (ORDER BY bemcultural.popularidade)
									   FROM bemcultural)
	GROUP BY bemcultural.cidade, bemcultural.estado
	ORDER BY 3 DESC, 2 ASC, 1 ASC;

-- Otimização: a otimização nesta consulta consiste em criar índices índividuais para 'bemcultural.cidade', 'bemcultural.estado' e 'bemcultural.popularidade'.



-- 4. Busca pelas visitas de todos os cidadãos que fizeram qualquer denúncia em 2020.
SELECT visita.cidadao, cidadao.nome, visita.espacopublico, bemcultural.titulo, visita.datahora, visita.review FROM visita
	INNER JOIN cidadao ON cidadao.idoauth = visita.cidadao
	INNER JOIN espacopublico ON espacopublico.id = visita.espacopublico
	INNER JOIN bemcultural ON bemcultural.id = visita.espacopublico
	WHERE cidadao.idoauth IN ( SELECT denuncia.cidadao FROM denuncia
							WHERE denuncia.datahora BETWEEN TIMESTAMP '2020-01-01' AND TIMESTAMP '2021-01-01'
						);

-- Otimização: esta é uma consulta bem complexa, e a quantidade de dados retornados pode ser extremamente grande.
-- Neste caso, a otimização maior fica a cargo da chave estrangeira em 'visita.espacopublico' e da criação de um índice em 'denuncia.datahora'.



-- 5. Busca por todas as informações de uma determinada obra.
SELECT obra.museu, obra.criacao, obra.genero, obra.tecnica, obra.dimensoes, obra_autores.autor FROM obra
	LEFT JOIN obra_autores ON obra_autores.id = obra.id
	WHERE obra.id = `OBRA-A-SER-BUSCADA` ;

-- Otimização: nenhuma, pois todas as colunas utilizadas na consulta são PK.



-- 6. Busca por todas as informações de uma determinada música.
SELECT musica.estudio, musica.gravacao, musica.producao, musica.duracao, musica.formato, musica_compositor.compositor, NULL FROM musica
	LEFT JOIN musica_compositor ON musica_compositor.id = musica.id
	WHERE musica.id = `MUSICA-A-SER-BUSCADA`
	UNION ALL SELECT musica.estudio, musica.gravacao, musica.producao, musica.duracao, musica.formato, NULL, musica_genero.genero FROM musica
	LEFT JOIN musica_genero ON musica_genero.id = musica.id
	WHERE musica.id = `MUSICA-A-SER-BUSCADA` ;

-- Otimização: quebrar esta consulta única em 3 consultas diferentes, evitando colunas com valor nulo.
-- Observe que apesar de parecer que sim, não há tuplas espúrias pois cada tupla retornada contém uma informação única e que será exibida para o usuário final.



-- 7. Inserção de uma nova denúncia.
INSERT INTO denuncia (Cidadao, BemCultural, DataHora, Descricao, TipoOcorrência) values
	(`ID-CIDADAO`, `ID-BEM-CULTURAL`, TIMESTAMP `DATA/HORA`, `DESCRICAO-FORNECIDA`, `TIPO-OCORRENCIA-SELECIONADO`);

-- Otimização: nenhuma necessária.



-- Observe que há também algumas consultas usadas na aplicação que não foram dispostas neste arquivo.
