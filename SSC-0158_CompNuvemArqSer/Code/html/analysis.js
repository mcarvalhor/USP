
ANs = [ ];
DBs = [ ];

UPDATING = false;

function updateData() {
	if(UPDATING) {
		return;
	}
	UPDATING = true;

	$(".content > div").hide()
	$("#content-loading").show()
	document.querySelector("#content-data").innerHTML = '<div class="an-add"><a href="#" onclick="newAN(); return false;">Criar Análise</a></div>';

	$.ajax({
		url: "server/analysis",
		cache: false,
		method: "GET",
		timeout: 30000,
		success: function(reqResults) {
			try {
				reqResults = JSON.parse(reqResults);
				ANs = reqResults['analysis'];
				DBs = reqResults['dbs'];
				for(let i = 0; i < ANs.length; i++) {
					db = null;
					for(let j = 0; j < DBs.length; j++) {
						if(ANs[i].db == DBs[j].id) {
							db = DBs[j];
							break;
						}
					}
					an_div = document.createElement("div");
					an_div.className = "an-item";
					an_div.innerHTML = '<h3>Name</h3><span>Status</span>'
					an_div.innerHTML += '<a href="#" class="an-edit-link" data-id="' + ANs[i].id + '" onclick="editAN(this); return false;">(editar)</a><p>Description</p>'
					an_div.innerHTML += '<div class="an-results"></div>'
					document.querySelector("#content-data").appendChild(an_div);
					an_div.querySelector("h3").textContent = ANs[i].name;
					an_div.querySelector("p").textContent = ANs[i].description;
					if(ANs[i].ready) {
						an_div.querySelector(".an-results").innerHTML = "Número de tweets: " + db.count;
						an_div.querySelector(".an-results").innerHTML += "<br>Número de tweets com poucas interações: " + ANs[i].results["fewinteractions"] + " (" + (100*ANs[i].results["fewinteractions"]/db.count).toFixed(2) + "%)";
						an_div.querySelector(".an-results").innerHTML += "<br>Número de contas: " + ANs[i].results["usercount"];
						an_div.querySelector(".an-results").innerHTML += "<br>Número de contas recentes: " + ANs[i].results["recentusercount"] + " (" + (100*ANs[i].results["recentusercount"]/ANs[i].results["usercount"]).toFixed(2) + "%)";
						an_div.querySelector(".an-results").innerHTML += "<br>Número de contas com perfil padrão: " + ANs[i].results["defaultprofilecount"] + " (" + (100*ANs[i].results["defaultprofilecount"]/ANs[i].results["usercount"]).toFixed(2) + "%)";
						an_div.querySelector(".an-results").innerHTML += "<br>Número de contas com foto de perfil padrão: " + ANs[i].results["defaultpicturecount"] + " (" + (100*ANs[i].results["defaultpicturecount"]/ANs[i].results["usercount"]).toFixed(2) + "%)";
						an_div.querySelector(".an-results").innerHTML += "<br>Número de contas com poucos 'seguindo': " + ANs[i].results["fewfollowings"] + " (" + (100*ANs[i].results["fewfollowings"]/ANs[i].results["usercount"]).toFixed(2) + "%)";
						an_div.querySelector(".an-results").innerHTML += "<br>Número de contas com poucos seguidores: " + ANs[i].results["fewfollowers"] + " (" + (100*ANs[i].results["fewfollowers"]/ANs[i].results["usercount"]).toFixed(2) + "%)";
						an_div.querySelector(".an-results").innerHTML += "<br>Número de contas com poucos tweets publicados: " + ANs[i].results["fewstatus"] + " (" + (100*ANs[i].results["fewstatus"]/ANs[i].results["usercount"]).toFixed(2) + "%)";
						an_div.querySelector(".an-results").innerHTML += "<br>Número de contas com poucos tweets favoritados: " + ANs[i].results["fewfavorites"] + " (" + (100*ANs[i].results["fewfavorites"]/ANs[i].results["usercount"]).toFixed(2) + "%)";
						an_div.querySelector("span").textContent = "Processamento completo (" + db.name + ")";
						an_div.querySelector("span").className += " an-item-done";
					} else {
						an_div.querySelector("span").textContent = "Processamento em progresso (" + db.name + ")";
						an_div.querySelector("span").className += " an-item-progress";
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

function editAN(linkId) {
	let i, an = null;

	linkId = linkId.getAttribute("data-id");
	for(i = 0; i < ANs.length; i++) {
		if(ANs[i].id == linkId) {
			an = ANs[i];
			break;
		}
	}

	if(an == null) {
		return;
	}
	$(".content > div").hide()

	document.querySelector("#content-edit form input[name='id']").value = an.id;
	document.querySelector("#content-edit form input[name='name']").value = an.name;
	document.querySelector("#content-edit form textarea[name='description']").value = an.description ? an.description : "";

	$("#content-edit").show()
}

function saveAN() {
	let i, an = null;

	anId = document.querySelector("#content-edit form input[name='id']").value;
	for(i = 0; i < ANs.length; i++) {
		if(ANs[i].id == anId) {
			an = ANs[i];
			break;
		}
	}

	if(an == null) {
		return;
	}
	UPDATING = true;
	$(".content > div").hide()
	$("#content-loading").show()

	$.ajax({
		url: "server/analysis/" + an.id,
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
			alert("Ocorreu um erro ao tentar editar esta análise!");
			UPDATING = false;
			updateData();
		}
	});

}

function removeAN() {
	let i, an = null;

	anId = document.querySelector("#content-edit form input[name='id']").value;
	for(i = 0; i < ANs.length; i++) {
		if(ANs[i].id == anId) {
			an = ANs[i];
			break;
		}
	}

	if(an == null) {
		return;
	}

	if(!confirm("Deseja realmente remover esta análise?")) {
		return;
	}
	UPDATING = true;
	$(".content > div").hide()
	$("#content-loading").show()

	$.ajax({
		url: "server/analysis/" + an.id,
		cache: false,
		method: "DELETE",
		timeout: 30000,
		success: function(reqResults) {
			UPDATING = false;
			updateData();
		},
		error: function() {
			alert("Ocorreu um erro ao tentar remover esta análise!");
			UPDATING = false;
			updateData();
		}
	});

}

function newAN() {
	let i, option;

	$(".content > div").hide()

	document.querySelector("#content-create form input[name='name']").value = "";
	document.querySelector("#content-create form textarea[name='description']").value = "";
	document.querySelector("#content-create form select[name='db']").innerHTML = '<option value="" disabled selected>(Base de Dados)</option>';
	for(i = 0; i < DBs.length; i++) {
		option = document.createElement("option");
		option.value = DBs[i].id;
		option.textContent = DBs[i].name;
		if(!DBs[i].ready) {
			option.disabled = true;
		}
		document.querySelector("#content-create form select[name='db']").appendChild(option);
	}
	document.querySelector("#content-create form select[name='db']").value = "";

	$("#content-create").show()
}

function createAN() {
	UPDATING = true;
	$(".content > div").hide()
	$("#content-loading").show()

	$.ajax({
		url: "server/analysis",
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
			alert("Ocorreu um erro ao tentar criar esta análise!");
			UPDATING = false;
			updateData();
		}
	});

}

function resetForm() {
	updateData();
}

function windowLoaded() {
	updateData();
	//setTimeout('updateData();', 10000);
}



window.onload = windowLoaded;


