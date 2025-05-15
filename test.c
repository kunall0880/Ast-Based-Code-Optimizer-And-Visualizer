int main() {
    // Constant folding test
    int x = 1 + 2 * 3;
    
    // Dead code test
    if (0) {
        int y = 10;
    }
    
    // Another constant folding test
    int z = (5 + 3) * 2;
    
    // Dead code after return
    return x + z;
    int unused = 100;  // This should be eliminated
} 