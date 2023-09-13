import numpy as np
import pandas as pd

path = 'csv/sales_data2.csv'

df = pd.read_csv(path);

print(df.info())

print('Max:', df['Revenue'].max())
print('Min:', df['Revenue'].min())
print('Mean:', df['Revenue'].mean())
print('Sum:', df['Revenue'].sum())