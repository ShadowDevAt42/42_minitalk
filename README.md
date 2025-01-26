# 📡 Minitalk

## Table of Contents / Table des Matières
- [English Version](#english-version)
- [Version Française](#version-française)

---

# English Version

## 📝 Description
Minitalk is a client-server communication program using only UNIX signals SIGUSR1 and SIGUSR2. This project implements an efficient and reliable bidirectional communication system between two processes.

## 🛠 Features

### Standard Version
- Client-server communication via UNIX signals
- Bit-by-bit message transmission
- Receipt confirmation for each bit
- Robust error handling
- Bidirectional synchronization

### Bonus Version
- Extended Unicode support
- Message receipt confirmation
- Enhanced user interface
- Multiple client handling
- Secure transmission

## 🚀 Installation

```bash
# Clone the repository
git clone [your-repo-url]
cd minitalk

# Compile standard version
make

# Compile bonus version
make bonus
```

## 📋 Available Make Commands

| Command | Description |
|----------|-------------|
| `make` | Compiles standard client and server programs |
| `make bonus` | Compiles bonus versions of client and server |
| `make clean` | Removes object files from standard version |
| `make clean_bonus` | Removes object files from bonus version |
| `make fclean` | Removes all generated files (standard version) |
| `make fclean_bonus` | Removes all generated files (bonus version) |
| `make re` | Recompiles standard version |
| `make bonus_re` | Recompiles bonus version |

## 💻 Usage

### 1. Start the Server
```bash
./server
```
The server will display its PID at startup. Keep this number for the client.

### 2. Send a Message (Client)
```bash
./client [SERVER_PID] "your message"
```

### Example
```bash
# Terminal 1 - Server
./server
> 4242  # Displayed PID

# Terminal 2 - Client
./client 4242 "Hello, World!"
```

## 🔧 Technical Architecture

### Communication Protocol
1. **Initialization**
   - Server starts and displays its PID
   - Client uses this PID to establish communication

2. **Transmission**
   - SIGUSR1: represents bit 0
   - SIGUSR2: represents bit 1
   - Each character is transmitted over 8 bits

3. **Confirmation**
   - Server confirms each received bit
   - Final confirmation at message end

## ⚠️ Important Notes
- Server must be started before client
- PID provided to client must be valid
- Messages are transmitted character by character
- Transmission is synchronized and reliable

## 🔍 Debugging
If issues occur:
1. Verify server is running
2. Confirm PID is correct
3. Ensure no other server instances are running
4. Check file execution permissions

## 🎯 Project Objectives
- Understand inter-process communication
- Master UNIX signal handling
- Implement reliable communication protocol
- Handle bit-by-bit data transmission
- Ensure process synchronization

## 🔒 Security
- User input validation
- Secure signal handling
- Interrupt protection
- Robust error handling
- Proper resource cleanup

## 📚 Technical Documentation
Source code is thoroughly documented with explanatory comments for:
- Communication protocol
- Signal handling
- Data structures
- Synchronization mechanisms
- Implementation choices

---

# Version Française

## 📝 Description
Minitalk est un programme de communication client-serveur utilisant uniquement les signaux UNIX SIGUSR1 et SIGUSR2. Ce projet implémente un système de communication bidirectionnelle efficace et fiable entre deux processus.

## 🛠 Fonctionnalités

### Version Standard
- Communication client-serveur via signaux UNIX
- Transmission bit par bit des messages
- Confirmation de réception pour chaque bit
- Gestion robuste des erreurs
- Synchronisation bidirectionnelle

### Version Bonus
- Support Unicode étendu
- Confirmation de réception des messages
- Interface utilisateur améliorée
- Gestion de multiples clients
- Transmission sécurisée

## 🚀 Installation

```bash
# Cloner le repository
git clone [votre-repo-url]
cd minitalk

# Compiler la version standard
make

# Compiler la version bonus
make bonus
```

## 📋 Commandes Make Disponibles

| Commande | Description |
|----------|-------------|
| `make` | Compile les programmes client et serveur standards |
| `make bonus` | Compile les versions bonus du client et serveur |
| `make clean` | Supprime les fichiers objets de la version standard |
| `make clean_bonus` | Supprime les fichiers objets de la version bonus |
| `make fclean` | Supprime tous les fichiers générés (version standard) |
| `make fclean_bonus` | Supprime tous les fichiers générés (version bonus) |
| `make re` | Recompile la version standard |
| `make bonus_re` | Recompile la version bonus |

## 💻 Utilisation

### 1. Démarrer le Serveur
```bash
./server
```
Le serveur affichera son PID au démarrage. Gardez ce numéro pour le client.

### 2. Envoyer un Message (Client)
```bash
./client [PID_SERVEUR] "votre message"
```

### Exemple
```bash
# Terminal 1 - Serveur
./server
> 4242  # PID affiché

# Terminal 2 - Client
./client 4242 "Hello, World!"
```

## 🔧 Architecture Technique

### Protocole de Communication
1. **Initialisation**
   - Le serveur démarre et affiche son PID
   - Le client utilise ce PID pour établir la communication

2. **Transmission**
   - SIGUSR1 : représente le bit 0
   - SIGUSR2 : représente le bit 1
   - Chaque caractère est transmis sur 8 bits

3. **Confirmation**
   - Le serveur confirme chaque bit reçu
   - Confirmation finale à la fin du message

## ⚠️ Notes Importantes
- Le serveur doit être lancé avant le client
- Le PID fourni au client doit être valide
- Les messages sont transmis caractère par caractère
- La transmission est synchronisée et fiable

## 🔍 Débogage
En cas de problème :
1. Vérifier que le serveur est bien lancé
2. Confirmer que le PID utilisé est correct
3. S'assurer qu'il n'y a pas d'autres instances du serveur en cours
4. Vérifier les permissions d'exécution des fichiers

## 🎯 Objectifs du Projet
- Comprendre la communication inter-processus
- Maîtriser la gestion des signaux UNIX
- Implémenter un protocole de communication fiable
- Gérer la transmission de données bit par bit
- Assurer la synchronisation entre processus

## 🔒 Sécurité
- Validation des entrées utilisateur
- Gestion sécurisée des signaux
- Protection contre les interruptions
- Gestion robuste des erreurs
- Nettoyage approprié des ressources

## 📚 Documentation Technique
Le code source est documenté en détail avec des commentaires explicatifs pour :
- Le protocole de communication
- La gestion des signaux
- Les structures de données
- Les mécanismes de synchronisation
- Les choix d'implémentation
