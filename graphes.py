import matplotlib.pyplot as plt
import numpy as np

data = np.loadtxt("temps_calcul.txt", skiprows=1)
points = data[:,0]
tListe = data[:,1]
tHachage = data[:,2]
tArbre = data[:,3]

# Graphique liste chaînée
plt.figure()
plt.plot(points, tListe, marker='o', color='blue', label='Liste chaînée')
plt.xlabel("Nombre total de points")
plt.ylabel("Temps (s)")
plt.title("Temps de calcul - Liste chaînée")
plt.grid(True)
plt.legend()
plt.savefig("liste_chainee.png")

# Graphique hachage + arbre
plt.figure()
plt.plot(points, tHachage, marker='o', color='red', label='Table de hachage')
plt.plot(points, tArbre, marker='o', color='green', label='Arbre quaternaire')
plt.xlabel("Nombre total de points")
plt.ylabel("Temps (s)")
plt.title("Temps de calcul - Hachage vs Arbre")
plt.grid(True)
plt.legend()
plt.savefig("hachage_arbre.png")

plt.show()
