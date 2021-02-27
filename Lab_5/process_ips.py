import os
import subprocess

folder = "processed_output/initial"
input_filename = "results.txt"

os.makedirs(folder, exist_ok=True)

with open(input_filename , "r") as input_file:
    lines = input_file.readlines()

    for line in lines:
        parts = line.split(" ")
        host = parts[0]
        type = parts[1]
        ip = parts[2].strip("\n")
        output_filename = None
        process = None

        print(parts)

        if type == "IPv4":
            output_filename = "v4-"+host+"-"+ip.replace(".", "")
            process = subprocess.run(['traceroute', '-4', '-q', '1', '-n', ip], check=True, stdout=subprocess.PIPE, universal_newlines=True)
        elif type == "IPv6":
            output_filename = "v6-"+host+"-"+ip.replace(":", "")
            process = subprocess.run(['traceroute', '-6', '-q', '1', '-n', ip], check=True, stdout=subprocess.PIPE, universal_newlines=True)

        output = process.stdout

        with open(folder + "/" + output_filename, "w") as output_file:
            print(output, file=output_file)
