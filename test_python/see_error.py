import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

def display_error_batch(file_path, batch_size=100, page=0):
    try:
        # Calculer les lignes à sauter pour la pagination
        skip = page * batch_size
        
        # Charger uniquement le lot (batch) d'erreurs souhaité
        df = pd.read_csv(file_path, header=None, skiprows=skip, nrows=batch_size)
        
        if df.empty:
            print("Plus d'erreurs à afficher ou fichier vide.")
            return

        real_labels = df.iloc[:, 0].values
        pred_labels = df.iloc[:, 1].values
        pixels = df.iloc[:, 2:].values

        # Déterminer la taille de la grille (ex: 10x10 pour 100 images)
        grid_size = int(np.ceil(np.sqrt(len(df))))
        fig, axes = plt.subplots(grid_size, grid_size, figsize=(15, 15))
        axes = axes.flatten()

        for i in range(len(df)):
            image = pixels[i].reshape(28, 28)
            axes[i].imshow(image, cmap='gray')
            axes[i].set_title(f"R:{int(real_labels[i])} P:{int(pred_labels[i])}", fontsize=8, color='red')
            axes[i].axis('off')

        # Cacher les sous-graphiques vides si le lot est incomplet
        for j in range(i + 1, len(axes)):
            axes[j].axis('off')

        plt.tight_layout()
        plt.suptitle(f"Erreurs - Page {page} (Images {skip} à {skip + len(df)})", fontsize=16)
        plt.subplots_adjust(top=0.92)
        plt.show()

    except FileNotFoundError:
        print("Le fichier 'errors.csv' est introuvable.")
    except Exception as e:
        print(f"Erreur : {e}")

# --- CONFIGURATION ---
# Changez 'page' pour voir les erreurs suivantes (0 = les 100 premières, 1 = les 100 suivantes, etc.)
display_error_batch("errors.csv", batch_size=100, page=2)