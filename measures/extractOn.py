#
# Course: High Performance Computing 2021/2022
# 
# Lecturer: Francesco Moscato	fmoscato@unisa.it
#
# Group:
# Briglia Maria Rosaria	0622701711	m.briglia1@studenti.unisa.it               
# Della Monica Pierpaolo	0622701701  p.dellamonica9@studenti.unisa.it 
# Giannino Pio Roberto		0622701713  p.giannino@studenti.unisa.it
#
# This file is part of FirstProjectOMP.
#
# FirstProjectOMP is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# Requirements: Parallelize and Evaluate Performances of "COUNTING SORT" Algorithm ,by using OpenMP.
# 
# To produce this analysis files produced by previous years group 02 have been used
#
# The counting sort function for O(n^2) implementation was taken here:
# https://github.com/ianliu/programacao-paralela/blob/master/omp-count-sort/main.c
#
# The counting sort function for O(n) implementation was taken here:
# https://en.wikipedia.org/wiki/Counting_sort
#
# FirstProjectOMP is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with FirstProjectOMP.  If not, see <http://www.gnu.org/licenses/>.
#

import os
import numpy as np
import scipy as sp
import matplotlib.pyplot as plt
import pandas as pd
from scipy import stats
import seaborn as sns
from prettytable import PrettyTable
from prettytable import MARKDOWN
from prettytable import MSWORD_FRIENDLY
import re


config = {
    "init": {"jpg": False, "speedup": False},
    "countingsort": {"jpg": False, "speedup": False},
    "user": {"jpg": False, "speedup": False},
    "sys": {"jpg": False, "speedup": False},
    "elapsed": {"jpg": True, "speedup": True},
}


def _extract(path_to_folder, plot_columns):
    prev = os.getcwd()
    os.chdir(path_to_folder)

    # List diresctory
    filenames = [f for f in os.listdir(".") if os.path.isfile(f)]
    if not os.path.exists("jpg"):
        os.mkdir("jpg")

    # Remove not csv files
    filenames = [
        f
        for f in os.listdir(".")
        if f.endswith(".csv") and re.match("SIZE-[0-9]+-NTH-[0-9]{2}-O[0-9]-?[0-9]*", f)
    ]
    print(filenames)

    filenames = sorted(filenames)
    means = {}

    for filename in filenames:
        file_mean = {}
        print("Processing : " + filename)
        ds = pd.read_csv(filename)
        for col in plot_columns.keys():
            print("Processing : " + filename + ", Col : " + col)
            # extract the selected column
            x_data = ds[col]
            mean, std = stats.norm.fit(x_data)
            # 68,3% = P{ μ − 1,00 σ < X < μ + 1,00 σ }
            x_data = ds[(ds[col] < (mean + std)) & (ds[col] > (mean - std))][col]
            mean, std = stats.norm.fit(x_data)
            file_mean[col] = mean

            if plot_columns[col]["jpg"]:
                sns.histplot(x_data, kde=True)
                plt.savefig("jpg/" + str(col) + "_" + filename.split(".")[0] + ".jpg")
                plt.close()

        means[filename] = file_mean
    os.chdir(prev)
    return means


def _compute_speedup(t, tp, nt, psize):
    speedup = t / tp
    efficiency = t / (tp * float(nt))
    return speedup, efficiency


def _make_table(header, rows, print_table=False, save=True, name=""):
    if save and not name:
        raise Exception("No filename to save file")
    x = PrettyTable()
    x.field_names = header
    x.add_rows(rows)
    if save:
        _save_table(x, name)
    if print_table:
        print(x)
    return x


def _save_table(table, filename):
    with open(filename, "w") as table_file:
        # table.set_style(MARKDOWN)
        table.set_style(MSWORD_FRIENDLY)
        data = table.get_string()
        table_file.write(data)


def _plot_from_table(header, rows, save=True, name="", show_plot=False):
    if save and not name:
        raise Exception("No filename to save file")

    x = [0]
    y = [0]
    speedup_pos = header.index("Speedup")
    thread_pos = header.index("Threads")
    for row in rows[1:]:
        x.append(row[thread_pos])
        y.append(row[speedup_pos])

    x_th = np.array(x)
    fig, ax = plt.subplots(figsize=(12, 8))
    ax.plot(x_th, y, "ro-", label="Experimental")
    ax.plot(x_th, x_th, color="blue", label="Ideal")
    # same as y_th, bisection
    plt.style.use("seaborn-whitegrid")

    plt.autoscale(enable=True, axis="x", tight=True)
    plt.autoscale(enable=True, axis="y", tight=True)

    plt.legend()
    plt.xlabel("Processors")
    plt.ylabel("Speedup")
    if show_plot:
        plt.show()
    if save:
        plt.savefig(name)
    plt.close()


def extraction(
    root=os.path.join(os.path.dirname(os.path.realpath(__file__)), "measureOn/"),
    cols=config,
    threads=[0, 1, 2, 4, 8],
):
    print("Listing folder for problem size")
    folders = [
        f
        for f in os.listdir(root)
        if (os.path.isdir(os.path.join(root, f)) and re.match("SIZE-[0-9]+-O[0-9]", f))
    ]
    print(f"Found folders : {folders}")

    for folder in folders:
        print(f"Folder : {folder}")
        joined_path = os.path.join(root, folder)
        means = _extract(joined_path, cols)
        header = {
            "values": [
                "Version",
                "Threads",
                "Init",
                "CountingSort",
                "User",
                "Sys",
                "Elapsed",
                "Speedup",
                "Efficiency",
            ]
        }
        cells = {"values": []}
        nt = -1
        for filename_key in means:
            cell = []
            splitted_filename = filename_key.split("-")
            if "NTH-00" in filename_key:
                seq = means[filename_key]["elapsed"]
                nt = 1
                cell.append("Serial")
                cell.append(nt)
            else:
                nt = int(splitted_filename[3])
                cell.append("Parallel")
                cell.append(nt)

            for col in cols:
                cell.append(means[filename_key][col])
                if cols[col]["speedup"]:
                    psize = splitted_filename[1]
                    speedup, efficiency = _compute_speedup(
                        seq, means[filename_key][col], nt, psize
                    )
                    cell.append(speedup)
                    cell.append(efficiency)
            cells["values"].append(cell)

        splitted_folder = folder.split("-")
        size = splitted_folder[1]
        opt = splitted_folder[2]
        table_filename = joined_path + "/psize-" + size + "-" + str(opt) + "-table.csv"
        plot_filename = joined_path + "/speedup-" + str(size) + "-" + str(opt) + ".jpg"

        table = _make_table(header["values"], cells["values"], name=table_filename)
        _plot_from_table(header["values"], cells["values"], name=plot_filename)


if __name__ == "__main__":
    extraction()
