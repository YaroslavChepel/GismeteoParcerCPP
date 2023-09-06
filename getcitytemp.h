#ifndef GETCITYTEMP_H
#define GETCITYTEMP_H

#include <QObject>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QUrl>
#include <iostream>
#include <html.hpp>
#include <QStringList>
#include <cstdio>
#include <QRegularExpression>
#include <QString>
class getCityTemp : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList searchResult READ searchResult NOTIFY searchResultChanged)
    Q_PROPERTY(QStringList currentTemp READ currentTemp NOTIFY currentTempChanged)
public:
    getCityTemp();
    void getTempForCity(QString link);
    void gismeteoSearch(QString searchQuery);
    QStringList searchResult() const;
    QStringList currentTemp() const;
signals:
    void getSucsessful();
    void searchResultChanged();
    void searchFinished();
    void currentTempChanged();
private:
    QStringList m_searchResult;
    QStringList m_currentTemp;

};

#endif // GETCITYTEMP_H
