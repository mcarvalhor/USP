CREATE TABLE Cidade (
	
	Nome VARCHAR(20) NOT NULL,
	Estado VARCHAR(26) NOT NULL,
	LocalLat REAL NOT NULL,
	LocalLong REAL NOT NULL,
	
	CONSTRAINT PK_CIDADE PRIMARY KEY(Nome, Estado),
	CONSTRAINT UN_CIDADE UNIQUE(LocalLat, LocalLong)
);

CREATE SEQUENCE S_BCULTURAL START WITH 10000;

-- Tabelas relacionadas à entidade Bem Cultural público ------------------------
CREATE TABLE BemCultural (
	
	Id INT NOT NULL DEFAULT nextval('S_BCULTURAL'),
	Titulo VARCHAR(50) NOT NULL,
	Cidade VARCHAR(20) NOT NULL,
	Estado VARCHAR(26) NOT NULL,
	Popularidade REAL DEFAULT 0.0,
	Natureza VARCHAR(20),
	Descricao VARCHAR(1000) DEFAULT 'Sem descrição.',
	DataCadastro DATE DEFAULT CURRENT_TIMESTAMP,
	
	CONSTRAINT PK_BCULTURAL PRIMARY KEY(ID),
	CONSTRAINT UN_BCULTURAL UNIQUE(Titulo, Cidade, Estado),
	CONSTRAINT FK_CIDADE FOREIGN KEY(Cidade, Estado) REFERENCES Cidade(Nome, Estado) ON DELETE NO ACTION,
	CONSTRAINT CK_NATUREZA CHECK(Natureza IN ('ESPACOPUBLICO', 'MONUMENTO', 'EDFTOMBADA', 'OBRA', 'MUSICA', 'FILME'))
);

ALTER SEQUENCE S_BCULTURAL OWNED BY BemCultural.Id;

CREATE TABLE BemCultural_PalavrasChave(
	
	Id INT NOT NULL,
	Palavra VARCHAR(20) NOT NULL,
	
	CONSTRAINT PK_BCULTURAL_PALCHAVE PRIMARY KEY(Id, Palavra),
	CONSTRAINT FK_BCULTURAL FOREIGN KEY(Id) REFERENCES BemCultural(Id) ON DELETE CASCADE
);

-- Tabelas relacionadas à entidade Espaço público ------------------------
CREATE TABLE EspacoPublico (
	
	Id INT NOT NULL,
	Localizacao POINT,
	EmFuncionamento BOOLEAN DEFAULT TRUE NOT NULL,
	Museu BOOLEAN DEFAULT FALSE,
	Estudio BOOLEAN DEFAULT FALSE,
	Cinemateca BOOLEAN DEFAULT FALSE,
	
	CONSTRAINT PK_EPUBLICO PRIMARY KEY(ID),
	CONSTRAINT FK_BCULTURAL FOREIGN KEY(Id) REFERENCES BemCultural(Id) ON DELETE CASCADE
);

CREATE TABLE Relatorio_EspacoPublico (
	
	EPublico INT NOT NULL,
	Periodo DATERANGE NOT NULL,
	DataHoraEmissao TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
	Obs VARCHAR(200) DEFAULT 'Sem observações.',
	Anexo BYTEA,
	
	CONSTRAINT PK_RELATORIO_ESPACOPUBLICO PRIMARY KEY(EPublico, Periodo),
	CONSTRAINT FK_RELATORIO_ESPACOPUBLICO FOREIGN KEY(EPublico) REFERENCES EspacoPublico(Id) ON DELETE CASCADE
);

CREATE TABLE Museu(

	Id INT NOT NULL,

	CONSTRAINT PK_MUSEU PRIMARY KEY(Id),
	CONSTRAINT FK_ESPACOPUBLICO FOREIGN KEY(Id) REFERENCES EspacoPublico(Id) ON DELETE CASCADE
);

CREATE TABLE Estudio(

	Id INT NOT NULL,
	
	CONSTRAINT PK_ESTUDIO PRIMARY KEY(Id),
	CONSTRAINT FK_ESPACOPUBLICO FOREIGN KEY(Id) REFERENCES EspacoPublico(Id) ON DELETE CASCADE
);

