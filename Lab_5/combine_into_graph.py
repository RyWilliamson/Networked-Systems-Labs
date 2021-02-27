import os

base = "processed_output"
main_dir = base + "/processed"
in_v4_dir = main_dir + "/IPv4"
in_v6_dir = main_dir + "/IPv6"

def output_to_graph(isV4):
    graph_data_lines = []
    if isV4:
        in_dir = in_v4_dir
        output_filename = main_dir + "/router-topology-v4.dot"
    else:
        in_dir = in_v6_dir
        output_filename = main_dir + "/router-topology-v6.dot"

    for entry in os.scandir(in_dir):
        if entry.is_file():
            with open(entry, "r") as in_file:
                lines = in_file.readlines()
                for line in lines:
                    graph_data_lines.append(line)

    sorted_graph_data_lines = ["graph routertopology {\n"] + sorted(set(graph_data_lines)) + ["}"]
    with open(output_filename, "w") as out_file:
        for line in sorted_graph_data_lines:
            print(line, file=out_file, end="")

output_to_graph(True)
output_to_graph(False)