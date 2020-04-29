# dungeon_worldbot
Um bot de Telegram em C++ para o jogo de RPG Dungeon World, feito especialmente para um jogo que estou jogando.

Este bot utiliza a biblioteca libtelegram, e a biblioteca boost. Foi feito baseado no exemplo_echo.
A maneira mais simples de compilar é simplesmente substituir o exemplo_echo disponibilizado em libtelegram, pelo arquivo fornecido e rodar:
  make TARGET=example_echo

Apesar da função principal conter a implementação específica para o jogo que estava jogando, espero que o arquivo dungeon.hpp seja genérico suficiente e contenha uma pequena "biblioteca" de funções para implementação direta de outros jogos do gênero, não sendo muito complicada de ser entendida e utilizada. Ainda em progresso