CREATE TABLE Cinemateca(

	Id INT NOT NULL,
	
	CONSTRAINT PK_CINEMATECA PRIMARY KEY(Id),
	CONSTRAINT FK_ESPACOPUBLICO FOREIGN KEY(Id) REFERENCES EspacoPublico(Id) ON DELETE CASCADE
); 


-- Tabelas relacionadas à entidade Monumento ------------------------
CREATE TABLE Monumento(

	Id INT NOT NULL,
	Localizacao POINT,
	
	CONSTRAINT PK_MONUMENTO PRIMARY KEY(Id),
	CONSTRAINT FK_BCULTURAL FOREIGN KEY(Id) REFERENCES BemCultural(Id) ON DELETE CASCADE
);


-- Tabelas relacionadas à entidade Edificacao tombada ------------------------
CREATE TABLE EdificacaoTombada(

	Id INT NOT NULL,
	Localizacao POINT,
	
	CONSTRAINT PK_EDFTOMBADA PRIMARY KEY(Id),
	CONSTRAINT FK_BCULTURAL FOREIGN KEY(Id) REFERENCES BemCultural(Id) ON DELETE CASCADE
);

CREATE TABLE EdificacaoTombada_Proprietarios(

	Id INT NOT NULL,
	CPF VARCHAR(14),
	Nome VARCHAR(100),
	
	CONSTRAINT PK_EDFTOMBADA_PROP PRIMARY KEY(Id, CPF),
	CONSTRAINT FK_EDFTOMBADA FOREIGN KEY(Id) REFERENCES EdificacaoTombada(Id) ON DELETE CASCADE,
	CONSTRAINT CK_CPF CHECK(CPF ~* '([0-9]{2}[\.]?[0-9]{3}[\.]?[0-9]{3}[\/]?[0-9]{4}[-]?[0-9]{2})|([0-9]{3}[\.]?[0-9]{3}[\.]?[0-9]{3}[-]?[0-9]{2})') -- checa se o cpf está no formato correto com regex
);


-- Tabelas relacionadas à entidade Obra de arte ------------------------
CREATE TABLE Obra(

	Id INT NOT NULL,
	Museu INT NOT NULL,
	Criacao INT,
	Genero VARCHAR(20),
	Tecnica VARCHAR(100),
	Dimensoes VARCHAR(20),
	
	CONSTRAINT PK_OBRA PRIMARY KEY(Id),
	CONSTRAINT FK_MUSEU FOREIGN KEY(Museu) REFERENCES Museu(Id) ON DELETE NO ACTION, -- nao deixa um Museu ser excluido da base caso ele ainda possua obras
	CONSTRAINT CK_CRIACAO CHECK(Criacao <= date_part('year', CURRENT_DATE))
);

CREATE TABLE Obra_Autores(

	Id INT NOT NULL,
	Autor VARCHAR(100),
	
	CONSTRAINT PK_OBRA_AUTORES PRIMARY KEY(Id, Autor),
	CONSTRAINT FK_OBRA FOREIGN KEY(Id) REFERENCES Obra(Id) ON DELETE CASCADE
);


-- Tabelas relacionadas à entidade Musica ------------------------
CREATE TABLE Musica(

	Id INT NOT NULL,
	Estudio INT NOT NULL, 
	Gravacao INT,
	Producao VARCHAR(20),
	Duracao INT,
	Formato VARCHAR(10),
	
	CONSTRAINT PK_MUSICA PRIMARY KEY(Id),
	CONSTRAINT FK_BCULTURAL FOREIGN KEY(Id) REFERENCES BemCultural(Id) ON DELETE CASCADE,
	CONSTRAINT FK_ESTUDIO FOREIGN KEY(Estudio) REFERENCES Estudio(Id) ON DELETE NO ACTION,	-- nao deixa um Estudio ser excluido da base caso ele ainda possua musica
	CONSTRAINT CK_CRIACAO CHECK(Gravacao <= date_part('year', CURRENT_DATE))
);

CREATE TABLE Musica_Genero(
	
	Id INT NOT NULL,
	Genero VARCHAR(20) NOT NULL,
	
	CONSTRAINT PK_MUSICA_GENERO PRIMARY KEY(Id, Genero),
	CONSTRAINT FK_MUSICA FOREIGN KEY(Id) REFERENCES Musica(Id) ON DELETE CASCADE
);

