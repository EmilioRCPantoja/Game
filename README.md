
# Fruit Ninja

Este projeto é um jogo inspirado em **Fruit Ninja**, desenvolvido em **C++ com SFML**.  
O jogador corta frutas usando o mouse enquanto evita bombas. O jogo possui pontuação, vidas, high score salvo em arquivo e um efeito fluido de slicing.

---

## 🎮 Funcionalidades Principais

- ✂️ Sistema de corte com rastro do mouse (slicing)
- 🍎 Frutas lançadas em trajetórias curvas (Bezier)
- 💣 Bombas que explodem e encerram o jogo
- ❤️ Sistema de vidas exibido na tela
- 🏆 High Score salvo automaticamente (`highscore.dat`)
- 🎨 Interface com botões via textura e HUD customizado

---

## 🧱 Arquitetura (POO)

O projeto utiliza os pilares da Programação Orientada a Objetos:

- **Abstração** — `Entity` define a interface base de todos os objetos
- **Herança**
  ```
  Entity
   └── AimEntity
         ├── FruitEntity
         └── BombEntity
  ```
- **Polimorfismo** — entidades manipuladas via `unique_ptr<AimEntity>`
- **Encapsulamento** — score, vidas e high score gerenciados por `GameState`

Outras classes importantes:
- `SliceEntity`: cuida do rastro do corte
- `Game`: controla o loop principal, atualizações e renderização

---

## 🧮 Matemática Usada

### Trajetória das frutas (Curva de Bézier)
```
P(t) = (1-t)²P0 + 2(1-t)tP1 + t²P2
```

### Detecção de corte (colisão circular)
```
(x - x0)² + (y - y0)² <= r²
```

### Queda após ser cortada
```
y(t) = y0 + v·t
```

---

# 🚀 Como Rodar o Jogo

Funciona tanto no **Linux** quanto no **Windows**.

---

# 🟢 Linux

### 1. Instale dependências:
sudo apt install g++ libsfml-dev

### 2. Dê permissão ao script:
chmod +x run.sh

### 3. Execute:
./run.sh

Isso irá compilar e rodar o jogo automaticamente.

---

# 🟦 Windows (MinGW)

### 1. Instale MinGW-w64

### 2. Baixe a SFML:
https://www.sfml-dev.org/download.php

### 3. Compile:
g++ -std=c++17 ^
    src/main.cpp ^
    src/Game.cpp ^
    src/GameState.cpp ^
    src/Entity/AimEntity.cpp ^
    src/Entity/FruitEntity.cpp ^
    src/Entity/BombEntity.cpp ^
    src/Entity/SliceEntity.cpp ^
    -IC:/SFML/include ^
    -LC:/SFML/lib ^
    -lsfml-graphics -lsfml-window -lsfml-system ^
    -O2 ^
    -o game.exe

### 4. Copie as DLLs da SFML para a pasta do jogo:
sfml-graphics-2.dll  
sfml-window-2.dll  
sfml-system-2.dll

### 5. Execute:
game.exe

---

## 📂 Estrutura do Projeto

```
include/
 ├── Game.hpp
 ├── GameState.hpp
 ├── Entity/
src/
 ├── Game.cpp
 ├── GameState.cpp
 ├── main.cpp
 ├── Entity/
assets/
 ├── fonts/
 ├── texture/
run.sh
README.md
highscore.dat
```

---

## 📝 Créditos

Desenvolvido por:

- **Emilio Pantoja** — [GitHub](https://github.com/EmilioRCPantoja)
- **Kauã Tavares** — [GitHub](https://github.com/Kauadt)
  
