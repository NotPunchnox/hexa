import tkinter as tk
from tkinter import ttk
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from mpl_toolkits.mplot3d import Axes3D
import subprocess
import math

class HexapodSimulator:
    def __init__(self, root):
        self.root = root
        self.root.title("Hexapod Simulator")

        self.create_widgets()

    def create_widgets(self):
        # Créer un frame pour les entrées X, Y, Z
        input_frame = ttk.Frame(self.root)
        input_frame.pack(side=tk.LEFT, padx=10)

        self.label_x = ttk.Label(input_frame, text="Coordonnée X:")
        self.entry_x = ttk.Entry(input_frame)
        self.entry_x.insert(0, 12)
        self.label_z = ttk.Label(input_frame, text="Coordonnée Z:")
        self.entry_z = ttk.Entry(input_frame)
        self.entry_z.insert(0, 8)
        self.label_y = ttk.Label(input_frame, text="Coordonnée Y (hauteur):")
        self.entry_y = ttk.Entry(input_frame)
        self.entry_y.insert(0, 8)
        self.label_rouli = ttk.Label(input_frame, text="Coordonnée inclinaison:")
        self.entry_rouli = ttk.Entry(input_frame)
        self.entry_rouli.insert(0, 0)
        self.label_Leg = ttk.Label(input_frame, text="Si la patte est à gauche:")
        self.entry_Leg = ttk.Entry(input_frame)
        self.entry_Leg.insert(0, "false")

        self.label_x.grid(row=0, column=0, sticky=tk.W)
        self.entry_x.grid(row=0, column=1, padx=5)
        self.label_z.grid(row=1, column=0, sticky=tk.W)
        self.entry_z.grid(row=1, column=1, padx=5)
        self.label_y.grid(row=2, column=0, sticky=tk.W)
        self.entry_y.grid(row=2, column=1, padx=5)
        self.label_rouli.grid(row=3, column=0, sticky=tk.W)
        self.entry_rouli.grid(row=3, column=1, padx=5)
        self.label_Leg.grid(row=4, column=0, sticky=tk.W)
        self.entry_Leg.grid(row=4, column=1, padx=5)

        # Créer un frame pour les boutons
        button_frame = ttk.Frame(self.root)
        button_frame.pack(side=tk.RIGHT, padx=10)

        self.update_button = ttk.Button(button_frame, text="Mettre à jour", command=self.update_position)
        self.matrice_button = ttk.Button(button_frame, text="Matrice", command=self.launch_matrice)
        self.reset_button = ttk.Button(button_frame, text="Réinitialiser", command=self.reset_drawings)

        self.update_button.pack(pady=5)
        self.matrice_button.pack(pady=5)
        self.reset_button.pack(pady=5)

        # Initialiser la zone pour afficher les vecteurs en 3D
        self.init_3d_plot()

    def reset_drawings(self):
        self.ax_3d.clear()
        self.canvas_3d.draw()

    def init_3d_plot(self):
        self.figure_3d = Figure()
        self.ax_3d = self.figure_3d.add_subplot(111, projection='3d')
        self.canvas_3d = FigureCanvasTkAgg(self.figure_3d, master=self.root)
        self.canvas_widget_3d = self.canvas_3d.get_tk_widget()
        self.canvas_widget_3d.pack(side=tk.TOP, fill=tk.BOTH, expand=1)

    def launch_matrice(self):
        Matrice = [
            [7, 6, 8, 0],
            [7, 6, 8, 0.5],
            [7, 6, 8, 1],
            [7, 6, 8, 1.5],
            [7, 6, 8, 2],
            [7, 6, 8, 2.5]
        ]

        for line in Matrice:
            x, y, z, deltaRouli = line
            result = self.run_algorithm(x, z, deltaRouli, y)
            self.show_result_3d(result)

    def update_position(self):
        x, z, y, deltaRouli = str(self.entry_x.get()), str(self.entry_z.get()), str(self.entry_y.get()), str(self.entry_rouli.get())
        result = self.run_algorithm(x, z, deltaRouli, y)
        print(result)
        self.show_result_3d(result, y)

    def show_result_3d(self, result, y):
        scale_factor = self.get_scale_factor()
    
        start_x, start_z, start_y = 0, 0, 0
        start_x, start_z, start_y = self.draw_3d_vector(start_x, start_y, start_z,  5 * scale_factor, result["rouli"], "Coxa", "r")
      
        start_x, start_z, start_y = self.draw_3d_vector(start_x, start_y, start_z,  6 * scale_factor, 90 - result["femur"], "Femur", "r")
     
        self.draw_3d_vector(start_x, start_z, start_y, 13.5 * scale_factor, (90 - result["femur"]) - result["tibia"], "Tibia", "r")

        leg_length = 13.5 * scale_factor
        self.ax_3d.set_xlim(-leg_length, leg_length)
        self.ax_3d.set_ylim(-leg_length, leg_length)
        self.ax_3d.set_zlim(-leg_length, leg_length)  # Assurez-vous que l'axe Z est également inversé

    
        self.canvas_3d.draw()

    def draw_3d_vector(self, x, z, y, length, angle, label, color):
        angle_rad = math.radians(angle)
        end_x = x + length * math.sin(angle_rad)
        end_z = z + length * math.cos(angle_rad)
        end_y = y  # Mettez à jour end_y avec la valeur correcte
        self.ax_3d.quiver(x, z, y, end_x - x, end_z - z, end_y - y, color=color, label=label, linewidth=2.0, arrow_length_ratio=0.1)
        return end_x, end_z, end_y




    def get_scale_factor(self):
        canvas_width = self.canvas_widget_3d.winfo_width()
        canvas_height = self.canvas_widget_3d.winfo_height()

        return min(canvas_width, canvas_height) / 20.0

    def run_algorithm(self, x, z, deltaRouli, y):
        result = {}
        args = ["go", "run", "./src/algorithme.go", "-x", str(x), "-y", str(y), "-z", str(z), "-deltaRouli", str(deltaRouli)]
        string = subprocess.run(args, capture_output=True, text=True)
        print(args)
        lines = string.stdout.strip().split('\n')
        for line in lines:
            if "femur:" in line:
                result["femur"] = float(line.replace("femur: ", "").replace(" ", ""))
            elif "tibia:" in line:
                result["tibia"] = float(line.replace("tibia: ", "").replace(" ", ""))
            elif "rouli " in line:
                result["rouli"] = float(line.replace("rouli ", "").replace(" ", ""))
            elif "coxa:" in line:
                result["coxa"] = float(line.replace("coxa: ", "").replace(" ", ""))
            elif "femur2" in line:
                result["femur2"] = float(line.replace("femur2: ", "").replace(" ", ""))
            elif "tibia2" in line:
                result["tibia2"] = float(line.replace("tibia2: ", "").replace(" ", ""))
            elif "rouli2" in line:
                result["rouli2"] = float(line.replace("rouli2 ", "").replace(" ", ""))
            elif "coxa2" in line:
                result["coxa2"] = float(line.replace("coxa2: ", "").replace(" ", ""))

        print(result)
        
        return result

if __name__ == "__main__":
    root = tk.Tk()
    app = HexapodSimulator(root)
    root.mainloop()
