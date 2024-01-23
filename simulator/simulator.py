import tkinter as tk
from tkinter import ttk
from matplotlib.figure import Figure
from matplotlib.patches import Arrow
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2Tk

import subprocess
import math

class HexapodSimulator:
    def __init__(self, root):
        self.root = root
        self.root.title("Hexapod Simulator")

        self.create_widgets()

    def create_widgets(self):
        self.label_x = ttk.Label(self.root, text="Coordonnée X:")
        self.entry_x = ttk.Entry(self.root)
        self.entry_x.insert(0, 12)
        self.label_y = ttk.Label(self.root, text="Coordonnée Y:")
        self.entry_y = ttk.Entry(self.root)
        self.entry_y.insert(0, 8)
        self.label_z = ttk.Label(self.root, text="Coordonnée inclinaison:")
        self.entry_z = ttk.Entry(self.root)
        self.entry_z.insert(0, 0)

        self.update_button = ttk.Button(self.root, text="Mettre à jour", command=self.update_position)

        self.label_x.pack()
        self.entry_x.pack()
        self.label_y.pack()
        self.entry_y.pack()
        self.label_z.pack()
        self.entry_z.pack()
        self.update_button.pack()

        self.reset_button = ttk.Button(self.root, text="Réinitialiser", command=self.reset_drawings)
        self.reset_button.pack()

        self.init_plot()

    def reset_drawings(self):
        for patch in self.ax.patches:
            patch.remove()

        for text in self.ax.texts:
            text.remove()

        self.canvas.draw()

    def init_plot(self):
        self.figure = Figure()
        self.ax = self.figure.add_subplot(111, aspect='equal')
        self.canvas = FigureCanvasTkAgg(self.figure, master=self.root)
        self.canvas_widget = self.canvas.get_tk_widget()
        self.canvas_widget.pack(side=tk.TOP, fill=tk.BOTH, expand=1)

        toolbar = NavigationToolbar2Tk(self.canvas, self.root)
        toolbar.update()
        self.canvas_widget.pack(side=tk.TOP, fill=tk.BOTH, expand=1)

    def update_position(self):
        x, y, z = str(self.entry_x.get()), str(self.entry_y.get()), str(self.entry_z.get())
        result = self.run_algorithm(x, y, z)
        print(result)
        self.show_result(result)

    def show_result(self, result):

        scale_factor = self.get_scale_factor()

        start_x, start_y = 0, 0

        start_x, start_y = self.draw_vector(start_x, start_y, 5 * scale_factor, result["rouli"], "Body", "g")
        start_x, start_y = self.draw_vector(start_x, start_y, 5 * scale_factor, result["rouli"], "Coxa", "r")
        start_x, start_y = self.draw_vector(start_x, start_y, 6 * scale_factor, 90 - result["femur"], "Femur", "r")
        #print("femur: ", 90 - result["femur"], "\ntibia: ", result["tibia"])
        self.draw_vector(start_x, start_y, 13.5 * scale_factor, (90 - result["femur"]) - result["tibia"], "Tibia", "r")

        leg_length = 13.5 * scale_factor
        self.ax.set_xlim(-leg_length, leg_length)
        self.ax.set_ylim(-leg_length, leg_length)

        self.canvas.draw()


    def get_scale_factor(self):
        canvas_width = self.canvas_widget.winfo_width()
        canvas_height = self.canvas_widget.winfo_height()

        return min(canvas_width, canvas_height) / 20.0


    def run_algorithm(self, x, y, z):
        result = {}
        args = ["go", "run", "algorithme.go", "-x", str(x), "-y", str(y), "-z", str(z)]
        string = subprocess.run(args, capture_output=True, text=True)

        print("Go process output:", string.stdout)  # Print the output for debugging
        lines = string.stdout.strip().split('\n')
        for line in lines:
            if "femur" in line:
                result["femur"] = float(line.replace("femur: ", "").replace(" ", ""))
            elif "tibia" in line:
                result["tibia"] = float(line.replace("tibia: ", "").replace(" ", ""))
            elif "rouli" in line:
                result["rouli"] = float(line.replace("rouli ", "").replace(" ", ""))
        
        return result

    def draw_vector(self, x, y, length, angle, label, color):
        angle_rad = math.radians(angle)

        end_x = x + length * math.cos(angle_rad)
        end_y = y + length * math.sin(angle_rad)

        arrow = Arrow(x, y, end_x - x, end_y - y, color=color, label=label, width=0.1)
        self.ax.add_patch(arrow)

        self.ax.text(end_x, end_y, label, fontsize=8, ha='right', va='bottom')

        return end_x, end_y


if __name__ == "__main__":
    root = tk.Tk()
    app = HexapodSimulator(root)
    root.mainloop()
