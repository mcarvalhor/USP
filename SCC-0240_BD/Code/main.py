from flask import Flask, request, redirect, send_from_directory, render_template, abort
from urllib.parse import urlencode



from utils import Config, DBConnection, Utils
import consts as Consts



app = Flask(__name__)



@app.before_first_request
def main():
	# This function executes before the first request to the server.
	if __name__ != '__main__':
		Config.Init()



@app.route('/', methods=['GET'])
@app.route('/home', methods=['GET'])
def index():
	try:
		db = DBConnection()
		found = db.fetchAll('SELECT id, titulo, cidade, estado, natureza, descricao FROM bemcultural ORDER BY popularidade DESC LIMIT %s;', [Consts.DB_MAX_HP_RESULTS])
		db.close()
	except:
		return redirect("/setup", code=302) # Redireciona se o esquema não tiver sido criado.
	return render_template('index.html', found=found)

@app.route('/static/<path:filename>', methods=['GET'])
def static_serve(filename):
	return send_from_directory('static', filename) # Serve static files.

@app.route('/search', methods=['GET'])
def search():
	formData = request.args.to_dict(flat=True)
	db = DBConnection()
	cities = db.fetchAll('SELECT nome, estado FROM cidade ORDER BY estado ASC, nome ASC;')
	if not Utils.ValidateField(formData, 'q', checkEmpty=True):
		db.close()
		return render_template('search.html', advanced_search = True, cities = cities, found = [ ])
	fullQuery = 'SELECT id, titulo, cidade, estado, natureza, descricao FROM bemcultural WHERE LOWER(titulo) LIKE LOWER(%s) '
	queryArgs = [ ('%' + formData['q'] + '%') ]
	current_page = 0
	order = 'popularidade'
	if Utils.ValidateField(formData, 'city', checkEmpty=True): # Filtrar por cidade...
		city_name = formData['city'].split("-")[0]
		city_estate = formData['city'].split("-")[1]
		fullQuery += 'AND cidade = %s AND estado = %s '
		queryArgs.append(city_name)
		queryArgs.append(city_estate)
	if Utils.ValidateField(formData, 'kind', equals=['ESPACOPUBLICO', 'MONUMENTO', 'EDFTOMBADA', 'OBRA', 'MUSICA', 'FILME']): # Filtrar por natureza...
		fullQuery += 'AND natureza = %s '
		queryArgs.append(formData['kind'])
	if Utils.ValidateField(formData, 'kind', equals=['museu', 'estudio', 'cinemateca']): # Filtrar por tipo do espaço-público...
		fullQuery += 'AND natureza = %s AND id IN (SELECT id FROM espacopublico WHERE ' + formData['kind'] + ' = TRUE) '
		queryArgs.append('ESPACOPUBLICO')
	if Utils.ValidateField(formData, 'order', equals=['titulo', 'datacadastro', 'id']): # Ordenação a ser considerada...
		order = formData['order']
	if order == 'popularidade' and not Utils.ValidateField(formData, 'order-inverted', equals='true'): # Considera ou não inverter ordenação...
		order += ' DESC'
	elif order == 'popularidade' and Utils.ValidateField(formData, 'order-inverted', equals='true'):
		order += ' ASC'
	elif Utils.ValidateField(formData, 'order-inverted', equals='true'):
		order += ' DESC'
	if Utils.ValidateField(formData, 'p', checkEmpty=True, regExp=r'^[0-9]+$'): # Paginação...
		current_page = int(formData['p'])
	fullQuery += 'ORDER BY ' + order + ' '
	fullQuery += 'LIMIT ' + str(Consts.DB_MAX_SEARCH_RESULTS) + ' OFFSET ' + str(current_page * Consts.DB_MAX_SEARCH_RESULTS) + ';'
	#print("Executing SQL query for search (%d arguments):" % (len(queryArgs)))
	#print("\t%s" % (fullQuery))
	found = db.fetchAll(fullQuery, queryArgs)
	#print('Found:')
	#print(found)
	db.close()
	previousPageURL = None
	nextPageURL = None
	if current_page > 0:
		previousPageURL = formData.copy()
		previousPageURL['p'] = str(current_page - 1)
		previousPageURL = '?' + urlencode(previousPageURL)
	if len(found) > 0 and len(found) >= Consts.DB_MAX_SEARCH_RESULTS:
		nextPageURL = formData.copy()
		nextPageURL['p'] = str(current_page + 1)
		nextPageURL = '?' + urlencode(nextPageURL)
	return render_template('search.html', advanced_search = False, query = formData['q'], previousPageURL = previousPageURL, nextPageURL = nextPageURL, current_page = current_page, cities = cities, found = found)

