import random
from collections import deque

import gymnasium as gym
import numpy as np
import torch
from torch import nn

steps = 40000
decay_step = 15000
epsilon_min = 0.1
discount_factor = 0.99
warmup = 1000
online_step = 4
target_step = 1000
memory_size = 20000

lr = 0.0005
batch_size = 64

seed = 0

rng = np.random.default_rng(0)
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
print(device)
env = gym.make("CartPole-v1")
state, info = env.reset(seed=seed)
action = env.action_space.sample()
next_state, reward, terminated, truncated, info = env.step(action)
print(state.shape, env.action_space.n)
print(reward, terminated, truncated)
print(torch.__version__, gym.__version__)
env.close()

def make_net():
    return nn.Sequential(
    nn.Linear(4, 32), nn.ReLU(),
    nn.Linear(32, 32), nn.ReLU(),
    nn.Linear(32, 32), nn.ReLU(),
    nn.Linear(32, 2)
    )

online = make_net().to(device)
target = make_net().to(device)
target.load_state_dict(online.state_dict())
target.requires_grad_(False)

# epsilon greedy
def select_action(state, epsilon, model=online, rng=rng, device=device):
    if rng.random() < epsilon:
        # Exploration
        return int(rng.integers(2))
    else:
        # Exploitation
        with torch.no_grad():
            state_tensor = (
                torch.tensor(state, dtype=torch.float32, device=device)
                .unsqueeze(0)
            )
            q_values = model(state_tensor)  
            q_np = q_values.cpu().numpy().squeeze(0)
            best = np.flatnonzero(q_np == q_np.max())
            return int(rng.choice(best))

memory = deque(maxlen=memory_size)
memory.append((state.copy(), action, reward, next_state.copy(), terminated))

optimizer = torch.optim.Adam(online.parameters(), lr=lr)
loss_fn = nn.SmoothL1Loss()

def optimize_batch():
    batch = random.sample(memory, batch_size)
    states, actions, rewards, next_states, terms = zip(*batch)
    S = torch.tensor(np.array(states), dtype=torch.float32)
    A = torch.tensor(actions, dtype=torch.int64).unsqueeze(1)
    R = torch.tensor(rewards, dtype=torch.float32)
    NS = torch.tensor(np.array(next_states), dtype=torch.float32)
    T = torch.tensor(terms, dtype=torch.float32)

    prediction = online(S).gather(1, A).squeeze(1)
    with torch.no_grad():
        next_q = target(NS).max(dim=1).values
        y = R + discount_factor * (1 - T) * next_q
        
    loss = loss_fn(prediction, y)
    optimizer.zero_grad()
    loss.backward()
    nn.utils.clip_grad_norm_(online.parameters(), 10)
    optimizer.step()
    return loss.item()  # TD Loss 기록용 반환

# Greedy 평가 함수
def evaluate_agent(model, n_eval_episodes=5):
    eval_env = gym.make("CartPole-v1")
    eval_rewards = []
    for i in range(n_eval_episodes):
        st, _ = eval_env.reset(seed=100 + i)
        term = False
        trunc = False
        tot_r = 0
        while not (term or trunc):
            with torch.no_grad():
                x = torch.as_tensor(st, dtype=torch.float32, device=device).unsqueeze(0)
                act = model(x).argmax(dim=1).item()
            st, rew, term, trunc, _ = eval_env.step(act)
            tot_r += rew
        eval_rewards.append(tot_r)
    eval_env.close()
    return np.mean(eval_rewards), np.std(eval_rewards)

# 시각화 데이터 기록용 리스트
episode_rewards_history = []
losses_history = []
epsilons_history = []
eval_steps_list = []
eval_means = []
eval_stds = []

state, _ = env.reset(seed=seed)
episode_count = 0  
current_episode_reward = 0  

for step in range(1, steps + 1):
    epsilon = max(epsilon_min, 1 - (1 - epsilon_min) * (step - 1) / decay_step) 
    epsilons_history.append(epsilon)
    
    action = select_action(state, epsilon)
    ns, r, term, trunc, _ = env.step(action)
    
    cart_pos = ns[0]
    cart_vel = ns[1]
    pole_angle = ns[2]
    pole_angular_vel = ns[3]
    
    r = 1.0
    
    if abs(ns[2]) > 0.04:
        r += -1
    if abs(ns[0]) > 1.6:
        r += -1

    if term:
        r += -20.0
    elif trunc:
        r += 100.0
        
    memory.append((state.copy(), action, r, ns.copy(), term))
    current_episode_reward += r  
    
    if len(memory) >= warmup and step % online_step == 0:
        loss_val = optimize_batch()
        losses_history.append(loss_val)
        
    if step % target_step == 0:
        target.load_state_dict(online.state_dict())
        
    state = ns
    
    if term or trunc:
        episode_count += 1
        episode_rewards_history.append((episode_count, current_episode_reward))
        print(f"step: {step} | episode: {episode_count}, reward: {current_episode_reward} | epsilon: {epsilon}")
        print(f"{ns[0]:.2f}, {ns[1]:.2f}, {ns[2]:.2f}, {ns[3]:.2f}")
        state, _ = env.reset()
        current_episode_reward = 0

    if step % 2000 == 0:
        mean_r, std_r = evaluate_agent(online)
        eval_steps_list.append(step)
        eval_means.append(mean_r)
        eval_stds.append(std_r)
        
