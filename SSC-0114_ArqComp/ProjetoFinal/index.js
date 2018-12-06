
/* Usado para poder exportar a arquitetura para carregar de arquivo posteriomente */
var definition = {
	memoryAlgorithm: "",
	nCores: "",
	cacheLines: "",
	cacheBlocks: "",
	cacheAlgorithm: "",
	memInitValues: [ ],
	coreRequests: [ ]
};

/* Usado para simulação em tempo real */
var simulation = {
	running: false,
	currentPosition: null,
	initialState: null,
	speed: 1000,
	requests: null
};

var scriptsLoaded = true;

function confirmDialog(message, okText, okAction, abortText, abortAction) {
	if(okText == null) {
		okText = "OK";
	}
	if(okAction == null) {
		okAction = "";
	}
	if(abortText == null) {
		abortText = "Cancelar";
	}
	if(abortAction == null) {
		abortAction = "";
	}
	
	document.getElementById("dialogMessage").innerHTML = message;
	document.getElementById("dialogButtons").innerHTML = "<a href='javascript:void(0);' onclick='closeDialog(" + okAction + ");'>" + okText + "</a>";
	document.getElementById("dialogButtons").innerHTML += "<a href='javascript:void(0);' onclick='closeDialog(" + abortAction + ");'>" + abortText + "</a>";
	$("#blurBackground").stop().fadeIn(500);
	$("#dialog").stop().slideDown(1000);
}

function alertDialog(message, okText, okAction) {
	if(okText == null) {
		okText = "Fechar";
	}
	if(okAction == null) {
		okAction = "";
	}
	
	document.getElementById("dialogMessage").innerHTML = message;
	document.getElementById("dialogButtons").innerHTML = "<a href='javascript:void(0);' onclick='closeDialog(" + okAction + ");'>" + okText + "</a>";
	$("#blurBackground").stop().fadeIn(500);
	$("#dialog").stop().slideDown(1000);
}

function closeDialog(action) {
	$("#dialog").stop().slideUp(500);
	$("#blurBackground").stop().fadeOut(1000);

	if(action != null) {
		try { action(); } catch(exc) { }
	}
}

function bytesToString(nBytes) {
	if(nBytes < 0) {
		return;
	}
	if(nBytes > 1048570756) {
		return (nBytes/1073741824).toFixed(2) + " GiB (" + nBytes.toLocaleString() + " bytes)";
	} else if(nBytes > 1023992) {
		return (nBytes/1048576).toFixed(2) + " MiB (" + nBytes.toLocaleString() + " bytes)";
	} else if(nBytes >= 1024) {
		return (nBytes/1024).toFixed(2) + " KiB (" + nBytes.toLocaleString() + " bytes)";
	} else if(nBytes != 1) {
		return nBytes.toLocaleString() + " bytes";
	} else {
		return "1 byte";
	}
}

function calculateMemBytes() {
	var nBytes, element;

	element = document.getElementById("memorySizeBytes");
	nBytes = document.getElementById("memorySize").value;

	if(nBytes == "" || parseInt(nBytes, 10) < 0 || parseInt(nBytes, 10) > 536870912) {
		element.innerHTML = "";
		return;
	}

	nBytes = parseInt(nBytes, 10) * 4;

	element.innerHTML = "RAM: " + bytesToString(nBytes);
}

function calculateCacheBytes() {
	var nLines, nBlocks, nBytes, element;

	element = document.getElementById("cacheSizeBytes");
	nLines = document.getElementById("cacheLines").value;
	nBlocks = document.getElementById("cacheBlocks").value;

	if(nLines == "" || nBlocks == "") {
		element.innerHTML = "";
		return;
	}

	nLines = parseInt(nLines, 10);
	nBlocks = parseInt(nBlocks, 10);
	nBytes = nLines * nBlocks;

	if(nBytes < 0 || nBytes > 536870912) {
		element.innerHTML = "";
		return;
	}

	nBytes *= 4;

	element.innerHTML = "Cache: " + bytesToString(nBytes);
}

function submitHome() {
	var i, algorithm, memSize, nCores, cacheLines, cacheBlocks, cacheAlgorithm;

	algorithm = document.getElementById("memoryAlgorithm").value;
	memSize = document.getElementById("memorySize").value;
	nCores = document.getElementById("nCores").value;
	cacheLines = document.getElementById("cacheLines").value;
	cacheBlocks = document.getElementById("cacheBlocks").value;
	cacheAlgorithm = document.getElementById("cacheAlgorithm").value;

	if(algorithm == "" || memSize == "" || nCores == "" || cacheLines == "" ||
		cacheBlocks == "" || cacheAlgorithm == "") {
		alertDialog("Por favor, defina todos os campos!");
		return false;
	}

	memSize = parseInt(memSize, 10);
	nCores = parseInt(nCores, 10);
	cacheLines = parseInt(cacheLines, 10);
	cacheBlocks = parseInt(cacheBlocks, 10);
	
	if(memSize < 0 || memSize > 536870912) {
		alertDialog("A capacidade da memória principal (em palavras) deve estar entre 0 e 536870912!");
	} else if(nCores < 0 || nCores > 4096) {
		alertDialog("O número de núcleos no processador deve estar entre 0 e 4096!");
	} else if(cacheLines < 0 || cacheBlocks < 0) {
		alertDialog("O número de linhas na cache e palavras por linha devem ser maiores que 0!");
	} else if(cacheLines * cacheBlocks > 536870912) {
		alertDialog("A capacidade total da cache em palavras deve estar entre 0 e 536870912!");
	} else if(cacheLines * cacheBlocks >= memSize) {
		alertDialog("Não faz sentido a cache ter capacidade maior ou igual a da memória principal!");
	} else {
		document.getElementById("homeDefinition").style.display = "none";
		document.getElementById("memInitValues").innerHTML = "";
		for(i = 0; i < memSize; i++) {
			document.getElementById("memInitValues").innerHTML += "<tr><td>0x" + (i*4).toString(16).toUpperCase() + "</td><td><input type='number' min='0' max='4294967295' step='1' placeholder='(valor inicial)' required='required'></td></tr>";
		}
		$("#memoryDefinition").stop().fadeIn(1000);
		return true;
	}
	return false;
}

