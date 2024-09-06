import argparse
import os
import subprocess

def exec_command(command):
    print("\nExecuting: " + command)
    try:
        result = subprocess.run(command, shell=True, check=True, text=True, capture_output=False)
    except subprocess.CalledProcessError as e:
        print(f"Error executing build: {e}")

def main():
    parser = argparse.ArgumentParser(description='Accepts build type and target.')

    parser.add_argument('build_type', nargs='?', choices=['release', 'debug'], default='debug', 
                        help="Choose build type: 'release' or 'debug' (default is 'debug')")
    parser.add_argument('target', nargs='?', choices=['all', 'server', 'client'], default='all', 
                        help="Choose target: 'all', 'server', or 'client' (default is 'all')")

    args = parser.parse_args()

    print(f"Build type: {args.build_type}")
    print(f"Target: {args.target}")

    bin_dir = 'bin'
    if not os.path.exists(bin_dir):
        print(f"Creating '{bin_dir}' directory...")
        os.makedirs(bin_dir)

    script_dir = os.path.dirname(os.path.abspath(__file__))

    command = 'odin build <path> -out:<out> -strict-style -show-timings -build-mode:exe'
    
    extension = '.exe' if os.name == 'nt' else ''
    mode = ' -debug' if args.build_type == "debug" else ''
    
    match args.target:
        case "all":
                exec_command(command.replace("<path>", "src/client").replace("<out>", "bin/client" + extension) + mode)
                exec_command(command.replace("<path>", "src/server").replace("<out>", "bin/server" + extension) + mode)
        case "client":
                exec_command(command.replace("<path>", "src/client").replace("<out>", "bin/client" + extension) + mode)
                exec_command(command.replace("<path>", "src/client").replace("<out>", "bin/client" + extension) + mode)
        case "server":
                exec_command(command.replace("<path>", "src/server").replace("<out>", "bin/server" + extension) + mode)
                exec_command(command.replace("<path>", "src/server").replace("<out>", "bin/server" + extension) + mode)
        case _:
            return "Error: incorrect command"

    os.chdir(script_dir)
    

if __name__ == "__main__":
    main()
