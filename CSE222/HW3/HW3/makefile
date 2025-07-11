SRC = src
BUILD = build
JAVAFILES = sources.txt

collect:
	@find $(SRC) -name "*.java" > sources.txt

build:
	@mkdir -p $(BUILD)
	@javac -d $(BUILD) @sources.txt

run:
	@java -cp $(BUILD) main.Main $(ARGS)

javadoc:
	@javadoc -d doc -encoding UTF-8 -sourcepath src/main @sources.txt

clean:
	@rm -rf $(BUILD)