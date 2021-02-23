import pandas as pd
import matplotlib.pyplot as plt

def makeScatter(fileName):
  df1 = pd.read_csv("s10r10000v12000s30l2000.csv")
  df = pd.read_csv(fileName)
  df2 = pd.read_csv("s10r10000v8000s30l2000.csv")
  df3 = pd.read_csv("s10r10000v6000s30l2000.csv")
  df4 = pd.read_csv("s10r10000v4000s30l2000.csv")
  df5 = pd.read_csv("s10r10000v2000s30l2000.csv")
  #I want to graph the 400, 4000, and 10000 on the same plot 
  plt.plot(df1['update'],df1['mean_coopval'])
  plt.plot(df['update'],df['mean_coopval'])
  plt.plot(df2['update'],df2['mean_coopval'])
  plt.plot(df3['update'],df3['mean_coopval'])
  plt.plot(df4['update'],df4['mean_coopval'])
  plt.plot(df5['update'],df5['mean_coopval'])
  plt.legend(["12000 Resource Value","10000 Resource Value", "8000 Resource Value", "6000 Resource Value", "4000 Resource Value","2000 Resource Value"])  # plots all 
  plt.xlim(0, 1800)
  plt.grid(True)
  plt.xlabel("Evolutionary Time (Updates)")
  plt.ylabel("Average Population Cooperation")
  
  plt.gcf().savefig("1000rssv4000rss.png")
  #columns against index # scatter plot
  #df.plot(kind='density')  # estimate density function
  # df.plot(kind='hist')  # histogram
makeScatter("s10r10000v10000s30l2000.csv")