from flask import Flask, request, send_from_directory
import datetime
import json


from utils import Config, DBConnection, Utils
from data_worker import TaskManager, TaskNode
from api_comm import APICommunicator



app = Flask(__name__)



@app.before_first_request
def main():
	# This function executes before the first request to the server.
	if __name__ != '__main__':
		Config.Init()
		APICommunicator.Init()
		TaskManager.Init()
		TaskNode.Init()


@app.route('/', methods=['GET'])
@app.route('/<filename>', methods=['GET'])
def index(filename = "index.html"):
	return send_from_directory('html', filename) # Serve static files.

@app.route('/server/databases', methods=['GET'])
def databases_get():
	db = DBConnection()
	items = db.fetchAll('SELECT `id`, `name`, `description`, `query`, `tweeted_before_date`, `tweeted_after_date`, `lang`, `ready`, (SELECT COUNT(*) FROM `tweets` WHERE `tweets`.`db_id` = `dbs`.`id`) AS `tweet_count` FROM `dbs` ORDER BY `name`')
	for i in range(len(items)):
		items[i] = {'id': items[i][0], 'name': items[i][1], 'description': items[i][2], 'query': items[i][3], 'tweeted_before_date': items[i][4], 'tweeted_after_date': items[i][5], 'lang': items[i][6], 'ready': True if items[i][7] == 1 else False, 'tweet_count': items[i][8]}
	db.close()
	return json.dumps(items) # Return all databases.

