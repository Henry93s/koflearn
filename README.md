## Koflearn: 온라인 코딩 강의 플랫폼

**1인 프로젝트**  
**이메일:** gudrjsdn8825@naver.com  
**개발 기간:** 2025-05-27 ~ 2025-06-03 (8일)

![koflearn_UML](https://github.com/user-attachments/assets/5ed923fc-021e-4a6a-bdd5-0e7af981c5a7)


---

## 📝 프로젝트 소개

**Koflearn**은 온라인 코딩 강의 플랫폼을 모티브로 한 **콘솔 기반의 통합 관리 시스템**.

---

## 🛠️ 기술 스택

- **C++**
- **Git**
- **Visual Studio 2022**

---

## ✨ 주요 기능

### 사용자 인증 및 관리
- 로그인 및 회원가입 기능 (이메일/닉네임/전화번호 중복 검사)
- 사용자별 로그인 세션 관리 (로그인 여부, 관리자 여부)
- 회원 정보 조회, 추가, 수정, 삭제 (관리자 권한 필요)

### 강의 콘텐츠 관리
- 강의 조회, 신규 등록, 수정, 삭제 (강사/관리자 권한 필요)
- 고유 ID(Primary Key)를 통한 강의 식별 및 관리

### 수강 및 개설 강의 관리
- 학생의 강의 수강 신청
- 강사의 개설 강의 목록 관리
- 수강/개설 강의 중복 및 제한 조건 확인

### 개인화된 마이페이지
- 로그인 사용자별 수강 중인 강의 목록 조회 및 관리
- 로그인 강사별 개설 강의 목록 조회 및 관리

### 콘솔 기반 사용자 인터페이스
- 직관적인 메뉴 시스템을 통한 기능 접근
- ANSI 이스케이프 시퀀스를 활용한 동적 화면 제어

---

## 🏛️ 아키텍처

### 모델-매니저 클래스 분리
- **모델 클래스** (`Member`, `Lecture`)  
  → 시스템에서 다루는 데이터 구조 (예: 회원 정보, 강의 정보)

- **매니저 클래스** (`KoflearnPlatManager`, `MemberManager`, `LectureManager`, `LoginManager`, `EnrollManager`, `MyPageManager`, `SessionManager`)  
  → 각 모델 데이터를 관리하거나 주요 기능 로직 수행

### 의존성 주입 (Dependency Injection, DI)
- `IKoflearnPlatManager`라는 **추상 인터페이스**를 정의하여, 모든 매니저에 대한 getter 메서드 포함
- `KoflearnPlatManager`는 이 인터페이스를 구현하며, **모든 서브 매니저 객체를 멤버 변수로 소유**
- 각 서브 매니저는 생성자에서 `IKoflearnPlatManager` 타입의 포인터(`program_interface`)를 주입받음
- 이를 통해 매니저 간 상호 의존성을 다음과 같이 해결  
  → `program_interface->getOOOManager()` 형태로 접근

---
