GCC = "C:/Program Files/CodeBlocks/MinGW/bin/gcc.exe"
FLAGS = -Wall -Wextra
SRC = src
BIN = bin
LOGS = logs

ORTAK = $(SRC)/can_frame.c $(SRC)/can_bus.c $(SRC)/can_node.c \
        $(SRC)/ecu_motor.c $(SRC)/ecu_abs.c $(SRC)/ecu_dashboard.c \
        $(SRC)/ecu_airbag.c $(SRC)/ecu_klima.c $(SRC)/ecu_direksiyon.c \
        $(SRC)/ecu_kabin.c $(SRC)/ecu_lastik.c

# varsayilan hedef: derle
all: sim test

sim:
	$(GCC) $(FLAGS) -o $(BIN)/can_sim.exe \
		$(SRC)/main.c $(ORTAK) \
		$(SRC)/can_json.c

test:
	$(GCC) $(FLAGS) -o $(BIN)/can_test.exe \
		$(SRC)/main.c $(ORTAK) \
		$(SRC)/can_logger.c $(SRC)/can_parser.c \
		$(SRC)/can_fuzzer.c $(SRC)/can_detector.c

# derle + calistir
run: sim
	start $(BIN)/can_sim.exe
	cd dashboard && npm run dev

clean:
	del /Q $(BIN)\*.exe 2>nul
	del /Q $(LOGS)\*.log 2>nul
