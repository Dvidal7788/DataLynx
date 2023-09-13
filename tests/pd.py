import numpy as np
import pandas as pd

# df = pd.read_csv('sales_data2.csv')

# print(df['state'].value_counts())

# header = ["symboling", "normalized-losses", "make", "fuel-type", "aspiration", "num-of-doors", "body-style",
#           "drive-wheels", "engine-location", "wheel-base", "length", "width", "height", "curb-weight", "engine-type",
#           "num-of-cylinders", "engine-size", "fuel-system", "bore", "stroke", "compression-ratio", "horsepower",
#           "peak-rpm", "city-mpg", "highway-mpg", "price"]

# file = 'https://archive.ics.uci.edu/ml/machine-learning-databases/autos/imports-85.data'

# df = pd.read_csv(file, names=header)

# print(df['body-style'].value_counts())
# print(df.describe())


df = pd.read_csv('../csv/staff.csv')

print(df.describe())