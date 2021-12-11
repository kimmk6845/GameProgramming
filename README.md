# GameProgramming
2021-1(4학년) 게임프로그래밍

## HW1
### GDI와 STL을 익히기 위한 WIN32 응용프로그램 작성

- 구현 요구사항
  - 화면에 네모 상자들을 아래에서 위쪽으로 쌓아서 표시해라.
    - 각 상자 안에는 학생 정보(이름, 성적)을 표시
    - 그래픽 처리는 GDI를 이용, 상자마다 색깔을 다르게 처리
    - 각 상자의 학생 정보는 벡터 컨테이너를 이용


  - 마우스 이벤트 처리
    - 가장 높은 위치의 상자가 클릭되면 삭제
    - 이외의 지점에서 클릭되면 새로운 학생 정보를 추가
      - 학생은 최대 8명
      - 학생 정보 구조체는 성명과 성적의 두 필드를 갖음
      - 이름은 3자리 한글성명이고 성적은 50~99 사이의 정수
      - <span style="color:red"> 학생 데이터가 벡터에 추가될 때마다, 성적에 따라 정렬되도록 sort알고리즘을 적용 </span>
    
   
   - 기타
     - 학생을 추가하거나 삭제할 수 없는 경우 메시지 창을 표시
     

