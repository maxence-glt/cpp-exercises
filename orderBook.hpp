#pragma once

#include <bits/stdc++.h>

/*
*
* Make an order book
* It should support these operations:
* getBestBid(), getBestAsk(), executeOrder(), cancelOrder(), addOrder()
*
* Follow up:
* With unlimited memory how can yo umake addOrder constant time
*
*/

using Price   = float;
using Volume  = std::uint32_t;
using OrderId = std::uint64_t;

enum class Side {
    Bid,
    Ask
};

struct Order {
    Order(OrderId orderId, Side side, Price limit, Volume vol)
    : id(orderId), side(side), limit(limit), volume(vol)
    {/* empty ctor */}

    OrderId id;
    Side side;
    Price limit;
    Volume volume;
};

class OrderBook {
private:
    using oq = std::pair<Volume, std::list<Order>>;

public:
    void addOrder(Order o) {
        if (o.side == Side::Bid) 
            addFromSide(bids, asks, [](Price a, Price b)->bool { return a <= b; }, o);
        else
            addFromSide(asks, bids, [](Price a, Price b)->bool { return a >= b; }, o);
    }

    Price getBestBid() { return getBest(Side::Bid); }
    Price getBestAsk() { return getBest(Side::Ask); }

    void cancelOrder(OrderId orderId) {
        if (!orders.count(orderId))
            return;

        auto order = orders[orderId];
        auto side = order->side;

        if (side == Side::Bid) cancelFromSide(bids, order);
        else cancelFromSide(asks, order);
    }

private:
    Price getBest(Side s) {
        if (s == Side::Bid)
            return bids.empty() ? 0.0 : bids.begin()->first;
        return asks.empty() ? 0.0 : asks.begin()->first;
    }

    template <class OrderMap, class OtherMap, class F>
    void addFromSide(OrderMap &side, OtherMap &otherSide, F comp, Order o) {
        oq &orderBookInfo = side[o.limit];
        auto &orderQueue = orderBookInfo.second;

        auto &vol = o.volume;
        auto limit = o.limit;
        auto otherS = (o.side == Side::Bid ? Side::Ask : Side::Bid);

        while (vol > 0 && !otherSide.empty() && comp(getBest(otherS), limit)) {
            auto otherLimit = getBest(otherS);
            auto &otherQueue = otherSide[otherLimit];
            auto &otherVol = otherQueue.first;
            auto otherOrder = otherQueue.second.begin();
            auto &otherOrderVol = otherOrder->volume;

            if (vol < otherOrderVol) {
                otherOrderVol -= vol;
                otherVol -= vol;
                vol = 0;
            } else {
                vol -= otherOrderVol;
                otherVol -= otherOrderVol;
                orders.erase(otherOrder->id);
                otherQueue.second.pop_front();
            }

            if (otherQueue.second.empty())
                otherSide.erase(otherLimit);
        }

        if (vol) {
            orderBookInfo.first += vol;
            orderQueue.push_back(o);
            orders[o.id] = --orderQueue.end();
        }
    }

    template <class Map>
    void cancelFromSide(Map& side, const std::list<Order>::iterator o) {
        auto limit = o->limit;
        auto vol = o->volume;
        auto id = o->id;

        side[limit].first -= vol;
        side[limit].second.erase(o);
        if (!side[limit].first)
            side.erase(limit);
        orders.erase(id);
    }


    std::map<Price, oq> asks;
    std::map<Price, oq, std::greater<Price>> bids;
    std::unordered_map<OrderId, std::list<Order>::iterator> orders;
};
