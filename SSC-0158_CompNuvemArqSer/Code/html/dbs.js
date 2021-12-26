
DBs = [ ];

UPDATING = false;

function _padInt(n) {
	if(n < 10) {
		return '0' + n;
	} else {
		return '' + n;
	}
}

function updateData() {
	if(UPDATING) {
		return;
	}
	UPDATING = true;

	$(".content > div").hide()
	$("#content-loading").show()
	document.querySelector("#content-data").innerHTML = '<div class="db-add"><a href="#" onclick="newDB(); return false;">Criar Base de Dados</a></div>';

	$.ajax({
		url: "server/databases",
		cache: false,
		method: "GET",
		timeout: 30000,
		success: function(reqResults) {
			try {
				DBs = JSON.parse(reqResults);
				for(let i = 0; i < DBs.length; i++) {
					db_div = document.createElement("div");
					db_div.className = "db-item";
					db_div.innerHTML = '<h3>Name</h3><span>Status/Number Tweets</span>'
					db_div.innerHTML += '<a href="#" class="db-edit-link" data-id="' + DBs[i].id + '" onclick="editDB(this); return false;">(editar)</a><p>Description</p>'
					document.querySelector("#content-data").appendChild(db_div);
					db_div.querySelector("h3").textContent = DBs[i].name;
					db_div.querySelector("p").textContent = DBs[i].description;
					if(DBs[i].ready) {
						db_div.querySelector("span").textContent = "Busca completa (" + DBs[i]['tweet_count'] + " tweets encontrados)";
						db_div.querySelector("span").className += " db-item-done";
					} else {
						db_div.querySelector("span").textContent = "Busca em progresso (" + DBs[i]['tweet_count'] + " tweets encontrados)";
						db_div.querySelector("span").className += " db-item-progress";
					}
				}
				$(".content > div").hide()
				$("#content-data").show()
			} catch(e) {
				$(".content > div").hide()
				$("#content-error").show()
			}
			UPDATING = false;
		},
		error: function() {
			$(".content > div").hide()
			$("#content-error").show()
			UPDATING = false;
		}
	});
	
}

function editDB(linkId) {
	let i, date, db = null;

	linkId = linkId.getAttribute("data-id");
	for(i = 0; i < DBs.length; i++) {
		if(DBs[i].id == linkId) {
			db = DBs[i];
			break;
		}
	}

	if(db == null) {
		return;
	}

	$(".content > div").hide()

	document.querySelector("#content-edit form input[name='id']").value = db.id;
	document.querySelector("#content-edit form input[name='timezone_offset']").value = new Date().getTimezoneOffset();
	document.querySelector("#content-edit form input[name='name']").value = db.name;
	document.querySelector("#content-edit form textarea[name='description']").value = db.description ? db.description : "";
	document.querySelector("#content-edit form input[name='query']").value = db.query;
	document.querySelector("#content-edit form select[name='lang']").value = db.lang ? db.lang : "";
	if(db.tweeted_before_date != null) {
		date = new Date(db.tweeted_before_date * 1000)
		document.querySelector("#content-edit form input[name='before_date']").value = _padInt(date.getUTCFullYear()) + "-" + _padInt(date.getUTCMonth() + 1) + "-" + _padInt(date.getUTCDate());
		document.querySelector("#content-edit form input[name='before_date']").disabled = false;
		document.querySelector("#content-edit form input[name='before_date']").required = true;
		document.querySelector("#content-edit form input[name='before_date_check']").checked = true;
	} else {
		document.querySelector("#content-edit form input[name='before_date']").value = "";
		document.querySelector("#content-edit form input[name='before_date']").required = false;
		document.querySelector("#content-edit form input[name='before_date']").disabled = true;
		document.querySelector("#content-edit form input[name='before_date_check']").checked = false;
	}
	if(db.tweeted_after_date != null) {
		date = new Date(db.tweeted_after_date * 1000)
		document.querySelector("#content-edit form input[name='after_date']").value = _padInt(date.getFullYear()) + "-" + _padInt(date.getMonth() + 1) + "-" + _padInt(date.getDate());
		document.querySelector("#content-edit form input[name='after_time']").value = _padInt(date.getHours()) + ":" + _padInt(date.getMinutes());
		document.querySelector("#content-edit form input[name='after_date']").disabled = false;
		document.querySelector("#content-edit form input[name='after_time']").disabled = false;
		document.querySelector("#content-edit form input[name='after_date']").required = true;
		document.querySelector("#content-edit form input[name='after_time']").required = true;
		document.querySelector("#content-edit form input[name='after_date_check']").checked = true;
	} else {
		document.querySelector("#content-edit form input[name='after_date']").value = "";
		document.querySelector("#content-edit form input[name='after_time']").value = "";
		document.querySelector("#content-edit form input[name='after_date']").required = false;
		document.querySelector("#content-edit form input[name='after_time']").required = false;
		document.querySelector("#content-edit form input[name='after_date']").disabled = true;
		document.querySelector("#content-edit form input[name='after_time']").disabled = true;
		document.querySelector("#content-edit form input[name='after_date_check']").checked = false;
	}
	document.querySelector("#content-edit form input[name='update']").checked = false;

	$("#content-edit").show()
}

