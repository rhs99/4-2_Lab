from matplotlib import markers
import numpy as np
import matplotlib.pyplot as plt
from numpy.lib.function_base import median

f1 = open("task_3.txt","r")

lines = f1.readlines()

uniform = [float(n) for n in lines[0].split()]
inter_arrival_expo = [float(n) for n in lines[1].split()]
service_expo = [float(n) for n in lines[2].split()]

uniform.sort()
inter_arrival_expo.sort()
service_expo.sort()


f2 = open("min_max_median.txt","w")

f2.write("uniform:\n")
f2.write("min: {}\n".format(min(uniform)))
f2.write("max: {}\n".format(max(uniform)))
f2.write("median: {}\n\n".format(median(uniform)))

f2.write("inter_arrival:\n")
f2.write("min: {}\n".format(min(inter_arrival_expo)))
f2.write("max: {}\n".format(max(inter_arrival_expo)))
f2.write("median: {}\n\n".format(median(inter_arrival_expo)))

f2.write("service:\n")
f2.write("min: {}\n".format(min(service_expo)))
f2.write("max: {}\n".format(max(service_expo)))
f2.write("median: {}\n".format(median(service_expo)))

f2.close()


f2 = open("input.txt","r")
lines = f2.readlines()

inter_arrival_beta = float(lines[0])
service_beta = float(lines[1])



uniform_bin = np.arange(0.0, 1.1,0.1)
uniform_bin_cnt =  np.zeros((len(uniform_bin)), dtype=float)


inter_arrival_expo_bin = [0,inter_arrival_beta/2,inter_arrival_beta,inter_arrival_beta*1.5,inter_arrival_beta*2,inter_arrival_beta*2.5,inter_arrival_beta*3,inter_arrival_beta*3.5,inter_arrival_beta*4,inter_arrival_beta*40]
inter_arrival_expo_bin_cnt =  np.zeros((len(inter_arrival_expo_bin),), dtype=float)

service_expo_bin = [0,service_beta/2,service_beta,service_beta*1.5,service_beta*2,service_beta*2.5,service_beta*3,service_beta*3.5,service_beta*4,service_beta*40]
service_expo_bin_cnt =  np.zeros((len(service_expo_bin),), dtype=float)



for val in uniform:
    for i in range(len(uniform_bin)-1):
        if uniform_bin[i]<=val<uniform_bin[i+1]: 
            uniform_bin_cnt[i] += 1
            break


for i in range(len(uniform_bin_cnt)):
    uniform_bin_cnt[i] /= len(uniform)


for val in inter_arrival_expo:
    for i in range(len(inter_arrival_expo_bin)-1):
        if inter_arrival_expo_bin[i]<=val<inter_arrival_expo_bin[i+1]: 
            inter_arrival_expo_bin_cnt[i] += 1
            break


for i in range(len(inter_arrival_expo_bin_cnt)):
    inter_arrival_expo_bin_cnt[i] /= len(inter_arrival_expo)




for val in service_expo:
    for i in range(len(service_expo_bin)-1):
        if service_expo_bin[i]<=val<service_expo_bin[i+1]: 
            service_expo_bin_cnt[i] += 1
            break


for i in range(len(service_expo_bin_cnt)):
    service_expo_bin_cnt[i] /= len(service_expo)






fig, ax = plt.subplots()
ax.set_title('uniform')
ax.set_xlabel('bin')
ax.set_ylabel('probalibily')
ax.set_xlim(0,1.0)
ax.set_ylim(0,1.0)
ax.set_xticks(np.arange(0, 1.1, .1))
ax.set_yticks(np.arange(0, 1.1, .1))
ax.plot(uniform_bin[:-1], uniform_bin_cnt[:-1], marker='o')
plt.savefig('uniform_distribution.png')


for i in range(1,len(uniform_bin_cnt)):
    uniform_bin_cnt[i] += uniform_bin_cnt[i-1]


fig, ax = plt.subplots()
ax.set_title('uniform_cumulative')
ax.set_xlabel('bin')
ax.set_ylabel('probalibily')
ax.set_xlim(0,1.0)
ax.set_ylim(0,1.0)
ax.set_xticks(np.arange(0, 1.1, .1))
ax.set_yticks(np.arange(0, 1.1, .1))
ax.plot(uniform_bin[:-1], uniform_bin_cnt[:-1], marker='o')
plt.savefig('uniform_cumulative_distribution.png')




fig, ax = plt.subplots()
ax.set_title('inter_arrival')
ax.set_xlabel('bin')
ax.set_ylabel('probalibily')
ax.set_xlim(0,4*inter_arrival_beta)
ax.set_ylim(0,1.0)
ax.set_xticks(np.arange(0, 4*inter_arrival_beta, .5))
ax.set_yticks(np.arange(0, 1.1, .1))
ax.plot(inter_arrival_expo_bin[:-1], inter_arrival_expo_bin_cnt[:-1], marker='o')
plt.savefig('inter_arrival_distribution.png')

for i in range(1,len(inter_arrival_expo_bin_cnt)):
    inter_arrival_expo_bin_cnt[i] += inter_arrival_expo_bin_cnt[i-1]


fig, ax = plt.subplots()
ax.set_title('inter_arrival_cumulative')
ax.set_xlabel('bin')
ax.set_ylabel('probalibily')
ax.set_xlim(0,4*inter_arrival_beta)
ax.set_ylim(0,1.0)
ax.set_xticks(np.arange(0, 4*inter_arrival_beta, .5))
ax.set_yticks(np.arange(0, 1.1, .1))
ax.plot(inter_arrival_expo_bin[:-1], inter_arrival_expo_bin_cnt[:-1], marker='o')
plt.savefig('inter_arrival_cumulative_distribution.png')


fig, ax = plt.subplots()
ax.set_title('service')
ax.set_xlabel('bin')
ax.set_ylabel('probalibily')
ax.set_xlim(0, 4*service_beta)
ax.set_ylim(0,1.0)
ax.set_xticks(np.arange(0, 4*service_beta, .5))
ax.set_yticks(np.arange(0, 1.1, .1))
ax.plot(service_expo_bin[:-1], service_expo_bin_cnt[:-1], marker='o')
plt.savefig('service_distribution.png')

for i in range(1,len(service_expo_bin_cnt)):
    service_expo_bin_cnt[i] += service_expo_bin_cnt[i-1]
 

fig, ax = plt.subplots()
ax.set_title('service_cumulative')
ax.set_xlabel('bin')
ax.set_ylabel('probalibily')
ax.set_xlim(0, 4*service_beta)
ax.set_ylim(0,1.0)
ax.set_xticks(np.arange(0, 4*service_beta, .5))
ax.set_yticks(np.arange(0, 1.1, .1))
ax.plot(service_expo_bin[:-1], service_expo_bin_cnt[:-1], marker='o')
plt.savefig('service_cumultaive_distribution.png')


f1.close()
f2.close()