@app.route('/view/<id>', methods=['GET'])
def view(id):
	db = DBConnection()
	item = db.fetchOne('SELECT id, titulo, cidade, estado, natureza, descricao, datacadastro FROM bemcultural WHERE id = %s ;', [ id ])
	if item is None:
		db.close()
		abort(404)
	keywords = db.fetchAll('SELECT palavra FROM bemcultural_palavraschave WHERE id = %s ;', [ item[0] ])
	keywords = [ keyword[0] for keyword in keywords ] if keywords is not None else [ ]
	keywords = ', '.join(keywords)
	specific_item = None
	if item[4] == 'ESPACOPUBLICO':
		specific_item = db.fetchOne('SELECT localizacao, emfuncionamento, museu, estudio, cinemateca FROM espacopublico WHERE id = %s ;', [ item[0] ])
		if specific_item is not None:
			specific_item = {
				'Localização': specific_item[0],
				'Em funcionamento': 'SIM' if specific_item[1] else 'NÃO',
				'É museu': 'SIM' if specific_item[2] else 'NÃO',
				'É estúdio': 'SIM' if specific_item[3] else 'NÃO',
				'É cinemateca': 'SIM' if specific_item[4] else 'NÃO'
			}
	elif item[4] == 'MONUMENTO':
		specific_item = db.fetchOne('SELECT localizacao FROM monumento WHERE id = %s ;', [ item[0] ])
		if specific_item is not None:
			specific_item = {
				'Localização': specific_item[0]
			}
	elif item[4] == 'EDFTOMBADA':
		specific_item = db.fetchAll('SELECT edificacaotombada.localizacao, edificacaotombada_proprietarios.cpf, edificacaotombada_proprietarios.nome FROM edificacaotombada LEFT JOIN edificacaotombada_proprietarios ON edificacaotombada_proprietarios.id = edificacaotombada.id WHERE edificacaotombada.id = %s ;', [ item[0] ])
		if specific_item is not None:
			specific_item = {
				'Localização': specific_item[0][0],
				'Proprietários': ', '.join([ (row[2] + ' (' + str(row[1]) + ')') for row in specific_item if row[1] is not None and row[1] != '' ])
			}
	elif item[4] == 'OBRA':
		specific_item = db.fetchAll('SELECT obra.museu, obra.criacao, obra.genero, obra.tecnica, obra.dimensoes, obra_autores.autor FROM obra LEFT JOIN obra_autores ON obra_autores.id = obra.id WHERE obra.id = %s ;', [ item[0] ])
		if specific_item is not None:
			specific_item = {
				'Museu': 'ID ' + str(specific_item[0][0]),
				'Criação': specific_item[0][1],
				'Gênero': specific_item[0][2],
				'Técnica': specific_item[0][3],
				'Dimensões': specific_item[0][4],
				'Autores': ', '.join([ row[5] for row in specific_item if row[5] is not None and row[5] != '' ])
			}
	elif item[4] == 'MUSICA':
		specific_item = db.fetchAll('SELECT musica.estudio, musica.gravacao, musica.producao, musica.duracao, musica.formato, musica_compositor.compositor, NULL FROM musica LEFT JOIN musica_compositor ON musica_compositor.id = musica.id WHERE musica.id = %s UNION ALL SELECT musica.estudio, musica.gravacao, musica.producao, musica.duracao, musica.formato, NULL, musica_genero.genero FROM musica LEFT JOIN musica_genero ON musica_genero.id = musica.id WHERE musica.id = %s ;', [ item[0], item[0] ])
		if specific_item is not None:
			specific_item = {
				'Estúdio': 'ID ' + str(specific_item[0][0]),
				'Gravação': specific_item[0][1],
				'Produção': specific_item[0][2],
				'Duração': specific_item[0][3],
				'Formato': specific_item[0][4],
				'Compositores': ', '.join([ row[5] for row in specific_item if row[5] is not None and row[5] != '' ]),
				'Gêneros': ', '.join([ row[6] for row in specific_item if row[6] is not None and row[6] != '' ])
			}
	elif item[4] == 'FILME':
		specific_item = db.fetchAll('SELECT filme.cinemateca, filme_creditos.nome, filme_creditos.funcao FROM filme LEFT JOIN filme_creditos ON filme_creditos.id = filme.id WHERE filme.id = %s ;', [ item[0] ])
		if specific_item is not None:
			specific_item = {
				'Cinemateca': 'ID ' + str(specific_item[0][0]),
				'Créditos': ', '.join([ (str(row[1]) + ' (' + str(row[2]) + ')') for row in specific_item if row[2] is not None and row[2] != '' ])
			}
	if specific_item is None:
		specific_item = { }
	try:
		db.execute('UPDATE bemcultural SET popularidade = popularidade + 0.01 WHERE id = %s ;', [ item[0] ])
	except:
		pass
	db.close()
	return render_template('view.html', id=item[0], title=item[1], city=(item[2], item[3]), kind=item[4], description = item[5], date=item[6], keywords=keywords, details=specific_item)

