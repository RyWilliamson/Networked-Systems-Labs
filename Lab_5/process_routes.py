import os

base = "processed_output"
in_dir = base + "/initial"
out_dir = base + "/processed"
out_v4_dir = out_dir + "/IPv4"
out_v6_dir = out_dir + "/IPv6"

os.makedirs(out_v4_dir, exist_ok=True)
os.makedirs(out_v6_dir, exist_ok=True)

for entry in os.scandir(in_dir):
    prev = ""
    if entry.is_file():
        with open(entry, "r") as in_file:
            out_filename = None
            if "v4-" in entry.name:
                out_filename = out_v4_dir + "/processed-" + entry.name
            elif "v6-" in entry.name:
                out_filename = out_v6_dir + "/processed-" + entry.name

            with open(out_filename, "w") as out_file:
                lines = in_file.readlines()
                for line in lines:
                    data = line.split()
                    if len(data) == 4:
                        if prev != "":
                            print("\"" + prev + "\" -- \"" + data[1] + "\"", file=out_file)
                        prev = data[1]