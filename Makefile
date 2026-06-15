# 1. Configurações do Compilador e Flags básicas
CXX = g++
CXXFLAGS = -O3 -Wall -std=c++17
TARGET = main

# 2. Listagem de arquivos de código
SRCS = src/main.cpp src/eventos.cpp
OBJS = $(SRCS:.cpp=.o)

# 3. DETECÇÃO AUTOMÁTICA DE SISTEMA OPERACIONAL
# Define as flags de renderização e janelas baseado no OS atual
ifeq ($(OS), Windows_NT)
    # Windows (Usando MinGW / MSYS2)
    LIBS = -lfreeglut -lglu32 -lopengl32 -lgdi32
    RM = del /Q /F
    TARGET := $(TARGET).exe
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S), Linux)
        # Linux (Ubuntu, Debian, Fedora, etc.)
        LIBS = -lGLEW -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lglut -lGLU
        RM = rm -rf
    endif
    ifeq ($(UNAME_S), Darwin)
        # macOS (Usando Homebrew para glfw/glew)
        # Ajuste os caminhos de Include/Lib se necessário
        CXXFLAGS += -I/opt/homebrew/include
        LIBS = -L/opt/homebrew/lib -lGLEW -lglfw -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
        RM = rm -rf
    endif
endif

# 4. Regras de Compilação
all: $(TARGET)

# Linkagem do executável final
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

# Compilação dos arquivos .cpp em .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpeza dos arquivos gerados
clean:
	$(RM) $(OBJS) $(TARGET)

.PHONY: all clean
