import matplotlib.pyplot as plt

f1 = open("task_3.txt","r")

lines = f1.readlines()

uniform = [float(n) for n in lines[0].split()]
interval_expo = [float(n) for n in lines[1].split()]
service_expo = [float(n) for n in lines[2].split()]

uniform.sort()
interval_expo.sort()
service_expo.sort()

fig, ax = plt.subplots()
ax.hist(uniform,100)
plt.savefig('uniform_hist.png')


fig, ax = plt.subplots()
ax.hist(interval_expo,100)
plt.savefig('interval_expo_hist.png')


fig, ax = plt.subplots()
ax.hist(service_expo,100)
plt.savefig('service_expo_hist.png')


f1.close()