function backToHome() {
	document.getElementById("memoryDefinition").style.display = "none";
	$("#homeDefinition").stop().fadeIn(1000);
}

function autoFillMem(method) {
	var i, j, elements;
	elements = document.getElementById("memInitValues").getElementsByTagName("input");
	switch(method) {
		case 0:
			for(i = 0; i < elements.length; i++) {
				elements[i].value = Math.floor(Math.random() * 4294967296);
			}
		break;
		case 1:
			for(i = 0; i < elements.length; i++) {
				elements[i].value = Math.floor(Math.random() * 1000);
			}
		break;
		case 2:
			for(i = 0; i < elements.length; i++) {
				elements[i].value = i;
			}
		break;
		case 3:
			for(i = 0; i < elements.length; i++) {
				elements[i].value = i + 100;
			}
		break;
		case 4:
			for(i = 0; i < elements.length; i++) {
				elements[i].value = (i * 4 * 10) % 4294967295;
			}
		break;
	}
	closeDialog();
}

function submitMemory() {
	var i, elements, memSize, value;

	elements = document.getElementById("memInitValues").getElementsByTagName("input");
	memSize = document.getElementById("memorySize").value;
	memSize = parseInt(memSize, 10);
	
	for(i = 0; i < elements.length; i++) {
		if(elements[i].value == "") {
			alertDialog("Por favor, defina todos os campos!");
			return false;
		}
		value = parseInt(elements[i].value, 10);
		if(value < 0 || value > 4294967295) {
			alertDialog("O valor de uma palavra de 32 bits deve estar entre 0 e 4294967295. Verifique os valores digitados!");
			return false;
		}
	}

	document.getElementById("memoryDefinition").style.display = "none";
	document.getElementById("coreRequests").innerHTML = "";
	$("#requestsDefinition").stop().fadeIn(1000);
	return true;
}

function backToMemory() {
	document.getElementById("requestsDefinition").style.display = "none";
	$("#memoryDefinition").stop().fadeIn(1000);
}

function addRequestItem() {
	var element, newElement, nCores, memSize;

	element = document.getElementById("coreRequests");
	memSize = document.getElementById("memorySize").value;
	memSize = parseInt(memSize, 10);
	nCores = document.getElementById("nCores").value;
	nCores = parseInt(nCores, 10);

	newElement = document.createElement("tr");
	newElement.innerHTML = "<td><input type='number' placeholder='(N)' step='1' min='0' max='" + (nCores - 1) + "' required='required'></td><td><select required='required' onchange='requestTypeChanged(this.value, this.parentNode);'><option value='' disabled='disabled' selected='selected'>(selecione)</option><option value='read'>lê</option><option value='write'>escreve</option></select> <input type='number' value='0' placeholder='(valor)' min='0' max='4294967295' required='required' style='display: none;'></td><td><input type='number' placeholder='(0, 4, ...)' min='0' max='" + (memSize - 1) * 4 + "' step='4' required='required'></td><td><a href='javascript:void(0);' onclick='moveRequestItemUp(this.parentNode.parentNode);'>&uarr;</a><a href='javascript:void(0);' onclick='moveRequestItemDown(this.parentNode.parentNode);'>&darr;</a><a href='javascript:void(0);' onclick='removeRequestItem(this.parentNode.parentNode);'>&times;</a></td>";

	element.appendChild(newElement);
	element = newElement.getElementsByTagName("input");
	element[element.length - 3].focus();
	return newElement;
}

function removeRequestItem(item) {
	var element;

	element = document.getElementById("coreRequests");
	element.removeChild(item);
}

function moveRequestItemUp(item) {
	var parent, previous;

	parent = item.parentNode;
	previous = item.previousElementSibling;

	if(previous != null)
		parent.insertBefore(item, previous);
}

function moveRequestItemDown(item) {
	var parent, next;

	parent = item.parentNode;
	next = item.nextElementSibling;

	if(next != null)
		parent.insertBefore(next, item);
}

function requestTypeChanged(value, item) {
	var element;

	element = item.getElementsByTagName("input")[0];

	if(value == "write") {
		element.value = "";
		element.style.display = "inline";
	} else {
		element.value = "0";
		element.style.display = "none";
	}
}

function findInLine(line, addr) {
	var i;

	for(i = 0; i < line.length; i++) {
		if(line[i].a == addr) {
			return i;
		}
	}

	return -1;
}

function invalidateInLine(line, pos) {
	var i, aux;

	aux = [ ];
	for(i = 0; i < line.length; i++) {
		if(i != pos) {
			aux.push({ a: line[i].a, v: line[i].v, s: line[i].s, c: line[i].c });
		}
	}

	return aux;
}

function updateInLine(line, pos, newValue, addr, state, counter) {
	var i, aux, aux2;

	aux = [ ];
	switch(definition.cacheAlgorithm) {
		case "fifo":
			if(pos < 0) { // adicionar
				for(i = 0; i < line.length; i++) {
					console.log(line[i]);
					aux.push(line[i]);
					console.log(aux);
				}
				aux.push({a: addr, v: newValue, s: state, c: -1});
			} else { // atualizar
				for(i = 0; i < line.length; i++) {
					aux.push(line[i]);
				}
				aux[pos].v = newValue;
			}
		break;
		case "lru":
			if(pos < 0) { // adicionar
				aux.push({a: addr, v: newValue, s: state, c: -1});
				for(i = 0; i < line.length; i++) {
					aux.push(line[i]);
				}
			} else { // atualizar
				for(i = 0; i < line.length; i++) {
					aux.push(line[i]);
				}
				aux[pos].v = newValue;
				aux2 = aux[0];
				aux[0] = aux[pos];
				aux[pos] = aux2;
			}
		break;
		default: // lfu
			if(pos < 0) { // adicionar
				for(i = 0; i < line.length && line[i].c <= counter; i++) {
					aux.push(line[i]);
				}
				aux.push({a: addr, v: newValue, s: state, c: counter});
				for(; i < line.length; i++) {
					aux.push(line[i]);
				}
			} else { // atualizar
				aux2 = line[pos];
				aux2.c++;
				for(i = 0; i < pos; i++) {
					aux.push(line[i]);
				}
				i++;
				for(; i < line.length && line[i].c < aux2.c; i++) {
					aux.push(line[i]);
				}
				aux.push(aux2);
				for(; i < line.length; i++) {
					aux.push(line[i]);
				}
			}
	}

	return aux;
}

