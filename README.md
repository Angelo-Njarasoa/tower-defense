# Tower Defense

Jeu de tower defense en C++17 avec la bibliothèque SFML 2.5.1.  
Les ennemis suivent un chemin en S, les tours les attaquent automatiquement dans leur portée.  
Architecture MVC + Observer + Factory. Sons procéduraux, plein écran.

> **Le jeu se trouve sur la branche `main`.**  
> Les autres branches (`feature/*`) sont des branches de développement déjà fusionnées.

---

## Prérequis communs

- [Git](https://git-scm.com/)
- [CMake](https://cmake.org/) 3.16 ou supérieur
- Compilateur C++17 (g++ ou MinGW)
- SFML 2.5.1 (graphics, window, system, **audio**)

---

## Option 1 — WSL (recommandée)

WSL (Windows Subsystem for Linux) est intégré à Windows 10/11.  
Sur Windows 11, WSLg gère l'affichage et le son automatiquement.  
Sur Windows 10, l'affichage nécessite VcXsrv et le son peut ne pas fonctionner (le jeu reste jouable).

### 1. Installer WSL

Ouvrir PowerShell en administrateur :

```powershell
wsl --install
```

Redémarrer le PC quand demandé. WSL installe Ubuntu par défaut.

### 2. Installer les dépendances dans WSL

```bash
sudo apt update
sudo apt install git cmake g++ libsfml-dev
```

### 3. Cloner et compiler

```bash
git clone https://github.com/Angelo-Njarasoa/tower-defense.git
cd tower-defense
cmake -B build
cmake --build build
```

### 4. Lancer le jeu

```bash
./build/TowerDefense
```

### Recompiler rapidement après modification

```bash
cmake --build build && ./build/TowerDefense
```

---

## Option 2 — Windows natif (MSYS2)

### 1. Installer MSYS2

Télécharger et installer depuis [msys2.org](https://www.msys2.org/).

### 2. Installer les dépendances

Ouvrir le terminal **MSYS2 MinGW 64-bit** :

```bash
pacman -Syu
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-sfml
```

### 3. Cloner et compiler

```bash
git clone https://github.com/Angelo-Njarasoa/tower-defense.git
cd tower-defense
cmake -B build -G "MinGW Makefiles"
cmake --build build
```

### 4. Lancer le jeu

```bash
./build/TowerDefense.exe
```

---

## Contrôles

| Touche / Action | Effet |
|---|---|
| Clic gauche | Poser une tour |
| Clic droit | Améliorer une tour (×3 max) |
| `1` | Sélectionner Gatling ($50) |
| `2` | Sélectionner Cannon ($80) |
| `3` | Sélectionner Rocket ($120) |
| `Espace` | Démarrer la vague suivante immédiatement |
| `P` | Pause |
| `Échap` | Quitter |

---

## Structure du projet

```
tower-defense/
├── assets/               # Sprites PNG (Kenney Tower Defense Top-Down)
├── include/
│   ├── controller/       # GameController, WaveController
│   ├── model/            # Game, Map, Enemy, Tower, Wave, entity
│   ├── patterns/         # Observer, Factory
│   └── view/             # GameView, SoundManager
├── src/
│   ├── main.cpp          # Point d'entrée et boucle de jeu
│   ├── controller/
│   ├── model/
│   ├── patterns/
│   └── view/
├── docs/                 # Documentation visuelle
└── CMakeLists.txt        # Configuration de compilation
```

---

## Branches

| Branche | Contenu |
|---|---|
| `main` | **Version jouable complète** — MVC + son + plein écran |
| `feature/entity` | Classes Enemy, Tower, Wave, Map (fusionné) |
| `feature/pattern` | Observer + Factory (fusionné) |
| `feature/controller` | Contrôleurs de jeu (fusionné) |
| `feature-Map` | Système de carte (fusionné) |
| `feature-Game` | Logique de jeu principale (fusionné) |
