# 21th_robit_intern_Hojin_Joo

결과 화면

<img width="1577" height="1024" alt="image" src="https://github.com/user-attachments/assets/adb5a88f-c716-4b4b-a9c8-80c298141ab6" />


---

사용한 기법

- median blur

ppt에 가우시안 블러와 메디안 블러가 있어서 둘의 특징을 봤는데 가우시안 블러는 경계선이 흐릿해질 수 있다는 특징이 있었다.

RGB 도형들의 경계가 확대해봤을 때 명확한 편이라고 생각이 들어서 굳이 가우시안을 사용해서 흐릿하게 만들 필요는 없다고 생각했고, 점같은 노이즈를 없애준다는 메디안 블러를 택해서 사용해봤다.


- Closing

Closing이 가장 깔끔하게 나와보여서 사용해봤다.


- 크기 작은 박스 거르기

점만한 박스가 자꾸 잡혀서 일정 크기 이상인 박스를 찾았을 때 출력하도록 했다.

