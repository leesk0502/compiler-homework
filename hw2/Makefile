CXX=cc

lexsy:	lexsy.l
		flex lexsy.l
		$(CXX) -o javaHighlighter lex.yy.c -lfl

.PHONY:	clean

clean:
	rm -f javaHighlighter *.c *.h