function lineToString(line) {
	var aux;

	aux = "";
	for(i = 0; i < line.length; i++) {
		aux += "<span title='No endereço 0x" + (parseInt(line[i].a, 10) * 4).toString(16).toUpperCase() + "'>" + line[i].v + "</span>";
	}

	return aux;
}

function submitRequests() {
	var i, j, n, elements, inputs, element, state, nCores, cacheLines, defaultCacheLines, memSize;

	elements = document.getElementById("coreRequests").childNodes;
	element = document.getElementById("requests");
	memSize = document.getElementById("memorySize").value;
	memSize = parseInt(memSize, 10);
	nCores = document.getElementById("nCores").value;
	nCores = parseInt(nCores, 10);
	cacheLines = document.getElementById("cacheLines").value;
	cacheLines = parseInt(cacheLines, 10);

	if(elements.length < 1) {
		alertDialog("Adicione ao menos uma requisição de um núcleo!");
		return false;
	}

	for(i = 0; i < elements.length; i++) {
		inputs = elements[i].getElementsByTagName("input");
		if(inputs[0].value == "" || inputs[1].value == "" || inputs[2].value == "" || elements[i].getElementsByTagName("select")[0].value == "") {
			alertDialog("Por favor, defina todos os campos!");
			return false;
		}
		inputs = [parseInt(inputs[0].value, 10), parseInt(inputs[1].value, 10), parseInt(inputs[2].value, 10)]
		if(inputs[0] < 0 || inputs[0] >= nCores) {
			alertDialog("O valor do núcleo deve estar entre 0 e " + (nCores - 1) + ". Verifique os valores digitados!");
			return false;
		}
		if(inputs[1] < 0 || inputs[1] > 4294967295) {
			alertDialog("O valor de uma palavra de 32 bits deve estar entre 0 e 4294967295. Verifique os valores digitados!");
			return false;
		}
		if(inputs[2] < 0 || inputs[2] > (memSize - 1) * 4 || inputs[2] % 4 != 0) {
			alertDialog("O endereço de acesso deve estar entre 0 e " + ((memSize - 1) * 4) + ", sendo múltiplo de 4. Verifique os valores digitados!");
			return false;
		}
	}

	document.getElementById("requestsDefinition").style.display = "none";

	definition.memoryAlgorithm = document.getElementById("memoryAlgorithm").value;
	definition.nCores = document.getElementById("nCores").value;
	definition.cacheLines = document.getElementById("cacheLines").value;
	definition.cacheBlocks = document.getElementById("cacheBlocks").value;
	definition.cacheAlgorithm = document.getElementById("cacheAlgorithm").value;

	definition.coreRequests = [ ];
	element.innerHTML = "";
	for(i = 0; i < elements.length; i++) {
		inputs = elements[i].getElementsByTagName("input");
		definition.coreRequests.push([ inputs[0].value, elements[i].getElementsByTagName("select")[0].value, inputs[1].value, inputs[2].value / 4 ]);
		aux = (definition.coreRequests[i][1] == "write") ? "escreve " + definition.coreRequests[i][2] + " no" : "lê do";
		element.innerHTML += "<div class='request'><a href='javascript:void(0);' onclick='simClick(" + i + ", this.parentNode);'>P" + definition.coreRequests[i][0] + " " + aux + " endereço 0x" + (definition.coreRequests[i][3] * 4).toString(16).toUpperCase() + "</a></div>";
	}

	definition.memInitValues = [ ];
	elements = document.getElementById("memInitValues").getElementsByTagName("input");
	document.getElementById("mainMemoryWords").innerHTML = "";
	for(i = 0; i < elements.length; i++) {
		definition.memInitValues.push(elements[i].value);
		document.getElementById("mainMemoryWords").innerHTML += "<tr><td>0x" + (i*4).toString(16).toUpperCase() + "</td><td>" + elements[i].value + "</td></tr>";
	}

	defaultCacheLines = "";
	for(i = 0; i < cacheLines; i++) {
		defaultCacheLines += "<tr><td>" + (i) + "</td><td></td></tr>";
	}
	n = parseInt(definition.nCores, 10);
	document.getElementById("coresCPU").innerHTML = "";
	for(i = 0; i < n; i++) {
		document.getElementById("coresCPU").innerHTML += "<div class='core'><div class='bus'></div><div class='notification' style='display: none;'></div><div class='coreCPU'>CPU " + i + "</div><div class='coreCache'><table><thead><tr><th>Linha</th><th>Palavras</th></tr></thead><tbody>" + defaultCacheLines + "</tbody></table></div></div>";
	}

	/* EXECUÇÃO DO ALGORITMO */
	state = { mem: [], cores: [ ] };
	simulation.initialState = [ ];
	for(i = 0; i < memSize; i++) {
		element = parseInt(definition.memInitValues[i], 10);
		state.mem.push(element);
		simulation.initialState.push(element);
	}
	for(i = 0; i < nCores; i++) {
		state.cores.push([ ]);
		for(j = 0; j < cacheLines; j++) {
			state.cores[i].push([ ]);
		}
	}

	simulation.requests = [ ];
	simulation.currentPosition = -1;
	simulation.running = null;
	elements = document.getElementById("coreRequests").childNodes;
	switch(definition.memoryAlgorithm) {
		case "msi":
			for(i = 0; i < definition.coreRequests.length; i++) {
				simulation.requests.push([ {action: "focus", value: definition.coreRequests[i][0]} ]);
				element = findInLine(state.cores[definition.coreRequests[i][0]][definition.coreRequests[i][3] % cacheLines], definition.coreRequests[i][3]);
				if(definition.coreRequests[i][1] == "write") {
					simulation.requests[i].push({action: "notify", value: [definition.coreRequests[i][0], "PrWrite"]});
					simulation.requests[i].push({action: "write-bus", value: "BusWrite (" + definition.coreRequests[i][0] + ")"});
					if(element >= 0) {
						if(state.cores[definition.coreRequests[i][0]][definition.coreRequests[i][3] % cacheLines][element].s == "M") {
							state.cores[definition.coreRequests[i][0]][definition.coreRequests[i][3] % cacheLines] = updateInLine(state.cores[definition.coreRequests[i][0]][definition.coreRequests[i][3] % cacheLines], element, definition.coreRequests[i][2]);
							simulation.requests[i].push({action: "update-line", value: [definition.coreRequests[i][0], definition.coreRequests[i][3] % cacheLines, lineToString(state.cores[definition.coreRequests[i][0]][definition.coreRequests[i][3] % cacheLines])] });
						} else { // .s == "S"
							moved = false;
							for(j = 0; j < nCores; j++) {
								if(j == definition.coreRequests[i][0]) {
									continue;
								}
								elementAux = findInLine(state.cores[j][definition.coreRequests[i][3] % cacheLines], definition.coreRequests[i][3]);
								if(elementAux < 0) {
									continue;
								}
								if(state.cores[j][definition.coreRequests[i][3] % cacheLines][elementAux].s != "S") {
									continue;
								}
								moved = true;
								simulation.requests[i].push({action: "focus", value: j});
								simulation.requests[i].push({action: "notify", value: [j, "Observed BusWrite!"]});
								state.cores[j][definition.coreRequests[i][3] % cacheLines] = invalidateInLine(state.cores[j][definition.coreRequests[i][3] % cacheLines], elementAux);
								simulation.requests[i].push({action: "update-line", value: [j, definition.coreRequests[i][3] % cacheLines, lineToString(state.cores[j][definition.coreRequests[i][3] % cacheLines])] });
							}
							if(moved == true) {
								simulation.requests[i].push({action: "focus", value: definition.coreRequests[i][0]});
							}
							state.cores[definition.coreRequests[i][0]][definition.coreRequests[i][3] % cacheLines] = updateInLine(state.cores[definition.coreRequests[i][0]][definition.coreRequests[i][3] % cacheLines], element, definition.coreRequests[i][2]);
							state.cores[definition.coreRequests[i][0]][definition.coreRequests[i][3] % cacheLines][element].s = "M";
							simulation.requests[i].push({action: "update-line", value: [definition.coreRequests[i][0], definition.coreRequests[i][3] % cacheLines, lineToString(state.cores[definition.coreRequests[i][0]][definition.coreRequests[i][3] % cacheLines])] });
						}
					} else {
						moved = false;
						for(j = 0; j < nCores; j++) {
							if(j == definition.coreRequests[i][0]) {
								continue;
							}
							elementAux = findInLine(state.cores[j][definition.coreRequests[i][3] % cacheLines], definition.coreRequests[i][3]);
							if(elementAux < 0) {
								continue;
							}
							moved = true;
							simulation.requests[i].push({action: "focus", value: j});
							if(state.cores[j][definition.coreRequests[i][3] % cacheLines][elementAux].s == "M") {
								simulation.requests[i].push({action: "notify", value: [j, "Observed BusWrite: flush..."]});
								simulation.requests[i].push({action: "update-ram", value: [state.cores[j][definition.coreRequests[i][3] % cacheLines][elementAux].a, state.cores[j][definition.coreRequests[i][3] % cacheLines][elementAux].v] });
								state.mem[state.cores[j][definition.coreRequests[i][3] % cacheLines][elementAux].a] = state.cores[j][definition.coreRequests[i][3] % cacheLines][elementAux].v;
							} else {
								simulation.requests[i].push({action: "notify", value: [j, "Observed BusWrite!"]});
							}
							state.cores[j][definition.coreRequests[i][3] % cacheLines] = invalidateInLine(state.cores[j][definition.coreRequests[i][3] % cacheLines], elementAux);
							simulation.requests[i].push({action: "update-line", value: [j, definition.coreRequests[i][3] % cacheLines, lineToString(state.cores[j][definition.coreRequests[i][3] % cacheLines])] });
						}
						if(moved == true) {
							simulation.requests[i].push({action: "focus", value: definition.coreRequests[i][0]});
						}
						if(state.cores[definition.coreRequests[i][0]][definition.coreRequests[i][3] % cacheLines].length >= definition.cacheBlocks) {
							elementAux2 = state.cores[definition.coreRequests[i][0]][definition.coreRequests[i][3] % cacheLines].shift();
							if(elementAux2.s == "M") {
								simulation.requests[i].push({action: "update-ram", value: [elementAux2.a, elementAux2.v] });
								state.mem[elementAux2.a] = elementAux2.v;
							}
						}
						console.log(state.cores[definition.coreRequests[i][0]][definition.coreRequests[i][3] % cacheLines]);
						state.cores[definition.coreRequests[i][0]][definition.coreRequests[i][3] % cacheLines] = updateInLine(state.cores[definition.coreRequests[i][0]][definition.coreRequests[i][3] % cacheLines], element, definition.coreRequests[i][2], definition.coreRequests[i][3], "M", 0);
						simulation.requests[i].push({action: "update-line", value: [definition.coreRequests[i][0], definition.coreRequests[i][3] % cacheLines, lineToString(state.cores[definition.coreRequests[i][0]][definition.coreRequests[i][3] % cacheLines])] });
					}
				} else { // read
					simulation.requests[i].push({action: "notify", value: [definition.coreRequests[i][0], "PrRead"]});
					simulation.requests[i].push({action: "write-bus", value: "BusRead (" + definition.coreRequests[i][0] + ")"});
					if(element >= 0) {
						state.cores[definition.coreRequests[i][0]][definition.coreRequests[i][3] % cacheLines] = updateInLine(state.cores[definition.coreRequests[i][0]][definition.coreRequests[i][3] % cacheLines], element, state.cores[definition.coreRequests[i][0]][definition.coreRequests[i][3] % cacheLines][element].v);
						simulation.requests[i].push({action: "update-line", value: [definition.coreRequests[i][0], definition.coreRequests[i][3] % cacheLines, lineToString(state.cores[definition.coreRequests[i][0]][definition.coreRequests[i][3] % cacheLines])] });
					} else {
						moved = false;
						for(j = 0; j < nCores; j++) {
							if(j == definition.coreRequests[i][0]) {
								continue;
							}
							elementAux = findInLine(state.cores[j][definition.coreRequests[i][3] % cacheLines], definition.coreRequests[i][3]);
							if(elementAux < 0) {
								continue;
							}
							if(state.cores[j][definition.coreRequests[i][3] % cacheLines][elementAux].s == "S") {
								continue;
							}
							moved = true;
							simulation.requests[i].push({action: "focus", value: j});
							simulation.requests[i].push({action: "notify", value: [j, "Observed BusRead: flush..."]});
							simulation.requests[i].push({action: "update-ram", value: [state.cores[j][definition.coreRequests[i][3] % cacheLines][elementAux].a, state.cores[j][definition.coreRequests[i][3] % cacheLines][elementAux].v] });
							state.mem[state.cores[j][definition.coreRequests[i][3] % cacheLines][elementAux].a] = state.cores[j][definition.coreRequests[i][3] % cacheLines][elementAux].v;
							simulation.requests[i].push({action: "update-line", value: [j, definition.coreRequests[i][3] % cacheLines, lineToString(state.cores[j][definition.coreRequests[i][3] % cacheLines])] });
							state.cores[j][definition.coreRequests[i][3] % cacheLines][elementAux].s = "S";
						}
						if(moved == true) {
							simulation.requests[i].push({action: "focus", value: definition.coreRequests[i][0]});
						}
						if(state.cores[definition.coreRequests[i][0]][definition.coreRequests[i][3] % cacheLines].length >= definition.cacheBlocks) {
							elementAux2 = state.cores[definition.coreRequests[i][0]][definition.coreRequests[i][3] % cacheLines].shift();
							if(elementAux2.s == "M") {
								simulation.requests[i].push({action: "update-ram", value: [elementAux2.a, elementAux2.v] });
								state.mem[elementAux2.a] = elementAux2.v;
							}
						}
						state.cores[definition.coreRequests[i][0]][definition.coreRequests[i][3] % cacheLines] = updateInLine(state.cores[definition.coreRequests[i][0]][definition.coreRequests[i][3] % cacheLines], element, state.mem[definition.coreRequests[i][3]], definition.coreRequests[i][3], "S", 0);
						simulation.requests[i].push({action: "update-line", value: [definition.coreRequests[i][0], definition.coreRequests[i][3] % cacheLines, lineToString(state.cores[definition.coreRequests[i][0]][definition.coreRequests[i][3] % cacheLines])] });
					}
				}
			}
		break;
		case "mesi":
			for (i = 0; i < definition.coreRequests.length; i++) {
				const processor = definition.coreRequests[i][0];
				const operation = definition.coreRequests[i][1];
				const writeValue = definition.coreRequests[i][2];
				const address = definition.coreRequests[i][3];
				const cacheLine = address % cacheLines;
				const element = findInLine(state.cores[processor][cacheLine], address);

				simulation.requests.push([ {action: "focus", value: processor} ]);
				if (operation == "write") {
					simulation.requests[i].push({action: "notify", value: [processor, "PrWrite"]});					

					if (element == -1 || state.cores[processor][cacheLine][element].s == 'S') {
						// current state is Invalid or Shared

						simulation.requests[i].push({action: "write-bus", value: "BusRdX (" + processor + ")"});

						moved = false;
						for (j = 0; j < nCores; j++) if (j != processor) {
							const elementAux = findInLine(state.cores[j][cacheLine], address);
							if (elementAux < 0) {
								continue;
							}
							
							const foreignLine = state.cores[j][cacheLine];
							const foreignState = foreignLine[elementAux].s;
							
							moved = true;
							simulation.requests[i].push({action: "focus", value: j});							
							simulation.requests[i].push({action: "notify", value: [j, "Observed BusRdX" + (foreignState == "M" ? ": flush..." : "!")]});
							
							if (foreignState == "M") {
								simulation.requests[i].push({action: "update-ram", value: [address, foreignLine[elementAux].v] });
								state.mem[address] = foreignLine[elementAux].v;
							}
							
							state.cores[j][cacheLine] = invalidateInLine(foreignLine, elementAux);
							simulation.requests[i].push({action: "update-line", value: [j, cacheLine, lineToString(state.cores[j][cacheLine])] });
							
							if (foreignState == "M" || foreignState == "E") {
								break;
							}
						}

						if (moved == true) {
							simulation.requests[i].push({action: "focus", value: processor});
						}

						if (element == -1 && state.cores[processor][cacheLine].length >= definition.cacheBlocks) {
							elementAux2 = state.cores[processor][cacheLine].shift();
							if (elementAux2.s == "M") {
								simulation.requests[i].push({action: "update-ram", value: [elementAux2.a, elementAux2.v] });
								state.mem[elementAux2.a] = elementAux2.v;
							}
						}

						state.cores[processor][cacheLine] = updateInLine(state.cores[processor][cacheLine], element, writeValue, address, "M", 0);
						simulation.requests[i].push({action: "update-line", value: [processor, cacheLine, lineToString(state.cores[processor][cacheLine])] });
					} else {
						// current state is Exclusive or Modified
						state.cores[processor][cacheLine] = updateInLine(state.cores[processor][cacheLine], element, writeValue);
						simulation.requests[i].push({action: "update-line", value: [processor, cacheLine, lineToString(state.cores[processor][cacheLine])] });
					}
				} else { // operation == "read"
					simulation.requests[i].push({action: "notify", value: [processor, "PrRead"]});

					if (element == -1) {
						// current state is Invalid

						simulation.requests[i].push({action: "write-bus", value: "BusRd (" + processor + ")"});

						var shared = false; // whether another processor has this line
						moved = false;
						for (j = 0; j < nCores; j++) if (j != processor) {
							const elementAux = findInLine(state.cores[j][cacheLine], address);
							if (elementAux < 0) {
								continue;
							}

							shared = true;

							const foreignLine = state.cores[j][cacheLine];
							const foreignState = foreignLine[elementAux].s;

							moved = true;
						
							if(foreignState == "S") {
								continue; // Soh pra nao mudar o foco sem necessidade na interface.
							}

							simulation.requests[i].push({action: "focus", value: j});							
							simulation.requests[i].push({action: "notify", value: [j, "Observed BusRd" + (foreignState == "M" ? ": flush..." : "!")]});
						
							if (foreignState == "M") {
								simulation.requests[i].push({action: "update-ram", value: [address, foreignLine[elementAux].v] });
								state.mem[address] = foreignLine[elementAux].v;
								state.cores[j][cacheLine][elementAux].s = "S";

								simulation.requests[i].push({action: "update-line", value: [j, cacheLine, lineToString(state.cores[j][cacheLine])] });
							
								break;
							} else if (foreignState == "E") {
								state.cores[j][cacheLine][elementAux].s = "S";

								simulation.requests[i].push({action: "update-line", value: [j, cacheLine, lineToString(state.cores[j][cacheLine])] });
							
								break;
							}
						}

						if (moved == true) {
							simulation.requests[i].push({action: "focus", value: processor});
						}

						if (state.cores[processor][cacheLine].length >= definition.cacheBlocks) {
							elementAux2 = state.cores[processor][cacheLine].shift();
							if (elementAux2.s == "M") {
								simulation.requests[i].push({action: "update-ram", value: [elementAux2.a, elementAux2.v] });
								state.mem[elementAux2.a] = elementAux2.v;
							}
						}

						state.cores[processor][cacheLine] = updateInLine(state.cores[processor][cacheLine], element, state.mem[address], address, shared ? "S" : "E", 0);
						simulation.requests[i].push({action: "update-line", value: [processor, cacheLine, lineToString(state.cores[processor][cacheLine])] });
					} else {
						// current state is Exclusive, Shared or Modified
						state.cores[processor][cacheLine] = updateInLine(state.cores[processor][cacheLine], element, state.cores[processor][cacheLine][element].v);
						simulation.requests[i].push({action: "update-line", value: [processor, cacheLine, lineToString(state.cores[processor][cacheLine])] });
					}
				}
			}
		break;
		default: // vi
			for(i = 0; i < definition.coreRequests.length; i++) {
				simulation.requests.push([ {action: "focus", value: definition.coreRequests[i][0]} ]);
				element = findInLine(state.cores[definition.coreRequests[i][0]][definition.coreRequests[i][3] % cacheLines], definition.coreRequests[i][3]);
				if(definition.coreRequests[i][1] == "write") {
					simulation.requests[i].push({action: "notify", value: [definition.coreRequests[i][0], "PrWrite"]});
					simulation.requests[i].push({action: "write-bus", value: "BusWrite (" + definition.coreRequests[i][0] + ")"});
					moved = false;
					for(j = 0; j < nCores; j++) {
						if(j == definition.coreRequests[i][0]) {
							continue;
						}
						elementAux = findInLine(state.cores[j][definition.coreRequests[i][3] % cacheLines], definition.coreRequests[i][3]);
						if(elementAux < 0) {
							continue;
						}
						moved = true;
						simulation.requests[i].push({action: "focus", value: j});
						simulation.requests[i].push({action: "notify", value: [j, "Observed BusWrite!"]});
						state.cores[j][definition.coreRequests[i][3] % cacheLines] = invalidateInLine(state.cores[j][definition.coreRequests[i][3] % cacheLines], elementAux);
						simulation.requests[i].push({action: "update-line", value: [j, definition.coreRequests[i][3] % cacheLines, lineToString(state.cores[j][definition.coreRequests[i][3] % cacheLines])] });
					}
					if(moved == true) {
						simulation.requests[i].push({action: "focus", value: definition.coreRequests[i][0]});
					}
					if(element >= 0) {
						state.cores[definition.coreRequests[i][0]][definition.coreRequests[i][3] % cacheLines] = updateInLine(state.cores[definition.coreRequests[i][0]][definition.coreRequests[i][3] % cacheLines], element, definition.coreRequests[i][2]);
						simulation.requests[i].push({action: "update-line", value: [definition.coreRequests[i][0], definition.coreRequests[i][3] % cacheLines, lineToString(state.cores[definition.coreRequests[i][0]][definition.coreRequests[i][3] % cacheLines])] });
						state.mem[definition.coreRequests[i][3]] = definition.coreRequests[i][2];
						simulation.requests[i].push({action: "update-ram", value: [definition.coreRequests[i][3], definition.coreRequests[i][2]] });
					} else {
						if(state.cores[definition.coreRequests[i][0]][definition.coreRequests[i][3] % cacheLines].length >= definition.cacheBlocks) {
							state.cores[definition.coreRequests[i][0]][definition.coreRequests[i][3] % cacheLines].shift();
						}
						state.cores[definition.coreRequests[i][0]][definition.coreRequests[i][3] % cacheLines] = updateInLine(state.cores[definition.coreRequests[i][0]][definition.coreRequests[i][3] % cacheLines], element, definition.coreRequests[i][2], definition.coreRequests[i][3], "V", 0);
						simulation.requests[i].push({action: "update-line", value: [definition.coreRequests[i][0], definition.coreRequests[i][3] % cacheLines, lineToString(state.cores[definition.coreRequests[i][0]][definition.coreRequests[i][3] % cacheLines])] });
						state.mem[definition.coreRequests[i][3]] = definition.coreRequests[i][2];
						simulation.requests[i].push({action: "update-ram", value: [definition.coreRequests[i][3], definition.coreRequests[i][2]] });
					}
				} else { // read
					simulation.requests[i].push({action: "notify", value: [definition.coreRequests[i][0], "PrRead"]});
					simulation.requests[i].push({action: "write-bus", value: "BusRead (" + definition.coreRequests[i][0] + ")"});
					if(element >= 0) {
						state.cores[definition.coreRequests[i][0]][definition.coreRequests[i][3] % cacheLines] = updateInLine(state.cores[definition.coreRequests[i][0]][definition.coreRequests[i][3] % cacheLines], element, state.cores[j][definition.coreRequests[i][3] % cacheLines][element].v);
						simulation.requests[i].push({action: "update-line", value: [definition.coreRequests[i][0], definition.coreRequests[i][3] % cacheLines, lineToString(state.cores[definition.coreRequests[i][0]][definition.coreRequests[i][3] % cacheLines])] });
					} else {
						if(state.cores[definition.coreRequests[i][0]][definition.coreRequests[i][3] % cacheLines].length >= definition.cacheBlocks) {
							state.cores[definition.coreRequests[i][0]][definition.coreRequests[i][3] % cacheLines].shift();
						}
						state.cores[definition.coreRequests[i][0]][definition.coreRequests[i][3] % cacheLines] = updateInLine(state.cores[definition.coreRequests[i][0]][definition.coreRequests[i][3] % cacheLines], element, state.mem[definition.coreRequests[i][3]], definition.coreRequests[i][3], "V", 0);
						simulation.requests[i].push({action: "update-line", value: [definition.coreRequests[i][0], definition.coreRequests[i][3] % cacheLines, lineToString(state.cores[definition.coreRequests[i][0]][definition.coreRequests[i][3] % cacheLines])] });
					}
				}
			}
	}

	for(i = 0; i < definition.coreRequests.length; i++) {
		definition.coreRequests[i][3] = (definition.coreRequests[i][3] * 4).toString();
	}

	document.getElementById("simulation").style.display = "block";
	return true;
}

