# COptiViz - Code Optimizer and Visualizer

COptiViz is a C code optimizer and AST visualizer that helps you understand and improve your code through:
- Constant folding
- Dead code elimination
- Loop unrolling
- AST visualization

## Prerequisites

- GCC compiler
- Flex lexical analyzer
- Bison parser generator
- Python 3.6+ (for web interface)
- Flask web framework

## Building

1. Install dependencies:
```bash
# Install build tools (Ubuntu/Debian)
sudo apt-get install gcc flex bison make

# Install Python dependencies
pip install -r requirements.txt
```

2. Build the project:
```bash
make
```

This will create two executables:
- `optimizer`: The code optimizer
- `visualizer`: The AST visualizer

## Usage

### Command Line Interface

1. Optimize C code:
```bash
./optimizer input.c output.c
```

2. Visualize AST:
```bash
./visualizer input.c ast.html
```

### Web Interface

1. Start the web server:
```bash
python src/web/app.py
```

2. Open http://localhost:5000 in your browser

3. Enter your C code in the editor and click "Optimize Code"

## Features

### Code Optimization

- **Constant Folding**: Evaluates constant expressions at compile time
- **Dead Code Elimination**: Removes unreachable code
- **Loop Unrolling**: Unrolls small loops for better performance

### Visualization

- Interactive AST visualization using D3.js
- Color-coded nodes by type
- Collapsible tree view
- Export to PNG/SVG

## Project Structure

```
COptiViz/
├── src/
│   ├── ast.h/c          # AST data structures
│   ├── codegen.h/c      # Code generation
│   ├── lexer.l          # Lexical analyzer
│   ├── parser.y         # Parser
│   ├── optimizer/       # Code optimization
│   ├── visualizer/      # AST visualization
│   └── web/            # Web interface
├── Makefile
└── requirements.txt
```

## Contributing

1. Fork the repository
2. Create a feature branch
3. Commit your changes
4. Push to the branch
5. Create a Pull Request

## License

This project is licensed under the MIT License - see the LICENSE file for details. 

