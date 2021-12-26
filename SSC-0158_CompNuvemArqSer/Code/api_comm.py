from utils import Config, DBConnection
from threading import Thread, Lock
from consts import TA_THREAD_SLEEP_TIMEOUT, TA_MAX_SEARCH_COUNT, TA_THREAD_SLEEP_RATELIMIT
from time import sleep
import datetime
import tweepy



class APICommunicator:

	_THREAD = None
	_MUTEX = None
	_STOP = None

	@staticmethod
	def Init():
		if APICommunicator._STOP != None:
			return
		APICommunicator._THREAD = Thread(target=APICommunicator._Run)
		APICommunicator._MUTEX = Lock()
		APICommunicator._THREAD.start()
		APICommunicator._STOP = False

	@staticmethod
	def _Run_InsertAll(con, found_tweets, oldest_tweet, most_recent_tweet, database):
		insert_tweets = [ ]
		for t in found_tweets:
			if oldest_tweet is not None and t.id_str == oldest_tweet[0]:
				continue
			if most_recent_tweet is not None and t.id_str == most_recent_tweet[0]:
				continue
			if database['tweeted_after_date'] is not None and datetime.datetime.timestamp(datetime.datetime.strptime(t._json['created_at'], '%a %b %d %H:%M:%S %z %Y')) < database['tweeted_after_date']:
				continue
			insert_tweets.append( (
				database['id'],
				t.id_str,
				int(datetime.datetime.timestamp(datetime.datetime.strptime(t._json['created_at'], '%a %b %d %H:%M:%S %z %Y'))),
				t.favorite_count,
				t.retweet_count,
				t.in_reply_to_status_id_str,
				t.lang,
				t.user.id_str,
				t.user.screen_name,
				int(datetime.datetime.timestamp(datetime.datetime.strptime(t._json['user']['created_at'], '%a %b %d %H:%M:%S %z %Y'))),
				1 if t.user.protected else 0,
				1 if t.user.verified else 0,
				t.user.followers_count,
				t.user.friends_count,
				t.user.listed_count,
				t.user.favourites_count,
				t.user.statuses_count,
				1 if t.user.default_profile else 0,
				1 if t.user.default_profile_image else 0,
			) )
		if len(insert_tweets) >= 1:
			return con.insertAll('INSERT IGNORE INTO `tweets` (`db_id`, `twitterid`, `createdat`, `favoritecount`, `retweetcount`, `replytostatusid`, `lang`, `user_id`, `user_screenname`, `user_createdat`, `user_protected`, `user_verified`, `user_followers`, `user_following`, `user_listedcount`, `user_favoritescount`, `user_statuscount`, `user_defaultprofile`, `user_defaultpicture`) VALUES(%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s)', insert_tweets)
		return 0

	@staticmethod
	def _Run():
		api_auth = tweepy.OAuthHandler(Config.TWITTER_API_KEY, Config.TWITTER_API_SECRET)
		if Config.TWITTER_ACCESS_TOKEN is not None and Config.TWITTER_ACCESS_TOKEN_SECRET is not None:
			api_auth.set_access_token(Config.TWITTER_ACCESS_TOKEN, Config.TWITTER_ACCESS_TOKEN_SECRET)
		api = tweepy.API(api_auth)
		while True:

			APICommunicator._MUTEX.acquire(timeout = TA_THREAD_SLEEP_TIMEOUT)
			if APICommunicator._STOP == True: # Stop thread?
				return

			con = DBConnection()
			database = con.fetchOne('SELECT `id`,`query`,`tweeted_before_date`,`tweeted_after_date`,`lang` FROM `dbs` WHERE `ready`=false')
			if database is None: # Any database waiting for data to be retrieved?
				con.close()
				continue
			database = { 'id': database[0], 'query': database[1], 'tweeted_before_date': database[2], 'tweeted_after_date': database[3], 'lang': database[4] }
			if database['tweeted_before_date'] is not None:
				database['tweeted_before_date'] = datetime.datetime.fromtimestamp(database['tweeted_before_date'], tz=datetime.timezone.utc).strftime('%Y-%m-%d')

			# Find oldest tweet to keep pagination from there.
			oldest_tweet = con.fetchOne('SELECT `twitterid` FROM `tweets` WHERE `db_id` = %s ORDER BY `createdat` ASC LIMIT 1', (database['id'], ))
			found_tweets = [ ]
			if oldest_tweet is None:
				# Found no tweets in database. In this case, try to gather a few tweets first.
				try:
					found_tweets = api.search(database['query'], count = TA_MAX_SEARCH_COUNT, lang = database['lang'], until = database['tweeted_before_date'])
				except tweepy.RateLimitError:
					APICommunicator._MUTEX.release()
					con.close()
					sleep(TA_THREAD_SLEEP_RATELIMIT)
					continue
				if APICommunicator._Run_InsertAll(con, found_tweets, None, None, database) > 0:
					APICommunicator._MUTEX.release()
					con.close()
					continue
			
			else:
				# Found a oldest tweet. This means we can try to continue pagination from there.
				try:
					found_tweets = api.search(database['query'], count = TA_MAX_SEARCH_COUNT, lang = database['lang'], until = database['tweeted_before_date'], max_id = oldest_tweet[0])
				except tweepy.RateLimitError:
					APICommunicator._MUTEX.release()
					con.close()
					sleep(TA_THREAD_SLEEP_RATELIMIT)
					continue
				if APICommunicator._Run_InsertAll(con, found_tweets, oldest_tweet, None, database) > 0:
					# Pagination worked for the past tweets.
					APICommunicator._MUTEX.release()
					con.close()
					continue

				# There's no oldest tweet anymore (finished pagination for past tweets). Now find most recent tweet.
				most_recent_tweet = con.fetchOne('SELECT `twitterid` FROM `tweets` WHERE `db_id` = %s ORDER BY `createdat` DESC LIMIT 1', (database['id'], ))
				try:
					found_tweets = api.search(database['query'], count = TA_MAX_SEARCH_COUNT, lang = database['lang'], until = database['tweeted_before_date'], since_id = most_recent_tweet[0])
				except tweepy.RateLimitError:
					APICommunicator._MUTEX.release()
					con.close()
					sleep(TA_THREAD_SLEEP_RATELIMIT)
					continue
				if APICommunicator._Run_InsertAll(con, found_tweets, oldest_tweet, None, database) > 0:
					# Pagination worked for the recent tweets.
					APICommunicator._MUTEX.release()
					con.close()
					continue

			# Now finished pagination in this database. In this case, mark it as complete.
			con.execute('UPDATE `dbs` SET `ready` = true WHERE `id` = %s',  (database['id'], ))
			APICommunicator._MUTEX.release()
			con.close()
	
	@staticmethod
	def WakeUp():
		if APICommunicator._STOP != False:
			return
		APICommunicator._MUTEX.release()
	
	@staticmethod
	def Stop():
		if APICommunicator._STOP != False:
			return
		APICommunicator._STOP = True
		APICommunicator._THREAD.join()
		APICommunicator._THREAD = None
		APICommunicator._MUTEX = None
		APICommunicator._STOP = None
	
	@staticmethod
	def WaitStop():
		if APICommunicator._STOP != False:
			return
		APICommunicator._THREAD.join()


if __name__ == '__main__':
	print('Running api_comm.py as main...')
	Config.Init()
	APICommunicator.Init()
	APICommunicator.WaitStop()


