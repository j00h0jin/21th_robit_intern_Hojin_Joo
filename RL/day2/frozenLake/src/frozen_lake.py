import gymnasium as gym
import numpy as np

episodes = 5000
test_episodes = 200

alpha = 0.1 # 높을수록 기존값을 낮게 평가
gamma = 0.90 # discount factor
decay_rate = 0.999
epsilon_min = 0.05
is_slippery = False

env = gym.make("FrozenLake-v1", map_name="4x4", is_slippery=is_slippery)
rng = np.random.default_rng(0)
Q = np.zeros((env.observation_space.n, env.action_space.n))
returns = []

# epsilon greedy
def choose_action(state, epsilon, rng=rng):
    if rng.random() < epsilon:
        return int(rng.integers(4)) # Exploration
    best = np.flatnonzero( Q[state] == Q[state].max()) # Exploitation
    return int(rng.choice(best)) # 확률이 가장 높은 값 (greedy)

for episode in range(episodes):
    state, _ = env.reset(seed=0 if episode == 0 else None)
    epsilon = max(epsilon_min, decay_rate ** episode) # decaying(episode가 커질 수록 작은 값을 가지도록, 최솟값 0.05)
    total = 0
    while True:
        action = choose_action(state, epsilon) # 상, 하, 좌, 우 중 하나
        ns, r, terminated, truncated, _ = env.step(action) # next state, reward
        target = r if terminated else r + gamma * Q[ns].max() # terminated일 때 r
        Q[state, action] = Q[state, action] + alpha * (target - Q[state, action])
        # Q(s,a) ← Q(s,a) + α × TD error
        total += r
        state = ns # 다음 state로 이동 후 while문 다시 돌기
        if terminated or truncated: # 구멍에 빠지거나, 도착 지점 도달 or truncated(gym.make() 기본 100턴)
            break
    returns.append(total)
np.save("q_table.npy", Q)

# 평가, epsilon = 0, Q 갱신 X
Q = np.load("q_table.npy")
print("returns")
print(returns)
print("Learned Q-table")
print(Q)
eval_env = gym.make("FrozenLake-v1", is_slippery=is_slippery)
eval_rng = np.random.default_rng(1000)
wins = 0
for ep in range(test_episodes):
    state, _ = eval_env.reset(seed=1000 if ep == 0 else None)
    while True:
        action = choose_action(state, 0.0, eval_rng) # epsilon = 0
        state, r, term, trunc, _ = eval_env.step(action)
        if term or trunc:
            wins += int(r == 1)
            break
print(wins / test_episodes)
eval_env.close()
env.close()


#visualization
import matplotlib.pyplot as plt

smoothed_returns = np.convolve(returns, np.ones(100)/100, mode='valid')

plt.plot(smoothed_returns)
plt.title("Learning Progress")
plt.xlabel("Episodes")
plt.ylabel("Win Rate (Average Reward)")
plt.show()