CREATE TABLE Musica_Compositor(
	
	Id INT NOT NULL,
	Compositor VARCHAR(50) NOT NULL,
	
	CONSTRAINT PK_MUSICA_COMPOSITOR PRIMARY KEY(Id, Compositor),
	CONSTRAINT FK_MUSICA FOREIGN KEY(Id) REFERENCES Musica(Id) ON DELETE CASCADE
);


-- Tabelas relacionadas à entidade Filme ------------------------
CREATE TABLE Filme(
	
	Id INT NOT NULL,
	Cinemateca INT NOT NULL,
	
	CONSTRAINT PK_FILME PRIMARY KEY(Id),
	CONSTRAINT FK_BCULTURAL FOREIGN KEY(Id) REFERENCES BemCultural(Id) ON DELETE CASCADE, 
	CONSTRAINT FK_CINEMATECA FOREIGN KEY(Cinemateca) REFERENCES Cinemateca(Id) ON DELETE NO ACTION 	-- nao deixa uma cinemateca ser excluida da base caso ela ainda possua filmes
);

CREATE TABLE Filme_Creditos(
	
	Id INT NOT NULL,
	Nome VARCHAR(100) NOT NULL,
	Funcao VARCHAR(100) NOT NULL,
	
	CONSTRAINT PK_FILME_CREDITOS PRIMARY KEY(Id, Nome, Funcao),
	CONSTRAINT FK_FILME FOREIGN KEY(Id) REFERENCES Filme(Id) ON DELETE CASCADE
);



-- Tabelas relacionadas ao Cidadão ------------------------
CREATE TABLE Cidadao(
	
	IdOauth VARCHAR(20), 
	LocalLat REAL NOT NULL,
	LocalLong REAL NOT NULL,
	Nome VARCHAR(100),
	CPF VARCHAR(14) NOT NULL,
	CookieSess VARCHAR(100),
	
	CONSTRAINT PK_CIDADAO PRIMARY KEY(IdOauth),
	CONSTRAINT UN_COOKIE UNIQUE(CookieSess),
	CONSTRAINT UN_CPF UNIQUE(CPF),
	CONSTRAINT CK_CPF CHECK(CPF ~* '([0-9]{2}[\.]?[0-9]{3}[\.]?[0-9]{3}[\/]?[0-9]{4}[-]?[0-9]{2})|([0-9]{3}[\.]?[0-9]{3}[\.]?[0-9]{3}[-]?[0-9]{2})') -- checa se o cpf está no formato correto com regex
);

CREATE TABLE AgPrefeitura(
	
	IdOauth VARCHAR(20), 
	Cidade VARCHAR(20) NOT NULL,
	Estado VARCHAR(26) NOT NULL,
	
	CONSTRAINT PK_AGPREFEITURA PRIMARY KEY(IdOauth),
	CONSTRAINT FK_CIDADAO FOREIGN KEY(IdOauth) REFERENCES Cidadao(IdOauth) ON DELETE NO ACTION,
	CONSTRAINT FK_CIDADE FOREIGN KEY(Cidade, Estado) REFERENCES Cidade(Nome, Estado) ON DELETE NO ACTION
);


-- Tabelas relacionadas às entidades restantes ------------------------
CREATE TABLE Visita(
	
	Cidadao VARCHAR(20) NOT NULL, 
	EspacoPublico INT NOT NULL,
	DataHora TIMESTAMP NOT NULL,
	REVIEW VARCHAR(200) DEFAULT 'O usuário não escreveu um review.',
	
	CONSTRAINT PK_VISITA PRIMARY KEY(Cidadao, EspacoPublico, DataHora),
	CONSTRAINT UN_VISITA UNIQUE(Cidadao, DataHora),
	CONSTRAINT FK_CIDADAO FOREIGN KEY(Cidadao) REFERENCES Cidadao(IdOauth) ON DELETE NO ACTION,
	CONSTRAINT FK_EPUBLICO FOREIGN KEY(EspacoPublico) REFERENCES EspacoPublico(Id) ON DELETE CASCADE
);

CREATE SEQUENCE S_DENUNCIA START WITH 10000;

