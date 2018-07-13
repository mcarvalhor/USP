# Inpainting de Objetos Largos
### Processamento de Imagens (SCC0251) - Projeto Final
Neste projeto, foi implementado um algoritmo avançado de Inpainting, capaz de remover objetos de grande dimensão e formato de uma imagem colorida, reconstruíndo-a. O domínio de espaço por trás deste objeto foi reconstruído através de outras regiões da imagem próximas ao objeto removido ou de semelhança com as extremidades deste, arranjando os blocos remendados de uma forma visualmente aceitável.

## Integrantes
* Gabriel Romualdo Silveira Pupo (Nº USP 9896250)
* Matheus Carvalho Raimundo (Nº USP 10369014)

## Organização do Repositório
O repositório foi organizado da seguinte maneira:
* [ImagesAndAttribution](ImagesAndAttribution): aqui se encontram as imagens que foram usadas como teste no projeto. O underscore (\_) determina diferentes variações de uma mesma imagem, como diferentes resoluções por exemplo.
* [FirstsResults](FirstsResults): aqui se encontram os resultados iniciais (parciais) obtidos nas primeiras versões do algoritmo. Para o relatório final do trabalho, desconsiderar tal diretório.
* [Results](Results): aqui se encontram os resultados obtidos nos casos de teste construídos.
* [TestCases](TestCases): casos de teste usados para executar o algoritmo.
* [InpaintingLargeObjects.py](InpaintingLargeObjects.py): Código em Python. Executado com Python 3; pode-se utilizar os casos de teste disponíveis em [_TestCases_](TestCases).
* [ProjectReport.ipynb](ProjectReport.ipynb): Relatório final do projeto.

## Relatório Final
O relatório final do projeto encontra-se em [ProjectReport](ProjectReport.ipynb).
