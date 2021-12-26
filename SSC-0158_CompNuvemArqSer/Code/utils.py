import re
import mysql.connector
import json
import sys
import os

mydb = mysql.connector.connect()



class Config:

	# DO NOT MODIFY THESE VARIABLES HERE!
	# Their values are updated dynamically from 'config.json' file as soon as the application is up.

	LOADED = False
	DB_HOSTNAME = None
	DB_USERNAME = None
	DB_PASSWORD = None
	DB_NAME = None

	TWITTER_API_KEY = None
	TWITTER_API_SECRET = None
	TWITTER_ACCESS_TOKEN = None
	TWITTER_ACCESS_TOKEN_SECRET = None

	KAFKA = None

	@staticmethod
	def Init(configFile: str = None):
		if Config.LOADED:
			return
		if configFile is None and len(sys.argv) == 2:
			configFile = sys.argv[1]
		elif configFile is None and 'TA_ConfigFile' in os.environ:
			configFile = os.environ['TA_ConfigFile']
		elif configFile is None:
			configFile = 'config.json'
		with open(configFile) as file:
			fileContent = file.read()
			config = json.loads(fileContent)
			if 'database' not in config or 'host' not in config['database'] or 'user' not in config['database'] or 'passwd' not in config['database'] or 'name' not in config['database']:
				raise RuntimeError('JSON not in correct format.')
			Config.DB_HOSTNAME = config['database']['host']
			Config.DB_USERNAME = config['database']['user']
			Config.DB_PASSWORD = config['database']['passwd']
			Config.DB_NAME = config['database']['name']
			if 'twitter' not in config or 'api_key' not in config['twitter'] or 'api_secret' not in config['twitter']:
				raise RuntimeError('JSON not in correct format.')
			Config.TWITTER_API_KEY = config['twitter']['api_key']
			Config.TWITTER_API_SECRET = config['twitter']['api_secret']
			if 'access_token' in config['twitter'] and 'access_token_secret' in config['twitter']:
				Config.TWITTER_ACCESS_TOKEN = config['twitter']['access_token']
				Config.TWITTER_ACCESS_TOKEN_SECRET = config['twitter']['access_token_secret']
			if 'kafka' not in config:
				raise RuntimeError('JSON not in correct format.')
			Config.KAFKA = config['kafka']
			Config.LOADED = True


class DBConnection:

	def __init__(self):
		if Config.LOADED == False:
			raise ConnectionError('Config not loaded yet. Please call "Config.Init()". If already called, wait a moment before continuing.')
		self.con = mysql.connector.connect(host=Config.DB_HOSTNAME, user=Config.DB_USERNAME, password=Config.DB_PASSWORD, database=Config.DB_NAME)
	
	def fetchAll(self, query: str, args: tuple = None):
		cursor = self.con.cursor(buffered=True)

		if args is None:
			cursor.execute(query)
		else:
			cursor.execute(query, args)
		
		items = cursor.fetchall()
		self.con.commit()
		cursor.close()

		return items

	def fetchOne(self, query: str, args: tuple = None):
		cursor = self.con.cursor(buffered=True)

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
		
		cursor = self.con.cursor(buffered=True)

		cursor.executemany(query, items)
		self.con.commit()
		row_count = cursor.rowcount
		cursor.close()

		return row_count

	def insertOne(self, query: str, item: tuple = None):
		if item is None:
			return 0
		
		cursor = self.con.cursor(buffered=True)

		cursor.execute(query, item)
		self.con.commit()
		row_count = cursor.rowcount
		cursor.close()

		return True if row_count > 0 else False
	
	def execute(self, query: str, args: tuple = None):
		cursor = self.con.cursor(buffered=True)

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
		if field is None:
			return False
		if field not in formData:
			return False
		if regExp is not None and not re.match(regExp, formData[field]):
			return False
		if checkEmpty and formData[field].strip() == '':
			return False
		if equals is not None and formData[field] != equals:
			return False
		if minLength is not None and len(formData[field]) < minLength:
			return False
		if maxLength is not None and len(formData[field]) > maxLength:
			return False
		return True