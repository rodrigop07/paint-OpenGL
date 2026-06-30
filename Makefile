# Configurações do Compilador e Flags básicas
CXX = g++
CXXFLAGS = -O3 -Wall -std=c++17
TARGET = app

# Listagem de arquivos de código
SRCS = src/main.cpp src/Eventos.cpp src/Globais.cpp src/Transformacoes.cpp
OBJS = $(SRCS:.cpp=.o)

# Define as flags de renderização e janelas baseado no OS atual
ifeq ($(OS), Windows_NT)
    # Windows (Usando MinGW / MSYS2)
    LIBS = -lfreeglut -lglu32 -lopengl32 -lgdi32
    RM = del /Q /F
    CLEAN_OBJS = $(subst /,\,$(OBJS))
    RUN_CMD = $(TARGET)
    EXT = .exe
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S), Linux)
        # Linux (Ubuntu, Debian, Fedora, etc.)
        LIBS = -lGLEW -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lglut -lGLU
        RM = rm -rf
        CLEAN_OBJS = $(OBJS)
        RUN_CMD = ./$(TARGET)
        EXT = 
    endif
    ifeq ($(UNAME_S), Darwin)
        # macOS (Usando Homebrew para glfw/glew)
        # Ajuste os caminhos de Include/Lib se necessário
        CXXFLAGS += -I/opt/homebrew/include
        LIBS = -L/opt/homebrew/lib -lGLEW -lglfw -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
        RM = rm -rf
        CLEAN_OBJS = $(OBJS)
        RUN_CMD = ./$(TARGET)
    endif
endif

# Regras de Compilação
all: $(TARGET)

# Linkagem do executável final
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

# Compilação dos arquivos .cpp em .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpeza dos arquivos gerados
clean:
	$(RM) $(CLEAN_OBJS) $(TARGET)$(EXT)

# executa o programa
run:
	$(RUN_CMD)

.PHONY: all clean run