function backToRequests() {
	document.getElementById("simulation").style.display = "none";
	$("#requestsDefinition").stop().fadeIn(1000);
}

function toggleSidebarFixed(fixed) {
	var element;

	element = document.getElementById("sidebar");
	if(fixed == true) {
		element.classList.add("fixed");
	} else {
		element.classList.remove("fixed");
	}
}

function toggleSpeed(newSpeed, max) {
	simulation.speed = parseInt(max, 10) - parseInt(newSpeed, 10) + 500;
}

function exportMachine() {
	var element;

	element = document.getElementById("downloadLink");
	element.href = "data:text/plain;charset=utf-8," + encodeURIComponent(JSON.stringify(definition));
	element.click();
}

function importMachine(file) {
	var i, fStream, stringJSON, element, elements;

	if(file.length != 1) {
		return;
	}
	if(file[0].size > 1073741824) { // 50 MiB
		alertDialog("O arquivo é muito comprido para ser processado! Tente algum menor que 1GiB.");
		return;
	}
	
	fStream = new FileReader();
	fStream.onload = function(e) {
		stringJSON = e.target.result;
		try {
			stringJSON = JSON.parse(stringJSON);
		} catch(exc) {
			alertDialog("Não é um arquivo JSON válido! Use o botão 'Exportar' na simulação.");
			return;
		}
		document.getElementById("memoryAlgorithm").value = stringJSON.memoryAlgorithm;
		document.getElementById("memorySize").value = stringJSON.memInitValues.length;
		document.getElementById("nCores").value = stringJSON.nCores;
		document.getElementById("cacheLines").value = stringJSON.cacheLines;
		document.getElementById("cacheBlocks").value = stringJSON.cacheBlocks;
		document.getElementById("cacheAlgorithm").value = stringJSON.cacheAlgorithm;
		if(submitHome() != true) {
			alertDialog("Não é um arquivo JSON válido! Use o botão 'Exportar' na simulação.");
			return;
		}
		elements = document.getElementById("memInitValues").getElementsByTagName("input");
		for(i = 0; i < stringJSON.memInitValues.length; i++) {
			elements[i].value = stringJSON.memInitValues[i];
		}
		if(submitMemory() != true) {
			alertDialog("Não é um arquivo JSON válido! Use o botão 'Exportar' na simulação.");
			return;
		}
		for(i = 0; i < stringJSON.coreRequests.length; i++) {
			element = addRequestItem();
			elements = element.getElementsByTagName("input");
			elements[0].value = stringJSON.coreRequests[i][0];
			elements[1].value = stringJSON.coreRequests[i][2];
			elements[2].value = stringJSON.coreRequests[i][3];
			element.getElementsByTagName("select")[0].value = stringJSON.coreRequests[i][1];
		}
		if(submitRequests() != true) {
			alertDialog("Não é um arquivo JSON válido! Use o botão 'Exportar' na simulação.");
			return;
		}
		document.getElementById("fileInput").value = "";
	}
	fStream.readAsText(file[0]);
}

