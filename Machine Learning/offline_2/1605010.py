import math
import numpy as np
from scipy.stats import norm


def viterbi(number_of_states, transition_matrix, means, standard_deviations, observations, stationary):
    
    time_steps = observations.shape[0]

    dp = np.zeros((number_of_states, time_steps + 2))
    trace = np.zeros((number_of_states, time_steps + 2), dtype=int)
 
    for i in range(number_of_states):
        dp[i][0] = math.log2(stationary[i])

    for k in range(1, time_steps + 2):
        if k == time_steps + 1:
            mx = -10**9
            prev = -1
            for i in range(number_of_states):
                if dp[i][k-1] > mx:
                    mx = dp[i][k-1]
                    prev = i
            trace[0][k] = prev
            continue

        for j in range(number_of_states):
            x = norm.pdf(observations[k-1], loc=means[j], scale=standard_deviations[j])
            emission_probability = math.log2(x)
            mx = -10**9
            prev = -1
            for i in range(number_of_states):
                temp = dp[i][k-1] + math.log2(transition_matrix[i][j]) + emission_probability
                if temp > mx:
                    mx = temp
                    prev = i
            dp[j][k] = mx
            trace[j][k] = prev
        
    path = np.zeros(time_steps + 1, dtype=int)

    cur = time_steps
    path[cur] = trace[0][cur+1]
    prev = path[cur]
    cur -= 1

    while cur > 0:
        path[cur] = trace[prev][cur+1]
        prev = path[cur]
        cur -= 1


    f2 = open("vit.txt", "w")

    for i in range(1, time_steps+1):
        f2.write(str(path[i]) + "\n")

    f2.close()



def read_input():
    f1 = open("Input/parameters.txt", "r")
    lines = f1.readlines()
    number_of_states = int(lines[0])
    transition_matrix = []
    for i in range(1,number_of_states + 1):
        transition_matrix.append([float(x) for x in lines[i].split()])

    transition_matrix = np.array(transition_matrix)

    means = np.array([float(x) for x in lines[number_of_states + 1].split()])
    standard_deviations = np.array([float(x) for x in lines[number_of_states + 2].split()])
    f1.close()

    f1 = open("Input/data.txt", "r")
    lines = f1.readlines()
    observations = np.array([float(x) for x in lines])
    f1.close()

    evals, evecs = np.linalg.eig(transition_matrix.T)
    evec1 = evecs[:,np.isclose(evals, 1)]
    evec1 = evec1[:,0]
    stationary = evec1 / evec1.sum()
    stationary = stationary.real
 
    return number_of_states, transition_matrix, means, standard_deviations, observations, stationary




def forward(number_of_states, transition_matrix, means, standard_deviations, observations, stationary):
    time_steps = observations.shape[0]
    dp = np.zeros((number_of_states, time_steps + 1))

    for i in range(number_of_states):
        dp[i][0] = stationary[i]

    for k in range(1, time_steps + 1):
        for j in range(number_of_states):
            emission_probability = norm.pdf(observations[k-1], loc=means[j], scale=standard_deviations[j])
            for i in range(number_of_states):
                dp[j][k] += dp[i][k-1] * transition_matrix[i][j] * emission_probability
                
    return dp


def backward(number_of_states, transition_matrix, means, standard_deviations, observations, stationary):
    time_steps = observations.shape[0]
    dp = np.zeros((number_of_states, time_steps + 1))

    for i in range(number_of_states):
        dp[i][time_steps] = 1.0

    for k in range(time_steps-1, 0, -1):
        for j in range(number_of_states):
            for i in range(number_of_states):
                emission_probability = norm.pdf(observations[k], loc=means[i], scale=standard_deviations[i])
                dp[j][k] += dp[i][k+1] * transition_matrix[j][i] * emission_probability
                
    return dp







if __name__ == '__main__':
    number_of_states, transition_matrix, means, standard_deviations, observations, stationary = read_input()

    viterbi(number_of_states, transition_matrix, means, standard_deviations, observations, stationary)
    