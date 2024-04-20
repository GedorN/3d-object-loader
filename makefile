# Variáveis para os compiladores e flags
CXX = g++
# CXXFLAGS = -Wall -O2
LDFLAGS = -lGL -lglfw -lGLEW

# Nome do executável
TARGET = mesh

# Arquivos fonte
SOURCES = mesh.cpp src/lib/fileloader.cpp src/render_model.cpp src/view.cpp

# Regra padrão
all: $(TARGET)

# Regra de linkagem
$(TARGET): $(SOURCES)
	$(CXX) $(SOURCES) -o $@ $(LDFLAGS)

# Regra para limpar arquivos compilados
clean:
	rm -f $(TARGET)

# Regra para 'make run'
run: $(TARGET)
	./$(TARGET)