function saveDB() {
	let i, db = null;

	dbId = document.querySelector("#content-edit form input[name='id']").value;
	for(i = 0; i < DBs.length; i++) {
		if(DBs[i].id == dbId) {
			db = DBs[i];
			break;
		}
	}

	if(db == null) {
		return;
	}
	UPDATING = true;
	$(".content > div").hide()
	$("#content-loading").show()

	$.ajax({
		url: "server/databases/" + db.id,
		cache: false,
		method: "POST",
		data: $("#content-edit form").serializeArray(),
		dataType: "text",
		timeout: 30000,
		success: function(reqResults) {
			UPDATING = false;
			updateData();
		},
		error: function() {
			alert("Ocorreu um erro ao tentar editar esta base de dados! Certifique-se de que ela não está sendo usada em nenhuma análise antes de modificar.");
			UPDATING = false;
			updateData();
		}
	});

}

function removeDB() {
	let i, db = null;

	dbId = document.querySelector("#content-edit form input[name='id']").value;
	for(i = 0; i < DBs.length; i++) {
		if(DBs[i].id == dbId) {
			db = DBs[i];
			break;
		}
	}

	if(db == null) {
		return;
	}

	if(!confirm("Deseja realmente remover esta base de dados?")) {
		return;
	}

	UPDATING = true;
	$(".content > div").hide()
	$("#content-loading").show()

	$.ajax({
		url: "server/databases/" + db.id,
		cache: false,
		method: "DELETE",
		timeout: 30000,
		success: function(reqResults) {
			UPDATING = false;
			updateData();
		},
		error: function() {
			alert("Ocorreu um erro ao tentar remover esta base de dados! Certifique-se de que ela não está sendo usada em nenhuma análise antes de remover.");
			UPDATING = false;
			updateData();
		}
	});

}

function newDB() {
	let date;

	$(".content > div").hide()

	document.querySelector("#content-create form input[name='timezone_offset']").value = new Date().getTimezoneOffset();
	document.querySelector("#content-create form input[name='name']").value = "";
	document.querySelector("#content-create form textarea[name='description']").value = "";
	document.querySelector("#content-create form input[name='query']").value = "";
	document.querySelector("#content-create form select[name='lang']").value = "";
	document.querySelector("#content-create form input[name='before_date']").value = "";
	document.querySelector("#content-create form input[name='before_date']").required = false;
	document.querySelector("#content-create form input[name='before_date']").disabled = true;
	document.querySelector("#content-create form input[name='before_date_check']").checked = false;
	document.querySelector("#content-create form input[name='after_date']").value = "";
	document.querySelector("#content-create form input[name='after_time']").value = "";
	document.querySelector("#content-create form input[name='after_date']").required = false;
	document.querySelector("#content-create form input[name='after_time']").required = false;
	document.querySelector("#content-create form input[name='after_date']").disabled = true;
	document.querySelector("#content-create form input[name='after_time']").disabled = true;
	document.querySelector("#content-create form input[name='after_date_check']").checked = false;
	document.querySelector("#content-create form input[name='donotupdate']").checked = false;

	$("#content-create").show()
}

function createDB() {
	UPDATING = true;
	$(".content > div").hide()
	$("#content-loading").show()

	$.ajax({
		url: "server/databases",
		cache: false,
		method: "POST",
		data: $("#content-create form").serializeArray(),
		dataType: "text",
		timeout: 30000,
		success: function(reqResults) {
			UPDATING = false;
			updateData();
		},
		error: function() {
			alert("Ocorreu um erro ao tentar criar esta base de dados! Certifique-se de que os dados inseridos fazem sentido.");
			UPDATING = false;
			updateData();
		}
	});

}

function checkboxChanged(checked, inputs) {
	let i;
	if(checked) {
		for(i = 0; i < inputs.length; i++) {
			inputs[i].disabled = false;
			inputs[i].required = true;
		}
	} else {
		for(i = 0; i < inputs.length; i++) {
			inputs[i].required = false;
			inputs[i].disabled = true;
			inputs[i].value = "";
		}
	}
}

function resetForm() {
	updateData();
}

function windowLoaded() {
	updateData();
	//setTimeout('updateData();', 10000);
}



window.onload = windowLoaded;


