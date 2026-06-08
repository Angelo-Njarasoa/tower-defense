# Tower Defense

Jeu de tower defense en C++17 avec la bibliothèque SFML 2.5.1.  
Les ennemis suivent un chemin en S, les tours les attaquent automatiquement dans leur portée.

---

## Prérequis communs

- [Git](https://git-scm.com/)
- [CMake](https://cmake.org/) 3.16 ou supérieur
- Compilateur C++17 (g++ ou MinGW)
- SFML 2.5.1

---

## Option 1 — WSL (recommandée)

WSL (Windows Subsystem for Linux) est intégré à Windows 10/11 et offre l'environnement le plus simple pour compiler ce projet.

### 1. Installer WSL

Ouvrir PowerShell en administrateur et taper :

```powershell
wsl --install
```

Redémarrer le PC quand demandé. WSL installe Ubuntu par défaut.

### 2. Installer les dépendances dans WSL

Ouvrir un terminal Ubuntu (WSL) et taper :

```bash
sudo apt update
sudo apt install git cmake g++ libsfml-dev
```

### 3. Cloner et compiler le projet

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

### Relancer rapidement après modification

```bash
cmake --build build && ./build/TowerDefense
```

---

## Option 2 — Windows natif (sans WSL)

Cette option utilise MSYS2 qui fournit un environnement MinGW et les packages SFML directement sous Windows.

### 1. Installer MSYS2

Télécharger et installer MSYS2 depuis [msys2.org](https://www.msys2.org/).

### 2. Installer les dépendances

Ouvrir le terminal **MSYS2 MinGW 64-bit** (pas MSYS2 MSYS) et taper :

```bash
pacman -Syu
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-sfml
```

### 3. Cloner et compiler le projet

Dans le terminal **MSYS2 MinGW 64-bit** :

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

## Structure du projet

```
tower-defense/
├── assets/          # Sprites PNG (Kenney Tower Defense Top-Down)
├── include/
│   └── model/       # Fichiers d'en-tête (.hpp)
├── src/
│   ├── main.cpp     # Point d'entrée et boucle de jeu
│   └── model/       # Implémentation des classes (.cpp)
├── docs/            # Documentation visuelle
└── CMakeLists.txt   # Configuration de compilation
```

---

## Branches

| Branche | Contenu |
|---|---|
| `feature/entity` | Classes Enemy, Tower, Wave, Map |
| `feature/controller` | Contrôleurs de jeu |
| `feature-Map` | Système de carte |
| `feature-Game` | Logique de jeu principale |
