import random

vertices_n = 250
EDGES_PERCENT = 0.75
OUTPUT_FILE = f"in_{vertices_n}_{int(EDGES_PERCENT*100)}.txt"

edges = [(i, j) for i in range(vertices_n) for j in range(i + 1, vertices_n)]

random.shuffle(edges)

edges = edges[:int(len(edges) * EDGES_PERCENT)]

with open(OUTPUT_FILE, "w") as f:
    f.write(f"{vertices_n}\n")

    for edge in edges:
        f.write(f"{edge[0] + 1} {edge[1] + 1} 1\n");
