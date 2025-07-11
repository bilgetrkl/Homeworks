SRC = src
BUILD = build
DOCS = docs

all: collect build

collect:
	@mkdir -p $(BUILD)
	@find $(SRC) -name "*.java" > sources.txt
	@echo "Collected all Java source files into sources.txt."

build: collect
	@javac -d $(BUILD) @sources.txt
	@echo "The build is complete. You can run the program using 'make run ARGS=\"<arguments>\"'."

run:
	@java -cp $(BUILD) src.Main.Main $(ARGS)

javadoc:
	@mkdir -p $(DOCS)
	@javadoc -d $(DOCS) @sources.txt
	@echo "Javadoc generated in the '$(DOCS)' directory."

clean:
	@rm -rf $(BUILD)
	@rm -f sources.txt
	@echo "Cleaned up build directory and source files."

re: clean all

.PHONY: all collect build run clean re javadoc