# ==========================================
# 1. Variables de configuration
# ==========================================
CXX      = g++
TARGET   = Neural_net
CXXFLAGS = -Wall -Wextra -std=c++17 -Iinclude -O3 -march=native -ffast-math
SRC_DIR  = src
OBJ_DIR  = obj
INC_DIR  = include

# ==========================================
# 2. Détection des fichiers
# ==========================================
# Sources principales
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Sources des tests
TEST_SRC = tests/main_test.cpp tests/test_matrix.cpp tests/test_nn.cpp tests/test_csv.cpp
TEST_OBJ = $(TEST_SRC:.cpp=.o)

# Objets nécessaires pour les tests (tout le programme sauf le main.o principal)
# On filtre 'obj/main.o' de la liste complète des objets
OBJ_NO_MAIN = $(filter-out $(OBJ_DIR)/main.o, $(OBJS))

# ==========================================
# 3. Règles Principales
# ==========================================
all: $(TARGET)

# Lance les tests (alias standard)
test: tests_bin
	./run_tests

# ==========================================
# 4. Compilation de l'exécutable principal
# ==========================================
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# ==========================================
# 5. Compilation des Tests
# ==========================================
# Édition de liens pour l'exécutable de test
tests_bin: $(TEST_OBJ) $(OBJ_NO_MAIN)
	$(CXX) $(CXXFLAGS) -o run_tests $(TEST_OBJ) $(OBJ_NO_MAIN)

# Règle de compilation pour les fichiers sources des tests (tests/*.cpp -> tests/*.o)
tests/%.o: tests/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ==========================================
# 6. Règles génériques
# ==========================================
# Compilation des fichiers objets du projet (src/*.cpp -> obj/*.o)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Création du dossier d'objets si inexistant
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Nettoyage
clean:
	rm -rf $(OBJ_DIR) $(TARGET) run_tests tests/*.o

.PHONY: all clean test tests_bin