from typing import Dict, List

lights: Dict[int, str] = {}
lightsOn: List[int] = []

numLights: int = 1000

for i in range(numLights):
    lights[i] = "off"

for i in range(numLights):
    if i == 0:
        continue
    for j in range(numLights):
        if j == 0:
            continue
        if i % j == 0:
            if lights[i] == "on":
                lights[i] = "off"
            else:
                lights[i] = "on"

for i in range(numLights):
    if lights[i] == "on":
        lightsOn.append(i)

print("The lights that are on are:")
print(lightsOn)
