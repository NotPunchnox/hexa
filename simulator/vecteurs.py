import tkinter as tk
from tkinter import ttk
import subprocess

class HexapodSimulator:
    def __init__(self, root):
        self.root = root
        self.root.title("Hexapod Simulator")

        self.create_widgets()

    def create_widgets(self):
        self.label_x = ttk.Label(self.root, text="Coordonnée X:")
        self.entry_x = ttk.Entry(self.root)
        self.label_y = ttk.Label(self.root, text="Coordonnée Y:")
        self.entry_y = ttk.Entry(self.root)
        self.label_z = ttk.Label(self.root, text="Coordonnée Z:")
        self.entry_z = ttk.Entry(self.root)

        self.update_button = ttk.Button(self.root, text="Mettre à jour", command=self.update_position)

        self.label_x.pack()
        self.entry_x.pack()
        self.label_y.pack()
        self.entry_y.pack()
        self.label_z.pack()
        self.entry_z.pack()
        self.update_button.pack()

    def update_position(self):
        x = self.entry_x.get()
        y = self.entry_y.get()
        z = self.entry_z.get()

        result = subprocess.run(["go", "run", "hexapod.go", x, y, z], capture_output=True, text=True)

        tk.messagebox.showinfo("Résultat", result.stdout)

if __name__ == "__main__":
    root = tk.Tk()
    app = HexapodSimulator(root)
    root.mainloop()
