#ifndef MODEL_SEARCH_TAG_H
#define MODEL_SEARCH_TAG_H

#include <QtCore/QObject>
#include <QtCore/QDate>
#include <QtCore/QUrl>
#include <QtCore/QAbstractListModel>
#include <QtCore/QTimer>

namespace model { class SearchTag; }

class model::SearchTag : public QAbstractListModel
{
    Q_OBJECT
public:
    static SearchTag* instance();
    void save();

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void clear();
    void add     (QString symbol, QString name, QString type, QString region, QString currency);
    void add_data(QString symbol, QString name, QString type,
                  QString region, QString currency, QString exchange);

public slots:
    void find_by_part(QString str);

private:
    SearchTag(QObject* parent = nullptr);
    SearchTag& operator = (const SearchTag&) = delete;

    void load();

    struct Matches {
        QString symbol;
        QString name;
        QString type;
        QString region;
        QString currency;
        QString exchange;
    };

    std::vector <Matches> _list;
    std::vector <Matches> _data;
};

#endif // MODEL_SEARCH_TAG_H