function resetMachine() {
	var elements;

	if(simulation.running !== null) {
		return;
	}
	
	elements = document.getElementById("coresCPU").getElementsByClassName("coreCache");
	for(i = 0; i < elements.length; i++) {
		elements2 = elements[i].getElementsByTagName("tbody")[0].childNodes;
		for(j = 0; j < elements2.length; j++) {
			elements2[j].childNodes[1].innerHTML = "";
		}
	}

	elements = document.getElementById("mainMemoryWords").childNodes;
	for(i = 0; i < simulation.initialState.length; i++) {
		elements[i].childNodes[1].innerHTML = simulation.initialState[i];
	}

	elements = document.getElementById("requests").getElementsByClassName("request");
	for(i = 0; i < elements.length; i++) {
		elements[i].classList.remove("done");
	}

	simulation.currentPosition = -1;
}

/*function debug() {
	document.getElementById("memoryAlgorithm").value = "msi";
	document.getElementById("memorySize").value = "5";
	document.getElementById("nCores").value = "40";
	document.getElementById("cacheLines").value = "2";
	document.getElementById("cacheBlocks").value = "1";
	document.getElementById("cacheAlgorithm").value = "fifo";
	submitHome();
	autoFillMem(3);
	submitMemory();
	newElement = document.getElementById("coreRequests");
	newElement.innerHTML = "<td><input value='9'></td><td><select><option value='read' selected='selected'>action</option></select><input value='512'></td><td><input value='0'></td><td></td>";
	newElement.innerHTML += "<td><input value='20'></td><td><select><option value='write' selected='selected'>action</option></select><input value='512'></td><td><input value='4'></td><td></td>";
	newElement.innerHTML += "<td><input value='27'></td><td><select><option value='read' selected='selected'>action</option></select><input value='512'></td><td><input value='4'></td><td></td>";
	newElement.innerHTML += "<td><input value='9'></td><td><select><option value='write' selected='selected'>action</option></select><input value='1024'></td><td><input value='4'></td><td></td>";
	newElement.innerHTML += "<td><input value='15'></td><td><select><option value='read' selected='selected'>action</option></select><input value='1024'></td><td><input value='4'></td><td></td>";
	newElement.innerHTML += "<td><input value='10'></td><td><select><option value='read' selected='selected'>action</option></select><input value='512'></td><td><input value='0'></td><td></td>";
	newElement.innerHTML += "<td><input value='10'></td><td><select><option value='read' selected='selected'>action</option></select><input value='512'></td><td><input value='4'></td><td></td>";
	submitRequests();
}*/

