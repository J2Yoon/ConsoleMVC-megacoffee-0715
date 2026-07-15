# ConsoleMVC - 반도체 시료 생산주문관리 시스템 (MVC 스켈레톤 PoC)

가상의 반도체 회사 "S-Semi"의 반도체 시료 생산주문관리 시스템을 구현하기 위한
**MVC 스켈레톤 코드 PoC**입니다. Model / Repository / Controller / View 계층을
분리한 구조를 검증하는 것이 목적이며, 콘솔 기반 C++20 애플리케이션으로 작성했습니다.

## PoC 범위

이 저장소는 미션1의 5개 PoC 항목 중 **MVC 스켈레톤 코드**에 해당합니다.
다음 항목은 별도 PoC 저장소에서 다룹니다.

| PoC 항목 | 설명 | 비고 |
| --- | --- | --- |
| MVC 스켈레톤 코드 | Model/Controller/View 패키지 구조와 역할 분리 | 본 저장소 |
| 데이터 영속성 처리 | 파일/JSON/DB 등으로 데이터 저장·조회(CRUD) | 별도 저장소 |
| 데이터 모니터링 Tool | 저장된 데이터를 콘솔에서 실시간 조회 | 별도 저장소 |
| Dummy 데이터 생성 Tool | 테스트용 더미 데이터 생성 | 별도 저장소 |

또한 생산 라인의 **실제 생산 소요 시간 경과 반영**(실시간 진행, 프로그램 재시작 후
경과 시간 복원 등)은 이 스켈레톤 단계에서는 다루지 않으며, 담당자가 콘솔에서
수동으로 "생산 완료 처리"를 트리거하는 것으로 대체했습니다. 해당 로직은 이후
본 프로젝트 개발 단계에서 구현할 예정입니다.

## 계층 구조

```
Models/         Sample, Order, OrderStatus, ProductionJob 등 순수 데이터
Repositories/   ISampleRepository / IOrderRepository 인터페이스 + 인메모리 구현체
                (추후 DataPersistence PoC 결과물로 교체 가능하도록 인터페이스로 분리)
Controllers/    SampleController, OrderController, ProductionController,
                ShipmentController, MonitoringController - 업무 로직 담당
Views/          ConsoleView(공용 입출력), MainMenuView(메뉴 흐름) - 화면 표시 담당
main.cpp        의존성 조립 및 진입점
```

## 주요 기능

- **시료 관리** : 시료 등록(ID/이름/평균 생산시간/수율), 목록 조회, 이름 검색
- **시료 주문** : 시료 ID·고객명·수량으로 주문 예약 (RESERVED)
- **주문 승인/거절** : 재고 충분 시 즉시 CONFIRMED, 부족 시 생산 라인(FIFO 큐)에
  등록 후 PRODUCING, 거절 시 REJECTED
- **생산 라인** : 현재 처리 중인 작업 및 FIFO 대기열 조회, 부족분 대비 실 생산량
  계산(`ceil(부족분 / 수율)`), 수동 완료 트리거 시 CONFIRMED로 전환
- **모니터링** : 상태별 주문 건수(REJECTED 제외), 시료별 재고 현황과
  여유/부족/고갈 상태 표시
- **출고 처리** : CONFIRMED 상태 주문을 RELEASED로 전환

## 빌드 및 실행

Visual Studio 2022 이상(C++20, MSVC v145 툴셋) 또는 MSBuild로 빌드합니다.

```
msbuild ConsoleMVC.vcxproj /p:Configuration=Debug /p:Platform=x64
```

빌드 후 `x64/Debug/ConsoleMVC.exe`를 실행하면 콘솔 메뉴가 표시됩니다.

## 참고 문서

`docs/` 폴더에 과제 기능 명세 PDF가 포함되어 있습니다.