CREATE TABLE Denuncia(
	
	IdDenuncia INT NOT NULL DEFAULT nextval('S_DENUNCIA'),
	Cidadao VARCHAR(20) NOT NULL, 
	BemCultural INT NOT NULL,
	DataHora TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	Descricao VARCHAR(500) NOT NULL,
	TipoOcorrência VARCHAR(25) NOT NULL,
	
	CONSTRAINT PK_DENUNCIA PRIMARY KEY(IdDenuncia),
	CONSTRAINT UN_DENUNCIA UNIQUE(Cidadao, BemCultural, DataHora),
	CONSTRAINT FK_CIDADAO FOREIGN KEY(Cidadao) REFERENCES Cidadao(IdOauth) ON DELETE NO ACTION,
	CONSTRAINT FK_BCULTURAL FOREIGN KEY(BemCultural) REFERENCES BemCultural(Id) ON DELETE CASCADE
);

ALTER SEQUENCE S_DENUNCIA OWNED BY Denuncia.IdDenuncia;

-- função que retorna o ID do cidadao que realizou uma dada denuncia
CREATE FUNCTION quem_denunciou(INT) RETURNS VARCHAR(20) LANGUAGE plpgsql AS
$$
BEGIN
	RETURN (SELECT Cidadao FROM Denuncia WHERE Denuncia.IdDenuncia = $1);
END;
$$;

-- função que verifica se o auditor é da mesma cidade da denuncia
CREATE FUNCTION checa_cidade(VARCHAR(20), INT) RETURNS BOOLEAN LANGUAGE plpgsql AS
$$
BEGIN
	RETURN not exists (SELECT Cidade, Estado FROM AgPrefeitura WHERE AgPrefeitura.IdOauth = $1 EXCEPT SELECT Cidade, Estado FROM (Denuncia JOIN BemCultural ON Denuncia.BemCultural = BemCultural.id) WHERE Denuncia.IdDenuncia = $2);
END;
$$;

CREATE TABLE Auditoria(
	
	IdDenuncia INT NOT NULL,
	AgPrefeitura VARCHAR(20) NOT NULL,
	DataHora TIMESTAMP NOT NULL,
	CONSTRAINT CK_AUDITOR CHECK (AgPrefeitura <> quem_denunciou(IdDenuncia)),
	CONSTRAINT PK_AUDITORIA PRIMARY KEY(IdDenuncia, AgPrefeitura, DataHora),
	CONSTRAINT FK_DENUNCIA FOREIGN KEY(IdDenuncia) REFERENCES Denuncia(IdDenuncia) ON DELETE NO ACTION, -- nao será possivel excluir uma denúncia que possui uma auditoria marcada
	CONSTRAINT FK_AGPREFEITURA FOREIGN KEY(AgPrefeitura) REFERENCES AgPrefeitura(IdOauth) ON DELETE NO ACTION,
	CONSTRAINT CK_CIDADE CHECK (checa_cidade(AgPrefeitura, IdDenuncia) = TRUE)

);

CREATE TABLE Evento(
	
	Titulo VARCHAR(100) NOT NULL,
	DataInicio DATE NOT NULL,
	DataFim DATE,
	HoraInicio TIME with time zone,
	HoraFim TIME with time zone,
	OndeOcorre INT NOT NULL,
	DataHoraEmissaoRelatorio TIMESTAMP,
	Obs VARCHAR(100),
	Anexo BYTEA,
	
	CONSTRAINT PK_EVENTO PRIMARY KEY(Titulo, DataInicio),
	CONSTRAINT CK_DATAFIM CHECK(DataFim > DataInicio OR (DataFim = DataInicio AND HoraFim >= HoraInicio)),
	CONSTRAINT FK_EPUBLICO FOREIGN KEY(OndeOcorre) REFERENCES EspacoPublico(Id) ON DELETE CASCADE
);

CREATE TABLE ConfirmaParticipacao(
	
	Cidadao VARCHAR(20) NOT NULL,
	Evento VARCHAR(100) NOT NULL,
	DataInicioEvento DATE NOT NULL,
	
	CONSTRAINT PK_CONFIRMAPART PRIMARY KEY(Cidadao, Evento, DataInicioEvento),
	CONSTRAINT FK_CIDADAO FOREIGN KEY(Cidadao) REFERENCES Cidadao(IdOauth) ON DELETE NO ACTION,
	CONSTRAINT FK_EVENTO FOREIGN KEY(Evento, DataInicioEvento) REFERENCES Evento(Titulo, DataInicio) ON DELETE CASCADE
);
