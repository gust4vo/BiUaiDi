import sys
import os
import subprocess

def generate_files(max_commands, percentage, command_type):
    step = 500
    current_commands = 500
    
    while (current_commands <= max_commands):
        command = [
            'python3', 'EVDENSITY.py', str(current_commands), 'geracarga.base', str(percentage), command_type
        ]
        
        subprocess.run(command)
        
        if os.path.exists("geracarga.ev"):
            # Execute the ./bin/tp3.out command with the generated file
            subprocess.run(['make'])

        current_commands += step

def main():
    if len(sys.argv) != 4:
        print("uso: python3 ITERATOR.py numero_maximo_de_comandos porcentagem -A/-D/-C")
        sys.exit(1)

    max_commands = int(sys.argv[1])
    percentage = float(sys.argv[2])
    command_type = sys.argv[3]

    if command_type not in ['-A', '-D', '-C']:
        print("Tipo de comando invalido")
        sys.exit(1)

    generate_files(max_commands, percentage, command_type)

if __name__ == '__main__':
    main()
