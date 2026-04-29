# SYSR - Lab 04 Ricart-Agrawala 83
# Requires Java 11+ and Swing (included in standard JDK)

JAVAC   = javac
JAVA    = java
SRC_DIR = src
OUT_DIR = out

SOURCES = $(SRC_DIR)/Message.java \
          $(SRC_DIR)/GUI.java \
          $(SRC_DIR)/Processus.java \
          $(SRC_DIR)/Main.java

compile:
	mkdir -p $(OUT_DIR)
	$(JAVAC) -d $(OUT_DIR) $(SOURCES)

run: compile
	$(JAVA) -cp $(OUT_DIR) Main

clean:
	rm -rf $(OUT_DIR) logs/*.log

.PHONY: compile run clean
