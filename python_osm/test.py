from dataclasses import dataclass

@dataclass
class P:
    x: float
    y: float

V = [P(0, 0), P(1, 0), P(1, 1), P(0, 1)]
E = [(V[0], V[1]), (V[1], V[2]), (V[2], V[0])]

sum = 0
for e in E:
    sum += (e[1].x - e[0].x) * (e[1].y + e[0].y)

print(sum)