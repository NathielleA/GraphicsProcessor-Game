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
    - [Processador do Kit de Desenvolvimento]()
        - [Componentes Internos da FPGA]()
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