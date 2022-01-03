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
    standard_deviations = np.array([math.sqrt(float(x)) for x in lines[number_of_states + 2].split()])
    f1.close()

    f1 = open("Input/data.txt", "r")
    lines = f1.readlines()
    observations = np.array([float(x) for x in lines])
    f1.close()

    return number_of_states, transition_matrix, means, standard_deviations, observations


def get_stationary(transition_matrix):
    evals, evecs = np.linalg.eig(transition_matrix.T)
    evec1 = evecs[:,np.isclose(evals, 1)]
    evec1 = evec1[:,0]
    stationary = evec1 / evec1.sum()
    stationary = stationary.real
    return stationary


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

        sm = 0.0   
        for j in range(number_of_states):
            sm += dp[j][k]
        for j in range(number_of_states):
            dp[j][k] /= sm

                
    return dp


def backward(number_of_states, transition_matrix, means, standard_deviations, observations):
    time_steps = observations.shape[0]
    dp = np.zeros((number_of_states, time_steps + 1))

    for i in range(number_of_states):
        dp[i][time_steps] = 1.0

    for k in range(time_steps-1, 0, -1):
        for j in range(number_of_states):
            for i in range(number_of_states):
                emission_probability = norm.pdf(observations[k], loc=means[i], scale=standard_deviations[i])
                dp[j][k] += dp[i][k+1] * transition_matrix[j][i] * emission_probability
        
        sm = 0.0   
        for j in range(number_of_states):
            sm += dp[j][k]
        for j in range(number_of_states):
            dp[j][k] /= sm
                
    return dp


def baum_welch(number_of_states, transition_matrix, means, standard_deviations, observations):
    lim = 10
    cnt = 0
    while cnt < lim:
    # E step
        stationary = get_stationary(transition_matrix)
        time_steps = observations.shape[0]

        forward_dp = forward(number_of_states, transition_matrix, means, standard_deviations, observations, stationary)
        backward_dp = backward(number_of_states, transition_matrix, means, standard_deviations, observations)

        forward_sink = np.sum(forward_dp[:,time_steps])

        # print(forward_sink)
       
        pi_star = np.zeros((number_of_states, time_steps + 1))
        
        for k in range(1, time_steps+1):
            for i in range(number_of_states):
                pi_star[i][k] = (forward_dp[i][k] * backward_dp[i][k])/forward_sink
            pi_star[:,k] /= np.sum(pi_star[:,k])
        
        
        pi_star_star = np.zeros((number_of_states, number_of_states, time_steps + 1))

        for k in range(1, time_steps):
            for i in range(number_of_states):
                for j in range(number_of_states):
                    emission_probability = norm.pdf(observations[k], loc=means[j], scale=standard_deviations[j])
                    pi_star_star[i][j][k] = (forward_dp[i][k] * backward_dp[j][k+1] * transition_matrix[i][j] * emission_probability)/forward_sink
            pi_star_star[:,:,k] /= np.sum(pi_star_star[:,:,k])

        for i in range(number_of_states):
            sm = 0
            w = 0
            for k in range(1, time_steps + 1):
                sm += (pi_star[i][k] * observations[k-1])
                w += pi_star[i][k]
            means[i] = sm/w
            
        
        for i in range(number_of_states):
            sm = 0
            w = 0
            for k in range(1, time_steps + 1):
                sm += (pi_star[i][k] * (observations[k-1] - means[i])**2)
                w += pi_star[i][k]
            standard_deviations[i] = math.sqrt(sm/w)
        
        for i in range(number_of_states):
            sm = 0.0
            for j in range(number_of_states):
                transition_matrix[i][j] = 0.0
                for k in range(1, time_steps):
                    transition_matrix[i][j] += pi_star_star[i][j][k]
                    sm += pi_star_star[i][j][k]

            for j in range(number_of_states):
                transition_matrix[i][j] /= sm
        
        cnt += 1
    
    print(means)
    print(standard_deviations**2)
    print(transition_matrix)





if __name__ == '__main__':
    number_of_states, transition_matrix, means, standard_deviations, observations = read_input()
    stationary = get_stationary(transition_matrix)

    # viterbi(number_of_states, transition_matrix, means, standard_deviations, observations, stationary)

    baum_welch(number_of_states, transition_matrix, means, standard_deviations, observations)
 

"""

baum-welch
1. E: params, obs -> forward, backward -> pi*, pi**
2. M: pi*, pi** -> params

mean = sum(x)/n
d = (sum(s-mean)^2)/n

obs:
F:
n = row sum of F in pi*[0]
mean =  sum(pi*[0][i] * obs[i])/n
d = sum(pi*[0][i] * (obs[i] - mean)^2)/n

transition:


"""