@app.route('/report/<id>', methods=['GET'])
def report(id):
	db = DBConnection()
	item = db.fetchOne('SELECT id, titulo FROM bemcultural WHERE id = %s ;', [ id ])
	users = db.fetchAll('SELECT idoauth, nome, cpf FROM cidadao ORDER BY nome ASC, cpf ASC;')
	if item is None:
		db.close()
		abort(404)
	db.close()
	return render_template('report.html', id=item[0], title=item[1], users=users, report_types=Consts.DB_REPORT_TYPES)

@app.route('/report/<id>', methods=['POST'])
def report_send(id):
	db = DBConnection()
	item = db.fetchOne('SELECT id, titulo FROM bemcultural WHERE id = %s ;', [ id ])
	if item is None:
		db.close()
		abort(404)
	formData = request.form.to_dict(flat=True)
	if not Utils.ValidateField(formData, 'user', checkEmpty=True) or db.fetchOne('SELECT idoauth FROM cidadao WHERE idoauth = %s ;', [ formData['user'] ]) is None:
		db.close()
		return 'Selecione um cidadão válido!', 403
	if not Utils.ValidateField(formData, 'date', regExp=r'^[0-9]{4,}-[0-9]{2}-[0-9]{2}$'):
		db.close()
		return 'Insira uma data válida! O formato correto é AAAA-MM-DD.', 403
	if not Utils.ValidateField(formData, 'time', regExp=r'^[0-9]{2}:[0-9]{2}$'):
		db.close()
		return 'Insira uma hora válida! O formato correto é HH:MM.', 403
	if not Utils.ValidateField(formData, 'description', checkEmpty=True, minLength=1, maxLength=499):
		db.close()
		return 'Insira uma descrição válida!', 403
	if not Utils.ValidateField(formData, 'type', checkEmpty=True, equals=[ type[0] for type in Consts.DB_REPORT_TYPES ]):
		db.close()
		return 'Selecione um tipo de ocorrência válido!', 403
	dateTime = formData['date'] + ' ' + formData['time'] + ':00'
	if db.fetchOne('SELECT IdDenuncia FROM denuncia WHERE Cidadao = %s AND BemCultural = %s AND DataHora = TIMESTAMP %s LIMIT 1;', [formData['user'], item[0], dateTime]) is not None:
		db.close()
		return 'Essa denúncia já existe!', 403
	result = db.insertOne('INSERT INTO denuncia (Cidadao, BemCultural, DataHora, Descricao, TipoOcorrência) values (%s, %s, TIMESTAMP %s, %s, %s);',
		[formData['user'], item[0], dateTime, formData['description'], formData['type']] )
	db.close()
	if not result:
		return 'Não foi possível inserir sua denúncia! Verifique se uma mesma denúncia já não existe no sistema com tal data/hora. Por favor, Tente novamente.', 403
	return render_template('report_sent.html', id=item[0], title=item[1])

@app.route('/setup', methods=['GET'])
def setup():
	return render_template('setup.html')

@app.route('/setup', methods=['POST'])
def setup_do():
	try:
		db = DBConnection()
	except:
		return 'Erro ao criar a conexão ao banco de dados! Verifique se o arquivo "config.json" está definido corretamente.', 500
	for sqlFile in Consts.DB_RECREATE_FILES:
		with open(sqlFile, 'r') as fStream:
			db.execute(fStream.read())
	db.close()
	return render_template('setup_done.html')

if __name__ == '__main__':
	Config.Init()
	app.run(host=Config.LISTEN_IP, port=Config.LISTEN_PORT)


