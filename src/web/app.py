from flask import Flask, render_template, request, jsonify
import os
import subprocess
import tempfile

app = Flask(__name__)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/optimize', methods=['POST'])
def optimize():
    code = request.form.get('code', '')
    
    # Create temporary files
    with tempfile.NamedTemporaryFile(mode='w', suffix='.c', delete=False) as f:
        input_file = f.name
        f.write(code)
    
    output_file = input_file + '.opt'
    dot_file = input_file + '.dot'
    html_file = input_file + '.html'
    
    try:
        # Run the optimizer
        subprocess.run(['./optimizer', input_file, output_file])
        
        # Generate visualization
        subprocess.run(['./visualizer', output_file, html_file])
        
        # Read optimized code
        with open(output_file, 'r') as f:
            optimized_code = f.read()
            
        # Read visualization HTML
        with open(html_file, 'r') as f:
            visualization = f.read()
            
        return jsonify({
            'success': True,
            'optimized_code': optimized_code,
            'visualization': visualization
        })
        
    except Exception as e:
        return jsonify({
            'success': False,
            'error': str(e)
        })
        
    finally:
        # Cleanup temporary files
        for f in [input_file, output_file, dot_file, html_file]:
            try:
                os.remove(f)
            except:
                pass

if __name__ == '__main__':
    app.run(debug=True) 