/* Funções para Simulação */

function simFocus(at) {
	var element, elements;

	element = document.getElementById("coresCPU");
	elements = element.getElementsByClassName("core");

	$(element).stop().animate({scrollLeft: elements[at].offsetLeft - element.offsetWidth/2 + 125}, 500);
}

function simNotify(at, text) {
	var element, elements;

	element = document.getElementById("coresCPU").getElementsByClassName("core")[at].getElementsByClassName("notification")[0];

	element.innerHTML = text;
	element.style.display = "block";
	$(element).stop();
	element.style.opacity = 1;
	$(element).fadeOut(simulation.speed, "easeInExpo");
}

function simWriteBus(text) {
	var element, elements;

	element = document.getElementById("memoryBus");

	element.innerHTML = text;
	if(text == "") {
		element.classList.remove("animated");
	} else {
		element.classList.add("animated");
	}
}

function simSetLine(at, line, text) {
	var elements;

	element = document.getElementsByClassName("coreCache")[at];
	elements = element.getElementsByTagName("tbody")[0].childNodes[line].childNodes[1];
	elements.innerHTML = text;
	elements.style.backgroundColor = "rgba(255, 255, 0, 0.5)";
	element.scrollTop = elements.offsetTop - 36;
	$(elements).stop().animate({backgroundColor: "transparent"}, simulation.speed, "easeOutExpo");
}

