import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import os
import shutil
from os import listdir
from os.path import isfile, join
from sigfig import round

should_output = True
# unit = 1000000000 #Just for Fibonacci
unit = 1000 #For the rest

def get_data(file_path):
  if file_path == "data/Fibonacci/WORepRecCpstDelCache.csv":
    print("HEY")
  df = pd.read_csv(file_path, header=None)
  if len(df.index) > 10:
    df = df.iloc[10:,]

  real_col = df.iloc[:,-1:].squeeze()
  real_mean = real_col.mean()/unit
  real_sd = real_col.std()/unit
  return {"real_mean":real_mean, "real_sd":real_sd}

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
      label = f'{patch.get_height():.2}'
      if "e" in label:
        label = "{:.20f}".format(float(label)).rstrip('0').rstrip('.')
      # label = "{:.2g}".format(patch.get_height())
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


# make_graphs('cppersist/experiments/data/Primes')
make_graphs('data/Primes')










#Trie stuff 
#279496 words, maximum length word is 15
#24887 number of valid prefixes 

# def make_trie(*words):
#      root = dict()
#      for word in words:
#          current_dict = root
#          for letter in word:
#              current_dict = current_dict.setdefault(letter, {})
#          current_dict[_end] = _end
#      return root

# # Using readlines()
# scrabble = open('words.txt', 'r')
# lines = scrabble.readlines()
# trie = make_trie(lines)

# lens = list(map(lambda x : len(x) - 1,lines))
# a = max(lens)
# print(a)
# for x in lens:
#   if type(x) != int:
#     print(x)
# print("hey")
# print(lens)

