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

fig, axes = plt.subplots(1, 3, figsize=(18, 5))

smoothed_returns = np.convolve(returns, np.ones(100)/100, mode='valid')
axes[0].plot(smoothed_returns, color='b', linewidth=2)
axes[0].set_title("Learning Success Rate Curve (Moving Avg)")
axes[0].set_xlabel("Episodes")
axes[0].set_ylabel("Win Rate")
axes[0].grid(True)

q_max_grid = Q.max(axis=1).reshape(4, 4)
cax = axes[1].matshow(q_max_grid, cmap='coolwarm')
fig.colorbar(cax, ax=axes[1])
axes[1].set_title("Q-Value Heatmap (Max Q per State)")

for i in range(4):
    for j in range(4):
        axes[1].text(j, i, f"{q_max_grid[i, j]:.2f}", ha='center', va='center', color='black', fontweight='bold')
        
        action_arrows = {0: '←', 1: '↓', 2: '→', 3: '↑'}
best_actions = np.argmax(Q, axis=1).reshape(4, 4)

axes[2].matshow(np.zeros((4, 4)), cmap='Pastel1', alpha=0.3)
axes[2].set_title("Optimal Policy Grid")
axes[2].set_xticks(range(4))
axes[2].set_yticks(range(4))
axes[2].grid(True)

for i in range(4):
    for j in range(4):
        action_idx = best_actions[i, j]
        arrow = action_arrows[action_idx]
        axes[2].text(j, i, arrow, ha='center', va='center', fontsize=24, fontweight='bold', color='darkblue')

plt.tight_layout()
plt.show()