#ifndef SDK_INSTRUMENT_ESTIMATE_H
#define SDK_INSTRUMENT_ESTIMATE_H

#include "sdk_def.h" // IWYU pragma: keep
#include <QtCore/QDate>

class sdk::Estimate
{
public:
    Estimate() = default;

    // ----------------------- Ananlyst -----------------------------------------------------------
    struct Analyst {
        double consensus_rate;
        double target_price;
        uint16_t strongbuy;
        uint16_t buy;
        uint16_t hold;
        uint16_t sell;
        uint16_t strongsell;
    };
    const Analyst& analyst() const;
    FieldTOpt setAnalystConsensusRate(double rate);
    FieldTOpt setAnalystTargetPrice(double price);
    FieldTOpt setAnalystStrongBuy(uint16_t count);
    FieldTOpt setAnalystBuy(uint16_t count);
    FieldTOpt setAnalystHold(uint16_t count);
    FieldTOpt setAnalystSell(uint16_t count);
    FieldTOpt setAnalystStrongSell(uint16_t count);
    // ============================================================================================

    double       wallStreetTargetPrice() const;
    FieldTOpt setWallStreetTargetPrice(double v);

    double       epsEstimateCurrentYear() const;
    FieldTOpt setEpsEstimateCurrentYear(double v);

    double       epsEstimateNextYear() const;
    FieldTOpt setEpsEstimateNextYear(double v);

    double       epsEstimateCurrentQuart() const;
    FieldTOpt setEpsEstimateCurrentQuart(double v);

    double       epsEstimateNextQuart() const;
    FieldTOpt setEpsEstimateNextQuart(double v);

    const QDate& recentQuart() const;
    FieldTOpt setRecentQuart(const QDate& date);

private:
    Trackable _track;
    Analyst _analyst;
    double _wall_street_target_price = 0.0;
    double _eps_estimate_current_year = 0.0;
    double _eps_estimate_next_year = 0.0;
    double _eps_estimate_current_quart = 0.0;
    double _eps_estimate_next_quart = 0.0;
    QDate _recent_quart;

    friend QDataStream& operator << (QDataStream& s, Wire <const Estimate> d);
    friend QDataStream& operator >> (QDataStream& s, Wire <      Estimate> d);
};

#endif // SDK_INSTRUMENT_ESTIMATE_H
