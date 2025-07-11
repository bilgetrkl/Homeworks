SOURCEFILES = sourcefiles.txt
BUILD_DIR = build

all: collect build

collect:
	@find . -name "*.java" > $(SOURCEFILES)
	@echo "Collected all Java source files into $(SOURCEFILES)."

build:
	@javac -d $(BUILD_DIR) @$(SOURCEFILES)
	@echo "The build is complete. You can run the program using 'make run'."

run:
	@java -cp $(BUILD_DIR) main.Main

clean:
	@rm -rf $(BUILD_DIR)
	@rm -f $(SOURCEFILES)
	@echo "Cleaned up build directory and source files."

re: clean all

javadoc: collect
	@javadoc -d doc -encoding UTF-8 -sourcepath main @$(SOURCEFILES)
	@echo "Javadoc generated in the 'docs' directory."

cleandoc:
	@rm -rf docs
	@echo "Cleaned up Javadoc directory."