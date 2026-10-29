import os

import torch
from torch import nn
from torch.optim import lr_scheduler
from torch.utils.data import DataLoader, random_split
from torchvision import datasets, transforms

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
DATA_DIR = os.path.abspath(os.path.join(BASE_DIR, "../data"))

batch_size = 256
learning_rate = 0.001
epochs = 20

torch.manual_seed(0)
# 학습 데이터용: 데이터 증강
train_transform = transforms.Compose([
    transforms.RandomAffine(degrees=10, translate=(0.05, 0.05), scale=(0.95, 1.05)),
    transforms.ToTensor(),
    transforms.Normalize((0.1307,), (0.3081,))
])
test_transform = transforms.Compose([
    transforms.ToTensor(),
    transforms.Normalize((0.1307,), (0.3081,))
])
full = datasets.MNIST(DATA_DIR, train=True, download=True, transform=train_transform)
train, val = random_split(full, [55000, 5000], generator=torch.Generator().manual_seed(0))
test = datasets.MNIST(DATA_DIR, train=False, download=True,
transform=test_transform)
train_loader = DataLoader(train, batch_size=batch_size, shuffle=True)
val_loader = DataLoader(val, batch_size=batch_size)
test_loader = DataLoader(test, batch_size=batch_size)
device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
print(f"device: {device}")

model = nn.Sequential(
nn.Flatten(),

nn.Linear(784, 1024),
nn.BatchNorm1d(1024),
nn.GELU(),
nn.Dropout(p=0.2),

nn.Linear(1024, 512),
nn.BatchNorm1d(512),
nn.GELU(),
nn.Dropout(p=0.10),

nn.Linear(512, 256),
nn.BatchNorm1d(256),
nn.GELU(),
nn.Dropout(p=0.1),

nn.Linear(256, 128),
nn.BatchNorm1d(128),
nn.GELU(),
nn.Dropout(p=0.05),

nn.Linear(128, 10)
).to(device)

def init_weights(m):
    if isinstance(m, nn.Linear):
        nn.init.kaiming_normal_(m.weight, nonlinearity='relu')

model.apply(init_weights)
criterion = nn.CrossEntropyLoss()
optimizer = torch.optim.AdamW(model.parameters(), lr=learning_rate, weight_decay=1e-4)
scheduler = lr_scheduler.OneCycleLR(
    optimizer,
    max_lr=learning_rate,
    steps_per_epoch=len(train_loader),
    epochs=epochs
)

def main():
    for epoch in range(epochs):
        model.train()
        total_loss = 0.0
        
        for images, labels in train_loader:
            images = images.to(device)
            labels = labels.to(device)
            optimizer.zero_grad()
            logits = model(images)
            loss = criterion(logits, labels)
            loss.backward()
            optimizer.step()
            scheduler.step()
            total_loss += loss.item() * images.size(0)
            
        train_loss = total_loss / len(train)
        val_loss, val_acc = evaluate(val_loader)
        print(f"\nepoch: {epoch + 1}")
        print(f"train_loss: {train_loss}, LR: {scheduler.optimizer.state_dict()['param_groups'][0]['lr']} | Validation Loss: {val_loss}, Accuracy: {val_acc * 100:.2f}%")
        
    test_loss, test_acc = evaluate(test_loader)
    print(f"\nTest Loss: {test_loss}, Accuracy: {test_acc * 100:.2f}%")
    visualize_wrong_sample(test_loader)


def evaluate(loader):
    model.eval()
    loss_sum, correct, count = 0.0, 0, 0
    
    with torch.no_grad():
        for images, labels in loader:
            images, labels = images.to(device), labels.to(device)
            logits = model(images)
            loss_sum += criterion(logits, labels).item() * len(labels)
            correct += (logits.argmax(1) == labels).sum().item()
            count += len(labels)
            
    return loss_sum / count, correct / count


import math

import matplotlib.pyplot as plt


class WrongSampleViewer:
    def __init__(self, wrong_samples, page_size=25):
        self.wrong_samples = wrong_samples
        self.page_size = page_size
        self.total_wrong = len(wrong_samples)
        self.total_pages = math.ceil(self.total_wrong / page_size) if self.total_wrong > 0 else 1
        self.current_page = 0
        self.fig = None

    def show(self):
        if self.total_wrong == 0:
            print("틀린 샘플이 없습니다!")
            return

        # 5x5 그리드 기준 큼직한 창 생성
        self.fig = plt.figure(figsize=(14, 9))
        
        # 키보드 이벤트 연결 (방향키 누름 감지)
        self.fig.canvas.mpl_connect('key_press_event', self.on_key)
        
        # 첫 페이지 그리기
        self.draw_page()
        plt.show()

    def draw_page(self):
        self.fig.clf()  # 기존 캔버스 초기화
        
        start_idx = self.current_page * self.page_size
        end_idx = min(start_idx + self.page_size, self.total_wrong)
        page_samples = self.wrong_samples[start_idx:end_idx]
        
        cols = 5
        rows = math.ceil(len(page_samples) / cols)
        
        for idx, (img, ans, pred) in enumerate(page_samples):
            ax = self.fig.add_subplot(rows, cols, idx + 1)
            ax.imshow(img.squeeze().numpy(), cmap='gray')
            ax.set_title(f"Ans: {ans} | Pred: {pred}", fontsize=11, fontweight='bold', pad=4)
            ax.axis('off')

        self.fig.suptitle(
            f'Wrong Samples - Page {self.current_page + 1}/{self.total_pages} (Total: {self.total_wrong})\n',
            fontsize=13, fontweight='bold', y=0.98
        )
        self.fig.canvas.draw()

    def on_key(self, event):
        # 오른쪽 방향키 또는 d 키: 다음 페이지
        if event.key in ['right', 'd']:
            if self.current_page < self.total_pages - 1:
                self.current_page += 1
                self.draw_page()
        # 왼쪽 방향키 또는 a 키: 이전 페이지
        elif event.key in ['left', 'a']:
            if self.current_page > 0:
                self.current_page -= 1
                self.draw_page()


def visualize_wrong_sample(test_loader):
    model.eval()
    wrong_samples = []

    # 전체 테스트 데이터에서 오답 추출
    with torch.no_grad():
        for images, labels in test_loader:
            images_dev = images.to(device)
            logits = model(images_dev)
            preds = torch.argmax(logits, dim=1).cpu()

            incorrect_mask = (preds != labels)
            for img, ans, pred in zip(images[incorrect_mask], labels[incorrect_mask], preds[incorrect_mask]):
                wrong_samples.append((img, ans.item(), pred.item()))

    print(f"\n총 틀린 샘플 개수: {len(wrong_samples)}개")
    
    # 키보드 조작 인터랙티브 뷰어 실행
    viewer = WrongSampleViewer(wrong_samples, page_size=25)
    viewer.show()


if __name__ == "__main__":
    main()