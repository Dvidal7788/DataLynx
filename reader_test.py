import csv
# READ
with open('csv/staff.csv', 'r') as file:
    # # V1 - ONE LONG STRING
    # text = file.read()
    # text = text.strip().split('\n')
    # for row in text:
    #     row = row.split(',')
    #     print(row)

    # # V2
    # for row in file:
    #     row = row.strip().split(',')
    #     print(row)

    # # V3
    # reader = csv.reader(file)
    # for row in reader:
    #     # print(row)
    #     print(row[0])

    # # V4
    # reader = csv.DictReader(file)
    # for row in reader:
    #     print(row['Ext'])

