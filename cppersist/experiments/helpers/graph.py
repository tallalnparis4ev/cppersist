import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import os
import shutil
from os import listdir
from os.path import isfile, join
from sigfig import round

should_output = False
unit = 1000000000
# unit = 1000

def get_data(file_path):
  df = pd.read_csv(file_path, header=None)
  real_col = df.iloc[:,-1:].squeeze()
  real_mean = real_col.mean()
  real_sd = real_col.std()
  return {"real_mean":real_mean/unit, "real_sd":real_sd/unit}



def make_graph(file_paths,save_to):
  names = ['With Cppersist and a\n cache is kept between runs','With Cppersist','Without Cppersist']
  
  plot_dict = {}
  for path in file_paths:
    if path.endswith("NoCpst.csv"):
      plot_dict[names[2]] = get_data(path)
    elif path.endswith("Cpst.csv"):
      plot_dict[names[0]] = get_data(path)
    elif path.endswith("CpstDelCache.csv"):
      plot_dict[names[1]] = get_data(path)
    
  real_means = [plot_dict[name]['real_mean'] for name in names]
  real_std = [plot_dict[name]['real_sd'] for name in names]
  plt.xlabel("Method")
  plt.ylabel("Mean Real Time Taken (s)")
  bar = plt.bar(names,real_means,alpha=0.5, color=['red', 'blue', 'green'])
  ytop = [std for i,std in enumerate(real_std)]
  ybot = [std if real_means[i] - std >= 0 else real_means[0] - 0.0001 for i,std in enumerate(real_std)]

  error_bars = plt.errorbar(names,real_means,yerr=(ybot,ytop) ,capsize=10,fmt='o', markersize=0)
  for patch in bar.patches:
      label = "{:.2f}".format(patch.get_height())
      plt.annotate(label, (patch.get_x() + (0.6 * patch.get_width()), patch.get_height()))
  if should_output:
    plt.savefig(save_to, bbox_inches='tight')
  plt.clf()


def make_graph_dirs(dir):
  if not should_output:
    return
  if os.path.exists(dir):
    shutil.rmtree(dir)
  os.makedirs(dir)


def make_graphs(dir):
  final_part = os.path.basename(os.path.normpath(dir))
  non_filtered = [join(dir, f) for f in listdir(dir) if isfile(join(dir, f))]
  files = sorted(list(filter(lambda x : "Rec" in x or "Iter" in x, non_filtered)))
  print(files)
  #Code adapted from https://www.geeksforgeeks.org/break-list-chunks-size-n-python/
  n = 3 
  chunks = [files[i * n:(i + 1) * n] for i in range((len(files) + n - 1) // n )]
  dirs_to_make = {"Iter" if "Iter" in x else "Rec" for x in [x for x in files if "Iter" in x or "Rec" in x]}
  for make_dir in dirs_to_make:
    make_graph_dirs(dir+"/"+make_dir)

  for chunk in chunks:
    word = chunk[0]
    save_to = ("WORep" if "WORep" in word else "wRep" if "WRep" in word else "Seq") + final_part
    new_dir = "Iter" if "Iter" in word else "Rec"
    make_graph(chunk,dir+"/"+new_dir+"/"+save_to+".png")


make_graphs('experiments/data/Fibonacci')


