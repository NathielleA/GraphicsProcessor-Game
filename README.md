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



- ### Software e Ferramentas

Além dos recursos já descritos acima, foram utilizadas algumas plataformas e linguagem para auxílio do desenvolvimento (que serão descritos melhor posteriormente). Para o desenho e redimensionamento, tanto das telas como das novas sprites foi utilizado o site [Pixilart](https://www.pixilart.com/). Ademais, os códigos para conversão dos pixel das imagens PNG em uma matriz RGB foi implementados em [linguagem Python](https://www.python.org/).

## Threads e Sincronização


