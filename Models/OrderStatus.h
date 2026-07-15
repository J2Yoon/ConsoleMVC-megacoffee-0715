#pragma once

// 주문 상태
// RESERVED  : 주문 접수
// REJECTED  : 주문 거절 (정상 흐름 외 상태, 모니터링에서 제외)
// PRODUCING : 주문 승인 완료 및 재고 부족으로 생산 중
// CONFIRMED : 주문 승인 완료 및 출고 대기 중
// RELEASED  : 출고 완료
enum class OrderStatus
{
    Reserved,
    Rejected,
    Producing,
    Confirmed,
    Released
};

inline const char* ToString(OrderStatus status)
{
    switch (status)
    {
    case OrderStatus::Reserved:  return "RESERVED";
    case OrderStatus::Rejected:  return "REJECTED";
    case OrderStatus::Producing: return "PRODUCING";
    case OrderStatus::Confirmed: return "CONFIRMED";
    case OrderStatus::Released:  return "RELEASED";
    }
    return "UNKNOWN";
}
