<div align="center">
   <img width="550px" src="img\readme_head(1).png" />
</div>

<div align="center">
<h2> Problema #3 - Desenvolvimento de um Jogo</h2>
</div>

> Documentação do projeto da disciplina TEC 499 - Sistemas Digitais, de desenvolvimento em linguagem C de um jogo utilizando o Processador Gráfico presente no kit de Desenvolvimento DE1-SoC.

## Índice

- [Introdução](#introdução)
- [Jogo Desenvolvido e seus Requisitos]()
- [Recursos Utilizados](#)
    - [Kit de Desenvolvimento]()
    - [Processador Gráfico]()
    - [Periféricos Utilizados]()
    - [Uso do Mouse]()
    - [Software e Ferramentas]()
- [Threads e Sincronização]()
- [Etapas da Implementação do Jogo]()
    - [Conexão com os periféricos (botões, display e mouse)]()
    - [Implementação das Threads]()
    - [Lógica das Mudanças de Telas]()
    - [Funções para movimentação das Sprites]()
    - [Lógica de Colisão]()
    - [Lógica do Jogo e Pontuação]()
    - [Novas Sprites e Visualização do Jogo]()
- [Regras do Jogo e Jogabilidade]()
- [Testes, Resultados e Desafios Encontrados]()
- [Conclusão]()

## Introdução

Este projeto foi desenvolvido para atender aos requisitos do Problema 3 da disciplina TEC499 - Módulo Integrador de Sistemas Digitais. O objetivo principal é desenvolver um jogo utilizando o Módulo Kernel e a Biblioteca de Funções já implementados anteriormente, que foram feitos com o propósito de possibilitar a comunicação entre o processador de propósito geral da placa DE1-SoC e o processador gráfico embarcado na FPGA da mesma placa, e a biblioteca feita para facilitar o acesso geral aos recursos do sistema e o envio de instruções para a GPU. O jogo foi construído utilizando como base esses dois componentes para conexção do hardware da placa com o software da lógica e visualização do jogo.

## Jogo Desenvolvido e seus Requisitos

O jogo digital desenvolvido leva o nome de Croak Rush e se baseia nos jogos: o online Frog Dash, o arcade Frogger e o mobile Crossy Road. Ele consiste em fazer uma família de sapos atravessar ruas movimentadas por veículos e rios com correntezas sem que nenhum deles seja atropelado.

Para que o produto final seguísse um padrão de desenvolvimento, alguns requisitos deveriam ser seguidos. O principal deles é, que o seu ator principal se movimente através de um mouse conectado à placa e outros atores se movimentando de forma independente no jogo. Nesse caso, o ator principal controlado pelo mouse é o sapo e os atores independentes são os carros e objetos em movimento. 

Ademais, alguns outros requisitos consistem em:

- As informações do vida e quantidade de sapos que atravessaram deve ser exibida no display de 7 segmentos;
- A movimentação do mouse deve refletir no ator principal tanto em direção como em velocidade;
- Ações do usuário, como pausar e reiniciar o jogo, devem ser feitas através dos botões da placa;

## Recursos Utilizados

- ### Kit de Desenvolvimento

O placa utilizada para o desenvolvimento do jogo foi a DE1-SoC (Figura 1). O sistema inclui um processador rígido (HPS) baseado em ARM, além de periféricos e interfaces de memória integradas ao FPGA. A placa também suporta uma distribuição Linux embarcada, proporcionando um grande ambiente para desenvolvimento.

O processador de propósito geral (HPS - Hard Processor System) presente na placa de desenvolvimento é um processador dual-core, permitindo a execução de múltiplas tarefas simultaneamente. Este HPS está integrado ao FPGA (Field Programmable Gate Array) Cyclone V, proporcionando uma combinação de processamento de software e hardware reconfigurável. A arquitetura do HPS inclui interfaces de memória como DDR3, interfaces de comunicação como Ethernet e USB, e periféricos adicionais como UART, SPI e I2C, que facilitam a conectividade e expansão do sistema.

A integração do HPS com o FPGA permite uma comunicação entre os dois, através de uma interconexão de barramento Lightweight HPS para FPGA. Isso possibilita que partes do processamento que demandam alta performance ou operações específicas sejam descarregadas para o FPGA, enquanto o HPS gerencia tarefas de controle, gerenciamento e operações complexas de software.

- ### Processador Gráfico

Neste trabalho, dentro da FPGA da placa DE1-SoC, está implementada uma Unidade de Processamento Gráfico (GPU) especialmente projetada para jogos bidimensionais. Essa GPU é baseada em sprites, que são elementos gráficos que podem ser movidos e controlados de forma independente na tela. A arquitetura da GPU foi desenvolvida para trabalhar com uma resolução de 640x480 pixels em um monitor VGA, permitindo a criação de jogos com gráficos claros e definidos.

A GPU implementada na FPGA utiliza uma estrutura de Pipeline para a renderização de gráficos, o que otimiza o desempenho ao dividir o processamento em etapas sequenciais. Isso é especialmente útil ao desenhar polígonos convexos, como quadrados e triângulos, que são fundamentais para muitos elementos gráficos em jogos. A modelagem e simulação dos módulos do Co-Processador gráfico foram fundamentais para validar essa abordagem, garantindo que os elementos gráficos possam ser manipulados de forma rápida e com precisão.

A integração da GPU com o processador de propósito geral HPS da placa foi crucial para o funcionamento harmonioso do sistema. A comunicação entre o HPS e a GPU foi implementada através de um módulo de kernel desenvolvido anteriormente, especificamente para esse propósito. Esse módulo atua como um driver de caractere, proporcionando funções de leitura e escrita nos barramentos Data A e Data B. Esses barramentos são responsáveis por receber informações da fila e transmiti-las para a GPU, onde as instruções são executadas. A construção desse módulo de kernel foi essencial para facilitar o acesso à GPU.

A utilização do sistema operacional Linux embarcado na placa, juntamente com a linguagem C, proporcionou o desenvolvimento não apenas do módulo de kernel, mas também de uma biblioteca com funções específicas para a escrita na GPU. Essa biblioteca foi projetada para facilitar o uso da GPU pelos programadores, abstraindo a complexidade das operações de baixo nível e permitindo uma interface mais amigável para o desenvolvimento de aplicações gráficas. Esse conjunto de ferramentas e bibliotecas simplificou significativamente o processo de desenvolvimento do jogo descrito neste documento, permitindo uma implementação mais rápida e eficiente dos elementos gráficos e das funcionalidades do jogo.

- ### Periféricos Utilizados

Para o desenvolvimento do jogo na plataforma DE1-SoC, diferentes periféricos e componentes da placa foram utilizados para proporcionar uma experiência interativa e cumprir os requisitos estabelecidos. A seguir, é descrito como cada um desses periféricos foi integrado e utilizado no projeto:

#### Entrada VGA
A entrada VGA da placa DE1-SoC foi utilizada para a conexão com um monitor externo, permitindo a exibição dos elementos gráficos do jogo. A GPU implementada na FPGA envia sinais de vídeo para o monitor através dessa interface, possibilitando a visualização de gráficos em resolução de 640x480 pixels.

IMAGEM DO PERIFÉRICO AQUI

#### Entrada USB
A entrada USB foi empregada para conectar um mouse à placa, que serve como dispositivo de controle do ator principal do jogo. A interação do jogador com o mouse é capturada e processada pelo HPS (Hard Processor System), que então comunica as ações do jogador à GPU para atualizar a posição e os movimentos do personagem principal na tela.

IMAGEM DO PERIFÉRICO AQUI

#### Botões
Os botões disponíveis na placa DE1-SoC foram configurados para funções específicas no jogo, como troca de telas, pausa e reinício. Esses botões oferecem uma forma simples e direta de controle adicional, permitindo ao jogador pausar o jogo, reiniciar o jogo ou navegar entre diferentes telas.

IMAGEM DO PERIFÉRICO AQUI

#### Display de 7 Segmentos
O display de 7 segmentos na placa foi utilizado para exibir informações críticas do jogo, como a quantidade de vidas e sapos restantes. Esse display proporciona uma forma clara e imediata de feedback visual ao jogador, ajudando a manter o controle do progresso e do estado do jogo.

IMAGEM DO PERIFÉRICO AQUI

- ### Uso do Mouse

Com relação ao uso do mouse, foi utilizadas as funçãos da biblioteca do Linux `input.h`, para abrir e fechar o arquivo do driver do mouse e realizar a leitura dos movimentos.

- ### Software e Ferramentas

Além dos recursos já descritos acima, foram utilizadas algumas plataformas e linguagem para auxílio do desenvolvimento (que serão descritos melhor posteriormente). Para o desenho e redimensionamento, tanto das telas como das novas sprites foi utilizado o site [Pixilart](https://www.pixilart.com/). Ademais, os códigos para conversão dos pixel das imagens PNG em uma matriz RGB foi implementados em [linguagem Python](https://www.python.org/).

## Threads e Sincronização

A utilização de threads foi uma parte crucial no desenvolvimento do jogo, permitindo a execução de múltiplos códigos em paralelo de forma sincronizada.

#### O que é uma Thread?
Uma thread é a menor unidade de processamento que pode ser realizada em um sistema operacional. Diferente de um processo, que possui seu próprio espaço de memória, uma thread compartilha o mesmo espaço de memória do processo ao qual pertence, permitindo uma comunicação rápida e eficiente entre threads dentro do mesmo processo.

#### Funcionamento de Threads
As threads permitem a execução de diferentes partes de um programa simultaneamente. Cada thread possui seu próprio conjunto de registros, pilha e contador de programa, mas compartilha recursos como memória e arquivos abertos com outras threads do mesmo processo. Isso facilita a execução concorrente de tarefas, como atualizações gráficas e controle de entrada do usuário no contexto de um jogo.

#### Benefícios de Usar Threads
- **Paralelismo**: Permite que diferentes tarefas sejam executadas simultaneamente, melhorando o desempenho e a eficiência do sistema.
- **Responsividade**: Melhora a responsividade do programa, especialmente em aplicações interativas como jogos, ao manter a interface do usuário ativa enquanto outras operações são realizadas em segundo plano.
- **Economia de Recursos**: Compartilhando o mesmo espaço de memória, as threads são mais leves e consomem menos recursos do que processos separados.

#### Malefícios de Usar Threads
- **Complexidade**: A programação com threads é mais complexa devido à necessidade de gerenciar a sincronização e comunicação entre elas.
- **Problemas de Sincronização**: Sem a devida sincronização, threads podem acessar recursos compartilhados de maneira conflitante, levando a problemas como race conditions.
- **Debugging Difícil**: Bugs em programas multithreaded são mais difíceis de reproduzir e corrigir devido à natureza concorrente das operações.

#### Problemas de Sincronização e Suas Soluções
Um dos principais desafios ao usar threads é garantir que elas acessem recursos compartilhados de maneira segura. Problemas de sincronização comuns incluem:

- **Race Conditions**: Ocorrências onde múltiplas threads tentam ler e escrever em um recurso compartilhado simultaneamente, causando resultados inesperados.
- **Deadlocks**: Situações onde duas ou mais threads ficam bloqueadas esperando que a outra libere um recurso, resultando em uma paralisação do programa.

##### Uso de Mutex para Sincronização
Para evitar problemas de sincronização, mecanismos como mutexes (mutual exclusions) são usados. Um mutex é uma variável que pode ser travada ou destravada por threads, garantindo que apenas uma thread acesse um recurso compartilhado por vez. Quando uma thread deseja acessar o recurso, ela tenta adquirir o mutex. Se o mutex já estiver em uso, a thread é bloqueada até que o mutex seja liberado.

#### Problemas de Data Race
Data race ocorre quando duas ou mais threads tentam acessar e modificar o mesmo registrador ou variável simultaneamente sem a devida sincronização. Isso pode levar a resultados incorretos e comportamento imprevisível do programa. Utilizando mutexes ou outras técnicas de sincronização, é possível evitar data races ao assegurar que apenas uma thread por vez modifique um dado específico.

#### Gerenciamento de Threads pelo Sistema Operacional
O sistema operacional (SO) é responsável por gerenciar a criação, execução e destruição de threads. Na DE1-SoC, que possui um processador dual-core ARM Cortex-A9, apenas duas threads podem ser executadas em paralelo. No entanto, o SO pode criar várias threads e utilizar técnicas de escalonamento (scheduling) para alternar entre elas, dando a impressão de que múltiplas threads estão sendo executadas simultaneamente.

O SO administra o tempo de CPU para cada thread, utilizando algoritmos como round-robin ou prioridade para decidir qual thread deve ser executada em um dado momento. Esse gerenciamento eficiente permite que todas as threads tenham a oportunidade de serem executadas, mesmo que a quantidade de threads seja maior que o número de núcleos disponíveis.

Em suma, a implementação de threads no desenvolvimento do jogo permitiu a execução paralela e sincronizada de diferentes partes do código, melhorando a eficiência e a responsividade da aplicação. No entanto, a utilização de threads também introduziu desafios de sincronização, que foram mitigados com o uso de mutexes e outras técnicas de controle de acesso a recursos compartilhados. O gerenciamento de threads pelo sistema operacional garantiu que todas as threads tivessem a oportunidade de serem executadas, mesmo com as limitações de hardware da DE1-SoC.

## Etapas da Implementação do Jogo

### Conexão com os periféricos (botões, display e mouse)

*precisa o mouse?

### Implementação das Threads
### Lógica das Mudanças de Telas
### Funções para movimentação das Sprites
### Lógica de Colisão
### Lógica do Jogo e Pontuação
### Novas Sprites e Visualização do Jogo

Em seguimento, com o objetivo de trazer uma visualização adequada ao tema e objetivos do jogo, foram implememntadas novas Sprites na Memória de Sprites presentes na GPU. Essa memória do processador gráfico possui um endereçamento por pixel, onde cada Sprite possui 400 pixels e é possível armazenar 32 Sprites ao todo. Para formar uma nova, é preciso enviar à GPU o local do pixel na memória e o seu valor em RGB, mudando a cor do respectivo pixel.

O jogo, Croak Rush, possui 11 novas sprites implementadas que são utiizadas, que são as apresentadas na figura a baixo.

<div align="center">
   <img width="330px" src="img\game-sprites.jpeg" />
    <p> Figura x. Novas Sprites implementadas para o jogo.</p>
</div>

Foram seguidas algumas etapas para elaboração das novas Sprites. Foi necessário realizar a conversão de imagem para uma matriz com os valores RGB de cada pixel. Para realizar essa conversão, foi utilizado um código em Python com a biblioteca Pillow (Python Imaging Library, ou PIL), que facilita a manipulação e o processamento de imagens. 

No caso de um sprite, a imagem é processada pixel a pixel para criar uma matriz que representa seus valores de cor. O procedimento é o seguinte:

1) **Desenho do Pixel Art 20x20:** Utilizando o software para desenho por pixel foram desenhadas as novas sprites.

2) **Leitura da Imagem:** A imagem do sprite é carregada utilizando a biblioteca Pillow. Cada pixel da imagem é lido e convertido para valores RGB.

3) **Normalização dos Valores:** Cada valor RGB (que varia de 0 a 255) é dividido por 32 para obter um valor entre 0 e 8. Isso reduz a quantidade de dados a serem manipulados e se adapta ao espaço de memória disponível na GPU da FPGA.

4) **Criação da Matriz:** Uma matriz de dimensões 400x3 é criada, onde cada linha da matriz representa os valores normalizados RGB de um pixel da imagem.

5) **Armazenamento na Memória de Sprites:** Os valores RGB normalizados são salvos na memória de sprites, correspondendo a cada um dos 400 pixels que compõem o sprite. Um loop percorre cada pixel da imagem para realizar essa conversão e armazenamento. 

Esso processo é ilustrado no diagrama presente na Figura x.

