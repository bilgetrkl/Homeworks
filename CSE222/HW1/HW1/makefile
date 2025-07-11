collect:
	find -name "*.java" > sources.txt

build:
	javac -d build @sources.txt

run:
	java -cp build main.Main

clean:
	rm -rf build sources.txt