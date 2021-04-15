import numpy as np
from numpy.polynomial import Polynomial
import pandas as pd
import matplotlib.pyplot as plt
import os
import shutil
from os import listdir
from os.path import isfile, join
from sigfig import round
import operator

#This is used specifically for the scatter graphs of Ghost

should_output = False #true to output graphs, false otherwise

# Set this depending on the precision for the computational problem 
unit = 1000000000 #nanoseconds
# unit = 1000 #milliseconds

scatter_dir = "GhostSearchNodesNoDec" #the directory that contains the .csv data files
prefix = "NoDec" #prefix of the output graphs

#read in the data at file_path
def get_data(file_path):
  df = pd.read_csv(file_path, header=None)
  real_means = []
  real_sds = []
  for i in range(0,int(len(df.index)/10)):
    df_chunk = df.iloc[(i*10):((i+1)*10),]
    real_chunk_col = df_chunk.iloc[:,-1:].squeeze()
    real_chunk_mean = real_chunk_col.mean()/unit
    real_chunk_sd = real_chunk_col.std()/unit
    real_means.append(real_chunk_mean)
    real_sds.append(real_chunk_sd)
  return real_means, real_sds

#speedups - an array of speedups due to the application of cppersist
#x - the workload that generated the 'speedups'
#outputs the corresponding speed up graph
def output_speedup_graph(speedups, x):
  fig = plt.figure()
  axes = fig.add_subplot(111)
  axes.set_xticks(x, minor=False)
  axes.scatter(x, speedups, s=2, c='g', marker="o")
  axes.plot(x, speedups, c='g',alpha=0.5)
  plt.xlabel("Number of Valid Prefixes Input")
  plt.ylabel("Speedup")
  plt.ticklabel_format(useOffset=False)
  plt.tight_layout()
  if should_output:
    plt.savefig("./data/{}/{}Speedup.png".format(scatter_dir,prefix), bbox_inches='tight')
  plt.clf()

#print some metrics and also output the speedup graph
def print_metrics(wo_cpst, w_cpst, x):
  speedups = list(map(operator.truediv, wo_cpst[0], w_cpst[0]))
  diffs = list(map(operator.sub, wo_cpst[0], w_cpst[0]))
  print("Differences: ", diffs)
  print("Speedups: ", speedups)
  print("Mean speedup: ", sum(speedups)/len(speedups))
  if should_output:
    output_speedup_graph(speedups,x)

#make the scatter plot for ghost
def make_scatter():
  # x = [x for x in range(5000000,16000000,1000000)]
  x = [x for x in range(10000000,21000000,1000000)]
  names = ['With Cppersist','Without Cppersist']
  wo_cpst = get_data("./data/{}/WORepRecNoCpst.csv".format(scatter_dir))
  w_cpst = get_data("./data/{}/WORepRecCpstDelCache.csv".format(scatter_dir))
  print_metrics(wo_cpst, w_cpst, x)
  colors = ['green','red'] if len(names) == 2 else ['red', 'blue', 'green']
    
  fig = plt.figure()
  axes = fig.add_subplot(111)
  axes.set_xticks(x, minor=False)
  axes.scatter(x, wo_cpst[0], s=2, c='r', marker="o", label=names[1])
  axes.plot(x, wo_cpst[0], c='r',alpha=0.5)
  axes.scatter(x, w_cpst[0], s=2, c='g', marker="o", label=names[0])
  axes.plot(x, w_cpst[0], c='g',alpha=0.5)
  for i in range(len(x)):
    axes.errorbar(x[i],wo_cpst[0][i],yerr=wo_cpst[1][i],ecolor="red",alpha=0.5)
    axes.errorbar(x[i],w_cpst[0][i],yerr=w_cpst[1][i],ecolor="green",alpha=0.5)
  plt.xlabel("Number of Valid Prefixes Input")
  plt.ylabel("Mean Real Time Taken (s)")
  plt.ticklabel_format(useOffset=False)
  plt.tight_layout()
  plt.legend(loc='upper left')

  if should_output:
    plt.savefig("./data/{}/{}Scatter.png".format(scatter_dir,prefix), bbox_inches='tight')
  plt.clf()

def rm_file_if_exists():
  if not should_output:
    return
  if os.path.exists("./data/{}/{}Scatter.png".format(scatter_dir,prefix)):
    os.remove("./data/{}/{}Scatter.png".format(scatter_dir,prefix))
  if os.path.exists("./data/{}/{}Speedup.png".format(scatter_dir,prefix)):
    os.remove("./data/{}/{}Speedup.png".format(scatter_dir,prefix))

def main():
  rm_file_if_exists()
  make_scatter()

main()