function simSetMemory(at, value) {
	var elements;

	element = document.getElementById("mainMemoryWords");
	elements = element.childNodes[at].childNodes[1];
	elements.innerHTML = value;
	elements.style.backgroundColor = "rgba(255, 255, 0, 0.5)";
	document.getElementById("mainMemory").scrollTop = elements.offsetTop - 36;
	$(elements).stop().animate({backgroundColor: "transparent"}, simulation.speed, "easeOutExpo");
}

function sim(req, n) {
	var simType, element;

	simulation.running = true;

	if(n >= simulation.requests[req].length) {
		simWriteBus("");
		element = document.getElementById("requests").getElementsByClassName("request")[req];
		element.classList.remove("running");
		element.classList.add("done");
		simulation.running = null;
		return;
	}

	simType = simulation.requests[req][n];
	switch(simType.action) {
		case "focus":
			simFocus(simType.value);
			if(simulation.speed > 0) {
				simulation.running = setTimeout(function() { sim(req, n + 1); }, 500);
			} else {
				sim(req, n + 1);
			}
			return;
		break;
		case "notify":
			simNotify(simType.value[0], simType.value[1])
			sim(req, n + 1);
			return;
		break;
		case "write-bus":
			simWriteBus(simType.value);
		break;
		case "update-line":
			simSetLine(simType.value[0], simType.value[1], simType.value[2]);
		break;
		case "update-ram": // update-ram
			simSetMemory(simType.value[0], simType.value[1]);
		break;
		default: // do-nothing
			;
	}

	if(simulation.speed > 0) {
		simulation.running = setTimeout(function() { sim(req, n + 1); }, simulation.speed);
	} else {
		sim(req, n + 1);
	}
}

