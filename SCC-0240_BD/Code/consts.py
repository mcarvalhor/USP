


# Consts will be placed in this file.

DB_MAX_HP_RESULTS = 15 # Number of items displayed in home page.
DB_MAX_SEARCH_RESULTS = 15 # Maximum number of results displayed in a single search.

# Allowed report types.
DB_REPORT_TYPES = [
	('Vandalismo', 'Vandalismo'),
	('Manutenção', 'Manutenção'),
	('Informação Incorreta', 'Informação Incorreta'),
	('Furto', 'Furto')
]

# Files that should be executed (in order) so that database schema can be recreated.
DB_RECREATE_FILES = [
	'../BD/DDL/drop.sql',
	'../BD/DDL/esquema.sql',
	'../BD/DML/dados.sql'
]
