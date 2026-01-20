import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

def visualize_digits(file_path, num_digits=10):
    try:
        # Lecture du fichier CSV (on ne lit que les n premières lignes pour gagner du temps)
        # La structure attendue est : label, pix0, pix1, ..., pix783
        df = pd.read_csv(file_path, header=None, nrows=num_digits)
        
        # Extraction des labels (première colonne) et des pixels
        labels = df.iloc[:, 0].values
        pixels = df.iloc[:, 1:].values
        
        # Configuration de l'affichage (2 lignes, 5 colonnes)
        plt.figure(figsize=(12, 5))
        
        for i in range(num_digits):
            # Reshape des 784 pixels en une matrice 28x28
            image = pixels[i].reshape(28, 28)
            
            plt.subplot(2, 5, i + 1)
            plt.imshow(image, cmap='gray')
            plt.title(f"Label: {int(labels[i])}")
            plt.axis('off')
            
        plt.tight_layout()
        plt.show()
        
    except FileNotFoundError:
        print(f"Erreur : Le fichier '{file_path}' est introuvable.")
    except Exception as e:
        print(f"Une erreur est survenue : {e}")

# Chemin vers votre fichier (ajustez selon votre structure locale)
# D'après votre code C++, le chemin est souvent "data/Mnist/test.csv"
visualize_digits("data/Mnist/test.csv")