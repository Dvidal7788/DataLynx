# # # import pandas as pd
# # # import numpy as np

# # # file = 'csv/autos.csv'

# # # df = pd.read_csv(file)

# # # print(df['highway-mpg'].std())

# # x = [110,76,34]

# # # del x[0]

# # i = x.index(34)
# # print(i)
# # x.pop(i)

# # # s = 'helo'

# # # del s

# # print(x)



# import pandas as pd

# # Creating a DataFrame with missing values
# data = {'Name': ['Alice', 'Bob', 'Charlie', None],
#         'Age': [25, None, 30, 35],
#         'Salary': [50000, 60000, None, 70000]}

# # df = pd.DataFrame(data)
# df = pd.read_csv('csv/staff.csv')

# # Display the original DataFrame
# print("Original DataFrame:")
# print(df)

# # Drop rows with missing values
# df_cleaned = df.dropna()

# # Display the cleaned DataFrame
# print("\nDataFrame after dropna:")
# print(df_cleaned)




import 