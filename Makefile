# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -O2 -Wall `pkg-config --cflags opencv4`
LDFLAGS = `pkg-config --libs opencv4` -lpthread

# Kaynak ve hedef dosyalar
SRC = src/main.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = edge_detection

# Tüm hedef: derle ve oluştur
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Temizlik hedefi
clean:
	rm -f $(OBJ) $(TARGET)

# Çalıştırmak için: make run input=<input_image> threads=<num_threads>
run: $(TARGET)
	./$(TARGET) $(input) $(threads)
