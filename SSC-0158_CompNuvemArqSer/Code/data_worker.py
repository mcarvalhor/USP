from utils import Config, DBConnection
from threading import Thread, Lock
from consts import TA_THREAD_SLEEP_TIMEOUT
import time

import kafka



class TaskManager:

	_THREAD = None
	_MUTEX = None
	_STOP = None

	_KAFKA_PRODUCER = None

	@staticmethod
	def Init():
		if TaskManager._STOP != None:
			return
		TaskManager._THREAD = Thread(target=TaskManager._Run)
		TaskManager._MUTEX = Lock()
		TaskManager._KAFKA_PRODUCER = kafka.KafkaProducer(bootstrap_servers=Config.KAFKA, value_serializer=lambda x: str(x).encode('utf-8'))
		TaskManager._THREAD.start()
		TaskManager._STOP = False

	@staticmethod
	def _Run():
		while True:

			TaskManager._MUTEX.acquire(timeout = TA_THREAD_SLEEP_TIMEOUT)
			if TaskManager._STOP == True: # Stop thread?
				con.close()
				return

			con = DBConnection()
			waiting_analytics = con.fetchOne('SELECT `id` FROM `analysis` WHERE `ready` = false')
			con.close()
			if waiting_analytics is None: # Any analytics waiting to be processed?
				continue

			TaskManager._KAFKA_PRODUCER.send('ta_analysis_jobs', waiting_analytics[0])
	
	@staticmethod
	def WakeUp():
		if TaskManager._STOP != False:
			return
		TaskManager._MUTEX.release()
	
	@staticmethod
	def Stop():
		if TaskManager._STOP != False:
			return
		TaskManager._STOP = True
		TaskManager._THREAD.join()
		TaskManager._THREAD = None
		TaskManager._MUTEX = None
		TaskManager._STOP = None
	
	@staticmethod
	def WaitStop():
		if TaskManager._STOP != False:
			return
		TaskManager._THREAD.join()


class TaskNode:

	_THREAD = None
	_STOP = None

	_KAFKA_CONSUMER = None

	@staticmethod
	def Init():
		if TaskNode._STOP != None:
			return
		TaskNode._THREAD = Thread(target=TaskNode._Run)
		TaskNode._KAFKA_CONSUMER = kafka.KafkaConsumer('ta_analysis_jobs', bootstrap_servers=Config.KAFKA)
		TaskNode._THREAD.start()
		TaskNode._STOP = False

	@staticmethod
	def _Run():
		for job in TaskNode._KAFKA_CONSUMER:

			con = DBConnection()
			analysis = con.fetchOne('SELECT `id`, `db_id` FROM `analysis` WHERE `ready` = false AND `id` = %s', (int(job.value), ))
			if analysis is None:
				con.close()
				continue
			analysis = {'id': analysis[0], 'db_id': analysis[1]}

			fewinteractions = con.fetchOne('SELECT COUNT(DISTINCT `user_id`) FROM `tweets` WHERE `db_id` = %s AND `favoritecount`+`retweetcount` <= %s', (analysis['db_id'], 5, ))[0] # 5 Interactions
			usercount = con.fetchOne('SELECT COUNT(DISTINCT `user_id`) FROM `tweets` WHERE `db_id` = %s', (analysis['db_id'], ))[0]
			recentusercount = con.fetchOne('SELECT COUNT(DISTINCT `user_id`) FROM `tweets` WHERE `db_id` = %s AND `user_createdat` >= %s', (analysis['db_id'], time.time() - 365*24*60*60, ))[0] # 1 year.
			defaultprofilecount = con.fetchOne('SELECT COUNT(DISTINCT `user_id`) FROM `tweets` WHERE `db_id` = %s AND `user_defaultprofile` = true', (analysis['db_id'], ))[0]
			defaultpicturecount = con.fetchOne('SELECT COUNT(DISTINCT `user_id`) FROM `tweets` WHERE `db_id` = %s AND `user_defaultpicture` = true', (analysis['db_id'], ))[0]
			fewfollowings = con.fetchOne('SELECT COUNT(DISTINCT `user_id`) FROM `tweets` WHERE `db_id` = %s AND `user_following` <= %s', (analysis['db_id'], 100, ))[0] # 100 Following
			fewfollowers = con.fetchOne('SELECT COUNT(DISTINCT `user_id`) FROM `tweets` WHERE `db_id` = %s AND `user_followers` <= %s', (analysis['db_id'], 30, ))[0] # 30 Followers
			fewstatus = con.fetchOne('SELECT COUNT(DISTINCT `user_id`) FROM `tweets` WHERE `db_id` = %s AND `user_statuscount` <= %s', (analysis['db_id'], 50, ))[0] # 50 Status
			fewfavorites = con.fetchOne('SELECT COUNT(DISTINCT `user_id`) FROM `tweets` WHERE `db_id` = %s AND `user_favoritescount` <= %s', (analysis['db_id'], 50, ))[0] # 50 Favorites

			results = (fewinteractions, usercount, recentusercount, defaultprofilecount, defaultpicturecount, fewfollowings, fewfollowers, fewstatus, fewfavorites)

			# Save results.
			con.execute('UPDATE `analysis` SET `ready` = true, `results_fewinteractions` = %s, `results_usercount` = %s, `results_recentusercount` = %s, `results_defaultprofilecount` = %s, `results_defaultpicturecount` = %s, `results_fewfollowings` = %s, `results_fewfollowers` = %s, `results_fewstatus` = %s, `results_fewfavorites` = %s WHERE `id` = %s',  results + (analysis['id'], ))
			con.close()
	
	@staticmethod
	def Stop():
		if TaskNode._STOP != False:
			return
		TaskNode._STOP = True
		TaskNode._THREAD.join()
		TaskNode._THREAD = None
		TaskNode._STOP = None
	
	@staticmethod
	def WaitStop():
		if TaskNode._STOP != False:
			return
		TaskNode._THREAD.join()

if __name__ == '__main__':
	print('Running data_worker.py as main...')
	Config.Init()
	TaskManager.Init()
	TaskNode.Init()
	TaskManager.WaitStop()
	TaskNode.WaitStop()