@app.route('/server/databases', methods=['POST'])
def databases_post():
	formData = request.form.to_dict(flat=True)
	if not Utils.ValidateField(formData, 'name', checkEmpty=True):
		return 'Missing required field!', 403
	if not Utils.ValidateField(formData, 'query', checkEmpty=True):
		return 'Missing required field!', 403
	if not Utils.ValidateField(formData, 'description', checkEmpty=True):
		formData['description'] = None
	if not Utils.ValidateField(formData, 'timezone_offset', checkEmpty=True, regExp=r'^[+-]?[0-9]+$'):
		formData['timezone_offset'] = '+0000'
	else:
		formData['timezone_offset'] = -1 * int(formData['timezone_offset'])
		if formData['timezone_offset'] >= 0:
			formData['timezone_offset'] = '+%02d%02d' % (abs(formData['timezone_offset']) // 60, abs(formData['timezone_offset']) % 60)
		else:
			formData['timezone_offset'] = '-%02d%02d' % (abs(formData['timezone_offset']) // 60, abs(formData['timezone_offset']) % 60)
	if not Utils.ValidateField(formData, 'lang', regExp=r'^(?:[a-zA-Z0-9]{2}|[a-zA-Z0-9]{2}-[a-zA-Z0-9]{2})$'):
		formData['lang'] = None
	if not Utils.ValidateField(formData, 'before_date', regExp=r'^[0-9]{4,}-[0-9]{2}-[0-9]{2}$'):
		formData['before_date'] = None
	else:
		formData['before_date'] = datetime.datetime.timestamp(datetime.datetime.strptime(formData['before_date'] + ' 23:59 +0000', '%Y-%m-%d %H:%M %z'))
	if not Utils.ValidateField(formData, 'after_date', regExp=r'^[0-9]{4,}-[0-9]{2}-[0-9]{2}$'):
		formData['after_date'] = None
	elif not Utils.ValidateField(formData, 'after_time', regExp=r'^[0-9]{2}:[0-9]{2}$'):
		formData['after_date'] = None
	else:
		formData['after_date'] = datetime.datetime.timestamp(datetime.datetime.strptime(formData['after_date'] + ' ' + formData['after_time'] + ' ' + formData['timezone_offset'], '%Y-%m-%d %H:%M %z'))
	if not Utils.ValidateField(formData, 'donotupdate', equals='true'):
		formData['update'] = False
	else:
		formData['update'] = True
	if (formData['after_date'] is not None and formData['before_date'] is not None) and (formData['before_date'] <= formData['after_date']):
		return 'Provided dates do not make sense!', 403

	newItem = (formData['name'], formData['description'], formData['query'], formData['before_date'], formData['after_date'], formData['lang'], formData['update'], )

	db = DBConnection()
	db.insertOne('INSERT INTO `dbs` (`name`, `description`, `query`, `tweeted_before_date`, `tweeted_after_date`, `lang`, `ready`) VALUES(%s, %s, %s, %s, %s, %s, %s)', newItem)
	db.close()
	return '', 204

@app.route('/server/databases/<id>', methods=['GET'])
def database_get(id):
	db = DBConnection()
	item = db.fetchOne('SELECT `id`, `name`, `description`, `query`, `tweeted_before_date`, `tweeted_after_date`, `lang`, `ready`, (SELECT COUNT(*) FROM `tweets` WHERE `tweets`.`db_id` = `dbs`.`id`) AS `tweet_count` FROM `dbs` WHERE `id`=%s', (id, ))
	db.close()
	if item is None:
		return 'Database does not exist!', 404
	item = {'id': item[0], 'name': item[1], 'description': item[2], 'query': item[3], 'tweeted_before_date': item[4], 'tweeted_after_date': item[5], 'lang': item[6], 'ready': True if item[7] == 1 else False, 'tweet_count': item[8]}
	return json.dumps(item) # Return this database.

@app.route('/server/databases/<id>', methods=['POST'])
def database_post(id):
	formData = request.form.to_dict(flat=True)
	if not Utils.ValidateField(formData, 'name', checkEmpty=True):
		return 'Missing required field!', 403
	if not Utils.ValidateField(formData, 'query', checkEmpty=True):
		return 'Missing required field!', 403
	if not Utils.ValidateField(formData, 'description', checkEmpty=True):
		formData['description'] = None
	if not Utils.ValidateField(formData, 'timezone_offset', checkEmpty=True, regExp=r'^[+-]?[0-9]+$'):
		formData['timezone_offset'] = '+0000'
	else:
		formData['timezone_offset'] = -1 * int(formData['timezone_offset'])
		if formData['timezone_offset'] >= 0:
			formData['timezone_offset'] = '+%02d%02d' % (abs(formData['timezone_offset']) // 60, abs(formData['timezone_offset']) % 60)
		else:
			formData['timezone_offset'] = '-%02d%02d' % (abs(formData['timezone_offset']) // 60, abs(formData['timezone_offset']) % 60)
	if not Utils.ValidateField(formData, 'lang', regExp=r'^(?:[a-zA-Z0-9]{2}|[a-zA-Z0-9]{2}-[a-zA-Z0-9]{2})$'):
		formData['lang'] = None
	if not Utils.ValidateField(formData, 'before_date', regExp=r'^[0-9]{4,}-[0-9]{2}-[0-9]{2}$'):
		formData['before_date'] = None
	else:
		formData['before_date'] = datetime.datetime.timestamp(datetime.datetime.strptime(formData['before_date'] + ' 23:59 +0000', '%Y-%m-%d %H:%M %z'))
	if not Utils.ValidateField(formData, 'after_date', regExp=r'^[0-9]{4,}-[0-9]{2}-[0-9]{2}$'):
		formData['after_date'] = None
	elif not Utils.ValidateField(formData, 'after_time', regExp=r'^[0-9]{2}:[0-9]{2}$'):
		formData['after_date'] = None
	else:
		formData['after_date'] = datetime.datetime.timestamp(datetime.datetime.strptime(formData['after_date'] + ' ' + formData['after_time'] + ' ' + formData['timezone_offset'], '%Y-%m-%d %H:%M %z'))
	if not Utils.ValidateField(formData, 'update', equals='true'):
		formData['update'] = False
	else:
		formData['update'] = True
	if (formData['after_date'] is not None and formData['before_date'] is not None) and (formData['before_date'] <= formData['after_date']):
		return 'Provided dates do not make sense!', 403

	editedItem = (formData['name'], formData['description'], formData['query'], formData['before_date'], formData['after_date'], formData['lang'], )

	db = DBConnection()
	item = db.fetchOne('SELECT `id`, `query`, `tweeted_before_date`, `tweeted_after_date`, `lang`, `ready`, (SELECT COUNT(*) FROM `analysis` WHERE `analysis`.`db_id` = `dbs`.`id`) AS `analysis_count` FROM `dbs` WHERE `id`=%s', (id, ))
	if item is None:
		db.close()
		return 'Database does not exist!', 404
	
	if item[1] != editedItem[2] or item[2] != editedItem[3] or item[3] != editedItem[4] or item[4] != editedItem[5]:
		if item[5] == 0 or item[6] > 0:
			db.close()
			return 'Database being used!', 403
		db.execute('DELETE FROM `tweets` WHERE `db_id` = %s', (item[0], ))
		db.execute('UPDATE `dbs` SET `ready` = false WHERE `id` = %s',  (item[0], ))

	db.execute('UPDATE `dbs` SET `name` = %s, `description` = %s, `query` = %s, `tweeted_before_date` = %s, `tweeted_after_date` = %s, `lang` = %s WHERE `id` = %s', editedItem + (item[0], ))
	if formData['update']:
		db.execute('UPDATE `dbs` SET `ready` = false WHERE `id` = %s',  (item[0], ))
	db.close()
	return '', 204

@app.route('/server/databases/<id>', methods=['DELETE'])
def database_delete(id):
	db = DBConnection()
	id = db.fetchOne('SELECT `id`, (SELECT COUNT(*) FROM `analysis` WHERE `analysis`.`db_id` = `dbs`.`id`) FROM `dbs` WHERE `id`=%s', (id, ))
	if id is None:
		db.close()
		return 'Database does not exist!', 404
	if id[1] > 0:
		db.close()
		return 'Database is being used!', 403
	db.execute('DELETE FROM `dbs` WHERE `id`=%s', (id[0], )) # Delete database.
	db.execute('DELETE FROM `tweets` WHERE `tweets`.`db_id` NOT IN (SELECT `id` FROM `dbs`)') # Delete tweets.
	db.close()
	return '', 204

@app.route('/server/analysis', methods=['GET'])
def analytics_get():
	db = DBConnection()
	items = db.fetchAll('SELECT `id`, `db_id`, `name`, `description`, `ready`, `progress`, `results_fewinteractions`, `results_usercount`, `results_recentusercount`, `results_defaultprofilecount`, `results_defaultpicturecount`, `results_fewfollowings`, `results_fewfollowers`, `results_fewstatus`, `results_fewfavorites` FROM `analysis` ORDER BY `name`')
	for i in range(len(items)):
		items[i] = {'id': items[i][0], 'db': items[i][1], 'name': items[i][2], 'description': items[i][3], 'ready': True if items[i][4] == 1 else False, 'progress': items[i][5], 'results': {
			'fewinteractions': items[i][6],
			'usercount': items[i][7],
			'recentusercount': items[i][8],
			'defaultprofilecount': items[i][9],
			'defaultpicturecount': items[i][10],
			'fewfollowings': items[i][11],
			'fewfollowers': items[i][12],
			'fewstatus': items[i][13],
			'fewfavorites': items[i][14],
		}}
	DBs = db.fetchAll('SELECT `id`, `name`, `ready`, (SELECT COUNT(*) FROM `tweets` WHERE `tweets`.`db_id` = `dbs`.`id`) AS count FROM `dbs` ORDER BY `name`')
	for i in range(len(DBs)):
		DBs[i] = {'id': DBs[i][0], 'name': DBs[i][1], 'ready': True if DBs[i][2] == 1 else False, 'count': DBs[i][3]}
	db.close()
	return json.dumps({'analysis': items, 'dbs': DBs}) # Return all analysis and databases.

@app.route('/server/analysis', methods=['POST'])
def analytics_post():
	formData = request.form.to_dict(flat=True)
	if not Utils.ValidateField(formData, 'name', checkEmpty=True):
		return 'Missing required field!', 403
	if not Utils.ValidateField(formData, 'db', checkEmpty=True):
		return 'Missing required field!', 403
	if not Utils.ValidateField(formData, 'description', checkEmpty=True):
		formData['description'] = None

	db = DBConnection()
	formData['db'] = db.fetchOne('SELECT `id` FROM `dbs` WHERE `id` = %s', (formData['db'], ))
	if formData['db'] is None:
		db.close()
		return 'Database does not exist!', 404
	formData['db'] = formData['db'][0]

	newItem = (formData['name'], formData['description'], formData['db'], False, )
	db.insertOne('INSERT INTO `analysis` (`name`, `description`, `db_id`, `ready`) VALUES(%s, %s, %s, %s)', newItem)
	db.close()
	return '', 204

@app.route('/server/analysis/<id>', methods=['GET'])
def analysis_get(id):
	db = DBConnection()
	item = db.fetchOne('SELECT `id`, `db_id`, `name`, `description`, `ready`, `progress`, `results_fewinteractions`, `results_usercount`, `results_recentusercount`, `results_defaultprofilecount`, `results_defaultpicturecount`, `results_fewfollowings`, `results_fewfollowers`, `results_fewstatus`, `results_fewfavorites` FROM `analysis` WHERE `id`=%s', (id, ))
	if item is None:
		return 'Analysis does not exist!', 404
	item = {'id': item[0], 'db': item[1], 'name': item[2], 'description': item[3], 'ready': True if item[4] == 1 else False, 'progress': item[5], 'results': {
		'fewinteractions': item[6],
		'usercount': item[7],
		'recentusercount': item[8],
		'defaultprofilecount': item[9],
		'defaultpicturecount': item[10],
		'fewfollowings': item[11],
		'fewfollowers': item[12],
		'fewstatus': item[13],
		'fewfavorites': item[14],
	}}
	db.close()
	return json.dumps(item)

@app.route('/server/analysis/<id>', methods=['POST'])
def analysis_post(id):
	formData = request.form.to_dict(flat=True)
	if not Utils.ValidateField(formData, 'name', checkEmpty=True):
		return 'Missing required field!', 403
	if not Utils.ValidateField(formData, 'description', checkEmpty=True):
		formData['description'] = None

	editedItem = (formData['name'], formData['description'], )

	db = DBConnection()
	item = db.fetchOne('SELECT `id` FROM `analysis` WHERE `id`=%s', (id, ))
	if item is None:
		db.close()
		return 'Analysis does not exist!', 404

	db.execute('UPDATE `analysis` SET `name` = %s, `description` = %s WHERE `id` = %s', editedItem + (item[0], ))
	db.close()
	return '', 204

@app.route('/server/analysis/<id>', methods=['DELETE'])
def analysis_delete(id):
	db = DBConnection()
	id = db.fetchOne('SELECT `id` FROM `analysis` WHERE `id`=%s', (id, ))
	if id is None:
		db.close()
		return 'Analysis does not exist!', 404
	db.execute('DELETE FROM `analysis` WHERE `id`=%s', (id[0], )) # Delete analysis.
	db.close()
	return '', 204


if __name__ == '__main__':
	Config.Init()
	APICommunicator.Init()
	TaskManager.Init()
	TaskNode.Init()
	app.run(host='0.0.0.0', port=8080)


