import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import sys
import re

def parse_graph(text):
    vertices = {}
    edges = set()
    
    lines = text.strip().split('\n')
    i = 0
    
    # Parse vertices
    while i < len(lines) and lines[i].strip() != 'vertices:':
        i += 1
    i += 1
    
    while i < len(lines) and lines[i].strip() and not lines[i].strip().endswith(':'):
        m = re.match(r'\s*(\d+):\s*([\d.+-]+)\s+([\d.+-]+)', lines[i])
        if m:
            vid, x, y = int(m.group(1)), float(m.group(2)), float(m.group(3))
            vertices[vid] = (x, y)
        i += 1
    
    # Parse outgoing edges (ignore incoming — they're duplicates)
    while i < len(lines) and 'outgoing edges:' not in lines[i]:
        i += 1
    i += 1
    
    while i < len(lines) and 'incoming edges:' not in lines[i]:
        m = re.match(r'\s*(\d+)\s*->\s*(\d+)', lines[i])
        if m:
            u, v = int(m.group(1)), int(m.group(2))
            edge = (min(u, v), max(u, v))
            edges.add(edge)
        i += 1
    
    return vertices, edges

def draw_graph(vertices, edges):
    fig, ax = plt.subplots(figsize=(8, 8))
    ax.set_aspect('equal')
    ax.axis('off')
    fig.patch.set_facecolor('#1e1e2e')
    ax.set_facecolor('#1e1e2e')
    
    # Draw edges
    for u, v in edges:
        x1, y1 = vertices[u]
        x2, y2 = vertices[v]
        ax.plot([x1, x2], [y1, y2], color="#ffffff", linewidth=2, alpha=0.7, zorder=1)
    
    # Draw nodes
    for vid, (x, y) in vertices.items():
        circle = plt.Circle((x, y), 0.18, color="#FF6D6D", zorder=3)
        ax.add_patch(circle)
        ax.text(x, y, str(vid), ha='center', va='center',
                fontsize=11, fontweight='bold', color="#0f0f3b", zorder=4)
    
    # Labels with white background
    for vid, (x, y) in vertices.items():
        ax.annotate(f'  ({x:.1f}, {y:.1f})', xy=(x, y),
                    fontsize=8, color='#cdd6f4',
                    xytext=(6, 6), textcoords='offset points', zorder=5)
    
    all_x = [p[0] for p in vertices.values()]
    all_y = [p[1] for p in vertices.values()]
    pad = 1.0
    ax.set_xlim(min(all_x) - pad, max(all_x) + pad)
    ax.set_ylim(min(all_y) - pad, max(all_y) + pad)
    
    ax.set_title(
        f'Graph — {len(vertices)} vertices, {len(edges)} edges',
        color='#cdd6f4', fontsize=13, pad=14
    )
    
    plt.tight_layout()
    plt.savefig('graph.png', dpi=150, bbox_inches='tight', facecolor=fig.get_facecolor())
    print("Saved to graph.png")
    plt.show()

def main():
    text = ''.join(open("graph.txt", "r").readlines())
    
    vertices, edges = parse_graph(text)
    print(f"Parsed {len(vertices)} vertices, {len(edges)} unique edges.")
    draw_graph(vertices, edges)

if __name__ == '__main__':
    main()