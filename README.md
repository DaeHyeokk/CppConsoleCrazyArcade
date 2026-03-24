# 🕹️ Console Crazy Arcade

C++로 구현한 콘솔 기반 크레이지 아케이드 프로젝트입니다.  
객체 지향 설계, 스마트 포인터 기반 메모리 관리, BFS 경로 탐색, 오브젝트 풀링 등  
게임 제작의 핵심 구조를 직접 구현하며 게임 엔진 아키텍처를 이해하는 것을 목표로 개발했습니다.

🎮 **시연 영상**  
[![Console Crazy Arcade | Gameplay Video](https://img.youtube.com/vi/ZfEcu8uUjn0/0.jpg)](https://www.youtube.com/watch?v=ZfEcu8uUjn0)

[이미지를 클릭하시면 유튜브로 이동합니다]

📦 **실행 파일 다운로드**  
[https://github.com/DaeHyeokk/CppConsoleCrazyArcade/releases/tag/v1.0.0](https://github.com/DaeHyeokk/Cpp-Console-Crazy-Arcade/releases/tag/v1.0.0)  
※ 최하단 `Crazy Arcade Project.zip` 파일 다운로드

---

## 📌 프로젝트 개요

- **프로젝트명**: Console Crazy Arcade  
- **개발 기간**: 2025.04.01 ~ 2025.04.14 (2주)  
- **개발 방식**: 1인 개발  
- **기술 스택**:  
  - C++, Visual Studio 2022  
  - C++ STL  
  - Smart Pointer  
  - FSM  
  - BFS  
  - Object Pooling  

---

## 🎯 개발 목표

- 객체 지향 특징을 살린 **확장성과 유지보수성이 높은 구조 설계**
- **스마트 포인터**를 활용한 안전한 메모리 관리
- 원작의 느낌을 살린 **물풍선·블록 파괴·아이템 획득 시스템 구현**
- **FSM + BFS 기반 AI**로 사람처럼 행동하는 적 캐릭터 구현

---

## 🔧 주요 기능

### 🧱 1. 게임 프레임워크 구조
- GameObject, Board, Character 기반의 **콘솔 게임 아키텍처 직접 구현**
- Board를 중심으로 전체 객체를 관리하도록 설계  
- FlamePool(오브젝트 풀)을 통해 Flame 생성/삭제 관리

### 🎮 2. 캐릭터 이동 & 아이템 시스템
- 이동 딜레이 기반 **이동 속도 시스템 구현**
- 블록 파괴 시 랜덤 아이템 생성
- Instant Item / Consume Item 구조 분리

### 💣 3. 폭탄 설치 & 폭발 시스템
- 3초 후 폭발하는 폭탄  
- 폭발 범위 내의 Block, Item, Character와 상호작용  
- GameObject의 `ExplosionDamageAction()` 가상 함수 기반으로 처리

### 🔥 4. 오브젝트 풀링
- Flame 오브젝트를 풀링하여 **반복 생성/파괴 비용 최소화**

### 🤖 5. AI – FSM + BFS 기반 행동 로직
- 7개의 상태 기반 FSM
- BFS를 통한 실시간 **폭탄 회피 경로 탐색**
- Danger Map 기반으로 가장 안전한 지점을 찾고 이동
- 사람처럼 아이템 탐색·추격·폭탄 설치 등 다양한 행동 수행

---

## 📈 개발 결과

- 직접 GameObject 구조를 구현하며 Unity의 GameObject, Unreal의 Actor 구조 원리를 깊이 이해
- 스마트 포인터를 활용하며 메모리 관리 및 디버깅 능력이 향상됨
- 이후 Unreal 프로젝트에서 본 아키텍처를 재사용하여 더 발전된 버전의 게임을 개발하는 기반이 됨
