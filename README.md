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
      - 학생 데이터가 벡터에 추가될 때마다, 성적에 따라 정렬되도록 sort알고리즘을 적용
    
   
   - 기타
     - 학생을 추가하거나 삭제할 수 없는 경우 메시지 창을 표시
     

---------------------

## HW2
### 변환 응용프로그램 작성

- 구현 요구사항
  - HW1에서의 변환 기능을 GDI가 아닌 Direct2D API를 사용해 구현
  - 학생 정보를 표시하는 상자의 높이와 너비가 1:4의 비율 크기
  - 윈도 창의 상단 중간에 희미한 가상의 직사각형을 항상 표시
    - 가상 직사각형은 삽입을 위한 목적이고 컬러는 회색으로 표시. 가상 직사각형의 높이는 하단에 배치될 실제 데이터 사각형의 높이와 같고, 너비는 높이의 두배(1:2 비율)
  - 데이터를 추가하는 방법과 삭제하는 방법은 왼쪽 마우스 버튼 드래깅 방식

<br>
  - 데이터 추가
    - 회색 가상 직사각형 내부에서 시작해 드래깅
      - 추가될 새 상자와 그 데이터를 생성한 후 드래깅하는 동안 새 상자의 모습을 현재 드래깅 위치에 보여줌
        - 데이터는 사용자의 입력 절차 없이 랜덤으로 생성
      - 드래깅되는 사각형의 너비는 가상 사각형의 크기에서 시작해 드래깅하여 놓여질 위치에 가까워질수록 실제 너비에 더 가깝게 되도록 스케일링
      - 드래깅되는 사각형이 반시계방향으로 회전하면서 움직임
        - 시작 위치보다 위인 경우나 놓여질 위치보다 아래인 경우도 연속적이도록 처리
    - 현재 마우스 위치가 쌓여진 상자들 중에서 다음 놓일 위치 내부로 들어간 경우, 새 상자가 실제로 추가
      - 삽입하기 전 드래깅을 포기하면 새로 만들어진 상자와 데이터는 무시
      - 새 상자의 추가는 HW1의 sort에 따라 기존 모든 상자들의 위치가 변경될 수 있음

<br>
  - 데이터 삭제
    - 왼쪽 마우스 버튼으로 쌓여진 상자들 중에서 가장 높은 위치의 상자 내부에서 시작해 드래깅
      - 드래깅하는 동안 가장 높은 위치의 상자 너비가 scaling, rotation(시계방향)되면서 시작 위치에서 가상 직사각형 위치로 움직이도록 함
    - 현재 마우스 위치가 상단의 회색 가상 직사각형 내부로 들어간 경우 상자를 삭제
  
  
  - 기타
    - 텍스트 정보 출력
      - 현재 마우스 커서 위치 표시
      - 끌기 도중 rotation인자와 scaling인자를 표시
