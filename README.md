<div align="center">
   <img width="550px" src="img\readme_head(1).png" />
</div>

<div align="center">
<h2> Problema #3 - Desenvolvimento de um Jogo</h2>
</div>

> Documentação do projeto da disciplina TEC 499 - Sistemas Digitais, de desenvolvimento em linguagem C de um jogo utilizando o Processador Gráfico presente no kit de Desenvolvimento DE1-SoC.

## Índice

- [Introdução](#introdução)
- [Jogo Desenvolvido e seus Requisitos](#jogo-desenvolvido-e-seus-requisitos)
- [Recursos Utilizados](#recursos-utilizados)
    - [Kit de Desenvolvimento](#kit-de-desenvolvimento)
    - [Processador Gráfico](#processador-gráfico)
    - [Periféricos Utilizados](#periféricos-utilizados)
    - [Uso do Mouse](#uso-do-mouse)
    - [Software e Ferramentas](#software-e-ferramentas)
- [Threads e Sincronização](#threads-e-sincronização)
- [Etapas da Implementação do Jogo](#etapas-da-implementação-do-jogo)
    - [Conexão com os periféricos (botões, display e mouse)](#conexão-com-os-periféricos-botões-display-e-mouse)
    - [Implementação das Threads](#implementação-das-threads)
    - [Lógica das Mudanças de Telas](#lógica-dos-mudancas-de-telas)
    - [Funções para movimentação das Sprites](#funções-para-movimentação-das-sprites)
    - [Lógica de Colisão](#lógica-de-colisão)
    - [Lógica do Jogo e Pontuação](#lógica-do-jogo-e-pontuação)
    - [Novas Sprites e Visualização do Jogo](#novas-sprites-e-visualização-do-jogo)
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

<div align="center">
   <img width="" src="img\perifericos-placa.jpeg" />
    <p> Figura x. Quais periféricos do kit de desenvolvimento foram utilizados.</p>
</div>

Para o desenvolvimento do jogo na plataforma DE1-SoC, diferentes periféricos e componentes da placa foram utilizados para proporcionar uma experiência interativa e cumprir os requisitos estabelecidos. A seguir, é descrito como cada um desses periféricos foi integrado e utilizado no projeto:

#### Entrada VGA
A entrada VGA da placa DE1-SoC foi utilizada para a conexão com um monitor externo, permitindo a exibição dos elementos gráficos do jogo. A GPU implementada na FPGA envia sinais de vídeo para o monitor através dessa interface, possibilitando a visualização de gráficos em resolução de 640x480 pixels.

<div align="center">
   <img width="" src="img\vga.jpeg" />
    <p> Figura x. Uso do periférico de entrada VGA.</p>
</div>

#### Entrada USB
A entrada USB foi empregada para conectar um mouse à placa, que serve como dispositivo de controle do ator principal do jogo. A interação do jogador com o mouse é capturada e processada pelo HPS (Hard Processor System), que então comunica as ações do jogador à GPU para atualizar a posição e os movimentos do personagem principal na tela.

<div align="center">
   <img width="" src="img\mouse.jpeg" />
    <p> Figura x. Uso do periférico de entrada USB para o mouse.</p>
</div>

#### Botões
Os botões disponíveis na placa DE1-SoC foram configurados para funções específicas no jogo, como troca de telas, pausa e reinício. Esses botões oferecem uma forma simples e direta de controle adicional, permitindo ao jogador pausar o jogo, reiniciar o jogo ou navegar entre diferentes telas.

<div align="center">
   <img width="" src="img\buttons.jpeg" />
    <p> Figura x. Uso dos botões.</p>
</div>

#### Display de 7 Segmentos
O display de 7 segmentos na placa foi utilizado para exibir informações críticas do jogo, como a quantidade de vidas e sapos restantes. Esse display proporciona uma forma clara e imediata de feedback visual ao jogador, ajudando a manter o controle do progresso e do estado do jogo.

<div align="center">
   <img width="" src="img\display.jpeg" />
    <p> Figura x. Uso do display de 7 segmentos.</p>
</div>

- ### Uso do Mouse

Com relação ao uso do mouse, foi utilizadas as funçãos da biblioteca do Linux `input.h`, para abrir e fechar o arquivo do driver do mouse e realizar a leitura dos movimentos.

- ### Software e Ferramentas

Além dos recursos já descritos acima, foram utilizadas algumas plataformas e linguagem para auxílio do desenvolvimento (que serão descritos melhor posteriormente). Para o desenho e redimensionamento, tanto das telas como das novas sprites foi utilizado o site [Pixilart](https://www.pixilart.com/). Ademais, os códigos para conversão dos pixel das imagens PNG em uma matriz RGB foi implementados em [linguagem Python](https://www.python.org/).

## Threads e Sincronização

A utilização de threads foi uma parte crucial no desenvolvimento do jogo, permitindo a execução de múltiplos códigos em paralelo de forma sincronizada.

#### O que é uma Thread?
Uma thread é a menor unidade de processamento que pode ser realizada em um sistema operacional. Diferente de um processo, que possui seu próprio espaço de memória, uma thread compartilha o mesmo espaço de memória do processo ao qual pertence, permitindo uma comunicação rápida e eficiente entre threads dentro do mesmo processo.

<div align="center">
   <img width="" src="img\threads.png" />
    <p> Figura x. Diagrama funcionamento de Threads (Fonte: https://medium.com).</p>
</div>

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

A conexão com os botões e o display é realizada por meio do mapeamento da memória da placa, técnica essencial para a integração eficaz entre hardware e software em sistemas computacionais. Inicialmente, o arquivo especial `/dev/mem` é aberto com permissões de leitura e escrita (`O_RDWR`) e sincronização (`O_SYNC`). Isso permite ao programa acessar e modificar diretamente a memória física do sistema. Em seguida, a função `mmap()` é utilizada para mapear uma região da memória física para o espaço de endereçamento do processo.

Com o mapeamento concluído e o endereço base obtido, denominado `LW_virtual`, são configurados ponteiros para acessar áreas específicas da memória. Os deslocamentos necessários para acessar os periféricos estão detalhados em um documento auxiliar sobre a arquitetura do processador gráfico. Por exemplo, deslocamentos como `HEX0_BASE (0x10)` e `HEX1_BASE (0x20)` permitem acessar diferentes registradores ou áreas de memória do display. Esse procedimento é utilizado tanto para a leitura dos botões quanto para o controle do display.

Após o mapeamento, a função `munmap()` é usada para desmapear a memória, liberando o espaço de endereços, e o descritor de arquivo para `/dev/mem` é fechado.

<div align="center">
   <img width="" src="img\Mapeamento.png" />
    <p> Figura x. Sequência de etapas para iniciar e finalizar o mapeamento de memória.</p>
</div>

Embora o mesmo método possa ser empregado para o acesso ao mouse, optou-se por utilizar a biblioteca `input.h`, que abstrai a complexidade do mapeamento direto da memória, oferecendo uma interface mais simplificada e eliminando a necessidade de processamento manual das informações obtidas.

<div align="center">
   <img width="" src="img\Mapeamento.png" />
    <p> Figura x. Diagrama funcionamento de Threads.</p>
</div>

### Implementação das Threads

Para gerenciar os diversos dispositivos de entrada e saída, foi implementada uma abordagem baseada em múltiplas threads, permitindo a execução simultânea de tarefas críticas sem bloqueio e otimizando o desempenho do sistema. A seguir, detalhamos a função de cada thread no jogo.

#### Thread para Gerenciamento dos Botões
A thread key_thread inicia sua operação mapeando a memória do sistema para o endereço base `LW_BRIDGE_BASE`, cobrindo a área definida por `LW_BRIDGE_SPAN`. Isso permite ao programa ler e escrever diretamente nos registradores de controle dos botões. Um ponteiro, `KEY_ptr`, é configurado para acessar a área de memória dos botões, usando o deslocamento definido por `KEY_BASE`. Um valor de captura de borda (edge_capture) é inicializado para detectar mudanças no estado dos botões.

No loop contínuo da thread, o estado dos botões é lido e comparado com o estado anterior. Se uma mudança for detectada, edge_capture é atualizado. A função `change_state()` é chamada a cada iteração para alterar o estado do jogo com base nas entradas dos botões e no estado atual. Dependendo do estado do jogo (START, GAME, PAUSE, GAMEOVER, VICTORY), o estado é atualizado conforme os botões pressionados e o valor de edge_capture.

Ao final de sua execução, a thread desmapeia a memória com `munmap()` e fecha o descritor de arquivo associado a `/dev/mem com` `close()`, garantindo que os recursos sejam liberados adequadamente. A thread termina sua execução com `pthread_exit()`.

<div align="center">
   <img width=" " src="img\Fluxograma_thread_button.png" />
    <p> Figura x. Fluxograma da thread que gerencia os botões.</p>
</div>

#### Thread para Gerenciamento do Display
Após a configuração inicial do mapeamento de memória, a thread display_thread configura ponteiros para acessar as áreas de memória do display, associadas aos segmentos de um display de sete segmentos, como HEX0_BASE, HEX1_BASE, entre outros.

<div align="center">
   <img width=" " src="img\Fluxograma_thread_display.png" />
    <p> Figura x. Fluxograma da thread que gerencia o display.</p>
</div>

No loop principal, a thread atualiza o display com base no estado atual do jogo, representado pela variável global state_game. Dependendo do estado:

- **Estado START:** Todos os segmentos do display são configurados para um padrão específico, indicando a tela de introdução ou a configuração inicial do jogo. O padrão 0b1111111 representa todos os segmentos apagados.

- **Estado GAME:** A thread alterna entre dois padrões de exibição a cada dois segundos. Se o valor de i é par, o display mostra padrões binários que representam o número de sapos (frogs). Se i é ímpar, exibe um padrão diferente que reflete o número de vidas (life), usando a função `num_to_bin()` para converter esses valores em binário.

- **Estado PAUSE:** Todos os segmentos do display são desligados, indicando que o jogo está em pausa.

Se o estado do jogo não corresponder a nenhum dos casos definidos, a thread continua sem alterar o display.

#### Thread para Gerenciamento do Mouse
A thread mouse_thread começa abrindo o dispositivo de entrada do mouse localizado em `/dev/input/event0` com permissões de leitura (`O_RDONLY`). Se a abertura falhar, uma mensagem de erro é exibida e a thread encerra sua execução. Após uma abertura bem-sucedida, a thread utiliza uma estrutura `input_event` para armazenar eventos do mouse lidos do dispositivo.

O cursor é configurado com parâmetros iniciais, como posição (coord_x, coord_y), velocidade de movimento (step_x, step_y), e outras características. No loop principal, a função `read()` é usada para ler eventos do mouse. A thread verifica se a leitura foi bem-sucedida e se o número de bytes lidos corresponde ao esperado. Se houver erro, uma mensagem de erro é exibida e a thread encerra.

Quando o jogo está no estado GAME, a thread ativa o cursor (`cursor.ativo = 1`) e processa eventos do mouse para atualizar a posição do cursor. Eventos do tipo `EV_REL` e `EV_KEY` ajustam a coordenada horizontal (coord_x) e vertical (coord_y) conforme o movimento do mouse e a interação com os botões do mouse. Quando o jogo não está em GAME, o cursor é desativado (`cursor.ativo = 0`).

<div align="center">
   <img width=" " src="img\Fluxograma_thread_mouse.png" />
    <p> Figura x. Fluxograma da thread que gerencia o mouse.</p>
</div>

#### Thread para Gerenciamento da Visualização dos Sprites
A thread visul_thread é responsável pela atualização contínua dos sprites no jogo. Ela configura variáveis que definem os limites horizontais da tela e um contador para o tempo de exibição dos sprites.

No loop infinito, a thread verifica o estado do jogo e decide se deve atualizar a tela ou aguardar. Quando o jogo está ativo, a função percorre todos os sprites (como carros e troncos), atualizando suas posições com base na velocidade e direção. A função também verifica se os sprites precisam ser reposicionados ou reiniciados se saírem dos limites da tela. Após atualizar a posição dos sprites, a tela é atualizada para refletir as mudanças.

É seguida a seguinte lógica para cada grupo de sprites.

#### Configuração Inicial das Sprites
No início, cada sprite é configurado com suas coordenadas iniciais, direção, velocidade e outros parâmetros. As pistas e as vias aquáticas são posicionadas verticalmente em intervalos de 40 unidades na coordenada y.

#### Carros na Primeira Pista
Os carros na primeira pista movem-se da direita para a esquerda. Cada carro é inicializado com sua posição x no final da tela e a posição y correspondente à primeira pista. A direção é definida como 0 (esquerda), e a velocidade é configurada para 2 unidades por ciclo de atualização.

#### Carros na Segunda Pista
Os carros na segunda pista movem-se da esquerda para a direita. Suas posições iniciais x estão no início da tela. A direção é definida como 1 (direita), e a velocidade é configurada para 4 unidades por ciclo de atualização.

#### Caminhões na Terceira Pista
Os caminhões na terceira pista também se movem da direita para a esquerda. Eles são inicializados com a mesma lógica dos carros da primeira pista, mas possuem uma velocidade diferente, definida como 3 unidades por ciclo de atualização. Cada caminhão é composto por duas partes (frente e traseira), que são posicionadas uma próxima à outra.

#### Carros na Quarta Pista
Os carros na quarta pista movem-se da esquerda para a direita. Suas posições iniciais x estão no início da tela, com a velocidade configurada para 1 unidade por ciclo de atualização.

#### Condições de Movimentação das Sprites
Dentro do loop principal, as posições das sprites são atualizadas continuamente, verificando se elas chegaram ao fim da tela para reposicioná-las no início, criando um efeito de loop infinito.

#### Movimentação das Sprites no Rio
A lógica de movimentação das sprites no rio é similar à dos carros, mas com diferentes velocidades e direções.

#### Verificação do Estado do Jogo
Dentro do loop principal da thread da movimentação, há uma condicional que verifica se o estado do jogo é GAME, se for ele ativa as Sprites com o bit 1, senão elas são desativadas com o bit de ativação igual a 0.

Para evitar uso excessivo de CPU, a função faz uma breve pausa no final de cada iteração do loop.

#### Thread para Gerenciamento das Colisões
A thread collision_thread é responsável pela detecção e gerenciamento de colisões entre o jogador e outros elementos do jogo. Ela verifica o estado geral do jogo e a posição do jogador. Quando o jogo está ativo, a thread entra em um loop infinito para percorrer todos os sprites ativos, como carros e troncos, e verificar possíveis colisões com o jogador.

A verificação de colisão é feita comparando as posições e tamanhos dos sprites para determinar se há sobreposição. Se uma colisão é detectada, o estado do jogo é atualizado para refletir a colisão, podendo incluir a reinicialização do jogador ou a mudança de seu status. A função também pode atualizar a pontuação ou registrar informações adicionais conforme a lógica do jogo. Após verificar todas as colisões possíveis, a função faz uma breve pausa antes de repetir o processo.

### Lógica das Mudanças de Telas

Para gerenciar a troca de telas, foi implementada uma máquina de estados que controla as diferentes fases do jogo: **START** (início do jogo), **GAME** (fase principal), **PAUSE** (pausa), **GAMEOVER** (fim do jogo) e **VICTORY** (vitória). A transição entre esses estados é controlada com base nas entradas dos botões.

A função `change_state()` atualiza o estado do jogo analisando o estado atual (`state_game`) e as entradas dos botões. Por exemplo, se o jogo está no estado **START** e o primeiro botão é pressionado, o estado é alterado para **GAME**. Em estados como **PAUSE**, diferentes botões podem levar o jogo a retornar ao estado START ou voltar ao estado **GAME**. A função também atualiza a variável previous_state para rastrear o estado anterior, o que ajuda a comparar e tomar decisões baseadas nas mudanças de estado.

No loop principal do jogo, o programa verifica continuamente o estado atual e realiza ações específicas para cada um. No estado **START**, a tela inicial é configurada. No estado **GAME**, a tela é atualizada, o movimento do cursor é gerenciado e colisões são verificadas. No estado **PAUSE**, a tela de pausa é exibida. Quando o jogo chega ao estado **GAMEOVER**, a tela de fim de jogo é mostrada e as variáveis são redefinidas. No estado **VICTORY**, a tela de vitória é exibida e as variáveis são resetadas.

A lógica inclui funções para limpar e inicializar elementos da tela, como sprites e fundo, garantindo que a exibição seja adequada ao estado atual e não contenha informações desatualizadas.

Em resumo, a troca de telas é gerenciada por uma máquina de estados que responde às entradas do usuário, atualizando o estado do jogo e a tela de acordo com o progresso e as ações do jogador.

### Funções para movimentação das Sprites

A função `increase_coordinate_sprite()` é projetada para atualizar a posição horizontal (**coordenada x**) de um sprite com base em sua velocidade e direção.
Ela recebe dois parâmetros: um ponteiro para a estrutura do sprite, que contém informações sobre a posição (**coord_x**), a velocidade (**speed**), o deslocamento por movimento (**step_x**) e a direção (**direction**), e um contador (**counter**) que ajuda a determinar quando o sprite deve se mover.

O funcionamento da função é o seguinte: primeiro, verifica se o contador é múltiplo da velocidade do sprite, usando a expressão `counter % sprite->speed == 0`p . Se essa condição for verdadeira, significa que é hora de atualizar a posição do sprite. Dependendo da direção do movimento, a coordenada x do sprite é ajustada. Se a direção é 1 (indicando movimento para a direita), a coordenada x é incrementada pelo valor de step_x, movendo o sprite para a direita. Se a direção é 0 (indicando movimento para a esquerda), a coordenada x é decrementada pelo valor de step_x, movendo o sprite para a esquerda.

<div align="center">
   <img width=" " src="img\mov1.png" />
    <p> Figura x. Exemplo de movimentação de uma sprite que tem velocidade 1.</p>
</div>

<div align="center">
   <img width=" " src="img\mov2.png" />
    <p> Figura x. Exemplo de movimentação de uma sprite que tem velocidade 2.</p>
</div>

Após atualizar a coordenada x, a função retorna 0, sinalizando que a operação de movimentação foi concluída com sucesso. Em resumo, a função garante que o sprite se mova de forma controlada e fluida, conforme definido pela sua velocidade e direção.

<div align="center">
   <img width=" " src="img\fluxograma_coordinate.png" />
    <p> Figura x. Fluxograma da função que realiza o deslocamento das sprites.</p>
</div>

### Lógica de Colisão

A verificação de colisão entre dois sprites é realizada pela função `collision()`, que determina se dois retângulos se sobrepõem. Cada sprite é representado por um retângulo cujas coordenadas iniciais são armazenadas em coord_x e coord_y. A função compara a posição desses retângulos para verificar a interseção.

Para verificar se dois retângulos estão colidindo, a função utiliza condições baseadas na posição e tamanho dos retângulos. Primeiro, verifica se a borda esquerda do primeiro retângulo (sprite1) está à esquerda da borda direita do segundo retângulo (sprite2). Em seguida, checa se a borda direita do primeiro retângulo está à direita da borda esquerda do segundo retângulo. Similarmente, a função verifica a sobreposição vertical, comparando a borda superior do primeiro retângulo com a borda inferior do segundo e a borda inferior do primeiro com a borda superior do segundo.

Se todas essas condições forem verdadeiras, significa que os retângulos se sobrepõem e houve uma colisão. Nesse caso, a função define o atributo collision de ambos os sprites como 1, indicando a colisão. Caso contrário, o atributo collision é definido como 0, indicando a ausência de colisão.

<div align="center">
   <img width=" " src="img\fluxograma_collision.png" />
    <p> Figura x. Fluxograma da função que verifica colisão entre sprites.</p>
</div>

### Lógica do Jogo e Pontuação

O jogo é inicialmente configurado com a definição dos sprites e o estado do jogo é estabelecido como **START**. A inicialização também inclui a criação de várias threads para gerenciar entradas de teclado, mouse, visualização, colisão e exibição.

No loop principal, o estado atual é monitorado e atualizado. No estado **START**, a tela inicial é configurada e limpa se houver uma transição de estado. No estado **GAME**, a tela do jogo é exibida, e o movimento do cursor é gerenciado com base na posição vertical. O `counter_river` é incrementado para controlar o movimento do cursor e ajustar sua posição. Se o cursor atinge a parte inferior da tela, a quantidade de sapos (`frogs`) é reduzida e o cursor é reposicionado. Se a quantidade de sapos chega a zero, o jogo muda para o estado **VICTORY**.

Durante o jogo, a detecção de colisões é crucial. Se o cursor colide com outros sprites e ainda há vidas restantes, a vida do jogador é reduzida e o cursor é reposicionado. Quando não há mais vidas, o jogo muda para o estado **GAMEOVER**, onde a tela de fim de jogo é exibida e as variáveis de vida e sapos são redefinidas.

No estado **PAUSE**, a tela de pausa é exibida se houver uma transição de estado, e o jogo permanece pausado até que o jogador retome ou saia do estado de pausa. Quando o jogo entra no estado **GAMEOVER**, a tela de fim de jogo é exibida e o jogo é resetado com vidas e sapos redefinidos. Da mesma forma, no estado **VICTORY**, a tela de vitória é mostrada e as variáveis são resetadas.

A pontuação do jogo é refletida na contagem de vidas e sapos. O jogador começa com um número fixo de vidas e sapos, e a perda de vidas ocorre quando o cursor colide com obstáculos. A diminuição dos sapos e a eventual vitória ou derrota do jogador são geridas por mudanças de estado no jogo e pela atualização da tela conforme o progresso ou falhas do jogador.

### Novas Sprites e Visualização do Jogo

Em seguimento, com o objetivo de trazer uma visualização adequada ao tema e objetivos do jogo, foram implememntadas novas Sprites na Memória de Sprites presentes na GPU. Essa memória do processador gráfico possui um endereçamento por pixel, onde cada Sprite possui 400 pixels e é possível armazenar 32 Sprites ao todo. Para formar uma nova, é preciso enviar à GPU o local do pixel na memória e o seu valor em RGB, mudando a cor do respectivo pixel.

O jogo, Croak Rush, possui 11 novas sprites implementadas que são utiizadas, que são as apresentadas na figura a baixo.

<div align="center">
   <img width="300px" src="img\game-sprites.jpeg" />
    <p> Figura x. Novas Sprites implementadas para o jogo.</p>
</div>

Foram seguidas algumas etapas para elaboração das novas Sprites. Foi necessário realizar a conversão de imagem para uma matriz com os valores RGB de cada pixel. Para realizar essa conversão, foi utilizado um código em Python com a biblioteca Pillow (Python Imaging Library, ou PIL), que facilita a manipulação e o processamento de imagens. 

No caso de um sprite, a imagem é processada pixel a pixel para criar uma matriz que representa seus valores de cor. O procedimento é o seguinte:

1) **Desenho do Pixel Art 20x20:** Utilizando o software para desenho por pixel foram desenhadas as novas sprites.

2) **Leitura da Imagem:** A imagem do sprite é carregada utilizando a biblioteca Pillow. Cada pixel da imagem é lido e convertido para valores RGB.

3) **Normalização dos Valores:** Cada valor RGB (que varia de 0 a 255) é dividido por 32 para obter um valor entre 0 e 8. Isso reduz a quantidade de dados a serem manipulados e se adapta ao espaço de memória disponível na GPU da FPGA.

4) **Criação da Matriz:** Uma matriz de dimensões 400x3 é criada, onde cada linha da matriz representa os valores normalizados RGB de um pixel da imagem.

5) **Armazenamento na Memória de Sprites:** Os valores RGB normalizados são salvos na memória de sprites, correspondendo a cada um dos 400 pixels que compõem o sprite. Um loop percorre cada pixel da imagem para realizar essa conversão e armazenamento. 

Esse processo é ilustrado no diagrama presente na Figura x.

<div align="center">
   <img width="500px" src="img\diagram-sprites.jpeg" />
    <p> Figura x. Processo de conversão da Sprite para Matriz RGB 400x3.</p>
</div>

Agora, com relação às telas do jogo, foi seguido uma lógica semelhantes à das Sprites. Porém, além da matriz possuir um tamanho maior, sendo 4800x3, o valor das cores em RGB não eram por pixel, e sim, por blocos de background.

O jogo possui as seguintes telas:

<div align="center">
   <img width="500px" src="img\screens.jpeg" />
    <p> Figura x. Telas do Jogo.</p>
</div>

Para a conversão da tela inteira utilizando o conceito de background block, o processo é similar ao das Sprites, mas adaptado para uma imagem maior e possui uma processo diferente na fase de desenho da imagem:

1) **Redimensionamento e pixelização de uma Imagem:** Paranão haver a necessidade de realizar um desenho do zero, porém, como precisaria ser uma imagem 80x60 pixels (quantidade total de blocos de fundo) que é considerada muito pequena, foi feito o redimensionamento de uma imagem já pronta para o tamanho adequado. Isso acarretou em uma pequena perda de nitidez da imagem, porém facilitou o processo de desenho.

2) **Leitura da Imagem:** A imagem de fundo é carregada usando a biblioteca Pillow, e cada pixel é convertido para valores RGB.

3) **Normalização dos Valores:** Assim como no caso do sprite, cada valor RGB é dividido por 32 para obter valores entre 0 e 8.

4) **Criação da Matriz:** Uma matriz de dimensões 4800x3 é montada, representando os valores normalizados de cada pixel da imagem de fundo.

5) **Armazenamento em Blocos de Background:** Os valores RGB normalizados de cada pixel correspondem a blocos de background que são armazenados nos endereços correspondentes na memória de cada um.

Esse processo é ilustrado no diagrama presente na Figura x.

<div align="center">
   <img width="500px" src="img\diagram-screens.jpeg" />
    <p> Figura x. Processo de conversão de uma Imagem em uma Matriz RGB 4800x3.</p>
</div>

Esse processo de conversão é fundamental para preparar os dados gráficos para serem manipulados e exibidos pela GPU na FPGA, garantindo uma renderização eficiente e precisa dos elementos gráficos no jogo.
