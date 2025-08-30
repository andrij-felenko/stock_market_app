#ifndef DATA_TICKER_SHARES_H
#define DATA_TICKER_SHARES_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>

namespace data {
    class Instrument;
    class Shares;
}

/*!
 * \class data::Shares
 * \brief Структура акціонерного капіталу, фрі-флоут та шорт-динаміка.
 *
 * \property data::Shares::shares_outstanding
 * Кількість акцій в обігу. База для Market Cap та розрахунків «на акцію».
 *
 * \property data::Shares::shares_float
 * Free float — акції у вільному обігу (без інсайдерських/контрольних пакетів).
 * Визначає ліквідність та потенційну волатильність.
 *
 * \property data::Shares::shares_short
 * Кількість зашорчених акцій. Високі рівні можуть призводити до «short squeeze»,
 * але зазвичай сигналізують про песимістичні очікування.
 *
 * \property data::Shares::pct_insiders
 * Частка акцій у інсайдерів (%).
 * Висока частка вирівнює інтереси менеджменту та акціонерів, але зменшує фрі-флоут.
 *
 * \property data::Shares::pct_institutions
 * Частка у інституційних інвесторів (%).
 * Побічний маркер «якісності» та покриття, але може посилювати проциклічні потоки.
 *
 * \property data::Shares::short_ratio
 * Short Interest Ratio («days to cover») — днів середнього обсягу торгів,
 * потрібних для покриття шортів. Високе значення ⇒ потенційно різкі рухи при позитивних новинах.
 *
 * \property data::Shares::short_pct_out
 * Частка шортів від кількості акцій в обігу (%).
 * Альтернатива для моніторингу «навантаження» шортами.
 *
 * \property data::Shares::short_pct_float
 * Частка шортів від фрі-флоут (%). Більш «справжній» індикатор тиску на ринку, ніж short_pct_out.
 */
class data::Shares : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double shares_outstanding READ shares_outstanding NOTIFY sharesOutstandingChanged)
    Q_PROPERTY(double shares_float       READ shares_float       NOTIFY       sharesFloatChanged)
    Q_PROPERTY(double shares_short       READ shares_short       NOTIFY       sharesShortChanged)
    Q_PROPERTY(float  pct_insiders       READ pct_insiders       NOTIFY       pctInsidersChanged)
    Q_PROPERTY(float  pct_institutions   READ pct_institutions   NOTIFY   pctInstitutionsChanged)
    Q_PROPERTY(float  short_ratio        READ short_ratio        NOTIFY        shortRatioChanged)
    Q_PROPERTY(float  short_pct_out      READ short_pct_out      NOTIFY       shortPctOutChanged)
    Q_PROPERTY(float  short_pct_float    READ short_pct_float    NOTIFY     shortPctFloatChanged)
public:
    double shares_outstanding() const;
    double shares_float() const;
    double shares_short() const;
    float pct_insiders() const;
    float pct_institutions() const;
    float short_ratio() const;
    float short_pct_out() const;
    float short_pct_float() const;

    void setSharesOutstanding(double newShares_outstanding);
    void setSharesFloat(double newShares_float);
    void setSharesShort(double newShares_short);
    void setPctInsiders(float newPct_insiders);
    void setPctInstitutions(float newPct_institutions);
    void setShortRatio(float newShort_ratio);
    void setShortPctOut(float newShort_pct_out);
    void setShortPctFloat(float newShort_pct_float);

signals:
    void sharesOutstandingChanged(double shares_outstanding);
    void sharesFloatChanged(double shares_float);
    void sharesShortChanged(double shares_short);
    void pctInsidersChanged(float pct_insiders);
    void pctInstitutionsChanged(float pct_institutions);
    void shortRatioChanged(float short_ratio);
    void shortPctOutChanged(float short_pct_out);
    void shortPctFloatChanged(float short_pct_float);

private:
    Shares(Instrument* parent);
    friend class Instrument;
    Instrument* instrument() const;

    double _shares_outstanding;
    double _shares_float;
    double _shares_short;
    float _pct_insiders;
    float _pct_institutions;
    float _short_ratio;
    float _short_pct_out;
    float _short_pct_float;

    friend QDataStream& operator << (QDataStream& s, const Shares& d);
    friend QDataStream& operator >> (QDataStream& s,       Shares& d);
};

#endif
