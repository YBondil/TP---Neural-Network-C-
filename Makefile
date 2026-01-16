# 1. Variables de configuration
CXX      = g++
CXXFLAGS = -Wall -Wextra -std=c++17
TARGET   = NN
SRC_DIR  = .
OBJ_DIR  = obj

# 2. Détection automatique des fichiers source et objet
# Trouve tous les fichiers .cpp dans le répertoire courant
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
# Génère les noms des fichiers .o dans le dossier obj/
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# 3. Règle principale (la première est celle exécutée par défaut)
all: $(TARGET)

# 4. Création de l'exécutable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# 5. Compilation des fichiers objets (.o) à partir des sources (.cpp)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 6. Création du dossier d'objets s'il n'existe pas
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# 7. Nettoyage des fichiers générés
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Indique que 'all' et 'clean' ne sont pas des fichiers
.PHONY: all clean
