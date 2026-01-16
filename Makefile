# 1. Variables de configuration
CXX      = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -Iinclude
TARGET   = NN
SRC_DIR  = src
OBJ_DIR  = obj
INC_DIR  = include

# 2. Détection automatique des fichiers source et objet
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# 3. Règle principale
all: $(TARGET)

# 4. Création de l'exécutable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# 5. Compilation des fichiers objets
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 6. Création du dossier d'objets
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# 7. Nettoyage
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean
