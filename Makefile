CC = gcc
CFLAGS = -Wall -Wextra -I./src -mconsole -g
FLEX = flex
BISON = bison

SRCS = src/ast.c src/codegen.c src/optimizer/optimizer.c
PARSER_SRCS = src/lexer.l src/parser.y
TEST_SRCS = src/optimizer_test.c src/visualizer_test.c

all: optimizer.exe visualizer.exe test.exe optimizer_test.exe visualizer_test.exe

# Generate parser files
src/parser.tab.c src/parser.tab.h: src/parser.y
	$(BISON) -d -o src/parser.tab.c src/parser.y

src/lex.yy.c: src/lexer.l src/parser.tab.h
	$(FLEX) -o src/lex.yy.c src/lexer.l

# Build optimizer
optimizer.exe: src/optimizer/main.c $(SRCS) src/lex.yy.c src/parser.tab.c
	$(CC) $(CFLAGS) -o $@ $^

# Build visualizer
visualizer.exe: src/visualizer/main.c src/visualizer/visualizer.c src/ast.c
	$(CC) $(CFLAGS) -o $@ $^

# Build tests
optimizer_test.exe: src/optimizer_test.c $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^

visualizer_test.exe: src/visualizer_test.c src/visualizer/visualizer.c src/ast.c
	$(CC) $(CFLAGS) -o $@ $^

test.exe: src/test_main.c src/ast.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	del /F /Q *.exe
	del /F /Q src\parser.tab.c src\parser.tab.h src\lex.yy.c
	del /F /Q *.o src\*.o src\optimizer\*.o src\visualizer\*.o
	del /F /Q test_output.c test_output.html test_output.html.dot

test: optimizer_test.exe visualizer_test.exe
	.\optimizer_test.exe
	.\visualizer_test.exe

.PHONY: all clean test 