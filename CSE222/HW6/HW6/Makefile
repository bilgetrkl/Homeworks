JC = javac
JFLAGS = -g
CLASSES = Entry.class GTUHashMap.class GTUHashSet.class GTUList.class SpellChecker.class

all: $(CLASSES)

%.class: %.java
	$(JC) $(JFLAGS) $<

clean:
	rm -f *.class

run:
	java SpellChecker

.PHONY: all clean run