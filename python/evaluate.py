import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('/home/hagedoorn/Documents/TUD/Code/tour4me/build/experiments2.csv')


# print(df)

# x = [5, 15, 60, 180,300]
# y = []
# for t in x:

#     total_t = len(df.loc[(df['name'] == 'ilp') & (df['timeBound'] == t)])
#     part_t = len(df.loc[(df['status'] == 3) & (df['name'] == 'ilp') & (df['timeBound'] == t)])

#     y.append(part_t/total_t)

# x = [1 * 1000, 2.5 * 1000, 5 * 1000, 10 * 1000, 25 * 1000, 50 * 1000, 100 * 1000]
# y = []

# for d in x:
#     y.append(df.loc[(df['name'] == 'sel') & (df['budget'] == d)].mean()['time'])

# # print(y)

# x = [1 * 1000, 2.5 * 1000, 5 * 1000, 10 * 1000, 25 * 1000, 50 * 1000, 100 * 1000]
# y = []

# for d in x:
#     y.append(df.loc[(df['name'] == 'jog') & (df['budget'] == d)].mean()['time'])

# print(y)

# d = {'timeBound': [], 'qualityImprov': [], 'budget': []}

# print("HOI")
# for index, row in df.loc[df['name'] == 'jog'].iterrows():
#     ilsRow = df.loc[(df['name'] == 'ils') & (df['lat'] == row['lat']) & (df['lon'] == row['lon']) & (df['budget'] == row['budget']) & (df['timeBound'] == row['timeBound']) & (df['importance'] == row['importance'])].iloc[0]

#     if len(ilsRow) == 0:
#         continue
  
#     qual = 0
#     if row['quality'] >= 0:
#         qual = (ilsRow['quality'] - row['quality']) / row['quality']

#     if qual < 0:
#         print(row)
#         print(ilsRow)
#         print()

#     d['timeBound'].append(row['timeBound'])
#     d['qualityImprov'].append(qual)
#     d['budget'].append(row['budget'])

# ndf = pd.DataFrame(d)

x = [1, 2, 5, 10]
y = []
p = [2.5 * 1000, 5 * 1000, 10 * 1000, 25 * 1000, 50 * 1000, 100 * 1000]

# print(df.columns.tolist())
# print(df.loc[df['budget'] >0])
for d in p:
    y = []
    for b in x:
        y.append(df.loc[(df['timeBound'] == b) & (df['budget'] == d)].mean()['qualityImprov'])
    plt.plot(x, y, label=f"{d / 1000}km")

plt.legend()
plt.title('ILS improvement')
plt.xlabel('time (s)')
plt.ylabel('relative improvement (%)')
plt.savefig('ils.png')

# print(y)
# df.loc(df[''])
# print (df)