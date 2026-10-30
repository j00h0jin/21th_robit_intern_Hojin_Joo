import random
import time
from collections import deque

import gymnasium as gym
import numpy as np
import torch
from torch import nn

steps = 40000
decay_step = 15000
epsilon_min = 0.05
discount_factor = 0.99
warmup = 1000
online_step = 4
target_step = 500
memory = 5000

lr = 0.001
batch_size = 64

rng = np.random.default_rng(0)
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
print(device)
env = gym.make("CartPole-v1")
state, info = env.reset(seed=0)
action = env.action_space.sample()
next_state, reward, terminated, truncated, info = env.step(action)
print(state.shape, env.action_space.n)
print(reward, terminated, truncated)
print(torch.__version__, gym.__version__)
env.close()

def make_net():
    return nn.Sequential(
    nn.Linear(4, 64), nn.ReLU(),
    nn.Linear(64, 64), nn.ReLU(),
    nn.Linear(64, 2)
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
        # 상태를 PyTorch 텐서로 변환하고 배치 차원 추가 ([4] -> [1, 4])
            state_tensor = (
                torch.tensor(state, dtype=torch.float32, device=device)
                .unsqueeze(0)
            )
            q_values = model(state_tensor)  # shape: [1, 2] ex. [[n1, n2]] // n1: 왼쪽 기댓값, n2: 오른쪽 기댓값

            # Tensor를 numpy로 변환 후, 최대 Q값을 가진 인덱스들 추출 (동점 처리 포함)
            q_np = q_values.cpu().numpy().squeeze(0)
            best = np.flatnonzero(q_np == q_np.max())
            return int(rng.choice(best))

memory = deque(maxlen=memory)
memory.append((state.copy(), action, reward, next_state.copy(), terminated))

# before training (once)
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

state, _ = env.reset(seed=0)
episode_count = 0  # 총 에피소드 수 카운트
current_episode_reward = 0  # 현재 에피소드의 누적 보상
episode_rewards = []  # 모든 에피소드의 보상을 담을 리스트

for step in range(1, steps + 1):
    epsilon = max(epsilon_min, 1 - (1 - epsilon_min) * (step - 1) / decay_step) # 1.0에서 0.05까지 15,000 step 동안 감소
    action = select_action(state, epsilon)
    ns, r, term, trunc, _ = env.step(action)
    memory.append((state.copy(), action, r, ns.copy(), term))
    current_episode_reward += r  # 스텝마다 보상 누적
    
    if len(memory) >= warmup and step % online_step == 0:
        optimize_batch()
        
    if step % target_step == 0:
        target.load_state_dict(online.state_dict())
        
    state = ns
    
    if term or trunc:
        episode_count += 1
        episode_rewards.append(current_episode_reward)
        print(f"step: {step} | episode: {episode_count}, reward: {current_episode_reward} | epsilon: {epsilon}")
        state, _ = env.reset()
        current_episode_reward = 0
        
torch.save(online.state_dict(), "dqn.pt")

# eval, epsilon = 0
env_eval = gym.make("CartPole-v1", render_mode="human")

model = make_net()
model.load_state_dict(torch.load("dqn.pt", weights_only=True))
model.eval()

state, _ = env_eval.reset()
terminated = False
truncated = False
total_reward = 0

while not (terminated or truncated):
    with torch.no_grad():
        x = torch.as_tensor(state, dtype=torch.float32).unsqueeze(0)
        action = model(x).argmax(dim=1).item()  # 가장 Q값이 높은 액션 선택

    state, reward, terminated, truncated, _ = env_eval.step(action)
    total_reward += reward
    time.sleep(0.02)

print(f"Test Total Reward: {total_reward}")
input("시뮬레이션이 종료되었습니다. 창을 닫으려면 Enter 키를 누르세요...")
env_eval.close()