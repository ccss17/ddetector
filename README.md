# Ddetector

---

### Overview
- Runtim monitoring tool using runtim interpositioning to detects & predicts **resource deadlocks**
- Costruct two dynamic libraries to override Pthread
  - online deadlock detector
  - offline deadlock predictor

---

### Usage & Demonstrate

Ubuntu Linux 시스템에서 테스트를 완료했습니다.

> Ubuntu Linux 시스템을 사용할 수 없다면 제가 만든 Docker 컨테이너 `ccss17/ubuntu` 를 트라이해보세요. 기존의 `ubuntu` 도커 컨테이너가 너무 미니멀하다면 `ccss17/ubuntu` 에는 `git`, `gcc`, `make` 등이 다 깔려있습니다. 다음의 명령어로 초간단하게 필수 패키지가 다 설치되어 있는 Ubuntu Linux 시스템을 사용할 수 있습니다.

```shell
$ docker run -it --rm ccss17/ubuntu
```

리눅스 시스템에 접속하였다면 다음의 명령어로 데드락 디덱터를 시험해보세요.

```shell
$ git clone https://github.com/ccss17/ddetector
$ cd ddetector
$ make
$ ./test_abba.sh
$ ./test_dinning_deadlock.sh
```

`test_abba.sh` 은 다음과 같이 [deadlock/abba.c](deadlock/abba.c) 의 프로그램이 의도적으로 발생시키는 ABBA 유형의 데드락을 시험합니다. 

![0kUPrtVRgT](https://user-images.githubusercontent.com/16812446/82648936-70635c80-9c53-11ea-9078-377e3cd33f21.gif)

`test_dinning_deadlock.sh` 은 다음과 같이 [deadlock/dinning_deadlock.c](deadlock/dinning_deadlock.c) 의 프로그램이 의도적으로 발생시키는 "식사하는 철학자" 유형의 데드락을 시험합니다. 

![yW7NYf78Bg](https://user-images.githubusercontent.com/16812446/82648991-84a75980-9c53-11ea-9449-75fe6532ee7c.gif)
