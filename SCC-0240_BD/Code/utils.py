import psycopg2
import json
import sys
import os
import re



class Config:

	# DO NOT MODIFY THESE VARIABLES HERE!
	# Their values are updated dynamically from 'config.json' file as soon as the application is up.

	LOADED = False

	LISTEN_IP = '0.0.0.0'
	LISTEN_PORT = 8080

	DB_HOSTNAME = None
	DB_PORT = 5432
	DB_USERNAME = None
	DB_PASSWORD = None
	DB_NAME = None

	@staticmethod
	def Init(configFile: str = None):
		if Config.LOADED: 
			return
		if configFile is None and len(sys.argv) == 2:
			configFile = sys.argv[1]
		elif configFile is None and 'BD_ConfigFile' in os.environ:
			configFile = os.environ['BD_ConfigFile']
		elif configFile is None:
			configFile = 'config.json'
		with open(configFile) as file:
			fileContent = file.read()
			config = json.loads(fileContent)
			if 'database' not in config or 'host' not in config['database'] or 'user' not in config['database'] or 'passwd' not in config['database'] or 'name' not in config['database']:
				raise RuntimeError('JSON not in correct format.')
			Config.LISTEN_IP = config['listen-ip']
			Config.LISTEN_PORT = config['listen-port']
			Config.DB_HOSTNAME = config['database']['host']
			Config.DB_HOSTNAME = config['database']['host']
			Config.DB_USERNAME = config['database']['user']
			Config.DB_PASSWORD = config['database']['passwd']
			Config.DB_NAME = config['database']['name']
			if 'port' in config['database']:
				Config.DB_PORT = config['database']['port']
			if 'listen-ip' in config:
				Config.LISTEN_IP = config['listen-ip']
			if 'listen-port' in config:
				Config.LISTEN_PORT = config['listen-port']
			Config.LOADED = True


class DBConnection:

	def __init__(self):
		if Config.LOADED == False:
			raise ConnectionError('Config not loaded yet. Please call "Config.Init()". If already called, wait a moment before continuing.')
		self.con = psycopg2.connect(host=Config.DB_HOSTNAME, port=Config.DB_PORT, dbname=Config.DB_NAME, user=Config.DB_USERNAME, password=Config.DB_PASSWORD)
	
	def fetchAll(self, query: str, args: tuple = None):
		cursor = self.con.cursor()

		if args is None:
			cursor.execute(query)
		else:
			cursor.execute(query, args)
		
		items = cursor.fetchall()
		self.con.commit()
		cursor.close()

		return items

	def fetchOne(self, query: str, args: tuple = None):
		cursor = self.con.cursor()

		if args is None:
			cursor.execute(query)
		else:
			cursor.execute(query, args)
		
		item = cursor.fetchone()
		self.con.commit()
		cursor.close()

		return item

	def insertAll(self, query: str, items: list = None):
		if items is None or len(items) < 1:
			return 0

		for item in items:
			if type(item) != tuple:
				raise TypeError('In function "insertAll", "items" should be a list of tuple.')
		
		cursor = self.con.cursor()

		cursor.executemany(query, items)
		self.con.commit()
		row_count = cursor.rowcount
		cursor.close()

		return row_count

	def insertOne(self, query: str, item: tuple = None):
		if item is None:
			return 0
		
		cursor = self.con.cursor()

		cursor.execute(query, item)
		self.con.commit()
		row_count = cursor.rowcount
		cursor.close()

		return True if row_count > 0 else False
	
	def execute(self, query: str, args: tuple = None):
		cursor = self.con.cursor()

		if args is None:
			cursor.execute(query)
		else:
			cursor.execute(query, args)

		self.con.commit()
		row_count = cursor.rowcount
		cursor.close()

		return row_count
	
	def close(self):
		self.con.close()
	
	def __enter__(self):
		return self

	def __exit__(self, type, value, traceback):
		self.close()


class Utils:

	@staticmethod
	def ValidateField(formData, field, regExp = None, checkEmpty = False, equals = None, minLength = None, maxLength = None):
		''' Validate if a given HTTP GET/POST field is valid. '''
		if field is None:
			return False
		if field not in formData:
			return False
		if regExp is not None and not re.match(regExp, formData[field]):
			return False
		if checkEmpty and formData[field].strip() == '':
			return False
		if equals is not None and not isinstance(equals, list) and formData[field] != equals:
			return False
		if equals is not None and isinstance(equals, list) and formData[field] not in equals:
			return False
		if minLength is not None and len(formData[field]) < minLength:
			return False
		if maxLength is not None and len(formData[field]) > maxLength:
			return False
		return True