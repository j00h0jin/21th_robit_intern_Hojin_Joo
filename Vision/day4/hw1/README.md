# 21th_robit_intern_Hojin_Joo

### Vision day4 HW1

카메라 입력부터 UI 표시까지 하나의 흐름으로 구현

구현: YOLOv26n 추론, Bounding box와 ID·confidence 표시, 지연시간 측정

구성: 카메라 노드와 추론 노드 분리, YAML 설정(ha, topic), Launch 실행

---

### 폴더 구조

<img width="261" height="91" alt="image" src="https://github.com/user-attachments/assets/8f4574d5-8922-4a25-94fa-b173a8eedc33" />

카메라 노드는 cpp, 추론 노드는 python으로 구성했고 launch 파일이 들어있는 빈 패키지를 만들어 총 3개의 패키지로 구성했다.

### 구현

#### 카메라 노드

<img width="224" height="80" alt="image" src="https://github.com/user-attachments/assets/19825f72-e9e2-4bdb-af1e-7f08a7b1b4a3" />

<img width="959" height="72" alt="image" src="https://github.com/user-attachments/assets/5919263d-a8e2-4dfd-895a-839d2370c03e" />

<img width="609" height="260" alt="image" src="https://github.com/user-attachments/assets/52f82db0-e065-4287-a2aa-7c441b22be60" />

카메라 노드는 yaml파일에서 Hz, topic 이름을 받아와서 Hz에 맞게 이미지 topic을 발행해준다.


#### 추론 노드

<img width="355" height="351" alt="image" src="https://github.com/user-attachments/assets/3c7e2819-9bd1-4420-8f4a-412f8f93a512" />

<img width="589" height="204" alt="image" src="https://github.com/user-attachments/assets/f8156feb-d65b-4785-b19a-b9352dc1f87e" />


추론 노드에서도 Hz, topic 이름을 yaml 파일에서 받아오고, 추가적으로 띄울 class 인덱스, 인덱스와 매칭할 이름을 받아온다.

<img width="592" height="65" alt="image" src="https://github.com/user-attachments/assets/2ae4c0a1-bd5b-4cb0-bca7-7c064acf1d48" />

<img width="125" height="48" alt="image" src="https://github.com/user-attachments/assets/6011f289-24dd-4e78-8e3a-9de92632f5a3" />

model은 코코 데이터셋에서 받아온 모델을 models/에 넣어 지정해줬다.

<img width="878" height="100" alt="image" src="https://github.com/user-attachments/assets/97e9d2f9-8639-4c84-8426-1df445656619" />

실시간으로 갱신되는 이미지기 때문에 depth를 1로 설정하여 가장 최근값만 받도록 한다.

<img width="247" height="31" alt="image" src="https://github.com/user-attachments/assets/638bbbd9-8edd-46ea-ba81-0d18754b44e6" />

<img width="661" height="105" alt="image" src="https://github.com/user-attachments/assets/66a65b31-2987-4827-b281-d6a6a10a2763" />

sub 노드는 이미지를 받아오면 저장한다.

<img width="343" height="105" alt="image" src="https://github.com/user-attachments/assets/79d8213f-b408-4afe-9b4b-c871280c998e" />

같은 주기로 설정된 타이머 콜백에서 이미지를 복사해온다. 이때, mutex 처리를 통해 이미지를 복사하는 도중에 다른 이미지(새로운 이미지)로 바뀌는 것을 방지한다.

<img width="888" height="234" alt="image" src="https://github.com/user-attachments/assets/a4e1e9a6-83ad-45ae-a10d-6fe48f576bea" />

이미지 크기와 연산에 필요한 크기를 맞추기 위해 r값을 구한 뒤, r값을 곱해 resize를 해주고, padding을 채워준다.

<img width="684" height="140" alt="image" src="https://github.com/user-attachments/assets/34889ebe-efc6-46e2-b5fe-d2753b884839" />

ppt에 나와있는 float32 RGB [1, 3, 640, 640] 형식으로 바꿔주기 위해 RGB 변환 · 0~1 정규화 · CHW 재배치 +N(NCHW) 추가를 해준다.

<img width="604" height="580" alt="image" src="https://github.com/user-attachments/assets/e0bcfe27-4ae3-4390-8e28-d8ca73d13d17" />

추론한 결과값에서 parameter로 받아온 index가 있는지 확인하고 있다면 아까 딕셔너리로 변환했던 value(class 이름)값과 confidence를 띄워준다.

지연시간은 노드가 subscribe한 시점 ~ 이미지 출력까지 걸린 시간을 띄운다.

---

### 작동 영상

[링크](https://drive.google.com/file/d/1b9lBTvm8HNSQcoZ2FmZ5s3TuY40SxKV7/view?usp=sharing)

<img width="1263" height="498" alt="image" src="https://github.com/user-attachments/assets/fb551c42-839d-4323-a71b-583477c8ec6b" />

<img width="1263" height="498" alt="image" src="https://github.com/user-attachments/assets/51976965-1b87-45d4-ae37-cd7cc318aed0" />