function simClick(request, element) {
	var i, speed;

	if(simulation.running !== null) {
		return;
	}
	if(request <= simulation.currentPosition) {
		return;
	}

	speed = simulation.speed;
	simulation.speed = 0;
	elements = document.getElementById("requests").getElementsByClassName("request");
	if(simulation.currentPosition < 0) {
		i = 0;
	} else {
		i = simulation.currentPosition;
	}
	for(; i < request; i++) {
		elements[i].classList.add("done");
		sim(i, 0);
	}
	
	simulation.speed = speed;
	simulation.currentPosition = request;
	elements[request].classList.add("running");
	sim(request, 0);
}

function showWindow() {
	$("#splashScreen").stop().fadeOut(1000);
	$("#homeDefinition").stop().fadeIn(1000);
}

function windowLoad() {
	var elements;

	$("#scrollLeft").mouseenter(function(){
		var element;
		if(simulation.running === null) {
			element = document.getElementById("coresCPU");
			$(element).stop().animate({scrollLeft: 0}, element.scrollLeft*3, "linear");
		}
	}).mouseleave(function(){
		if(simulation.running === null) {
			$(document.getElementById("coresCPU")).stop();
		}
	});

	$("#scrollRight").mouseenter(function(){
		var element;
		if(simulation.running === null) {
			element = document.getElementById("coresCPU");
			$(element).stop().animate({scrollLeft: element.scrollWidth}, (element.scrollWidth - element.scrollLeft)*3, "linear");
		}
	}).mouseleave(function(){
		if(simulation.running === null) {
			$(document.getElementById("coresCPU")).stop();
		}
	});

	elements = document.getElementById("sidebar").getElementsByClassName("settings")[0].getElementsByTagName("input");
	toggleSidebarFixed(elements[0].checked);
	toggleSpeed(elements[1].value, elements[1].max)

	setTimeout(showWindow, 1500);
	console.log("Scripts iniciados com sucesso.");
}