torch.save(online.state_dict(), "dqn.pt")


print("\n" + "="*50)
print(">>> 50회 Greedy 평가 및 무작위 정책 비교 시작")
print("="*50)

eval_env = gym.make("CartPole-v1")
model = make_net()
model.load_state_dict(torch.load("dqn.pt", weights_only=True))
model.eval()

dqn_scores = []
random_scores = []
n_test_episodes = 50

# 1. DQN 50회 Greedy 평가
for i in range(n_test_episodes):
    state, _ = eval_env.reset(seed=1000 + i)
    terminated = False
    truncated = False
    total_reward = 0
    while not (terminated or truncated):
        with torch.no_grad():
            x = torch.as_tensor(state, dtype=torch.float32, device=device).unsqueeze(0)
            action = model(x).argmax(dim=1).item()
        state, reward, terminated, truncated, _ = eval_env.step(action)
        total_reward += reward
    dqn_scores.append(total_reward)

# 2. 무작위 정책 50회 평가
for i in range(n_test_episodes):
    state, _ = eval_env.reset(seed=2000 + i)
    terminated = False
    truncated = False
    total_reward = 0
    while not (terminated or truncated):
        action = eval_env.action_space.sample()
        state, reward, terminated, truncated, _ = eval_env.step(action)
        total_reward += reward
    random_scores.append(total_reward)

eval_env.close()

# 통계 출력 (평균, 표준편차, 최소, 최대)
print(f"[DQN 모델 50회 Greedy 평가 결과]")
print(f" - 평균 (Mean): {np.mean(dqn_scores):.2f}")
print(f" - 표준편차 (Std): {np.std(dqn_scores):.2f}")
print(f" - 최소 (Min): {np.min(dqn_scores):.1f}")
print(f" - 최대 (Max): {np.max(dqn_scores):.1f}")

print("-" * 40)

print(f"[무작위 정책(Random) 50회 비교 평가 결과]")
print(f" - 평균 (Mean): {np.mean(random_scores):.2f}")
print(f" - 표준편차 (Std): {np.std(random_scores):.2f}")
print(f" - 최소 (Min): {np.min(random_scores):.1f}")
print(f" - 최대 (Max): {np.max(random_scores):.1f}")
print("="*50)

import matplotlib.pyplot as plt

fig, axs = plt.subplots(2, 2, figsize=(14, 10))

# 1. 학습 보상
episodes, train_returns = zip(*episode_rewards_history)
window = 10
train_ma = np.convolve(train_returns, np.ones(window)/window, mode='valid')
axs[0, 0].plot(episodes, train_returns, alpha=0.3, color='orange', label='Raw Return')
axs[0, 0].plot(episodes[window-1:], train_ma, color='red', linewidth=2, label=f'Moving Average (MA-{window})')
axs[0, 0].set_title("Training Reward")
axs[0, 0].set_xlabel("Episode")
axs[0, 0].set_ylabel("Return")
axs[0, 0].legend()
axs[0, 0].grid(True)

# 2. Greedy 평가
eval_means = np.array(eval_means)
eval_stds = np.array(eval_stds)
axs[0, 1].plot(eval_steps_list, eval_means, marker='o', color='green', label='Mean Reward')
axs[0, 1].fill_between(eval_steps_list, eval_means - eval_stds, eval_means + eval_stds, color='green', alpha=0.2, label='± Std')
axs[0, 1].set_title("Greedy Evaluation")
axs[0, 1].set_xlabel("Environment Step")
axs[0, 1].set_ylabel("Reward (Mean ± Std)")
axs[0, 1].legend()
axs[0, 1].grid(True)

# 3. TD Loss
axs[1, 0].plot(losses_history, color='purple', alpha=0.3, linewidth=1)
if len(losses_history) >= 50:
    loss_ma = np.convolve(losses_history, np.ones(50)/50, mode='valid')
    axs[1, 0].plot(range(49, len(losses_history)), loss_ma, color='blue', linewidth=1.5, label='MA-50')
axs[1, 0].set_title("TD Loss")
axs[1, 0].set_xlabel("Optimizer Update Step")
axs[1, 0].set_ylabel("Loss")
axs[1, 0].legend()
axs[1, 0].grid(True)

# 4. 탐험률 ε
axs[1, 1].plot(range(1, steps + 1), epsilons_history, color='darkorange', linewidth=2)
axs[1, 1].set_title("Exploration Rate ε")
axs[1, 1].set_xlabel("Environment Step")
axs[1, 1].set_ylabel("Epsilon (ε)")
axs[1, 1].grid(True)

plt.suptitle(f"DQN Training Curves (Seed: {seed}cc)", fontsize=16, fontweight='bold')
plt.tight_layout()
plt.savefig("training_curves.png", dpi=160)
plt.show()