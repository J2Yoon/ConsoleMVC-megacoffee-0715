#pragma once

#include <string>

#include "OrderStatus.h"

// 주문(Order) : 고객이 요청한 시료에 대한 생산/출고 요청 단위
struct Order
{
    std::string orderId;                    // 주문번호 (예: ORD-20260416-0001)
    std::string sampleId;                   // 대상 시료 ID
    std::string customerName;               // 고객명
    int quantity = 0;                       // 주문 수량
    OrderStatus status = OrderStatus::Reserved;
};
